package methods;

import annotation.Template;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Paths;

@Template(path= "cgi-bin/cgijava/src/ressource/post.html")
public class POST implements Method
{
	Template template;
	String response;
	
	{
		template = this.getClass().getDeclaredAnnotation(Template.class);
	}

	public POST()
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
