package javacgi.methods;

import java.nio.file.Files;
import java.nio.file.Paths;

import javacgi.annotation.Template;

@Template(path="ressource/get.html")
public class GET implements Method
{
	Template template;
	String response;
	
	{
		template = this.getClass().getDeclaredAnnotation(Template.class);
	}

	public GET()
	{
		loadFile();
	}
	
	void loadFile()
	{		
		try {
			response = new String(Files.readAllBytes(Paths.get(template.path())));
		} catch (Exception e) {
			response = "Error loading resource: " + e.getMessage();
		}
	}
	
	@Override
	public String getResopnse()
	{
		return response;
    }
}
