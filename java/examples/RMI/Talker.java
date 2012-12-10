import java.rmi.*;

public interface Talker extends Remote 
{
    public void say(String s) throws RemoteException;
    public void callback(Talker t, String s, long wait) throws RemoteException;
}
