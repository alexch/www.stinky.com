import java.rmi.*;
import java.rmi.server.*;

public class TalkerImpl extends UnicastRemoteObject implements Talker, Runnable
{
    public TalkerImpl() throws RemoteException
    {
    }

    public void say(String s)
    {
        System.out.println(this + " says '" + s + "'");
    }
    
    public void callback(Talker t, String s, long wait)
    {
        this.target = t;
        this.message = s;
        this.wait = wait;
        new Thread(this).start();
    }

    Talker target;
    String message;
    long wait;
    
    public void run() {
        try {
            Thread.sleep(wait);
            target.say(message);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}
