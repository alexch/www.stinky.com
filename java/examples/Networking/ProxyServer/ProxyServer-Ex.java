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

                    ProxyServer proxy1 = new ProxyServer(socketClient.getInputStream(), socketServer.getOutputStream(), clientId + "->" + serverId);
                    ProxyServer proxy2 = new ProxyServer(socketServer.getInputStream(), socketClient.getOutputStream(), serverId + "->" + clientId);

                    System.out.println("Spawning " + proxy1 + " and " + proxy2);
                    proxy1.start();
                    proxy2.start();
                }
                catch (Exception e) {  e.printStackTrace(); }
            }
        }
        catch (Exception e) {  e.printStackTrace(); }
    }

    // instance variables and methods follow

    /** the stream to read from **/
    InputStream in;
    /** the stream to write to **/
    OutputStream out;
    /** debugging name **/
    String id;

    ProxyServer(InputStream in, OutputStream out, String id)
    {
        this.in = in;
        this.out = out;
        this.id = id;
    }

    public void run()
    {
        try
        {
            int b;
            while ((b = in.read()) != -1)
            {
                out.write(b);
            }
            in.close();
            out.close();
        }
        catch (Exception e) {
            System.out.println(id + " received " + e);
            e.printStackTrace();
        }
   }
}
