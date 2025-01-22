package javacgi;

import javacgi.annotation.RequestMethod;
import javacgi.methods.Method;

@RequestMethod(method="javacgi.methods.Get")
public class Main
{
	RequestMethod request;

	{
		request = getClass().getDeclaredAnnotation(RequestMethod.class);
	}

	public Main()
	{
		Method method = null;
		try
		{
			method = (Method) Class.forName(request.method()).getDeclaredConstructor().newInstance();
			System.out.println(method.getResopnse());
		}
		catch (Exception e)
		{
			System.out.println(e.getMessage());
		}
	}

	public static void main(String[] args)
	{
		new Main();
	}
}
