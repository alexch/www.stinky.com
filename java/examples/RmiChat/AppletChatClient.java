import java.rmi.*;

/**
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class AppletChatClient extends ChatClient
{
    RmiChatApplet applet;

    public AppletChatClient(String url, RmiChatApplet applet) throws RemoteException
    {
        super(url, null, null);
        this.applet = applet;
    }

    public void sendString(String s) throws RemoteException
    {
        Message m = new Message(id, Message.ALL, s);
        server.send(m);
    }

    /** receive a message and process it **/
    public void receiveMessage(Message message) throws RemoteException
    {
        applet.textArea.append(message.getSource() + ": " + message.getBody() + "\n");
    }

}
