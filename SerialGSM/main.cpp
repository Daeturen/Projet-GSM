#include <iostream>
#include <fstream>
#include <stdio.h>
#include <wiringSerial.h>
#include <wiringPi.h>

#define TIMEOUT 200000

using namespace std;

unsigned char InitGSM(int fd);
char GetEtatSys();
unsigned char NbrSmsGsm(int fd);

int main()
{
    char etat_sys = '2';
    char res_init_gsm = -1, nbr_sms_recu=0;
    int fd_serie = -3, err_init=0;
    FILE* file_etat_sys = NULL;
    wiringPiSetup(); //INIT WIRINGPI

    // Initialisation port série
    cout << "\nInitialisation port serie... ";
    fd_serie = serialOpen ("/dev/ttyAMA0", 9600);
    if(fd_serie<0)
    {
        err_init++;
        file_etat_sys = fopen("cfg/etatsys.cfg", "w+"); //ouverture en écriture + supp contenu fichier
        if(file_etat_sys!=NULL)
        {
            fputc('0', file_etat_sys);
            fclose(file_etat_sys);
            cout << "\nErreur init serie.";
        }
        else
        {
            cout << "\nErreur ouverture etatsys.cfg et erreur init serie";
            err_init++;
        }
    }
    else
    {
        cout << "OK.";
    }

    // Initialisation module GSM
    cout << "\nInitialisation module GSM... ";
    res_init_gsm = InitGSM(fd_serie);
    if(res_init_gsm!=0)
    {
        err_init++;
        file_etat_sys = fopen("cfg/etatsys.cfg", "w+"); //ouverture en écriture + supp contenu fichier
        if(file_etat_sys!=NULL)
        {
            fputc('0', file_etat_sys);
            fclose(file_etat_sys);
        }
        else
        {
            printf("\nErreur ouverture etatsys.cfg et erreur init module GSM");
            err_init++;
        }
    }
    else
    {
        cout << "OK.";
    }
    etat_sys = GetEtatSys();
    printf("\nEtat du systeme : %c",etat_sys);

    // BOUCLE PRINCIPALE //
    if(err_init==0) // Pas d'erreurs != systeme eteint
    {
        while(1)
        {
            if(etat_sys=='1') //Si le systeme actif
            {
                nbr_sms_recu = NbrSmsGsm(fd_serie);
                if(nbr_sms_recu!=0)
                {

                }

            }
            //On reteste si le systeme est actif
            etat_sys = GetEtatSys();
            delay(5000);
        }
    }
    else //Erreurs d'initialisation
    {
        cout << "\n" << err_init << " erreurs a l'initialisation.";
    }
    return 0;
}

unsigned char InitGSM(int fd)
{
    string reponse("");
    unsigned char res = -1;
    serialPuts (fd, "AT&F");
    serialPutchar(fd, 0x0D); //CF
    serialPutchar(fd, 0x0A); //LF
    while(((serialDataAvail (fd)) <= 0)&&(cptserial<TIMEOUT)) // on attend qu'il y ait des data
    {
		delayMicroseconds(83);
	}
    while (serialDataAvail (fd))
    {
      reponse += serialGetchar(fd);
      delayMicroseconds(83);
    }
    //TEST REPONSE GSM
    if(reponse.find("OK") != string::npos)
    {
        res = 0;
    }

    return res;
}

char GetEtatSys()
{
	ifstream etat("cfg/etatsys.cfg");
    char etatsys = 0;
    etat.get(etatsys);
    return etatsys;
}

unsigned char NbrSmsGsm(int fd)
{
    int cptserial = 0;
    string reponse("");
    unsigned char nbr_sms = 0;
    serialPuts (fd, "AT+CPMS=\"MT\""); // interrogation nbr sms reçus
    serialPutchar(fd, 0x0D); //CR
    serialPutchar(fd, 0x0A); //LF
    while(((serialDataAvail (fd)) <= 0)&&(cptserial<TIMEOUT)) // on attend qu'il y ait des data
    {
		delayMicroseconds(83);
	}
    while (serialDataAvail (fd))
    {
      reponse += serialGetchar(fd);
      delayMicroseconds(83);
    }

    ///analyse reponse
    return nbr_sms;
}
