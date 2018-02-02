<?php 
$success = 0;
if(isset($_POST['newnum']))
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
?>
<!DOCTYPE html>
<html lang="fr">
  <head>
    <meta charset="utf-8">
    <title>Ajout d'un numéro</title>
    <link rel="stylesheet" href="design.css">
  </head>
  <body>
    <p>
	<?php
	if($success)
	{
		echo "Numéro ".$newnum."ajouté !";
	}
	else
	{
		echo "Erreur dans l'ajout du numéro ".$newnum;
	}
	?>
	<br/>
	<a href="index.php">Retour</a>
	</p>
	
  </body>
</html>