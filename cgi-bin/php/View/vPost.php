<head>
	<link rel="stylesheet" href="php/View/style/error&post.css">
</head>
<body>
	<div class="container">
		<div class="error-container">
			<h1>Hiii dear POST!</h1>
			
			<p>your name is <b><?=$_POST["email"] ?? "nothing"?></b></p>
			<p>your are <b><?=$_POST["phone"] ?? "nothing"?></b></p>
			<a href="/php">Go Back Home</a>
		</div>
	</div>
</body>