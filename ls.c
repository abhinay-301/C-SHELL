#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"
#include "pwd.h"
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include "function.h"

void ls(char **argv, int argc, char *root)
{

	if (argc == 1)
	{
		function_ls(0, "."); // ls
	}
	else if (argc == 2)
	{
		// printf(" @%s@ ",argv[1]);
		if (strcmp(argv[1], ".") == 0) // ls .
		{
			function_ls(0, ".");
		}
		else if (strcmp(argv[1], "-a") == 0)
		{
			function_ls(1, "."); // ls -a
		}
		else if (strcmp(argv[1], "-l") == 0)
		{
			function_lsl(0, "."); // ls -l
		}
		else if (strcmp(argv[1], "..") == 0)
		{
			int buffer_size1 = 1024;
			char *pwd1 = (char *)malloc(buffer_size1 * sizeof(char));
			getcwd(pwd1, 1024);
			if (chdir("..") < 0)
				perror("Error");
			int buffer_size = 1024;
			char *pwd = (char *)malloc(buffer_size * sizeof(char));
			getcwd(pwd, 1024);

			function_ls(0, pwd); // ls ..

			chdir(pwd1);
		}
		else if (strcmp(argv[1], "~") == 0)
		{

			function_ls(0, root); // ls ~
		}
		else if ((strcmp(argv[1], "-al") == 0) || (strcmp(argv[1], "-la") == 0))
		{
			function_lsl(1, "."); // ls -al <dir/file>  ls -la <dir/file>
		}
		else
		{
			function_ls(0, argv[1]); // ls <filename>/<dirpath>/<dirname>
		}
	}
	else if (argc == 3)
	{
		if ((strcmp(argv[1], "-a") == 0 && strcmp(argv[2], "-l") == 0) || (strcmp(argv[1], "-l") == 0 && strcmp(argv[2], "-a") == 0)) // ls -a -l
			function_lsl(1, ".");																									  // ls -a -l
		else if (strcmp(argv[1], "-a") == 0)
		{
			function_ls(1, argv[2]); // ls -a <dir/file>
		}
		else if (strcmp(argv[1], "-l") == 0)
		{
			function_lsl(0, argv[2]); // ls -l <dir/file>
		}
		else if ((strcmp(argv[1], "-al") == 0) || (strcmp(argv[1], "-la") == 0))
		{

			function_lsl(1, argv[2]); // ls -al <dir/file>  ls -la <dir/file>
		}
	}
	else if (argc == 6)
	{
		function_lsl(1, argv[5]);
		printf("\n%s:\n",argv[1]);
		function_lsl(1, argv[1]);
		printf("\n%s:\n",argv[3]);
		function_lsl(1, argv[3]);
	}
	else
	{
		printf("ls: cannot access 'a': No such file or directory");
	}
}

int function_ls(int flag, char *drt)
{
	struct dirent **lsf;
	struct stat info;
	if (stat(drt, &info) < 0)
	{
		perror("Error");
	}
	if (!S_ISDIR(info.st_mode))
	{
		if (info.st_mode & S_IXUSR)
		{
			printf("\033[0;32m"); // set green for executable file 32
			printf("%s\n", drt);
			printf("\033[0m"); // reset
		}
		else
		{
			printf("\033[0;37m"); // set white for other file 37
			printf("%s\n", drt);
			printf("\033[0m"); // reset
		}
	}
	else if (S_ISDIR(info.st_mode))
	{
		int lsn = scandir(drt, &lsf, 0, alphasort);
		if (lsn >= 0)
		{
			for (int i = 0; i < lsn; ++i)
			{
				if (flag == 0 && lsf[i]->d_name[0] == '.')
					continue;
				struct stat abhi3;
				if (stat(lsf[i]->d_name, &abhi3) < 0)
					perror("Error");
				if (S_ISDIR(abhi3.st_mode))
				{
					printf("\033[0;34m"); // blue for directory 34
					printf("%s\n", lsf[i]->d_name);
					printf("\033[0m");
				}
				else
				{
					if (abhi3.st_mode & S_IXUSR)
					{
						printf("\033[0;32m"); // set green for executable file 32
						printf("%s\n", lsf[i]->d_name);
						printf("\033[0m"); // reset
					}
					else
					{
						printf("\033[0;37m"); // set white for other file 37
						printf("%s\n", lsf[i]->d_name);
						printf("\033[0m"); // reset
					}
				}
			}
		}
	}
	else
		perror("Error in ls");
	return 1;
}

int function_lsl(int flag, char *drt)
{
	char last_mod_time[16];

	struct dirent **lsf;
	struct stat info;

	if (stat(drt, &info) < 0)
	{
		perror("Error");
	}
	if (!S_ISDIR(info.st_mode))
	{
		if (info.st_mode & S_IXUSR)
		{
			
			printf("-");

			if (info.st_mode & S_IRUSR)
				printf("r");
			else
				printf("-");

			if (info.st_mode & S_IWUSR)
				printf("w");
			else
				printf("-");

			if (info.st_mode & S_IXUSR)
				printf("x");
			else
				printf("-");

			if (info.st_mode & S_IRGRP)
				printf("r");
			else
				printf("-");

			if (info.st_mode & S_IWGRP)
				printf("w");
			else
				printf("-");

			if (info.st_mode & S_IXGRP)
				printf("x");
			else
				printf("-");

			if (info.st_mode & S_IROTH)
				printf("r");
			else
				printf("-");

			if (info.st_mode & S_IWOTH)
				printf("w");
			else
				printf("-");

			if (info.st_mode & S_IXOTH)
				printf("x");
			else
				printf("-");

			printf("%2lld ", info.st_nlink);				 // Number of hard links
			printf("%s ", (getpwuid(info.st_uid))->pw_name); // owner name
			printf("%s ", (getgrgid(info.st_gid))->gr_name); // group name

			printf("%10lld ", info.st_size); // Total size, in bytes

			strftime(last_mod_time, 14, "%h %d %H:%M", localtime(&info.st_mtime)); // Time of last modification
			printf("%s ", last_mod_time);

			printf("\033[0;32m"); // set green for executable file 32
			printf("%s\n", drt);

			printf("\033[0m"); // reset
		}
		else
		{
			
			printf("-");

			if (info.st_mode & S_IRUSR)
				printf("r");
			else
				printf("-");

			if (info.st_mode & S_IWUSR)
				printf("w");
			else
				printf("-");

			if (info.st_mode & S_IXUSR)
			{
				printf("x");
			}
			else
				printf("-");

			if (info.st_mode & S_IRGRP)
				printf("r");
			else
				printf("-");

			if (info.st_mode & S_IWGRP)
				printf("w");
			else
				printf("-");

			if (info.st_mode & S_IXGRP)
				printf("x");
			else
				printf("-");

			if (info.st_mode & S_IROTH)
				printf("r");
			else
				printf("-");

			if (info.st_mode & S_IWOTH)
				printf("w");
			else
				printf("-");

			if (info.st_mode & S_IXOTH)
				printf("x");
			else
				printf("-");

			printf("%2lld ", info.st_nlink);				 // Number of hard links
			printf("%s ", (getpwuid(info.st_uid))->pw_name); // owner name
			printf("%s ", (getgrgid(info.st_gid))->gr_name); // group name

			printf("%10lld ", info.st_size); // Total size, in bytes

			strftime(last_mod_time, 14, "%h %d %H:%M", localtime(&info.st_mtime)); // Time of last modification
			printf("%s ", last_mod_time);

			printf("\033[0;37m"); // set white for other file 37
			printf("%s\n", drt);

			printf("\033[0m"); // reset
		}
	}

	else if (S_ISDIR(info.st_mode))
	{

		long long int total = 0;
		// printf("%s ",drt);
		int lsn = scandir(drt, &lsf, 0, alphasort);
		if (lsn > 0)
		{
			for (int i = 0; i < lsn; ++i)
			{
				if (flag == 0 && lsf[i]->d_name[0] == '.')
					continue;
				// stat() retrieve information about the file pointed to by pathname
				if (stat(lsf[i]->d_name, &info) == 0)
					total += info.st_blocks;
			}
		}
		else
		{
			perror("Error in ls -l");
			return 1;
		}

		printf("total %lld\n", total / 2);

		for (int i = 0; i < lsn; ++i)
		{
			int ex = 0, fl = 1;

			if (flag == 0 && lsf[i]->d_name[0] == '.')
				continue;
			if (stat(lsf[i]->d_name, &info) == 0)
			{
				// st_mode gives us the file mode
				if (S_ISDIR(info.st_mode))
				{
					fl = 0;
					printf("d");
				}
				else
					printf("-");

				if (info.st_mode & S_IRUSR)
					printf("r");
				else
					printf("-");

				if (info.st_mode & S_IWUSR)
					printf("w");
				else
					printf("-");

				if (info.st_mode & S_IXUSR)
				{
					ex = 1;
					printf("x");
				}
				else
					printf("-");

				if (info.st_mode & S_IRGRP)
					printf("r");
				else
					printf("-");

				if (info.st_mode & S_IWGRP)
					printf("w");
				else
					printf("-");

				if (info.st_mode & S_IXGRP)
					printf("x");
				else
					printf("-");

				if (info.st_mode & S_IROTH)
					printf("r");
				else
					printf("-");

				if (info.st_mode & S_IWOTH)
					printf("w");
				else
					printf("-");

				if (info.st_mode & S_IXOTH)
					printf("x");
				else
					printf("-");

				printf("%2lld ", info.st_nlink);				 // Number of hard links
				printf("%s ", (getpwuid(info.st_uid))->pw_name); // owner name
				printf("%s ", (getgrgid(info.st_gid))->gr_name); // group name

				printf("%10lld ", info.st_size); // Total size, in bytes

				strftime(last_mod_time, 14, "%h %d %H:%M", localtime(&info.st_mtime)); // Time of last modification
				printf("%s ", last_mod_time);
				if (ex && fl)
				{
					printf("\033[0;32m"); // set green for executable file 32
					printf("%s\n", lsf[i]->d_name);
					printf("\033[0m"); // reset
				}
				else if (!fl)
				{
					printf("\033[0;34m"); // blue for directory 34
					printf("%s\n", lsf[i]->d_name);
					printf("\033[0m"); // reset
				}
				else
				{
					printf("\033[0;37m"); // set white for other file 37
					printf("%s\n", lsf[i]->d_name);
					printf("\033[0m"); // reset
				}
				
			}
		}
	}

	return 1;
}
