import java.lang.annotation.ElementType;
import java.util.function.Consumer;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.nio.file.Files;
import java.nio.file.Paths;

@Template(path= "cgi-bin/java/ressource/")
public class Main
{
	String response;
	Template template;
	Consumer<String> consumer;

	{
		consumer = this::loadFile;
		template = this.getClass().getDeclaredAnnotation(Template.class);
	}
	
	void loadFile(String path)
	{
		try {
			response = new String(Files.readAllBytes(Paths.get(path)));
		} catch (Exception e) {
			response = "Error loading resource: " + System.getProperty("user.dir") + "/" + path;
		}
	}
	
	public Main()
	{
		// consumer.accept(template.path() + System.getenv("REQUEST_METHOD") + ".html");
		consumer.accept(template.path() + "POST" + ".html");
		System.out.println(response);
	}

	public static void main(String [] args)
	{
		new Main();
	}
}

@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@interface Template
{
	String path();
}
