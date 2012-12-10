/*
    AdderImpl.java
    Copyright(c) 1997 Alex Chaffee (alex@stinky.com)
    Permission to use for non-commercial, educational purposes
    All other uses please contact the author
*/
import java.rmi.*;
import java.rmi.server.*;

public class AdderImpl extends UnicastRemoteObject implements Adder
{
    public AdderImpl() throws RemoteException
    {
    }

    public int add(int x, int y) throws RemoteException
    {
        return x + y;
    }

}