<?php
    $name=$_REQUEST['name'];
    $email=$_REQUEST['email'];
    $comments=$_REQUEST['comments'];
    $contact=$_REQUEST['contacted'];
    mail("artflowpdx@gmail.com","Feedback", $comments . " Can we contact: " .$contact, "From: ".$email);
	echo "Thank you for your feedback."
?>
