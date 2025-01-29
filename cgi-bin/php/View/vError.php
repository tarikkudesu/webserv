<head>
	<link rel="stylesheet" href="php/View/style/error&post.css">
</head>
<body>
	<div class="container">
		<div class="error-container">
			<h1>Oops!</h1>
			
			<p>An error has occurred. Please try again later.</p>
			<p><b>Details:</b> <?= $message ?? "Something went wrong!" ?></p>
			<a href="/php">Go Back Home</a>
		</div>
	</div>
</body>