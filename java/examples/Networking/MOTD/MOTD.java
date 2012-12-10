/*
    MOTD.java
    Copyright (c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact author
*/

import java.net.*;
import java.io.*;
import java.applet.*;
import java.awt.*;

/**
    A simple Message-Of-The-Day applet.
    Reads a file from the HTML DocumentBase, displays
    its contents.
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class MOTD extends Applet
{
    String message;

    public void init() {
        String file = getParameter("FILE");
        if (file == null)
            file = "motd.txt";
        try {
            URL urlFile = new URL( getDocumentBase(), file );
            URLConnection conn = urlFile.openConnection();
            DataInputStream in =
                new DataInputStream(conn.getInputStream());
            message = in.readLine();
            in.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        if (message == null || message.equals("")) {
            message = "No message today...";
        }
        setBackground(Color.yellow);
        setForeground(Color.blue);
}

    public void paint(Graphics g) {
        g.drawString(message, 0, 20);
    }
}



