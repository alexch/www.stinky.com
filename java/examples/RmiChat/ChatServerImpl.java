/*
    ChatServerImpl.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/
import java.rmi.*;
import java.rmi.server.*;

/**
 * <p>see <a href="objects.jpg">Object Diagram</a> for RmiChatServer application
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class ChatServerImpl extends UnicastRemoteObject implements ChatServer {
    public static void main(String[] args) {

        try {
            // make a new Chat Server
            System.out.println("// make a new Chat Server");
            ChatServer server = new ChatServerImpl();

            // register it with RMI Registry
            System.out.println("// register it with RMI Registry");
            Naming.rebind("ChatServer", server);
            System.out.println("Chat Server bound");
        }
        catch (RemoteException re) {
            re.printStackTrace();
        }
        catch (java.net.MalformedURLException me) {
            me.printStackTrace();
        }


        try {
            // test
            System.out.println("Make new client");
            ChatClient client = new ChatClient();
            client.run();
        }
        catch (RemoteException re) {
            re.printStackTrace();
        }

    }

    MessageQueue messages;
    Dispatcher dispatcher;

    ChatServerImpl() throws RemoteException
    {
        // make a new message queue
        System.out.println("// make a new message queue");
        messages = new MessageQueue();

        // make a new dispatcher
        System.out.println("// make a new dispatcher");
        dispatcher = new Dispatcher(messages);

        // start the dispatcher thread
        System.out.println("// start the dispatcher thread");
        new Thread(dispatcher).start();
    }

    public int login(MessageReceiver client) throws RemoteException
    {
        System.out.println("// received login from " + client);
        return dispatcher.add(client);
    }

    public void send(Message message) throws RemoteException
    {
        System.out.println("// received message " + message);
        messages.enqueue(message);
    }
}
