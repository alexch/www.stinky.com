/*
    Browser.java
    Copyright (c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact author
*/

import java.net.*;
import java.io.*;

/**
 *    Simple "web browser" -- connects to a host, makes HTTP connection,
 *    downloads file.
 *    
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class Browser {
    
    public static void main(String[] args)
    {
        String host = "www.stinky.com";
        int port = 80;
        String file = "/index.html";
        
        if (args.length>0) host = args[0];
        if (args.length>1) file = args[1];
        
        try {
            Socket s = new Socket(host, port);
            InputStream strIn = s.getInputStream();
            OutputStream strOut = s.getOutputStream();
            
            PrintStream out = new PrintStream(strOut);
            DataInputStream in = new DataInputStream(strIn);
            
            out.println("GET " + file + " HTTP/1.0");
            out.println();
            
            String line;
            while ((line = in.readLine()) != null) {
                System.out.println(line);
            }
            
            in.close();
            out.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}