
/*
        Code written by sangjun (best_collin@naver.com)
        2022-06-24-Friday        
        you need patch
        15, 16 , 32, 128 lines
*/


#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>

char gdb_path[]="/usr/bin/gdb";
char gdb_log_path[]="/root/gdb_log";//You can patch this
char target_program_path[]="/root/pdfinfo";//You can Patch this
int fd;


int write_why_exited(int argc, char *argv[], char **env, char *file_path)
{
        if((fd=open(gdb_log_path,O_RDWR|O_CREAT|O_APPEND,0777))<0)
        {
            perror("open failed\n");
            exit(1);
        }
        write(fd,argv[1],strlen(argv[1]));
        write(fd,"\n",1);
        close(fd);
        int status;
        pid_t pid;
        char *fake_argv[]={target_program_path,"-meta","-box",file_path,0};//You can patch this this is arguments of target programs
        if((pid=fork())==0)
        {
            execve(target_program_path, fake_argv, env);
        }
        else if(pid>0)
        {
            wait(&status);
            if((fd=open(gdb_log_path,O_RDWR|O_CREAT|O_APPEND,0777))<0)
            {
                perror("open failed\n");
                exit(1);
            }
        
            if (WTERMSIG(status) == SIGSEGV)
            {
                write(fd, "SIGSEGV\n", strlen("SIGSEGV\n"));
                write(fd, file_path, strlen(file_path));
                write(fd, "\n",1);
            }
            else if (WTERMSIG(status) == SIGABRT)
            {
                //close(fd);
                //return 1;
               write(fd, "SIGABRT\n", strlen("SIGABRT\n"));
               write(fd, file_path, strlen(file_path));
               write(fd, "\n",1);
            }
            close(fd);
        }
        else
        {
            perror("fork");
            exit(1);
        }
        return 0;

}



void handler_child(int signo)
{
    //kill(getpid(),SIGINT);
    exit(0);
}
int gdb_trace(int argc, char *argv[], char **env,char *file_path)
{
    int input_pipe[2];
    int output_pipe[2];
    //input_pipe[0] : 부모로부터 데이터를 입력받음 -> child의 stdin
    //input_pipe[1] : 부모가 자식한테 데이터를 씀 -> parent가 child한테 데이터 넘길때
    //output_pipe[0] :부모가 child의 데이터를 읽을 때
    //output_pipe[1] : 부모한테 출력값 전달 -> child가 parent에게 데이터 넘길 때
    pid_t cpid;
    char buf;


    if ((pipe(input_pipe) == -1) || (pipe(output_pipe)==-1)) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0) {    /* Child reads from pipe */
        close(input_pipe[1]);
        close(output_pipe[0]);          /* Close unused write end */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        // while (read(input_pipe[0], &buf, 1) > 0)
        //     write(output_pipe[1], &buf, 1);

        // write(output_pipe[1], "\n", 1);
        dup(input_pipe[0]);
        dup(output_pipe[1]);
        dup(output_pipe[1]);

        signal(SIGALRM,handler_child);
        alarm(1);
        //ualarm(100000,0);
        char *fake_argv[]={gdb_path,target_program_path,"-q",0};
        execve(gdb_path,fake_argv,env);
        close(input_pipe[0]);
        _exit(EXIT_SUCCESS);
    //input_pipe[0] : 부모로부터 데이터를 입력받음 -> child의 stdin
    //input_pipe[1] : 부모가 자식한테 데이터를 씀 -> parent가 child한테 데이터 넘길때
    //output_pipe[0] :부모가 child의 데이터를 읽을 때
    //output_pipe[1] : 부모한테 출력값 전달 -> child가 parent에게 데이터 넘길 때
    } else {            /* Parent writes argv[1] to pipe */
        close(input_pipe[0]);          /* Close unused read end */
        close(output_pipe[1]);
        //gdb command
        char arguments[0x100];
        sprintf(arguments,"set args -box -meta %s\n",file_path);//You can patch this, the arguments of target programs
        write(input_pipe[1],arguments,strlen(arguments));//gdb command
        write(input_pipe[1], "r\n", 2);//gdb command

        if((fd=open(gdb_log_path,O_RDWR|O_CREAT|O_APPEND,0777))<0)
        {
            perror("open failed\n");
            exit(1);
        }
        

        while (read(output_pipe[0], &buf, 1) > 0)
            write(fd, &buf, 1);

        write(fd,"\n\n\n\n",4);

        close(fd);
        close(input_pipe[1]);          /* Reader will see EOF */
        close(output_pipe[0]);
        wait(NULL);                /* Wait for child */
        return 0;
    }
}
int main(int argc, char *argv[],char **env)
{
    struct  dirent **namelist;
    int     count;
    int     idx;

    if((count = scandir(".", &namelist, NULL, alphasort)) == -1) {
        fprintf(stderr, "%s Directory Scan Error: %s\n", ".", strerror(errno));
        return 1;
    }
	if(count<=3)
		return 0;

	for(idx = 0; idx < count; idx++)
    {
        if(!strcmp(".",namelist[idx]->d_name) || !strcmp("..",namelist[idx]->d_name))
            continue;

        char file_path[0x100];
        memset(file_path,0,sizeof(file_path));
        sprintf(file_path,"%s",namelist[idx]->d_name);

        if(write_why_exited(argc, argv, env, file_path))
            continue;
        if(gdb_trace(argc, argv , env, file_path))
        {
            perror("gdb_trace failed\n");
            exit(1);
        }
        else{
            printf("done\n");
        }
    }
}

