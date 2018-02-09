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
$file_pin = fopen('cfg/pin.cfg', 'r');
$pin_actuel = fgets($file_pin, 4096);
fclose($file_pin);

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

<div>
<h3>Etat du système : </h3>
<form method="post" action="index.php">

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
</div>
<br/>

<div>
<h3>Code PIN : </h3>
<p>Code PIN actuel : <strong><?php echo $pin_actuel; ?></strong></p>
<form method="post" action="newpin.php">
<label><strong>Nouveau code PIN :</strong> </label><input type="text" name="pin" id="pin"/> <input type="submit" value="Changer" /> 
<br/>

</form>
</div>
<br/>

<div>
<form method="post" action="addnum.php?act=new">
   <h3>Numéros de téléphone autorisés :</h3><br/>
   <ul>
   <?php
   $count = 0;
   $file_num = fopen("cfg/numauto.cfg", "r");
	if ($file_num) {
		while (($numero = fgets($file_num, 4096)) !== false) {
			echo '<li>'.$numero.' - <a href="addnum.php?act=del&id='.$count.'"><img src="bin.png" height=16 width=16 /></a></li> ';
			$count++;
		}
		if (!feof($file_num)) {
			echo "Erreur lecture fichier numéros de telephone\n";
		}
    fclose($file_num);
	}
	?>   
   </ul>
   </p>
   <label><strong>Ajouter un numéro </strong>(0xxxxxxxxx)</label> : <input type="num" name="newnum" id="newnum"/> <input type="submit" value="Ajouter" />

</form>
</div>
<br/>
<div>
<h3>Logs du système : </h3>
<form method="post" action="clrlog.php">
	 <textarea name="log" rows=40 readonly=true>
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
     </textarea><br/>
     <input type="submit" value="Supprimer les logs" name="boutonlog"/>
</form>
</div>
<br/>
</body>

</html>
