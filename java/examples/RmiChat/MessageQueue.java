/*
    MessageQueue.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/
import java.util.*;

/**
 * Implements a queue in which to store incoming messages.
 * It's thread-safe.
 * <p>see <a href="objects.jpg">Object Diagram</a> for RmiChatServer application
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class MessageQueue
{
    protected Vector contents = new Vector();

    /** add a message to the list **/
    public void enqueue(Message o)
    {
        contents.addElement(o);

        // let waiting threads know there's a message waiting
        synchronized (this) {
            notifyAll();
        }
    }

    /** is the queue empty? **/
    public boolean isEmpty() {
        return (contents.size() == 0) ? true : false;
    }

    /** remove a message from the list **/
    public Message dequeue()
    {
        if (contents.size() == 0)
            return null;

        Message m;
        synchronized (contents) {
            m = (Message)contents.elementAt(0);
            contents.removeElementAt(0);
        }
        return m;
    }
}
