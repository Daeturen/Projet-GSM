read -p "Fichier entree: " namei
read -p "Executable sortie : " nameo
sudo g++ $namei -o $nameo -lwiringPi 
sudo chmod +x $nameo
echo "done"
 
