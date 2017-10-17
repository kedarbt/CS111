#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <poll.h>


char * buffer;
int process_id;
int shell;
char cr = 0xD; 
char lf = 0xA;
struct termios saved_attr;
int pipeto[2], pipefrom[2];


int timeout = 0;
int ret;
int i;

void restore()
{
	tcsetattr(0, TCSANOW, &saved_attr);
	if(shell)
	{
		int status = 0;
		waitpid(process_id, &status, 0);
		const int stat = WEXITSTATUS(status);
		const int sign = WTERMSIG(status);
		//status = (unsigned int)(status)& 0xFF;
		//int sign = ((unsigned int)(status) >> 8) & 0xFF;
		fprintf(stderr, "SHELL EXIT SIGNAL= %d STATUS=%d \n", sign, stat);
	}

}

void sig_handler(int signal)
{
	if(shell)
	{
		if(signal == SIGINT)
		{
			kill(process_id, SIGINT);
		}
		if(signal == SIGPIPE)
		{

			exit(1);
		}
	}
}

void readwrite()
{
	struct pollfd fds[2];
	fds[0].fd = 0;
	fds[1].fd = pipefrom[0];
	fds[0].events = POLLIN|POLLHUP|POLLERR;
	fds[1].events = POLLIN|POLLHUP|POLLERR;
	

	char c;
	ssize_t bytes = 1; 
	int readshell = 0;
	while(1)
	{	
		readshell = 0;
		if(shell)
		{
			while(1)
			{
				int value = poll(fds, 2, 0);
				if(value == -1)
				{
					perror("Polling failed");
					exit(1);
				}
				if(value > 0)
				{
					if(fds[0].revents & POLLIN)
					{
						bytes = read(0, buffer, 1);
						break;
					}
					if(fds[0].revents & (POLLHUP + POLLERR))
					{
						perror("Error in shell");
						exit(0);
						break;
					}
					if(fds[1].revents & POLLIN)
					{
						bytes = read(pipefrom[0], buffer, 1);
						readshell = 1;
						break;
					}
					if(fds[1].revents & (POLLHUP + POLLERR))
					{
						perror("Error in shell");
						exit(0);
						break;
					}
				}
			}
		}
		else
		{
			bytes = read(0, buffer, 1);
		}

		
		c = *buffer;
		if( bytes < 0)
		{
			perror("Read failed.");
			exit(1);
		}
		if(c == 4)
		{
			if(shell)
			{
				close(pipeto[0]);
				close(pipeto[1]);
				close(pipefrom[0]);
				close(pipefrom[1]);
		//		kill(process_id, SIGHUP);
			}
			exit(0);
		}
		if(c == 3)
		{
			if(shell)
			{
				close(pipeto[0]);
                                close(pipeto[1]);
                                close(pipefrom[0]);
                                close(pipefrom[1]);
			}
			kill(process_id, SIGINT);
			exit(0);
		}
		if((c == '\r'|| c == '\n'))
		{	
			char cr_lfmap[2] = {'\r','\n'};
			
			if(shell)
			{
				c = '\n';
				write(pipeto[1], cr_lfmap + 1, 1);
				c = ' ';
			}
			int success = write(1, cr_lfmap, 2);
			if(success < 0)
			{
				perror("<cr><lf> mapping failed.");
				exit(1);
				
			}

			continue;
			
		}
	
		
		write(1, buffer, 1);
		if(shell && !readshell)
		{
			write(pipeto[1], buffer, 1);
		}

		

	}
	
}



int main(int argc, char * argv[])
{
	int option = 0;
	buffer = (char*)malloc(1024*sizeof(char));

	static struct option long_opts[] =
	{
		{"shell", no_argument, 0, 's'}
	};

	while((option = getopt_long(argc,argv,"s",long_opts,NULL)) != -1)
	{
		switch(option)
		{
			case 's':
				signal(SIGINT, sig_handler);
				signal(SIGPIPE, sig_handler);
				shell = 1;
				break;
			default:
				exit(1);
				break;
		}
	}


	struct termios new_attr;
	tcgetattr(0,&saved_attr);
	atexit(restore);


	tcgetattr(0,&new_attr);
	new_attr.c_iflag = ISTRIP;
	new_attr.c_oflag = 0;
	new_attr.c_lflag = 0;

	int success = tcsetattr(0, TCSANOW, &new_attr);
	if(success < 0)
	{
		perror("Error setting attributes.");
	}

	if(shell)
	{
		if(pipe(pipeto) == -1 || pipe(pipefrom) == -1)
		{
			perror("Pipes could not be created");
			exit(1);
		}
		process_id = fork();
		if(process_id == -1)
		{
			perror("Fork error.");
			exit(1);
		}
		if(process_id == 0) /* child process */
		{
			close(pipeto[1]);
			close(pipefrom[0]);
			dup2(pipeto[0], 0);
			dup2(pipefrom[1], 1);
			dup2(pipefrom[1], 2);
			close(pipeto[0]);
			close(pipefrom[1]);
			char* const *args = NULL;
			int success = execvp("/bin/bash", args);
			if(success == -1)
			{
				perror("Could not execute. Exiting.");
				exit(1);
			}
			
		}
		else
		{
			close(pipeto[0]);
			close(pipefrom[1]);
		}

	}
	readwrite();

	exit(0);
}
