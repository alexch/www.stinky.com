import java.rmi.*;
import java.rmi.server.*;
import java.net.*;

class TalkerClient {
    public static void main(String[] args)
    {
        try {

            System.out.println(System.getSecurityManager());

            String url = args.length>0 ? args[0] : "Talker";

            Talker remote = (Talker) Naming.lookup(url);
            Talker local = new TalkerImpl();
            
            remote.say("I'm remote");
            local.say("I'm local");
            
            System.out.println("Setting callback");
            remote.callback(local, "Callback", 2000);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}
