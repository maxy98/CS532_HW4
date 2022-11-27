#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/wait.h>
#define TRUE 1
#define FALSE 0
#define MAX_PATH_SIZE 2000

struct cmd
{
	char* cmd;
	char** opt;
	int nofopt;
};
struct cmd cmdstruct;
char** allfiles;
int filecount = 0;

void cmdopt(char* cmd)
{
	cmdstruct.cmd = strtok(cmd, " ");
	cmdstruct.opt = malloc(16 * sizeof(char*));
	int i = 0;
	if (cmdstruct.cmd != NULL)
	{
		char* temp = strtok(NULL, " ");
		while (temp != NULL)
		{			
			cmdstruct.opt[i] = temp;
			temp = strtok(NULL, " ");
			i++;
		}
	}
	else
	{
		printf("No cmd!");
		exit(-1);
	}
	cmdstruct.nofopt = i - 1;
	printf("cmd : %s\n", cmdstruct.cmd);
	for (i = 0; i <= cmdstruct.nofopt; i++)
		printf("option : %s\n", cmdstruct.opt[i]);

}
void cmdfile(char* file)
{

	int pid = fork();
	if (pid < 0)
	{
		printf("fork error\n");
		exit(-1);
	}
	else if (pid != 0)
	{
		wait(NULL);
	}
	else if (pid == 0)
	{
		char* arr[32];
		arr[0] = cmdstruct.cmd;
		int i = 1;
		int o=0;
		for (o = 0; o <= cmdstruct.nofopt; i++, o++)
		{
			arr[i] = cmdstruct.opt[o];
		}
		arr[i] = file;
		i++;
		arr[i] = NULL;

		execvp(cmdstruct.cmd, arr);
	}
}
void filescmd()
{
	int pid = fork();
	if (pid < 0)
	{
		printf("fork error\n");

		exit(-1);
	}
	else if (pid != 0)
	{
		wait(NULL);
	}
	else if (pid == 0)
	{
		char* arr[200];
		arr[0] = cmdstruct.cmd;
		int i = 1;
		int o=0;

		for (o = 0; o <= cmdstruct.nofopt; i++, o++)
		{
			arr[i] = cmdstruct.opt[o];
		}
		int fileno=0;
		for ( fileno = 0; fileno < filecount; fileno++)
		{
			arr[i] = allfiles[fileno];
			i++;
		}
		arr[i] = NULL;
		execvp(cmdstruct.cmd, arr);
	}
}
typedef struct Flags {

    int is1; 

    
    int is2; 
    int arg2;

    int is3; 
    char *arg3;
    int depth;

    int is4;
    char *arg4;
    
    int is5;
    char *arg5;
    
    int is6;
    char *arg6;
} Flags;


Flags* init_Flags(Flags *flags) {

    flags->is1 = FALSE;
    flags->is2 = FALSE;
    flags->arg2 = 0;
    flags->is3 = FALSE;
    flags->arg3 = NULL;
    flags->depth = 0;

    return flags;
} 

int check_File(char *fileName, struct stat *stats, Flags flags, int tabSpaces) {



    if (flags.is2 && !(stats->st_size <= flags.arg2)) {
        return FALSE;
    }

    if (flags.is3 && (strstr(fileName, flags.arg3) == NULL || !(tabSpaces <= flags.depth))) {
            return FALSE;
    }

    if (flags.is4 && (strcmp(flags.arg4, "f") == 0) && ((stats->st_mode & S_IFMT) != S_IFREG)) {
        return FALSE;
    }
    if (flags.is4 && (strcmp(flags.arg4, "d") == 0) && ((stats->st_mode & S_IFMT) != S_IFDIR)) {
        return FALSE;
    }

    return TRUE;
}


typedef void MyFunc_1 (char *fileName, struct stat *stats, int *count, int tabSpaces, Flags flags,char *filePath);

void print_File(char *fileName, struct stat *stats, int *count, int tabSpaces, Flags flags,char *filePath) {



    if (!flags.is1) 
	{
        printf("%*s[%d] %s\n", 4 * tabSpaces, "", *count, fileName); 
         
        
        
	          if (flags.is6)
	        {
	        	allfiles[filecount]= (char*)malloc(100);
	        	strcpy(allfiles[filecount],filePath);
	        	filecount++;	
			}
			else if(flags.is5)
	        {
				cmdfile(filePath);
	    	}
	
    }
    else {

        if ((stats->st_mode & S_IFMT) == S_IFDIR) {
        	printf("%*s[%d] %s %d Bytes, %o, %s\n", 4 * tabSpaces, "", *count, fileName, 0, stats->st_mode & 0777, ctime(&stats->st_mtime)); 
       
	         if (flags.is6)
	        {
	        	allfiles[filecount]= (char*)malloc(100);
	        	strcpy(allfiles[filecount],filePath);
	        	filecount++;	
			}	
        	 else if(flags.is5)
	        {
				cmdfile(filePath);
	    	}
		}

        else {
            printf("%*s[%d] %s %ld Bytes, %o, %s\n", 4 * tabSpaces, "", *count, fileName, stats->st_size, stats->st_mode & 0777, ctime(&stats->st_mtime));
        	
	    if (flags.is6)
	        {
	        	allfiles[filecount]= (char*)malloc(100);
	        	strcpy(allfiles[filecount],filePath);
	        	filecount++;	
			}
		else if(flags.is5)
	        {
				cmdfile(filePath);
	    	}
		} 
    }
    *(count) += 1;
}



void traverse_Directory(char *path, int tabSpaces, Flags flags, MyFunc_1 funcPtr) {

    struct dirent *dirent;
    struct stat stats;
    DIR *parentDir;

    if (tabSpaces == 0) {
        printf("Starting Directory: %s\n", path);
    }

    parentDir = opendir(path);
    if (parentDir == NULL) { 
        printf ("Error opening directory '%s'\n", path); 
        exit(-1);
    }

    int count = 1; 
    while((dirent = readdir(parentDir)) != NULL) {

        if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0) {
            continue;
        }

        char *filePath = (char *) malloc(MAX_PATH_SIZE);
        strcpy(filePath, path);
        strcat(filePath, "/");
        strcat(filePath, dirent->d_name);

        if (stat(filePath, &stats) == -1) {
            printf("Error with stat '%s\n", filePath);
            exit(-1);
        }

        if (check_File(dirent->d_name, &stats, flags, tabSpaces)) {

            funcPtr(dirent->d_name, &stats, &count, tabSpaces, flags,filePath);
        }

        if (dirent->d_type == DT_DIR) {
            
            traverse_Directory(filePath, tabSpaces + 1, flags, funcPtr);
        }

        free(filePath);

    }
    closedir(parentDir);
}

int main(int argc, char **argv) {

    int opt;
    Flags flags;

    init_Flags(&flags);
	allfiles = malloc(1000 * sizeof(char*));
    while ((opt = getopt(argc, argv, "Ss:f:t:e:E:")) != -1) {
        switch(opt) {
            case 'S':
                flags.is1 = 1;
                break;
            case 's':
                flags.is2 = 1;
                flags.arg2 = atoi(optarg);
                break;
            case 'f':
                flags.is3 = 1;
                flags.arg3 = strtok(optarg, " ");
                flags.depth = atoi(strtok(NULL, " "));
                break;
            case 't':
                flags.is4 = 1;
                flags.arg4 = optarg;
                
                break;
            case 'e':
                flags.is5 = 1;
                flags.arg5 = optarg;
                cmdopt(flags.arg5);
                break;
            case 'E':
                flags.is6 = 1;
                flags.arg6 = optarg;
				allfiles = malloc(1000 * sizeof(char*));
				cmdopt(flags.arg6);
				
                break;
                
        }
    }
    if (optind == argc) {
        
        traverse_Directory(".", 0, flags, &print_File);
        
    }
    else {
        
        traverse_Directory(argv[argc - 1], 0, flags, &print_File);
    }
    if (flags.is6)
		filescmd();
if (allfiles)
	free(allfiles);
    return 0;
}
