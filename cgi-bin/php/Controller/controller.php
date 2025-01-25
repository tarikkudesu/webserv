<?php
require_once("Controller/response.php");
require_once ("Model/manager.php");

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