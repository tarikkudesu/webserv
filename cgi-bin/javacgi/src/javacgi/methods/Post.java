package javacgi.methods;

import java.nio.file.Files;
import java.nio.file.Paths;

import javacgi.annotation.Template;

@Template(path="ressource/post.html")
public class Post implements Method
{
	Template template;
	String response;
	
	{
		template = this.getClass().getDeclaredAnnotation(Template.class);
	}

	public Post()
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