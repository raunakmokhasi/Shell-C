//Group - 21	Anjanay 2017021		Raunak 2017085		Meeha 2017300

#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/wait.h> 
#include <readline/readline.h>

int globalPID = -5;

int checkpipe(char* st){

	int i = 0;
	int flag = 0;
	while(i < strlen(st)){
		if(st[i] == 124){
			flag = 1;
			break;
		}
		i++;
	}
	return flag;

}

void execRed(char* p1[],int n){
	int flagoutputredirect = 0,flagoutappend=0;
	int flag12 = 0;

	char *filename;
	char* temp[1000];
	int j=0;
	for(int i = 0; i < n; i++){
		
		if(strcmp(p1[i],">")==0){
			flagoutputredirect = 1;
			filename = p1[i + 1];
			flag12 = 0;
			break;
		}
		else if(strcmp(p1[i],">>")==0){
			flagoutappend = 1;
			filename = p1[i + 1];
			flag12 = 0;
			break;	
		}
		else if((strcmp(p1[i],"1>")==0)){
			filename = p1[i + 1];
			close(1);
			strcat(filename,".txt");
			FILE *f1 = fopen(filename,"w");
			int ef1 = fileno(f1);
			dup2(ef1,1);
			flag12 = 1;
		}
		else if((strcmp(p1[i],"2>")==0)){
			filename = p1[i + 1];
			close(1);
			strcat(filename,".txt");
			FILE *f1 = fopen(filename,"w");
			int ef1 = fileno(f1);
			dup2(ef1,2);
			flag12 = 1;
		}
		else if((strcmp(p1[i],"<")==0)){
			flag12 = 0;
			filename = p1[i + 1];
			strcat(filename,".txt");
			FILE *f1 = fopen(filename,"r");
			int ef1 = fileno(f1);
			dup2(ef1,0);
		}
		else if((strcmp(p1[i],"2>&1")==0)){
			dup2(2,1);
			flag12 = 1;
		}
		else{
			temp[j]=p1[i];
			j++;
		}

	}
	if(flagoutputredirect){
		close(1);
		strcat(filename,".txt");
		FILE *f1=fopen(filename,"w");
		int ef1=fileno(f1);
		dup2(ef1,1);
	}
	if(flagoutappend){
		close(1);
		strcat(filename,".txt");
		
		FILE* f2=fopen(filename,"a");
		int ef2=fileno(f2);
		dup2(ef2,1);	

	}

	if(flag12 == 0){
		if(execvp(temp[0],temp) < 0){
			printf("INVALID Command\n");
		}
	}
	

}


void execute(char* p[],int n){
	int p1 = fork();
	globalPID = p1;
	if(p1 < 0){
		printf("Fork Error");
	}
	else if(p1 == 0){
		execRed(p,n);

		exit(1);
	}
	else{
		wait(NULL);
	}
	
}


void execRedPipe(char* p1[],int n){
	int flagoutputredirect = 0,flagoutappend=0;
	int flag12 = 0;

	char *filename;
	char* temp[1000];
	int j=0;
	for(int i = 0; i < n; i++){
		
		if(strcmp(p1[i],">")==0){
			flagoutputredirect = 1;
			filename = p1[i + 1];
			flag12 = 0;
			break;
		}
		else if(strcmp(p1[i],">>")==0){
			flagoutappend = 1;
			filename = p1[i + 1];
			flag12 = 0;
			break;	
		}
		else if((strcmp(p1[i],"1>")==0)){
			filename = p1[i + 1];
			close(1);
			strcat(filename,".txt");
			FILE *f1 = fopen(filename,"w");
			int ef1 = fileno(f1);
			dup2(ef1,1);
			flag12 = 1;
		}
		else if((strcmp(p1[i],"2>")==0)){
			filename = p1[i + 1];
			close(1);
			strcat(filename,".txt");
			FILE *f1 = fopen(filename,"w");
			int ef1 = fileno(f1);
			dup2(ef1,2);
			flag12 = 1;
		}
		else if((strcmp(p1[i],"<")==0)){
			flag12 = 0;
			filename = p1[i + 1];
			strcat(filename,".txt");
			FILE *f1 = fopen(filename,"r");
			int ef1 = fileno(f1);
			dup2(ef1,0);
		}
		else if((strcmp(p1[i],"2>&1")==0)){
			dup2(2,1);
			flag12 = 1;
		}
		else{
			temp[j]=p1[i];
			j++;
		}

	}
	if(flagoutputredirect){
		close(1);
		strcat(filename,".txt");
		FILE *f1=fopen(filename,"w");
		int ef1=fileno(f1);
		dup2(ef1,1);
	}
	if(flagoutappend){
		close(1);
		strcat(filename,".txt");
		
		FILE* f2=fopen(filename,"a");
		int ef2=fileno(f2);
		dup2(ef2,1);	

	}

	if(flag12 == 0){
		if(execvp(temp[0],temp) < 0){
			printf("INVALID Command\n");
		}
	}
	
}

void executepipe(char* p1[],char* p2[], int n1, int n2){
	int pid;
	int fd[2];
	int ret;
	ret = pipe (fd);
	if (ret == -1) {
		printf ("Unable to create Pipe\n");
		return ;
	}
	pid = fork ();
	globalPID = pid;
	if (pid == 0) {

		close(0);
		close(fd[0]);
		dup2(fd[1],1);

		execRedPipe(p1,n1);

		printf ("Failed to exec");
	}
	else if (pid > 0) {				

		wait(NULL);
		close(fd[1]);
		dup2(fd[0],0);
		
		execRedPipe(p2,n2);

		printf ("Failed to exec\n");
	}
	else {
		printf ("Unable to Fork\n");
	}

}

int parse_space(char* t,char* p[]){
	char* token = strtok(t," ");
	p[0] = token;
	int c = 1;
	int n=0;
	while(1){
		token = strtok(NULL," ");
		n++;
		if(token == NULL||c == 999) 
			break;
		p[c]=token;
		c++;
	}
	token=NULL;
	return n;
}

int parse_pipe(char* t,char* p[]){
	char* token=strtok(t,"|");
	p[0]=token;
	int c = 1,n=0;
	while(1){
		token=strtok(NULL,"|");
		if(token == NULL||c == 999)
		 break;
		p[c]=token;
		c+=1;
		n++;
	}
	return n;
}

void parsewr(char *p1,char *p2,char* s1[],char* s2[],int n){
	int n1=parse_space(p1,s1);
	int n2=parse_space(p2,s2);
	char *s3[2];
	s3[1] = s1[1];
	s3[0] = s1[0];
	
	executepipe(s3,s2,n1,n2);
}

void exit_handler(int signal){
	if(globalPID != -5){
		kill(globalPID, SIGINT);
	}
}

int main(int argc, char *argv[]){
	signal(SIGINT,exit_handler);
	while(1){
		char *parsepipe[1000];
		char *t;
		char baseUser[9999];
		char machine[200];
		char user[200];
		char dir[200];
		strcpy(baseUser, "");			//To print the Hostname and Login Details
		getlogin_r(machine, 1000);
		strcat(baseUser, machine);
		strcat(baseUser, "@");
		gethostname(user,1000);
		strcat(baseUser, user);
		strcat(baseUser, ":~/");
		getcwd(dir,50);
		strcat(baseUser, dir);
		strcat(baseUser, "$ ");
		printf("%s", baseUser);

		t = malloc(255);
		t = readline("\n");
		if(strcmp(t,"exit") == 0){
			return 0;
		}
		int k = 0,j = 0;
		
		int a = checkpipe(t);
		int n = parse_pipe(t,parsepipe);
		
		if(a){
			char* strip1[2];
			char* strip2[2];
			parsewr(parsepipe[0],parsepipe[1],strip1,strip2,n);
		}
		else{
			char *p[100];
			int n=parse_space(parsepipe[0],p);
			execute(p,n);
		}
		
	}

	return 0;

}