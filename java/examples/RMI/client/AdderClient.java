import java.rmi.*;
import java.rmi.server.*;
import java.net.*;

class AdderClient {
    public static void main(String[] args)
    {
        try {

            System.setSecurityManager(new RMISecurityManager());

            System.out.println(System.getSecurityManager());
            
            // client-side
            String url = args.length>0 ? args[0] : "adder";

            Adder a = (Adder) Naming.lookup(url);
            
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
