/*
    GuestBook.java
    Copyright (c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for educational purposes
    All other uses please contact author
*/

import java.awt.*;
import java.applet.*;
import java.io.*;
import java.net.*;

/**
    Displays a form, sends its contents to a CGI script.
    Demonstrates both GET and POST protocols.
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class GuestBook extends Applet {

    TextField textName;
    TextField textEmail;
    TextArea textMessage;
    TextArea textResponse;

    /** use the HTTP GET protocol **/
    boolean doGet = true;
    /** use URLConnections **/
    boolean useURLConnection = true;

	public void init() {
        if (getParameter("POST") != null)
            doGet=false;

        setLayout(new FlowLayout());

        Panel p = new Panel();
        p.setLayout(new FlowLayout());
        p.add(new Label("Name:"));
        p.add(textName = new TextField(12));
        p.add(new Label("Email:"));
        p.add(textEmail = new TextField(12));
        add(p);

        p = new Panel();
        p.add(new Label("Comments:"));
        p.add(textMessage = new TextArea(6,50));
        add(p);

        add(new Button("Send"));

        add(textResponse = new TextArea(6,50));

    }

    public boolean action(Event e, Object arg) {
        if (arg.equals("Send")) {
            String name = textName.getText();
            String email = textEmail.getText();
            String message = textMessage.getText();

            StringBuffer query = new StringBuffer(name.length() + email.length() + message.length() + 50);
            query.append("name=" + URLEncoder.encode(name));
            query.append("&email=" + URLEncoder.encode(email));
            query.append("&message=" + URLEncoder.encode(message));

            if (doGet) {
                getQuery("guest.cgi", query.toString());
            }
            else
            {
                postQuery("guest.cgi", query.toString());
            }
            return true;
        }
        else
            return false;
    }   // action


    void getQuery(String scriptname, String query)
    {
        try {
            showStatus("Sending data using HTTP GET protocol");
            URL url = new URL(getCodeBase(), scriptname + "?" + query.toString());
            URLConnection con = url.openConnection();
            con.connect();
            showStatus( new DataInputStream( con.getInputStream() ).readLine() );
        } catch (Exception e) {
            showStatus(e.toString());
        }
    }

    void postQuery(String scriptname, String query) {
        Socket s = null;
        try {
            showStatus("Sending data using HTTP POST protocol");
            URL script = new URL(getCodeBase(), scriptname);

            PrintStream out;
            DataInputStream in;
            if (!useURLConnection) {
                showStatus("Opening socket");
                String host = script.getHost();
                int port = script.getPort();
                s = new Socket( host, port == -1 ? 80 : port );
                out = new PrintStream( s.getOutputStream() );

                out.println("POST " + script.getFile() + " HTTP/1.0");
                out.println("Content-type: application/octet-stream");
                out.println("Content-length: " + query.length());
                out.println();
                out.print(query);

                in = new DataInputStream( s.getInputStream() );
            }
            else {
                showStatus("opening URLConnection");
                URLConnection con = script.openConnection();
                con.setDoOutput(true);
                con.setDoInput(true);
                con.setAllowUserInteraction(false);

                showStatus("outputting");
                out = new PrintStream( con.getOutputStream() );
                out.print(query);

                showStatus("reading");
                in = new DataInputStream( con.getInputStream() );
            }

            // read data and display it
            String line;
            while ((line = in.readLine()) != null)
                showStatus(line);
            in.close();
            out.close();
        } catch (Exception e) {
            showStatus(e.toString());
            e.printStackTrace();
        } finally {
            if (s != null) {
                try { s.close(); }
                catch (IOException ioe) {}
            }
        }
    }

    public void showStatus(String status) {
        System.err.println(status);
        textResponse.append(status + "\n");
        super.showStatus(status);
    }

}
