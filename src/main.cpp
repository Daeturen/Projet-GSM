#include "fonctions.h"


int main()
{
    char etat_sys = '2';
    char res_init_gsm = -1;
    int fd_serie = -3, err_init=0, nbr_sms_recu=0;
    string bufferSMS("");
    string bufferSMSenvoi("");
    ofstream file_smsrecept;
    ifstream file_smsenvoi;


    wiringPiSetup(); //INIT WIRINGPI
	writeLog("");
    // Initialisation port serie
    cout << "\nInitialisation port serie... ";
    writeLog("\nInitialisation port serie... ");
    fd_serie = serialOpen ("/dev/ttyAMA0", 9600);
    if(fd_serie<0)
    {
        err_init++;
        if(!setEtatSys('0'))
        {
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
    res_init_gsm = initGSM(fd_serie);
    if(res_init_gsm!=0)
    {
        err_init++;
        if(!setEtatSys('0'))
        {
            cout << "\nErreur init module GSM.";
        }
        else
        {
            cout << "\nErreur ouverture etatsys.cfg et erreur init module GSM";
            err_init++;
        }
    }
    else
    {
        cout << "OK.";
    }
    etat_sys = getEtatSys();
    cout << "\nEtat du systeme :" << etat_sys;

    // BOUCLE PRINCIPALE //
    if(err_init==0) // Pas d'erreurs != systeme eteint
    {
        while(1)
        {
            if(etat_sys=='1') //Si le systeme actif
            {

                /// PARTIE LECTURE SMS
                nbr_sms_recu = nbrSmsGsm(fd_serie);
                while(nbr_sms_recu!=0)
                {
                    cout << endl << nbr_sms_recu << " SMS recus.";
                    bufferSMS = readSMS(nbr_sms_recu, fd_serie);

                    ///ECRITURE FICHIER///
                    file_smsrecept.open("cfg/sms.recept", std::ios_base::app);
                    file_smsrecept << bufferSMS << endl;
                    file_smsrecept.close();
                    bufferSMS = "";

                    if(deleteSMS(nbr_sms_recu,fd_serie)==0)
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
                    sendSMS(DEFAULT_PHONE, bufferSMSenvoi, fd_serie);
                    bufferSMSenvoi = "";

                }
                else
                {
                    cout << "\nAucun SMS a envoyer.";
                }
                file_smsenvoi.close();
                file_smsrecept.open("cfg/sms.envoi", ios::trunc);
				file_smsrecept.close();
                
                
                
            }
            else
            {
                cout << "\nSysteme inactif.";
            }
            //On reteste si le systeme est actif
            etat_sys = getEtatSys();
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
