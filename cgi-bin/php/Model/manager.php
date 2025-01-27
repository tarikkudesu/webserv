<?php 

function getData()
{
    static $data = [
        "leader"=> [
            "name" => "tarik amehri",
            "image" => "tamehri",
            "github" => "tarikkudesu",
            "linkdein" => "tarik-amehri"
        ],
		"member1"=>[
            "name" => "otman oulcaid",
            "image" => "ooulcaid",
			"github" => "otmanoulcaid",
            "linkdein" => "otmanoulcaid"
        ],
		"member2"=>[
            "name" => "omar ghazi",
            "image" => "omghazi",
			"github" => "	Om7gh",
            "linkdein" => "shghvj"
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