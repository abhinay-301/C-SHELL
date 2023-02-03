#ifndef NODE_H
#define NODE_H
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

// int fg_flag;
// int foreground_pid;
// int ctrz;

// int proc_start(char **command,int bg_flag,int* ar,char **namepid,char *root);
void pinfo(char **command,char* root);
char *c_path(char *root, char *cwd);
void promt(char *root);
void ls_fun(char** argv,int argc);
int proc_start(char **command,int bg_flag,int* ar,char **namepid,char *root);
void ls(char **argv, int argc,char* root);
int ls_func(int flag, char *drt);
int lsl_func(int flag,char* drt);
#endif