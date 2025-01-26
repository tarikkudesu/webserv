<head>
	<style>
		body {
			font-family: Arial, sans-serif;
			margin: 0;
			padding: 0;
			background-color: #f4f4f9;
			color: #333;
			display: flex;
			justify-content: center;
			align-items: center;
			height: 100vh;
		}
		.error-container {
			text-align: center;
			max-width: 600px;
			background-color: #ffffff;
			border: 1px solid #ddd;
			border-radius: 8px;
			box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
			padding: 20px 30px;
		}
		.error-container h1 {
			color: #ff6b6b;
			font-size: 48px;
			margin-bottom: 10px;
		}
		.error-container p {
			font-size: 18px;
			margin: 10px 0 20px;
		}
		.error-container a {
			display: inline-block;
			margin-top: 20px;
			padding: 10px 20px;
			font-size: 16px;
			text-decoration: none;
			background-color: #007bff;
			color: white;
			border-radius: 4px;
			transition: background-color 0.3s ease;
		}
		.error-container a:hover {
			background-color: #0056b3;
		}
	</style>
</head>
<body>
	<div class="error-container">
		<h1>Oops!</h1>
		
		<p>An error has occurred. Please try again later.</p>
		<p><b>Details:</b> <?= $message ?? "Something went wrong!" ?></p>
		<a href="/php">Go Back Home</a>
	</body>
</div>