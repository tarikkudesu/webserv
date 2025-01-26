import methods.Method;

public class Main
{
	String request;

	{
//		request = System.getenv("QUERY_STRING");
		request = "POST";
	}

	public Main()
	{
		Method method = null;
		try
		{
			method = (Method) Class.forName("methods." + request).getDeclaredConstructor().newInstance();
			System.out.println(method.getResopnse());
		}
		catch (Exception e)
		{
			System.out.println(">>>> " + e.getMessage() + "<<<<<");
		}
	}

	public static void main(String[] args)
	{
		new Main();
	}
}
