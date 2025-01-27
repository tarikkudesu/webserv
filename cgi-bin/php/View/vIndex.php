<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Our Team</title>
    <link rel="stylesheet" href="/php/View/style/index.css">
</head>
<body style="background-color: rgb(35, 40, 47);">
    <div class="title">
        <h1 style="font-size: 200px; margin-bottom: 10px; margin-top: 10px;">Webserv</h1>
        <p style="font-size: 18px; color: rgba(210, 215, 223, 0.7);">This is when you finally understand why a URL starts with HTTP</p>
    </div>
    <div class="container">
        <div class="header">
            <h1 style="font-size: 40px; margin-bottom: 10px;">Our Team</h1>
            <p style="font-size: 18px; color: rgba(210, 215, 223, 0.7);">Meet the amazing people behind our success</p>
        </div>
		<div class="team-grid">
		<?php 
			foreach ($members as $key => $value) {
		?>
				<div class="team-member">
					<img src="/php/View/assets/<?=$value["image"]?>.jpg" alt="Team Member 1" class="member-image">
					<h2 class="member-name"><?=$value["name"]?></h2>
					<div class="member-role"><?=$value["task"]?>.</div>
					<p class="member-bio">A computer science student at 1337 coding school</p>
					<div class="social-links">
						<a href="https://github.com/<?=$value["github"]?>" class="social-link">GitHub</a>
						<a href="https://linkedin.com/in/<?=$value["linkdin"]?>" class="social-link">LinkedIn</a>
					</div>
				</div>
		<? } ?>
        </div>
    </div>
</body>
</html>
