<?php 
$success = 0;
if((isset($_POST['newnum'])) && ($_GET['act'] == "new"))
{
	$newnum = $_POST['newnum'];
	if(is_numeric($newnum)) //contient uniquement nombre
	{
		if(strlen($newnum)==10) //format 0xxxx xxxxx
		{
			$liste_nums = "";
			$num_read = "";
			$file_num = fopen('cfg/numauto.cfg', 'r');
			while (($num_read = fgets($file_num, 4096)) !== false) {
			$liste_nums .= $num_read;			
			}
			fclose($file_num);
			
			if((strpos($liste_nums, $newnum) !== false)) // check doublons
			{
				$success = 0;
			}
			else
			{
				$file_num = fopen('cfg/numauto.cfg', 'a');
				fputs($file_num,$newnum.PHP_EOL);
				fclose($file_num);
				$success = 1;
			}
		
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
		
	 $DELETE =  $_GET['id']; ;
	 $data = file("cfg/numauto.cfg");
	 $out = array();

	 foreach($data as $line) {
		 if(trim($line) != $DELETE) {
			 $out[] = $line;
		 }
	 }
	 $fp = fopen("cfg/numauto.cfg", "w+");
	 flock($fp, LOCK_EX);
	 foreach($out as $line) {
		 fwrite($fp, $line);
	 }
	 flock($fp, LOCK_UN);
	 fclose($fp);  

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
