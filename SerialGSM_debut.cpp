#include <stdio.h>
///#include <wiringSerial.h>
///#include <wiringPi.h>
#include <string.h>
#include <ctype.h>

unsigned char InitGSM(int fd);
unsigned char NbrSmsGsm(int fd);
char GetEtatSys();

int main()
{
    char etat_sys = '2';
    char res_init_gsm = -1, nbr_sms_recu=0;
    int fd_serie;
    FILE* file_etat_sys = NULL;

    ///wiringPiSetup();
    ///fd_serie = serialOpen ("/dev/ttyAMA0", 9600);
    if(fd_serie<0)
    {
        file_etat_sys = fopen("cfg/etat_sys.cfg", "w+"); //ouverture en écriture + supp contenu fichier
        if(file_etat_sys!=NULL)
        {
            fputc('0', file_etat_sys);
            fclose(file_etat_sys);
        }
        else
        {
            printf("\nErreur ouverture etat_sys.cfg et erreur init serie");
        }
    }
    res_init_gsm = InitGSM(fd_serie);
    if(res_init_gsm!=0)
    {
        file_etat_sys = fopen("cfg/etat_sys.cfg", "w+"); //ouverture en écriture + supp contenu fichier
        if(file_etat_sys!=NULL)
        {
            fputc('0', file_etat_sys);
            fclose(file_etat_sys);
        }
        else
        {
            printf("\nErreur ouverture etat_sys.cfg et erreur init module GSM");
        }
    }

    etat_sys = GetEtatSys();
    printf("\nEtat du systeme : %c",etat_sys);

    while(1)
    {
        while(etat_sys=='1')
        {
            nbr_sms_recu = NbrSmsGsm(fd_serie);
            if(nbr_sms_recu!=0)
            {

            }
        }

    }


    return 0;
}

unsigned char InitGSM(int fd)
{
    char reponse[255] = "";
    signed short cpt=0;
    serialPuts (fd, "AT&F");
    serialPutchar(fd, 0x0D); //CF
    serialPutchar(fd, 0x0A); //LF
    while (serialDataAvail (fd))
    {
      reponse[cpt] = serialGetchar(fd);
      cpt++;
    }
    cpt=0;
    ///AJOUTER TEST REPONSE GSM
    return 0;
}

char GetEtatSys()
{
        char etatsys = 0;
        f = fopen("cfg/etat_sys.cfg", "r"); //ouverture en écriture + supp contenu fichier
        if(f!=NULL)
        {
            etatsys = fgetc(f);
            fclose(f);
        }
        else
        {
            printf("\nErreur lecture etat_sys.cfg");
        return etatsys;
        }
}

unsigned char NbrSmsGsm(int fd)
{
    char reponse[255] = "";
    unsigned char nbr_sms = 0;
    signed short cpt=0;
    serialPuts (fd, "AT+CPMS=\"MT\""); // interrogation nbr sms reçus
    serialPutchar(fd, 0x0D); //CR
    serialPutchar(fd, 0x0A); //LF
    while (serialDataAvail (fd))
    {
      reponse[cpt] = serialGetchar(fd);
      cpt++;
    }
    cpt=0;
    ///analyse reponse
    return nbr_sms;
}
