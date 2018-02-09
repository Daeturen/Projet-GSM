<?php 
$success = 0;
if(isset($_POST['boutonlog']))
{
		$fp = fopen("cfg/log.txt", "w");

		fclose($fp);
		
		$success = 1;
}
?>
<!DOCTYPE html>
<html lang="fr">
  <head>
    <meta charset="utf-8">
    <title>Suppression logs</title>
    <link rel="stylesheet" href="design.css">
  </head>
  <body>
	  <div>
		  <h1>Configuration du serveur</h1>
    <p>
	<?php
	if($success)
	{
		echo "Les logs ont été supprimés !";
	}
	else
	{
		echo "Erreur dans la suppression des logs";
	}
	?>
	<br/><br/>
	<a href="index.php">Retour</a>
	</p>
	</div>
	
  </body>
</html>
