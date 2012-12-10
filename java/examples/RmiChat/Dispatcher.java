/*
    Dispatcher.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/
import java.util.*;
import java.io.*;
import java.net.*;
import java.rmi.*;

/**
 * Keeps a list of all clients, and dispatches messages to them all
 * <p>see <a href="objects.jpg">Object Diagram</a> for RmiChatServer application
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class Dispatcher implements Runnable {

    protected MessageQueue messages;

    public Dispatcher(MessageQueue messages)
    {
        this.messages = messages;
    }

    Vector clients = new Vector();

    /** add a client to the list **/
    public int add(MessageReceiver receiver) {
        int id;
        synchronized (clients) {
            clients.addElement(receiver);
            id = clients.size()-1;
        }
        try {
            receiver.setId(id);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return id;
    }

    /** remove a client from the list **/
    public void remove(MessageReceiver receiver) {
        try {
            remove(receiver.getId());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    /** remove a client from the list **/
    public void remove(int id) {
        // to preserve id safety, set its index to null
        clients.setElementAt(null, id);
    }

    /** send a message to all the clients **/
    public void dispatch(Message msg) {

        System.out.println("Dispatching " + msg);
       try {
        if (msg.target == Message.ALL) {
            // make a copy for thread safety
            Vector vCopy = (Vector)clients.clone();
            Enumeration e = vCopy.elements();
            while (e.hasMoreElements()) {

                MessageReceiver receiver = (MessageReceiver)e.nextElement();
                System.out.println("Dispatching to " + receiver);

                if (receiver != null)
                {
                    try {
                        receiver.receiveMessage(msg);
                    } catch (RemoteException re) {
                        re.printStackTrace();
                        int idRemove = vCopy.indexOf(receiver);
                        System.err.println("Removing " + idRemove);
                        remove(idRemove);
                    }
                }
            }
        }
        else
        {
            MessageReceiver receiver = (MessageReceiver)clients.elementAt(msg.target);
            try {
                receiver.receiveMessage(msg);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
       } catch (Throwable t) {
        t.printStackTrace();
       }
    }

    /** wait for the queue to be full, then send its messages **/
    public void run() {

        Message message;
        while (true) {
            synchronized (messages) {
                while (messages.isEmpty())
                {
                    try {
                        messages.wait();
                    } catch (InterruptedException e) {}
                }
                System.out.println("// Dispatcher waking up");
                message = messages.dequeue();
            }
            dispatch(message);
        }
    }

}

