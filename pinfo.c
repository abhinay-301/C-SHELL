#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include"function.h"

/*
  
	Whenever you start a new process in Linux it creates a file in /proc/ folder with the same name
	 as that of the process id of the process. In that folder, there is a file named “status” which 
	has all the details of the process.

*/
void pinfo(char **command,char* root)
{
	char file_path[1024];
	char pr_status;
	char curr_path[1024];
	int pid,v_mem;
//------------creating file path which contain info
	if(command[1] != NULL)
	{
		int check_pid = atoi(command[1]);
		if(kill(check_pid,0) == -1)
		{
			perror("Invalid PID specified");
			return 1;
		}
		strcpy(file_path,"/proc/");
		strcat(file_path,command[1]);
	}
	else
	{
		strcpy(file_path,"/proc/self");
	}

	strcat(file_path,"/stat");
//-------------------------------------------------
// -----------this file contain all info-----------
	FILE *stat = fopen(file_path, "r");
	if(stat == NULL)
	{
		perror("error in pinfo");
		return 1;
	}

	fscanf(stat,"%d",&pid);
	printf("pid : %d\n", pid);

	//printing the process status
	char pp[100];
	fscanf(stat,"%s",pp);
	fscanf(stat," %c",&pr_status);
	printf("process status : {%c}\n", pr_status);

	fclose(stat);
	strcat(file_path,"m");

	FILE* statm = fopen(file_path,"r");
	if(stat == NULL)
	{
		perror("error in pinfo");
		return 1;
	}

	fscanf(statm, "%d", &v_mem);
	printf("memory : %d {Virtual Memory}\n",v_mem);

	fclose(statm);

	int i,j;
	for(i=0;file_path[i]!='\0';++i);
	for(j=0;j<=4;++j)
		file_path[i-1-j]='\0';

	strcat(file_path,"exe");

	int len = readlink(file_path,curr_path,sizeof(curr_path));
	if(len<0)
	{
		perror("Error");
		return 1;
	}

	curr_path[len] = '\0';
	char *exp = NULL;
	exp = c_path(root,curr_path);
	printf("executable Path : %s\n",exp);

}