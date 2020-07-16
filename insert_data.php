<?php
    header('Access-Control-Allow-Origin: *');
	$servername = "localhost";
	$username = "";
	$password = "";
	$db="";
	// Create connection
	$conn = new mysqli($servername, $username, $password,$db);
	// Check connection
		if ($conn->connect_error) {
	    die("Connection failed: " . $conn->connect_error);
	} 
	
	else
	{
       $bus_id ="b002";
       $dst_pt = $_GET["dst_pt"];
       $latitude = $_GET["latitude"];
       $longitude =$_GET["longitude"];
            
     
    date_default_timezone_set("Asia/Calcutta");
	$date= date("Y-M-d"); 
    $time= date("H:i:s");  
       

		    
	$sql="insert into `school_dps` (`bus_id`,`dst_pt`,`latitude`,`longitude`,`date`,`time` ) values('$bus_id','$dst_pt','$latitude','$longitude','$date','$time')";
		
	
	$run_sql=mysqli_query($conn,$sql);
     
    if ($run_sql) {
        echo "Inserted successflly" ;
    } 
    else{
        echo mysqli_errorno($conn);   
    }
     
    $conn->close();
}

?>