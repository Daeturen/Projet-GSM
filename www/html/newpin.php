<?php 
$success = 0;
if(isset($_POST['pin']))
{
	$pin = $_POST['pin'];
	if(is_numeric($pin)) //contient uniquement nombre
	{
		if(strlen($pin)==4)
		{
			$fpin = fopen('cfg/pin.cfg', 'w');
			fputs($fpin,$pin);
			fclose($fpin);
			$success = 1;
	
		}
		else
		{
			$success = 0;
		}
	}
	else
	{
		$success = 0;
	}
	
}
?>
<!DOCTYPE html>
<html lang="fr">
  <head>
    <meta charset="utf-8">
    <title>Changement du code PIN</title>
    <link rel="stylesheet" href="design.css">
  </head>
  <body>
	  <h1>Configuration du serveur</h1>
	  <div>
		  
    <p>
	<?php
	if($success)
	{
		echo "Nouveau code PIN : ".$pin;
	}
	else
	{
		echo "Erreur dans le changement du code PIN pour ".$pin;
	}
	?>
	<br/><br/>
	<a href="index.php">Retour</a>
	</p>
	</div>
	
  </body>
</html>
