<?php 
require_once("Controller/response.php");
/*
 * This script is based on an MVC architecture.
 * 
 * in this php script all the app is accessible only via the index.php
 * http://address:[port]
 * http://address:[port]?action=listAll
 * http://address:[port]?action=leader
 * remember to route the cgi location in the config file to index.php
 * a simple representation of webserv team members will be rendred
 * in this version you can test this php script using only GET method
 * cgi POST logic will be implemented as soon as possible
 */
try
{
	/* 
	 * we can add a controller Router to make routing dynamic 
	 * but it is just a simple cgi test
	 */	
	if(file_exists("Controller/controller.php")) 
	{
		require_once ("Controller/controller.php");
		if ($_SERVER["REQUEST_METHOD"] == "GET")
		{
			//Action controller Routing
			$action = $_GET["action"] ?? "index";
			if (!is_callable($action))
				throw new Exception("this action does not exist");
			$action();
		}
		else if ($_SERVER["REQUEST_METHOD"] == "POST")
			postAction();
		else
			throw new Exception("on CGI only POST and GET request are allowed");
	}
	else
		throw new Exception("the controller doesn't exist");
}
catch(Exception $e)
{
	render("vError.php", ["message" => $e->getMessage()]);
}
