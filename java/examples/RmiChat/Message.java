/*
    Message.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/

/**
 * see <a href="objects.jpg">Object Diagram</a> for RmiChatServer application
 * @version 1.0 Oct 1997
 * @author Alex Chaffee (<a href="mailto:alex@stinky.com">alex@stinky.com</a>)
 */
public class Message implements java.io.Serializable
{
    /** all possible targets **/
    public static final int ALL = -1;

    int source;
    int target;
    String body;

    public Message(int source, int target, String body)
    {
        this.source = source;
        this.target = target;
        this.body = body;
    }

    public String getBody() {   return body;    }
    public int getSource() {   return source;    }
    public int getTarget() {   return target;    }

    public String toString() {
        return "Message[" + source + "," + target + ",\"" + body + "\"]";
    }

}
