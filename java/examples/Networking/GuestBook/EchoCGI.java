/*
    EchoCGI.java
    Copyright (c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for educational purposes
    All other uses please contact author
    Decoder based on URLDecoder by Elliote Rusty Harold
*/

import java.io.*;
import java.util.*;

/**
    A simple CGI script. Uses POST protocol.
    Lives on the web server. Echoes its parameters.
    Note: to activate, you need to provide a wrapper script,
    as follows:

    <pre>
    UNIX: echo.cgi
    #!/bin/bash
    cat - | java EchoCGI

    DOS: echo.bat
    java EchoCGI
    </pre>

    May need to set up PATH and CLASSPATH as well, depending
    on configuration.

 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class EchoCGI {

    public static String decode(String s)
    {
        StringBuffer out = new StringBuffer(s.length());
        for (int i=0; i<s.length(); ++i) {
            char c = s.charAt(i);
            if (c=='+') {
                out.append(' ');
            }
            else if (c=='%') {
                int c1 = Character.digit(s.charAt(++i), 16);
                int c2 = Character.digit(s.charAt(++i), 16);
                out.append((char)(c1*16 + c2));
            }
            else {
                out.append(c);
            }
        } // for
        return out.toString();
    }

    public static Dictionary parseQuery(String in)
    {
        Hashtable params = new Hashtable();

        StringTokenizer tok = new StringTokenizer(in, "&");
        while (tok.hasMoreTokens()) {
            String param = tok.nextToken();
            int equals = param.indexOf('=');
            String name, value;
            if (equals == -1) {
                name = param;
                value = "";
            }
            else {
                name = param.substring(0, equals);
                value = param.substring(equals+1);
            }
            name = decode(name);
            value = decode(value);
            if (params.get(name) == null) {
                // add the parameter to the hashtable
                params.put(name, value);
            } else {
                // if it already exists, append the new value
                params.put(name, params.get(name) + "," + value);
            }
        }
        return params;
    }

    public static void main(String[] args)
    {
        System.out.println("Content-type: text/plain\n");

        // for a POST, the query is received
        // on standard input, and should contain
        // no newlines
        DataInputStream in = new DataInputStream(System.in);
        String query = "";
        try {
            query = in.readLine();
        } catch (IOException e) {}

        // let's echo the raw query
        System.out.println(query);

        // print all params
        Dictionary params = parseQuery(query);
        Enumeration e = params.keys();
        while (e.hasMoreElements()) {
            String name = (String)e.nextElement();
            String value = (String)params.get(name);
            System.out.println(name + "=\"" + value + "\"");
        }
    }
}
