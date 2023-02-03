
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
#include "function.h"
//---------------------------------------------------------------------------------
char oldir[256] = "OLDDIR is not set";
char hist[21][256];
int l;
//---------------------------------------------------------------------------------

void fun_cd(char **command, char *root)
{
    char cwd[256];

    char temp[256];

    strcpy(temp, root);
    getcwd(cwd, sizeof(cwd));

    if (!command[1] || (strcmp(command[1], "~") == 0) || (strcmp(command[1], "~/") == 0))
    {
        strcpy(oldir, cwd);
        chdir(temp);
    }
    else if (strcmp(command[1], "-") == 0)
    {
        char temp[256];
        strcpy(temp, oldir);
        strcpy(oldir, cwd);
        chdir(temp);
        printf("%s\n", temp);
    }
    else if (strcmp(command[1], ".") == 0)
    {
        strcpy(oldir, cwd);
    }
    else
    {
        strcpy(oldir, cwd);
        if (chdir(command[1]) < 0)
            perror("Error");
    }
}

//------------------------------------------------------------------------
char **token(char *buffer, char *delim)
{

    int no_tok = 0;
    char *token;
    char *temp = NULL;
    //  printf("%s\n",buffer);
    temp = malloc(sizeof(char) * strlen(buffer));
    strcpy(temp, buffer);
    // printf("%s \n",temp);
    token = strtok(temp, delim);
    while (token != NULL)
    {
        no_tok++;
        token = strtok(NULL, delim);
    }
    no_tok++;
    char **argv;
    argv = (char **)malloc(sizeof(char *) * no_tok);
    token = strtok(buffer, delim);
    int in = 0;
    while (token != NULL)
    {
        argv[in] = (char *)malloc(sizeof(char) * strlen(token));
        strcpy(argv[in], token);
        in++;
        token = strtok(NULL, delim);
    }
    argv[in] = NULL;
    return argv;
}
//------------------------------------------------------------------------

void fun_pwd()
{
    int buffer_size = 1024;
    char *pwd = (char *)malloc(buffer_size * sizeof(char));
    getcwd(pwd, 1024);
    printf("%s\n", pwd);
    free(pwd);
}

//-------------------------------------------------------------------------

void clear()
{
    system("clear");
    // system("COLOR\07");
}

//------------------------------------------------------------------

void exits()
{
    exit(1);
}

//-----------------------------------------------------------------------
void history(char **his, char *cmd)
{
    int s = 0;
    int flag = -1;
    while (s < l)
    {
        if (strcmp(his[s], cmd) == 0)
            flag = s;
        s++;
    }
    if (flag > -1)
    {
        for (int i = flag; i < s - 1; i++)
        {
            his[i] = his[i + 1];
        }
        s = s - 1;
    }

    if (s < 20)
    {
        his[s] = cmd;
        s++;
    }
    else
    {
        for (int i = 0; i < 19; i++)
        {
            his[i] = his[i + 1];
        }
        his[19] = cmd;
        his[20] = NULL;
        s = 20;
    }
    if (s < 20)
        his[s] = NULL;
    else
        his[20] = NULL;

    l = s;
}

void print_his(char **his, int n)
{
    int s = 0;
    while (his[s] != NULL)
    {
        s++;
    }
    if (n <= s)
    {
        //  printf("222");
        for (int i = s - n; (i < s && i < 20); i++)
        {
            if (his[i] != NULL)
                printf("%d  %s\n", i + 1, his[i]);
        }
    }
    else
    {
        // printf("111");
        for (int i = 0; i < s; i++)
        {
            if (his[i] != NULL)
                printf("%d  %s\n", i + 1, his[i]);
        }
    }
}
//-----------------------------------------------------------------------

// void temp()
// {
//     int in_f = 0, out_f = 0, ap_f = 0;
//     char inp_file[64], out_file[64], ap_file[64];
//     int fd;

//     for (int i = 0; argv[i] != '\0'; i++)
//     {
//         if (!(strcmp(argv[i], "<")))
//         {
//             argv[i] = NULL;
//             strcpy(inp_file, argv[i + 1]);
//             in_f = 1;
//         }
//         else if (!(strcmp(argv[i], ">")))
//         {
//             argv[i] = NULL;
//             strcpy(out_file, argv[i + 1]);
//             out_f = 1;
//         }
//         else if (!(strcmp(argv[i], ">>")))
//         {
//             argv[i] = NULL;
//             strcpy(ap_file, argv[i + 1]);
//             ap_f = 1;
//         }
//     }

//     /*
//         dup() : The dup() system call creates a copy of a file descriptor.
//                 It uses the lowest-numbered unused descriptor for the new descriptor.
//         dup2() : The dup2() system call is similar to dup() but the basic difference between them is that
//                  instead of using the lowest-numbered unused file descriptor,
//                  it uses the descriptor number specified by the user.
//                  As in dup2(), in place of newfd any file descriptor can be put.
//                  This will lead all the printf() statements to be written in the file
//                   referred by the old file descriptor.

//     */

//     if (in_f)
//     {
//         fd = open(inp_file, O_RDONLY, 0);
//         if (fd < 0)
//             perror("Error opening input file");
//         if (dup2(fd, 0) < 0)
//             perror("Error - duping the input");
//         close(fd);
//     }

//     if (out_f)
//     {
//         fd = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
//         if (fd < 0)
//             perror("Error opening output file");
//         if (dup2(fd, 1) < 0)
//             perror("Error - duping the output");
//         close(fd);
//     }

//     if (ap_f)
//     {
//         fd = open(ap_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
//         if (fd < 0)
//             perror("Error opening the to be appended file");
//         if (dup2(fd, 1) < 0)
//             perror("Error - duping while appending");
//         close(fd);
//     }
// }

//----------------------------------------------------------------------

int main()
{

    //*************************************

    FILE *fp;
    l = 0;
    char *path = "history1.txt";
    fp = fopen(path, "r");
    if (fp != NULL)
    {
        char line[256];

        while (fgets(line, sizeof(line), fp))
        {
            if (strcmp(line, "\n") != 0)
            {
                strcpy(hist[l], line);
                // printf("%s", hist[l]);
                l++;
            }
        }
        fclose(fp);
    }

    //**************************************
    char root[256];
    if (getcwd(root, sizeof(root)) == NULL)
        perror("error");

    //**************************************

    while (1)
    {
        promt(root); // prompt printing

        // reading input----------------------
        char *buffer = NULL;
        ssize_t num_read1;
        size_t n = 0;
        num_read1 = getline(&buffer, &n, stdin);
        //------------------------------------------
        char **temp;
        history(hist, buffer);

        //------------------------------------------------
        // hist_cmd(buffer);

        // printf("%s\n",buffer);
        char delim1[256];
        strcpy(delim1, ";");
        char **argv1 = token(buffer, delim1);
        int a = 0;
        while (argv1[a] != NULL)
        {
            a++;
        }
        if (a == 0)
            continue;
        // printf("%s",argv1[0]);
        int in1 = -1;
        while (argv1[++in1] != NULL)
        { /* execute the commands */
            char delim[256];
            strcpy(delim, " \n\t\r\a");
            char **argv = token(argv1[in1], delim);
            //----------------------------------------------------------------

            int in_f = 0, out_f = 0, ap_f = 0;
            char inp_file[64], out_file[64], ap_file[64];
            int fd;

            for (int i = 0; argv[i] != '\0'; i++)
            {
                if (!(strcmp(argv[i], "<")))
                {
                    argv[i] = NULL;
                    strcpy(inp_file, argv[i + 1]);
                    in_f = 1;
                }
                else if (!(strcmp(argv[i], ">")))
                {
                    argv[i] = NULL;
                    strcpy(out_file, argv[i + 1]);
                    out_f = 1;
                }
                else if (!(strcmp(argv[i], ">>")))
                {
                    argv[i] = NULL;
                    strcpy(ap_file, argv[i + 1]);
                    ap_f = 1;
                }
            }

            /*
                dup() : The dup() system call creates a copy of a file descriptor.
                        It uses the lowest-numbered unused descriptor for the new descriptor.
                dup2() : The dup2() system call is similar to dup() but the basic difference between them is that
                         instead of using the lowest-numbered unused file descriptor,
                         it uses the descriptor number specified by the user.
                         As in dup2(), in place of newfd any file descriptor can be put.
                         This will lead all the printf() statements to be written in the file
                          referred by the old file descriptor.

            */

            if (in_f)
            {
                fd = open(inp_file, O_RDONLY, 0);
                if (fd < 0)
                    perror("Error opening input file");
                if (dup2(fd, 0) < 0)
                    perror("Error - duping the input");
                close(fd);
            }

            if (out_f)
            {
                fd = open(out_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
                if (fd < 0)
                    perror("Error opening output file");
                if (dup2(fd, 1) < 0)
                    perror("Error - duping the output");
                close(fd);
            }

            if (ap_f)
            {
                fd = open(ap_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
                if (fd < 0)
                    perror("Error opening the to be appended file");
                if (dup2(fd, 1) < 0)
                    perror("Error - duping while appending");
                close(fd);
            }

            //---------------------------------------------------------
            int i = 0;
            while (argv[i] != NULL)
            {
                i++;
            }
            if (i == 0)
                continue;
            if (strcmp(argv[0], "echo") == 0)
            {
                for (int j = 1; j < i; j++)
                {
                    printf("%s ", argv[j]);
                }
                printf("\n");
            }
            else if (strcmp(argv[0], "cd") == 0)
            {
                if (i > 3)
                {
                    perror("Error");
                }
                fun_cd(argv, root);
            }
            else if (strcmp(argv[0], "pwd") == 0)
            {

                fun_pwd();
            }
            else if (strcmp(argv[0], "ls") == 0)
            {

                ls(argv, i, root);
            }
            else if (strcmp(argv[0], "pinfo") == 0)
            {

                pinfo(argv, root);
            }
            else if (strcmp(argv[0], "history") == 0)
            {
                if (argv[1] != NULL)
                {
                    int y = atoi(argv[1]);
                    print_his(hist, y);
                    // printf("%d", y);
                }
                else
                    print_his(hist, 10);
            }
            if (strcmp(argv[0], "clear") == 0)
            {

                clear();
            }
            else if (strcmp(argv[0], "exit") == 0)
            {

                exits();
            }
            else if (strcmp(argv[0], "discover") == 0)
            {
            }
            else
            {
                continue;
            }
        }
    }
    fp = fopen("history1.txt", "w");
    if (fp == NULL)
    {
        perror("error");
    }
    for (int i = 0; i < l; i++)
    {
        printf("%s", hist[i]);
        fprintf(fp, "%s\n", hist[i]);
    }
    fclose(fp);

    return 0;
}
