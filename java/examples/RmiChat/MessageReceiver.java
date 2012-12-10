import java.rmi.*;

/**
 * <p>see <a href="objects.jpg">Object Diagram</a> for RmiChatServer application
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
interface MessageReceiver extends Remote
{
    int getId() throws RemoteException;
    void setId(int id) throws RemoteException;

    /** receive a message and process it **/
    void receiveMessage(Message message) throws RemoteException;
}

