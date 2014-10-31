import java.util.*;

public class Jni2DDE
{
	/** One stop connect and get data */
	public native byte[] doDDE(String system, String topic, String item, int timeout);

	/** Full functions */
	public native int initDDE(int timeout);
	public native int connectDDE(String system, String topic);	
	public native byte[] queryDDE(String item);
	public native int disconnectDDE();
	public native int uninitDDE();

	static
	{
		System.loadLibrary("jni2dde");
	}

	/** You may need to replace the static loader, as some older versions of microsoft's JVM do not support static initialisers */
/*	Jni2DDE()
	{
		System.loadLibrary("jni2dde");
	}
*/

	/** Main function for testing */
	public static void main(String args[])
	{
		byte[] buf = new byte[0];
		int queries = 5;
        try { if (args.length > 0) queries = Integer.parseInt(args[0]); }
        catch (Exception e) { }

		//Create class instance
		Jni2DDE ddeclass = new Jni2DDE();

		long start;
		long end;
		
		start = System.currentTimeMillis();
		ddeclass.initDDE(5000);
		ddeclass.connectDDE("EXCEL","Sheet1");
        for(int i=0;i<queries;i++)
		    buf = ddeclass.queryDDE("R1C1:R2C2");
		ddeclass.disconnectDDE();
		ddeclass.uninitDDE();
		end = System.currentTimeMillis();
		System.out.println("Without connecting / disconnecting "+queries+" queries took: " + (end-start));

		start = System.currentTimeMillis();
        for(int i=0;i<queries;i++)
		    buf = ddeclass.doDDE("EXCEL","Sheet1","R1C1:R2C2",5000);		
		end = System.currentTimeMillis();
		System.out.println("With connecting / disconnecting "+queries+" queties took: " + (end-start));
		
		//Excel returns +3 extra bytes from a cell Carriage Return, Line Feed, Null (because it treats data as string)
		System.out.println(""+buf.length+" byte(s) returned");

		//Print
		for(int i=0;i<buf.length;i++)
		{
			System.out.print((char)buf[i]);
		}
		System.out.println("--------------------------------");
		for(int i=0;i<buf.length;i++)
		{
			System.out.print(buf[i]);
		}		
	}
} 
