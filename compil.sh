read -p "Fichier entree: " namei
read -p "Executable sortie : " nameo
gcc -Wall -o $nameo $namei -lwiringPi 
chmod +x $nameo
echo "done"