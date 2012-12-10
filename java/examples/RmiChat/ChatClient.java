/*
    ChatClient.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/
import java.rmi.*;
import java.io.*;
import java.rmi.server.*;

/**
 * <p>see <a href="objects.jpg">Object Diagram</a> for RmiChatServer application
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */

class ChatClient extends UnicastRemoteObject implements MessageReceiver, Runnable
{
    DataInputStream in;
    PrintStream out;
    ChatServer server;
    int id;

    ChatClient( String url, DataInputStream in, PrintStream out ) throws RemoteException
    {
        id = -1;
        this.in = in;
        this.out = out;

        try {
            server = (ChatServer) Naming.lookup(url);
            server.login(this);
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }

    ChatClient(String url) throws RemoteException
    {
        this(url, new DataInputStream(System.in), new PrintStream(System.out));
    }

    ChatClient() throws RemoteException
    {
        this("ChatServer");
    }

    public int getId() throws RemoteException { return id; }
    public void setId(int id) throws RemoteException { this.id = id; }


    /** receive a message and process it **/
    public void receiveMessage(Message message) throws RemoteException
    {
        out.println("" + message.getSource() + ": " + message.getBody());
    }

    /** user input **/
    public void run() {
        String line;
        try {
            while ((line = in.readLine()) != null)
            {
                Message m = new Message(id, Message.ALL, line);
                server.send(m);
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }


    /** fire it up **/
    public static void main(String[] args)
    {
        try {

//            System.setSecurityManager(new RMISecurityManager());
            String url = args.length>0 ? args[0] : "ChatServer";

            System.out.println("Make new client at " + url);
            ChatClient client = new ChatClient(url);
            client.run();
        }
        catch (RemoteException re) {
            re.printStackTrace();
        }
    }
}
