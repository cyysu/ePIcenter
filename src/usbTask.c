#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void logPrint(char *c)
{
/*********************************************************************
 * DESCRIPTION: Função responsável por imprimir no logfile 
 * 				Sempre que é invocada acrescenta uma nova linha ao 
 * 				ficheiro logfile.
 * INPUT:		String a guardar no logfile
 * OUTPUT: 		None
 *********************************************************************/
	FILE *logfile;
	
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char timeStamp[50];

	sprintf(timeStamp,"[\%02d-\%02d-%d \%02d:\%02d:\%02d]", tm.tm_mday ,tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
 
	while((logfile=fopen("./Logs/logFile","a+"))==NULL);
	
	fprintf(logfile,"%s %s\n",timeStamp,c);
	fclose(logfile);	
}

int findTTY()
{
	char line[500];
	FILE *fd=NULL;
	int found=0,tty=-1;
	
	
	logPrint("[usbTask] Searching for ttyUSB file");
	
	fd=popen("dmesg |grep ttyUSB -a5 | tail","r");
	
	while(fgets(line,sizeof(line),fd)!=NULL && !found)
		{
			//printf("%s\n",line);
			if(strstr(line,"ttyUSB")!=NULL)
				{
					sscanf(line,"[%*d.%*d] usb %*d-%*d: %*s %*s %*s %*s %*s %*s %*s ttyUSB%d",&tty); //obtenho tty
					//printf("tty:%d\n",tty);
				}
			//procurar por wwan, quando encontrar faço break e tenho o tty
			if(strstr(line,"wwan")!=NULL && tty!=-1)
					found=1;
		}
	if(tty==-1)
		{
			
			logPrint("[usbTask] No ttyUSB found");
			exit(EXIT_FAILURE);
		}
	logPrint("[usbTask] Found ttyUSB");
	fclose(fd);
	return tty;
}

int wvdial(int tty)
{
	char string[100];
	
	
	system("sed -i '$ d' Conf/wvdial.conf"); //apagar ultima linha do ficheiro USBx de configuração
	
	
	sprintf(string,"echo 'Modem = /dev/ttyUSB%d' >> Conf/wvdial.conf",tty);
	system(string);//apend do modem tty ao fim do ficheiro de configuração
	
	logPrint("[usbTask] connecting wvdial, check Logs/wvdial.log");
	system("wvdial -C Conf/wvdial.conf 2> Logs/wvdial.log");							
	return 0;
}
int main()
{
	char string[50];
	int try=0;
	logPrint("[usbTask] usbTask started");

	while(try<5)
		{
			wvdial(findTTY());
			logPrint("[usbTask] usbTask stoped, wvdial fail");
			sprintf(string,"[usbTask] retry# %d",try);
			logPrint(string);
			sleep(10);
		}
	logPrint("[usbTask] ePicenter fail");
	return 0;
}
