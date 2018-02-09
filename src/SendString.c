
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define CRLF 10

int main ()
{
  int fd ;
  unsigned int delai=1000;

  if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

  if (wiringPiSetup () == -1)
  {
    fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
    return 1 ;
  }
  printf("\nDuree delay(ms)?");
  scanf("%u",&delai);
  while(1)
  {
	serialPuts(fd , "AT&F\r"); 
	serialPutchar(fd,13); //cr
	serialPutchar(fd,10); //lf 

    /*while ((serialDataAvail (fd)) <= 0){}
    while (serialDataAvail (fd))
    {

      printf("%c",serialGetchar(fd));
      fflush (stdout);
      delayMicroseconds(83);
	}*/
	delay(delai);
  }
  printf ("\n") ;
  return 0 ;
}
