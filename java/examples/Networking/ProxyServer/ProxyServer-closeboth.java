/*
    ProxyServer.java
    Copyright(c) 1997 Alex Chaffee
    alex@stinky.com
    Permission to use for educational purposes
    All other uses please contact the author
*/

import java.util.*;
import java.io.*;
import java.net.*;

/**
    A simple threaded proxy server. Routes data between two hosts transparently.
    Usage:
        java ProxyServer host [port [localport]]
    Defaults:
        port=80
        localport=80
**/
class ProxyServer extends Thread
{
    public static void main(String[] args)
    {
        /** the host to connect to **/
        String host = "127.0.0.1";
        /** the port to connect to **/
        int port = 80;
        /** the port to accept connections **/
        int localport = 80;
        /** the socket connecting to the client **/
        Socket socketClient;
        /** the socket connecting to the server **/
        Socket socketServer;

        // read arguments
        if (args.length == 0) {
            System.err.println("Usage: java ProxyServer host [port [localport]]");
            System.exit(1);
        }
        host = args[0];
        if (args.length>1) port = Integer.parseInt(args[1]);
        if (args.length>2) localport = Integer.parseInt(args[2]);

        try
        {
            ServerSocket ss = new ServerSocket(localport);
            System.out.println("Listening on " + ss);
            for (;;)
            {
                try {
                    socketClient = ss.accept();
                    System.out.println("Received connection " + socketClient);
                    socketServer = new Socket(host, port);
                    System.out.println("Made connection " + socketServer);

                    String clientId = socketClient.getInetAddress().getHostName();
                    String serverId = socketServer.getInetAddress().getHostName();

                    ProxyServer proxy1 = new ProxyServer(socketClient, socketServer, clientId + "->" + serverId);
                    ProxyServer proxy2 = new ProxyServer(socketServer, socketClient, serverId + "->" + clientId);

                    proxy1.setFriend(proxy2);
                    proxy2.setFriend(proxy1);
                    
                    System.out.println("Spawning " + clientId + " and " + serverId);
                    proxy1.start();
                    proxy2.start();
                }
                catch (Exception e) {  e.printStackTrace(); }
            }
        }
        catch (Exception e) {  e.printStackTrace(); }
    }

    // instance variables and methods follow
    /** socket to read from **/
    Socket socketIn;
    /** socket to write to **/
    Socket socketOut;
    /** debugging name **/
    String id;
    /** closed flag **/
    boolean isClosed = false;
    /** pointer to other proxy **/
    ProxyServer friend;
        
    ProxyServer(Socket socketIn, Socket socketOut, String id)
    {
        this.socketIn = socketIn;
        this.socketOut = socketOut;
        this.id = id;
    }
    
    public void setFriend(ProxyServer friend) {   this.friend = friend;   }

    public void run()
    {
        try
        {
            /** the stream to read from **/
            InputStream in = socketIn.getInputStream();
            /** the stream to write to **/
            OutputStream out = socketOut.getOutputStream();
            
            int b;
            while ((b = in.read()) != -1)
            {
                System.err.println(id + ": " + (char)b);
                out.write(b);
                
                if (friend.isClosed)
                    break;
            }
        }
        catch (Exception e) {
            System.out.println(id + " received " + e);
            e.printStackTrace();
        }
        finally {
            close();
        }
        System.out.println(id + " stopping");
   }

    void close() {
        // since we share sockets, we can't close
        // one socket until both threads are ready 
        // to close, so in this method we need to 
        // wait for the other thread to finish
        synchronized (this) {
            isClosed = true;
        }
        synchronized (friend) {
            if (friend.isClosed == true) {
            // friend has already closed, so take responsibility 
            // and close everything
                System.out.println(id + " closing both sockets");
                try { socketIn.close();  } catch (java.io.IOException e1) {}
                try { socketOut.close(); } catch (java.io.IOException e1) {}
            }
        }   
    }
}
