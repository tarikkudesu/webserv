<?php 

function getData()
{
    static $data = [
        "leader"=> [
            "name" => "tarik amehri",
            "image" => "tamehri"
        ],
		"member1"=>[
            "name" => "otman oulcaid",
            "image" => "ooulcaid"
        ],
		"member2"=>[
            "name" => "omar ghazi",
            "image" => "omghazi"
        ]
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