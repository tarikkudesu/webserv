package java;

public class Main
{
    public Main()
    {
        System.out.println(
                            getContentType()
                            + getPage()
                        );
    }

    getContentType()
    {
        return """
                Content-type: text/html
                """;
    }

    private String getPage()
    {
        return """
                <h1 align="center">coming soon...</br>check back later</h1>
                """;
    }

    public static void main(String[] args) {
        new Main();
    }
}
