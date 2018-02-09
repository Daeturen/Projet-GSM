#include "fonctions.h"


char getEtatSys()
{
    cout << "\nAppel fonction getEtatSys()";
	ifstream etat("cfg/etatsys.cfg");
    char etatsys = 0;
    etat.get(etatsys);
    return etatsys;
}

bool setEtatSys(char etat)
{
	bool res = true;
	FILE* file_etat_sys = NULL;
	
	file_etat_sys = fopen("cfg/etatsys.cfg", "w+"); //ouverture en ecriture + supp contenu fichier
    if(file_etat_sys!=NULL)
    {
        fputc(etat, file_etat_sys);
        fclose(file_etat_sys);
        res = false;
    }
	return res;
}


int nbrSmsGsm(int fd)
{
    cout << "\nAppel fonction nbrSmsGsm()";
    string reponse("");
    int nbr_sms = 0;
    reponse = sendSerial("AT+CPMS=\"MT\"", fd);
    ///analyse reponse provisoire
    nbr_sms=atoi(reponse.c_str());
    return nbr_sms;
}

string readSMS(int id, int fd)
{
    cout << "\nAppel fonction readSMS()";
    string commande("AT+CMGR=");
    string reponse("");        
	commande += static_cast<ostringstream*>( &(ostringstream() << id) )->str();
    reponse = sendSerial(commande, fd);
    return reponse;
}

bool deleteSMS(int id, int fd)
{
	bool result = true;
    cout << "\nAppel fonction deleteSMS()";
    string commande("AT+CMGD=");
    string reponse("");
	commande += static_cast<ostringstream*>( &(ostringstream() << id) )->str();
    reponse = sendSerial(commande, fd);
    if(reponse.find("OK") != string::npos)
    {
        result = false;
    }
    else
    {
        result = true;
    }
    
    return result;
}

bool sendSMS(string numero, string texte,int fd)
{
	bool result = true;
    cout << "\nAppel fonction sendSMS()";
    string commande("AT+CMGS=\"");
    string reponse("");
    int cptserial = 0;
    commande += numero + "\"";
    /// ON ENVOIE LE NUMERO DU SMS
    reponse = sendSerial(commande, fd);
    texte += 0x1A; // on rajoute ^Z a la fin (datasheet)
    /// ON ENVOIE LE TEXTE DU SMS
    reponse = sendSerial(texte, fd);
    if(reponse.find("CMGS") != string::npos)
    {
        result = false;
    }
    else
    {
        result = true;
    }
    
    return result;
}

bool file_empty(ifstream& pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}


unsigned char initGSM(int fd)
{
    string reponse("");
    unsigned char res = -1, err=0;

    ///INIT MODEM///
    reponse = sendSerial("AT&F", fd);
    //TEST REPONSE GSM
    if(reponse.find("OK") == string::npos)
    {
        err++;
    }
    
    ///FORMAT NUMERIQUE PR REPONSE MODEM///
    reponse = sendSerial("AT+CMEE=1", fd);
    //TEST REPONSE GSM
    if(reponse.find("OK") == string::npos)
    {
        err++; //erreur
    }

    ///FORMAT TEXTE PR REPONSE MODEM///
    reponse = sendSerial("AT+CMGF=1", fd);
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

string sendSerial(string requete, int fd)
{
	int cptserial = 0;
	string reponse = "";
	
	serialPuts (fd, requete.c_str());
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
    
    if(reponse=="")
    {
		reponse = "Timeout ou pas de reponse.";
	}
    
	return reponse;
}

bool writeLog(string message)
{
	ofstream logfile;
	time_t now = time(0);
	char* dt = ctime(&now);
    string date = dt;
	bool success = true;
	
	logfile.open("cfg/log.txt", std::ios_base::app);
    logfile << "\n" << date << " : " << message;
    logfile.close();
    success = false;
	
	return success;
}
