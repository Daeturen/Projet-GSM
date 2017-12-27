read -p "Fichier entree: " namei
read -p "Executable sortie : " nameo
g++ $namei -o $nameo -lwiringPi 
chmod +x $nameo
echo "done"
 
