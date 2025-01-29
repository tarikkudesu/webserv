<?php 
require_once ("cgi-bin/php/Controller/controller.php");
require_once("cgi-bin/php/Controller/response.php");

try
{
	if ($_SERVER["REQUEST_METHOD"] == "GET")
	{
		setSuperGlobal($_GET);
		$action = $_GET["action"] ?? "index";
		if (!is_callable($action))
			throw new Exception("this action does not exist");
		$action();
	}
	else if ($_SERVER["REQUEST_METHOD"] == "POST")
	{
		setSuperGlobal($_POST);
		postAction();
	}
	else
		throw new Exception("on CGI only POST and GET request are allowed");
}
catch(Exception $e)
{
	render("vError.php", ["message" => $e->getMessage()]);
}
