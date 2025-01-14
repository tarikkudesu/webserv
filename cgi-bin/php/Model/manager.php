<?php 

function getData()
{
    static $data = [
        "leader"=>"tarik amehri",
		"member1"=>"otman oulcaid",
		"member2"=>"omar ghazi"
    ];

    return $data;
}

function getAll()
{
    return getData();
}

function getOne($fetch)
{
    $data = getData();

    foreach ($data as $key => $value)
        if (strcmp($key, $fetch) == 0)
            return [$key => $value];
    return ["erreur" => $fetch." not found"];
}