<?php 
function remove_line($file, $remove) {
    $lines = file($file, FILE_IGNORE_NEW_LINES);
    foreach($lines as $key => $line) {
        if($line === $remove) unset($lines[$key]);
    }
    $data = implode(PHP_EOL, $lines);
    file_put_contents($file, $data);
}

$success = 0;
if((isset($_POST['newnum'])) && ($_GET['act'] == "new"))
{
	$newnum = $_POST['newnum'];
	if(is_numeric($newnum)) //contient uniquement nombre
	{
		if(strlen($newnum)==10)
		{
			$file_num = fopen('cfg/numauto.cfg', 'a');
			fputs($file_num,PHP_EOL.$newnum);
			fclose($file_num);
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

if($_GET['act'] == "del") {
	$num = $_GET['id']; // TO INT
		$file_num = fopen('cfg/numauto.cfg', 'a');
		remove_line($file_num,$GET['id']);
		fclose($file_num);
		$success = 1;

}

?>
<!DOCTYPE html>
<html lang="fr">
  <head>
    <meta charset="utf-8">
    <title>Ajout d'un numéro</title>
    <link rel="stylesheet" href="design.css">
  </head>
  <body>
	  <h1>Configuration du serveur</h1>
	  <div>
    <p>
	<?php
	if($_GET['act']=="new") // nouveau num
	{
		if($success)
		{
			echo "Numéro ".$newnum." ajouté !";
		}
		else
		{
			echo "Erreur dans l'ajout du numéro ".$newnum;
		}
		
	} else if ($_GET['act']=="del") { //supprimer num
		if($success)
		{
			echo "Numéro ".$numdelete." supprimé !";
		}
		else
		{
			echo "Erreur dans la suppression du numéro";
		}
	} else {
		echo "Erreur. Action inconnue";
	}

	?>
	<br/><br/>
	<a href="index.php">Retour</a>
	</p>
	</div>
  </body>
</html>
