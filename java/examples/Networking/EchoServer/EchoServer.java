/*
    EchoServer.java
    Copyright (c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact author
*/

import java.net.*;
import java.io.*;

/**
    A simple Echo Server. Listens for connections on port 1234.
    Echoes all its input back to the client.
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
 public class EchoServer {
    public static void main(String[] args)
    {
      try {
        ServerSocket ss = new ServerSocket(1234);
        Socket s = ss.accept();
        InputStream in = s.getInputStream();
        OutputStream out = s.getOutputStream();
        int ch;
        while ((ch = in.read()) != -1) {
            out.write(ch);
        }
        out.close();
        in.close();
      }
      catch (Exception e) {
        e.printStackTrace();
      }
    }
}


