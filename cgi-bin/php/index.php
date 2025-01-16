<?php 
require_once("cgi-bin/php/Controller/response.php");
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
	 *  
	 */	
	if(file_exists("cgi-bin/php/Controller/controller.php")) 
	{
		require_once ("cgi-bin/php/Controller/controller.php");
		if ($_SERVER["REQUEST_METHOD"] == "GET")
		{
			//Action controller Routing
			$action = $_GET["action"] ?? "index";
			if (is_callable($action))
				$action();
			else
				throw new Exception("Cette action n'est pas autorisée");
		}
		else if ($_SERVER["REQUEST_METHOD"] == "POST")
			postAction();
		//else
		//in case we want to implement an other logic for other METHODS 
	}
	else
		throw new Exception("le controleur n'est pas pris en charge!...");
}
catch(Exception $e)
{
	echo $e->getMessage();
	exit();
	AfficherReponse("vError.php", ["message" => $e->getMessage()]);
}
