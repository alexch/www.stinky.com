/*
    AdderClient.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/

import java.rmi.*;
import java.rmi.server.*;
import java.net.*;

class AdderClient {
    public static void main(String[] args)
    {
        try {

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