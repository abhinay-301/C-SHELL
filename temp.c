#include <stdio.h>
#include <string.h>
int main()
{
    char hist[21][256];
    FILE *fp;
    int l = 0;
    fp = fopen("history1.txt", "r");
    if (fp != NULL)
    {
        char line[256];

        while (fgets(line, sizeof(line), fp))
        {
            if (strcmp(line, "\n") != 0)
            {
                strcpy(hist[l], line);
                printf("%s", hist[l]);
                l++;
            }
        }
        fclose(fp);
    }

    // for (int i = 0; i < l; i++)
    // {
    //     printf("%s ", hist[i]);
    // }
    // strcpy(hist[l],"new content");
    // l=l+1;

    fp = fopen("history1.txt", "w+");
    for (int i = 0; i < l; i++)
    {
        printf("%s",hist[i]);
        fprintf(fp,"%s\n",hist[i]);
    }
    fclose(fp);
}