#ifndef FONCTIONS_INC
#define FONCTIONS_INC

#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <locale>
#include <sstream>
#include <ctime>
#include "define.h"


using namespace std;

unsigned char initGSM(int fd);
void initOutput();

char getEtatSys();
string getPin();
string getPhones();
bool checkPhone(string numero);

int nbrSmsGsm(int fd);
string readSMS(int id, int fd);
bool deleteSMS(int id, int fd);
bool sendSMS(string numero, string texte, int fd);

bool file_empty(ifstream& pFile);
bool writeLog(string message, bool print = false);

string actionneuse(string action);


string sendSerial(string requete, int fd);
bool setEtatSys(char etat);

#endif
