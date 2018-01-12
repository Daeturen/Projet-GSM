#include <iostream>
#include <fstream>
#include <stdio.h>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <string>
#include <cstdlib>

#define TIMEOUT 200000

using namespace std;

unsigned char InitGSM(int fd);
char GetEtatSys();
int NbrSmsGsm(int fd);
string ReadSMS(unsigned char id, int fd);
bool DeleteSMS(unsigned char id, int fd);
bool SendSMS(string numero, string texte, int fd);
bool file_empty(ifstream& pFile);

int main()
{
    char etat_sys = '2';
    char res_init_gsm = -1;
    int fd_serie = -3, err_init=0, nbr_sms_recu=0;
    string bufferSMS("");
    string bufferSMSenvoi("");
    FILE* file_etat_sys = NULL;
    ofstream file_smsrecept;
    ifstream file_smsenvoi;


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
    cout << "\nEtat du systeme :" << etat_sys;

    // BOUCLE PRINCIPALE //
    if(err_init==0) // Pas d'erreurs != systeme eteint
    {
        while(1)
        {
            if(etat_sys=='1') //Si le systeme actif
            {

                /// PARTIE LECTURE SMS
                nbr_sms_recu = NbrSmsGsm(fd_serie);
                while(nbr_sms_recu!=0)
                {
                    cout << endl << nbr_sms_recu << " SMS recus.";
                    bufferSMS = ReadSMS(nbr_sms_recu, fd_serie);

                    ///ECRITURE FICHIER///
                    file_smsrecept.open("cfg/sms.recept", std::ios_base::app);
                    file_smsrecept << bufferSMS << endl;
                    file_smsrecept.close();
                    bufferSMS = "";

                    if(DeleteSMS(nbr_sms_recu,fd_serie)==0)
                    {
                        nbr_sms_recu -= 1;
                    }
                    else
                    {
                        cout << "\nErreur suppression SMS.";
                    }
                }

                /// PARTIE ENVOI SMS
                file_smsenvoi.open("cfg/sms.envoi");
                if(!file_empty(file_smsenvoi))
                {
                    getline(file_smsenvoi, bufferSMSenvoi);
                }
                else
                {
                    cout << "\nAucun SMS a envoyer.";
                }
                file_smsenvoi.close();
                SendSMS("0768588348", bufferSMSenvoi, fd_serie);
                bufferSMSenvoi = "";
            }
            else
            {
                cout << "\nSysteme inactif.";
            }
            //On reteste si le systeme est actif
            etat_sys = GetEtatSys();
            cout << "\nEtat du systeme :" << etat_sys;
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
    int cptserial = 0;
    string reponse("");
    unsigned char res = -1, err=0;

    ///INIT MODEM///
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
    cptserial = 0;
    //TEST REPONSE GSM
    if(reponse.find("OK") == string::npos)
    {
        err++;
    }

    ///FORMAT NUMERIQUE PR REPONSE MODEM///
    serialPuts (fd, "AT+CMEE=1");
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
    cptserial = 0;
    //TEST REPONSE GSM
    if(reponse.find("OK") == string::npos)
    {
        err++; //erreur
    }

    ///FORMAT TEXTE PR REPONSE MODEM///
    serialPuts (fd, "AT+CMGF=1");
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
    cptserial = 0;
    //TEST REPONSE GSM
    if(reponse.find("OK") == string::npos)
    {
        err++; //erreur
    }

    if(err!=0)
    {
        res = 1;
    }
    else
    {
        res = 0;
    }
    return res;
}

char GetEtatSys()
{
    cout << "\nAppel fonction GetEtatSys()";
	ifstream etat("cfg/etatsys.cfg");
    char etatsys = 0;
    etat.get(etatsys);
    return etatsys;
}

int NbrSmsGsm(int fd)
{
    cout << "\nAppel fonction NbrSmsGsm()";
    int cptserial = 0;
    string reponse("");
    int nbr_sms = 0;
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

    ///analyse reponse provisoire
    nbr_sms=atoi(reponse.c_str());
    return nbr_sms;
}

string ReadSMS(unsigned char id, int fd)
{
    cout << "\nAppel fonction ReadSMS()";
    string commande("AT+CMGR=");
    int cptserial = 0;
    string reponse("");
    commande += id;
    serialPuts (fd, commande.c_str());
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
    return reponse;
}

bool DeleteSMS(unsigned char id, int fd)
{
    cout << "\nAppel fonction DeleteSMS()";
    string commande("AT+CMGD=");
    int cptserial = 0;
    string reponse("");
    commande += id;
    serialPuts (fd, commande.c_str());
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
    if(reponse.find("OK") != string::npos)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

bool SendSMS(string numero, string texte,int fd)
{
    cout << "\nAppel fonction SendSMS()";
    string commande("AT+CMGS=\"");
    string reponse("");
    int cptserial = 0;
    commande += numero + "\"";
    /// ON ENVOIE LE NUMERO DU SMS
    serialPuts (fd, commande.c_str());
    serialPutchar(fd, 0x0D); //CR
    serialPutchar(fd, 0x0A); //LF
    while((serialDataAvail (fd))){} // On attend que le modem reponde ">"
    texte += 0x1A; // on rajoute ^Z a la fin (datasheet)
    /// ON ENVOIE LE TEXTE DU SMS
    serialPuts (fd, texte.c_str());
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
    if(reponse.find("CMGS") != string::npos)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

bool file_empty(ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}
