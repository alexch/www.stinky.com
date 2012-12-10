/*
    ChatServer.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/
import java.rmi.*;

/**
 * <p>see <a href="objects.jpg">Object Diagram</a> for RmiChatServer application
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public interface ChatServer extends Remote
{
    /**
        log in to system
        @return id the id assigned to this client.
        Note that the client's setId() method is called directly by login().
    **/
    int login(MessageReceiver client) throws RemoteException;
    void send(Message message) throws RemoteException;
}

