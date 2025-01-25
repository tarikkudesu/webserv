<?php
require_once("cgi-bin/php/Controller/response.php");
require_once ("cgi-bin/php/Model/manager.php");

function index()
{
	$team = [
		"members" => getAll()
	];
	render("vIndex.php", $team);
}

function errorAction($e)
{
	$reasons = [
		"message" => $e->getMessage()
	];
	render("vError.php", $reasons);
}

function postAction()
{
	render("vPost.php", $_POST);
}

function formAction()
{
	render("vForm.php");
}

function setSuperGlobal(&$arr)
{
	$split = explode("&", getenv("QUERY_STRING"));
	for ($i = 0; $i <  count($split); $i++)
	{
		$t = explode("=", $split[$i]);
		$arr[$t[0]] = $t[1];
	}
}