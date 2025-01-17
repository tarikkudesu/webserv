<?php
require_once("Controller/response.php");
require_once ("Model/manager.php");

function listAll()
{
	$team = [
		"members" => getAll()
	];
	afficherReponse("list.php",$team);
}

function leader()
{
	$team = [
		"members" => getOne("leader")
	];
	afficherReponse("list.php",$team);
}

function index()
{
	afficherReponse("vIndex.php");
}

function errorAction($e)
{
	$reasons = [
		"message" => $e->getMessage()
	];
	afficherReponse("vError.php", $reasons);
}

function postAction()
{
	afficherReponse("vPost.php", $_POST);
}

function formAction()
{
	afficherReponse("vForm.php");
}