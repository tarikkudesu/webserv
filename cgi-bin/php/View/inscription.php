<head>
	<link rel="stylesheet" href="/php/View/style/form.css">
</head>

<body style="background-color: rgb(35, 40, 47);">
    <div style="border: 1px solid rgba(210, 215, 223, 0.26); border-radius: 4px; margin: 100px auto; background-color: rgb(22, 27, 34); padding: 20px; max-width: 400px;">
        <h2 style="font-size: 25px; font-family: sans-serif; text-align: center; padding: 0px 0px 25px 0px; margin: 0px; color: rgb(210, 215, 223);">Contact Form</h2>
    
        <form action="/php/Controller/session.php" method="post" style="display: flex; flex-direction: column; align-items: center; gap: 15px;">
            <label for="firstName" class="form-label">First Name:</label>
            <input type="text" id="firstName" name="firstName" class="text-input" required>
            
            <label for="lastName" class="form-label">Last Name:</label>
            <input type="text" id="lastName" name="lastName" class="text-input" required>
            
            <label for="email" class="form-label">Email:</label>
            <input type="email" id="email" name="email" class="text-input" required>
            
            <label for="phone" class="form-label">Phone Number:</label>
            <input type="tel" id="phone" name="phone" class="text-input">
            
            <button type="submit" class="submit-btn">Submit</button>
        </form>
    </div>
</body>

