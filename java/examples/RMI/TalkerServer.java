import java.rmi.*;
import java.rmi.server.*;
import java.net.*;

class TalkerServer {
    public static void main(String[] args)
    {
        try {
            RemoteServer.setLog(System.err);
            TalkerImpl server = new TalkerImpl();
            Naming.rebind("Talker", server);
            System.out.println("Talker bound");
        }
        catch (RemoteException re) {
            re.printStackTrace();
        }
        catch (MalformedURLException me) {
            me.printStackTrace();
        }
    }
}
