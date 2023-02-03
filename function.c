// #include "stdio.h"
// #include "string.h"
// #include "stdlib.h"
// #include "sys/wait.h"
// #include "unistd.h"
// #include "pwd.h"
// #include <errno.h>
// #include <netdb.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
#include "function.h"

char *c_path(char *root, char *cwd)
{
    int i, len;

    len = strlen(cwd) < strlen(root) ? strlen(cwd) : strlen(root);

    for (i = 0; i < len; i++)
    {
        if (cwd[i] != root[i])
            break;
    }

    if (i == strlen(root))
    {
        cwd[0] = '~';
        for (i = 1; i <= strlen(cwd) - strlen(root); i++)
            cwd[i] = cwd[i + strlen(root) - 1];
        cwd[i] = '\0';

        return cwd;
    }
    else
        return cwd;
}

void promt(char *root)
{
    char temp[1024];
    getcwd(temp, 1023);

    char *lgn, *pwd;
    struct passwd *pw;
    if ((lgn = getlogin()) == NULL || (pw = getpwnam(lgn)) == NULL)
    {
        fprintf(stderr, "Get of user information failed.\n");
        exit(1);
    }
    char hostbuffer[256];
    char *IPbuffer;
    struct hostent *host_entry;
    int hostname;
    hostname = gethostname(hostbuffer, sizeof(hostbuffer));

    char cwd[256];
    pwd = c_path(root, temp);
    if (getcwd(cwd, sizeof(cwd)) == NULL)
        perror("getcwd() error");
    printf("\033[0;31m"); // Set the text to the color red.
    printf("<%s@%s:%s>", pw->pw_name, hostbuffer, pwd);
    printf("\033[0m"); // Resets the text to default color.
}