/*
    AdderServer.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/
import java.rmi.*;
import java.rmi.server.*;
import java.net.*;

class AdderServer {
    public static void main(String[] args)
    {
        try {
//            RemoteServer.setLog(System.err);
            
            AdderImpl adder = new AdderImpl();
            
/*
            alternate method of locating registry:
            java.rmi.registry.Registry r = 
                java.rmi.registry.LocateRegistry.getRegistry("127.0.0.1");
            r.rebind("adder",adder);          
*/
            Naming.rebind("adder", adder);
            
            System.out.println("Adder bound");
        }
        catch (RemoteException re) {
            re.printStackTrace();
        }
        catch (MalformedURLException me) {
            me.printStackTrace();
        }
        
        
        try {            
            // client-side
            Adder a = (Adder) Naming.lookup("adder");
            
            int sum = a.add(2,2);
            System.out.println("2+2=" + sum);
            
            for (int i=0; i<10; ++i) {
                System.out.println("" + i + "+" + i + "=" + a.add(i,i));
            }
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}
