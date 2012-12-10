/*
	A basic extension of the java.applet.Applet class
 */

import java.awt.*;
import java.applet.*;
import java.rmi.*;

public class AdderApplet extends Applet
{
    TextField textX, textY, textSum;
    Label sum;
    
    Adder adder;
    
	public void init()
	{
        setLayout(new FlowLayout());
        add(textX = new TextField(5));
        add(new Label("+"));
        add(textY = new TextField(5));
        add(new Label("="));
        add(textSum = new TextField(5));
        add(new Button("Add"));
        
        try {
            adder = (Adder)Naming.lookup("rmi://" + getCodeBase().getHost() + "/adder");
        } catch (Exception e) {
            e.printStackTrace();
            showStatus(e.toString());
        }
    }

    public boolean action(Event event, Object arg)
    {
        if (arg.equals("Add")) {
            int x = Integer.parseInt( textX.getText() );
            int y = Integer.parseInt( textY.getText() );

            try {
                int sum = adder.add(x,y);
                textSum.setText("" + sum);
            } catch (Exception e) {
                e.printStackTrace();
                showStatus(e.toString());
            }
            return true;
        }
        return false;
    }
	
}
