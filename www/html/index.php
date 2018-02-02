<?php 

// TRAITEMENT ETAT SYSTEME
if (isset($_POST['submit'])) {
    $file_etatsys = fopen('cfg/etatsys.cfg', 'w+');
	if($_POST['submit']=='Arrêter le système')
	{
		fputs($file_etatsys,'0');
	}
	else
	{
		fputs($file_etatsys,'1');
	}

	fclose($file_etatsys);
}

///
$file_etatsys = fopen('cfg/etatsys.cfg', 'r+');

$etatsys = fgetc($file_etatsys);

fclose($file_etatsys);
?>

<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>Configuration Module GSM</title>
<link rel = "stylesheet" type = "text/css" href = "design.css" />
</head>

<body>
<h1>Configuration du serveur</h1>
<form method="post" action="index.php">
   <p>
   Etat du système : 
   <?php if($etatsys) { 
			echo "Activé"; 
			echo "&nbsp;&nbsp;&nbsp;<input id=\"bt-stop\" type=\"submit\" name=\"submit\" value=\"Arrêter le système\">";
		} else { 
			echo "Désactivé"; 
			echo "&nbsp;&nbsp;&nbsp;<input id=\"bt-start\" type=\"submit\" name=\"submit\" value=\"Démarrer le système\">";
		} 
	?>
	<br/>
</form>
<form method="post" action="addnum.php">
   Numéros de téléphone autorisés :<br/>
   <ul>
   <?php
   $file_num = fopen("cfg/numauto.cfg", "r");
	if ($file_num) {
		while (($numero = fgets($file_num, 4096)) !== false) {
			echo "<li>".$numero."</li>";
			
		}
		if (!feof($file_num)) {
			echo "Erreur lecture fichier numéros de telephone\n";
		}
    fclose($file_num);
	}
	?>   
   </ul>
   </p>
   <label>Ajouter un numéro</label> : <input type="num" name="newnum" id="newnum"/> <input type="submit" value="Ajouter" />

</form>
<h3>Logs du système : </h3>
<form>
	 <textarea name="log" rows="50" cols="100" readonly=true>
		 <?php
			$file_log = fopen("cfg/log.txt", "r");
			while (($log = fgets($file_log, 4096)) !== false) {
			echo $log;
			
			}
			if (!feof($file_log)) {
				echo "Erreur lecture fichier logs.\n";
			}
			fclose($file_log);
		 ?>
     </textarea>
</form>

</body>

</html>
