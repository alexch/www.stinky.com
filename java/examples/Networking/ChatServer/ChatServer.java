/*
    ChatServer.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/

import java.util.*;
import java.io.*;
import java.net.*;

/**
    Handles a single client connection. It extends Thread so that it can process
    the connection independently.

 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
class ClientHandler extends Thread
{
    /** the socket connecting to the client **/
    Socket incoming;
    /** a unique id for this client handler **/
    int id;
    /** A pointer back to the central dispatcher **/
    Dispatcher dispatcher;

    ClientHandler(Socket i, int id, Dispatcher d) {
        incoming = i; this.id = id; dispatcher = d;
    }

    public void run()
    {
      try
      {
        // generate filter streams for high-level IO based on socket streams
        DataInputStream in = new DataInputStream(incoming.getInputStream());
        PrintStream out = new PrintStream(incoming.getOutputStream());

        System.out.println("Accepted from " + incoming);
        out.println( "Hello! Enter BYE to exit.\r" );

        boolean done = false;
        while (!done)
        {
            String str = in.readLine();     // read a line
            if (str == null) done = true;   // break if end of file
            else
            {
                System.out.println(incoming.getInetAddress() + ": " + str);
                dispatcher.dispatch(str, id);   // send line to dispatcher

                if (str.trim().equalsIgnoreCase("BYE"))   // exit if user says bye
                    done = true;
            }
        }

        synchronized (this) {       // synchronize for thread safety
            incoming.close();
            incoming = null;
        }
      }
      catch (Exception e)
      {  System.out.println(e);     // lame error checking
      }
   }

    /** send a message to the client. This method is synchronized so we don't
        accidentally close a socket in mid-message **/
    public synchronized void send(String msg)
    {
      try
      {
        if (incoming != null) {     // don't try to send to an already-closed socket
            PrintStream out = new PrintStream(incoming.getOutputStream());
            out.println(msg + "\r" );
        }
      }
      catch (IOException e) {
        e.printStackTrace();
      }
    }
}

/** Runs a Chat Server on the host machine.
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
class ChatServer
{
    public static void main(String[] args )
    {
        int i = 1;
        try
        {
            ServerSocket s = new ServerSocket(8765);
            Dispatcher d = new Dispatcher();

            System.out.println("Listening on " + s);
            for (;;)
            {
                Socket incoming = s.accept();
                System.out.println("Spawning " + i);
                ClientHandler ch = new ClientHandler(incoming, i, d);
                ch.start(); // start a new thread inside the ClientHandler
                d.add(ch);
                i++;
            }
        }
        catch (Exception e) {  System.out.println(e); }
   }
}

/** Keeps a list of all clients, and dispatches messages to them all
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
class Dispatcher {
    Vector v = new Vector();

    /** add a client to the list **/
    public void add(ClientHandler ch) {
        v.addElement(ch);
    }

    /** remove a client from the list **/
    public void remove(ClientHandler ch) {
        v.removeElement(ch);
    }

    /** send a message to all the clients **/
    public void dispatch(String msg, int from) {

        System.out.println("Dispatching from " + from + ": " + msg);
        // make a copy for thread safety
        Vector vCopy = (Vector)v.clone();
        Enumeration e = vCopy.elements();
        while (e.hasMoreElements()) {
            ClientHandler ch = (ClientHandler)e.nextElement();
            ch.send("" + from + ": " + msg);
        }
    }
}

