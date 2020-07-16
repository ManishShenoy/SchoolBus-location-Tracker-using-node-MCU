<?php
    header('Access-Control-Allow-Origin: *');
	$servername = "";
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
       $bus_id = "b002";
       $dst_pt = $_GET["dst_pt"];
       $latitude = $_GET["latitude"];
       $longitude =$_GET["longitude"];
            
     
    date_default_timezone_set("Asia/Calcutta");
	$date= date("Y-M-d"); 
    $time= date("H:i:s");  
       
    
    $sql="update `school_dps`set `bus_id`='$bus_id',`dst_pt`='$dst_pt',`latitude`='$latitude',`longitude`='$longitude',`date`='$date',`time`='$time' where `dst_pt`='currentP'";
		    
		
	
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