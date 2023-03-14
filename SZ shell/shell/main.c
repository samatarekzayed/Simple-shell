/*SAMA ZAYED*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <linux/limits.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#include <sys/resource.h>
void logfile();
char str[2000];
char *managedCommand[2000];
char cwd[PATH_MAX];
char *backgroundd;
int fp;
// FILE *fp;
char* sama;
char*ss;
/////////////////////////////////////////////////////////////////////////////////////////////////
void removeGarbage(char semooo[])
{
    int i=0;
    while(semooo[i] != '\n')
        i++;
    semooo[i]='\0';
}
////////////////////////////////////////////////////////////////////////////////////////////////
void commandManager()
{
        managedCommand[0] = strtok(str, " ");
        if(strcmp(managedCommand[0],"export")==0)
        {
            int i = 0;
            while (managedCommand[i++] != NULL) {
                managedCommand[i] = strtok(NULL, "=");
                
            }
        }
        else {
            int i = 0;
            while (managedCommand[i++] != NULL) {
                managedCommand[i] = strtok(NULL, " ");
              
            }
        }
}
////////////////////////////////////////////////////////////////////////////////
void execute_shell_builtin()
{
// printf("waslna execute shell bultin");
 if (strcmp(managedCommand[0],"cd")==0)
         {
    //    printf("fol");
            if(managedCommand[1]!=NULL && strcmp(managedCommand[1],"~")!=0 && strcmp(managedCommand[1],"..")!=0)
            {
              int newdir;
              if( newdir= chdir(managedCommand[1])){
                printf("%s",managedCommand[1]);
                printf(":No such file or directory\n");
              }
              else
              { 
                  if (getcwd(cwd, sizeof(cwd)) != NULL){
                      printf("%s",cwd);
                }}
                
            }
            else if(managedCommand[1]!=NULL && strcmp(managedCommand[1],"..")==0 )
            {
                int newdir;
              if( newdir= chdir(managedCommand[1])){
                printf("%s",managedCommand[1]);
                printf(":No such file or directory\n");
              }
              else
              { 
                  if (getcwd(cwd, sizeof(cwd)) != NULL){
                      printf("%s",cwd);
                }}
            }
            else if(managedCommand[1]==NULL || strcmp(managedCommand[1],"~")==0 )
            {
                chdir("HOME");
                char cwd[PATH_MAX];
                if (getcwd(cwd, sizeof(cwd)) != NULL){
                 fprintf(stdout, "%s\n", cwd);
            }
 
            }
         }

 else if (strcmp(managedCommand[0], "echo")==0) 
     {
            char *first,*second;
            if (managedCommand[2]!=NULL)
            {
               first=managedCommand[1];second=managedCommand[2];
               if(first[0]=='\"')
               {
               second[strlen(managedCommand[2])-1]='\0';
               first=first+1;
               }
               if(first[0]=='$')
               {
                first=first+1;
                printf("%s ", getenv(first));
               }
               else if(first[0]!='$')
               {
                printf("%s ",first);
               }
                if(second[0]=='$')
               {
                second=second+1;
                printf("%s ", getenv(second));
               }
               else if(second[0]!='$')
               {
                printf("%s ",second);
               }

            }
            else
            {
                 first=managedCommand[1];
                   if(first[0]=='\"')
               {
                 first[strlen(managedCommand[1])-1]='\0';
                 first=first+1;
               }
                 if(first[0]=='$') 
                 {
                    first=first+1;
                    printf("%s\n", getenv(first));
                 }
                 else 
                 {
                    printf("%s\n", first);
                 }
            }
     }


 else if (strcmp(managedCommand[0], "export")==0) 
    {
        // printf("wasal1");
            char *afterEqual=managedCommand[2];
            if(afterEqual[0]=='\"' && afterEqual[strlen(managedCommand[2])-1]=='\"' ){
                 afterEqual=afterEqual+1;
                afterEqual[strlen(managedCommand[2])-2]='\0';
                //   printf(afterEqual);
                setenv(managedCommand[1],afterEqual,1);
                // printf(getenv("x"));
            }else{
                // printf("wasal2");
                setenv(managedCommand[1],managedCommand[2],1);
                //  printf(getenv("x"));
            }
    }
}
/// //////////////////////// ///////////////////////////////////////////////////////////////////////////////////////////
void execute_command() {
 int pid = fork();
 if(pid==0)//we are in child process
   {
       
        int x;
        char *yamosahl;
          if(managedCommand[1]!=NULL) {
        char *key=managedCommand[1];
        if(key[0]=='$')
        {
            printf("el7");
            key=key+1;
            yamosahl=getenv(key);
            char *ch[1000];
            ch[0]=managedCommand[0];
            ch[1] = strtok(yamosahl, " ");
           
            int i=1;
            while (ch[i] != NULL) {
                        i++;
                        ch[i] = strtok(NULL, " ");
                    }
                // printf(managedCommand[1]);
                 x= execvp(managedCommand[0],ch);
        }
        else
            x= execvp(managedCommand[0],managedCommand);
          }
        else
           x= execvp(managedCommand[0],managedCommand);
        if(x!=0){
            printf("Unknown Command!!\n");
            exit(x);
        }
           exit(0);
   }

 else if( pid ==-1 )//child failed
   {
        printf("Failed to fork the child.\n");
        // background=0;
        exit(0);
   }
 else//we are in parent process
   {
        getbackground();
        if(*backgroundd!='&')
          waitpid(pid, NULL, 0);
          
   }

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void shell()
{
    // printf("wasl shell");
   fgets(str, 2000,  stdin);
   memset(managedCommand, 0, sizeof managedCommand);
   commandManager();
   removeGarbage(managedCommand[0]);
//    for(int i=0;managedCommand[i]!=NULL;i++)
//      removeGarbage(managedCommand[i]);
   if(strcmp(managedCommand[0],"cd")==0|| strcmp(managedCommand[0],"export")==0|| strcmp(managedCommand[0],"echo")==0)
            execute_shell_builtin();

        else if(strcmp(managedCommand[0],"exit")==0){
            // fclose(fp);
            exit(0);

        }else{
       execute_command();
           
        }
}
//////////////////////////////////////////////////////////////////////
void signalhandler()
{
    int state;
    int pid;
    pid = wait3 (&state, WNOHANG, (struct rusage *)NULL );
       logfile();
    if (pid == 0 || pid == -1) {  //no zombie processes
        return;
    }
  
}
///////////////////////////////////////////////////////////////////////// 

int main()
{
    signal (SIGCHLD, signalhandler);
    fp = open("logfile.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    chdir(getcwd(cwd, sizeof(cwd)));
    while(1){
    printf("SZ shell > ");
    shell();
    printf("\n");
    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////
void logfile()
{
    if(fp != 1){
        write(fp,"Child process is terminated\n",strlen("Child process is terminated\n"));
        // fprintf(fp,"Child process is terminated\n");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void getbackground()
{
 int j=0;
 while (managedCommand[j]!=NULL)
   j++;
 j--;
 backgroundd=managedCommand[j];

}
/////////////////////////////////////////////////////////////////////////////////////////////////////