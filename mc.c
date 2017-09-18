//author: Spyridon Antonatos

//function..

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <string.h>
#include <ctype.h>

#include "mc.h"

#define max_number_commands 25
#define max_background_processes 50

int number_of_running_bgcommands;

int main(int argc,char* argv[]){
	int number_of_nCommands = 3;
	ppCommand commands = (ppCommand)malloc((max_number_commands)*sizeof(pCommand));
	commands[0] = (pCommand) NULL;
	commands[1] = (pCommand) NULL;
	commands[2] = (pCommand) NULL;

	ppbgCommand bgCommands = (ppbgCommand)malloc((max_background_processes)*sizeof(pbgCommand));
	int number_of_bgCommands = 0;
	


	int e = 0;
	while(!e){
		//printf("before start\n");
		e = start_commander(&number_of_nCommands,&commands[0],&bgCommands[0],&number_of_bgCommands);
	}
	//ATTENTION MAYBE THE SIZE WILL CHANGE
/*
	for(int v = 0;v < 10;v++){
		free(commands[v]);
		//free(bgCommands[v]);	
	}
*/
	free(commands);
	free(bgCommands);

	return 0;

}

int start_commander(int* number_of_nCommands,ppCommand pCommands,ppbgCommand pbgCommands,int* number_of_bgcommands){
	char* option = (char*)malloc(4*sizeof(char));
	char* number_commands[max_number_commands];
	char* letter_commands[5];
	int e;


	number_commands[0] = "\t0. whoami : Prints out the result of the whoamicommand\n";
	number_commands[1] = "\t1. last   : Prints out the result of the last command\n";
	number_commands[2] = "\t2. ls     : Prints out the result of a listing on a user-specified path\n";
	letter_commands[0] = "\ta. add command : Adds a new command to the menu (enter ' &' at the end to make it background e.g. sleep 5 &)\n";
	letter_commands[1] = "\tc. change directory : Change process working directory\n";
	letter_commands[2] = "\te. exit : Leave Mid-Day Commander\n";
	letter_commands[3] = "\tp. pwd : Prints working directory\n";
	letter_commands[4] = "\tr. r : Prints list of running background processes\n";

	printIntro(number_commands,letter_commands,*number_of_nCommands,&(*pCommands));

	option = askInput();
	if(option == NULL){
		e = 0;
	}else{
		e = takeAction(option,&(*pCommands),&(*number_of_nCommands),&(*pbgCommands),&(*number_of_bgcommands));
	}
	if(feof(stdin)){
		return 1;
	}
	//free(option);
	return e;

}

/*void printNew(pCommand commands[10]){
	//printf("3rd command cmd,options,prompt: @%s@@%s@@%s@\n",commands[3]->command,commands[3]->options,commands[3]->prompt); 
	command_added_command(commands[3]);
}*/

void printIntro(char** commands1,char** commands2,int number_of_ncommands,ppCommand pCommands){
	
	
	pCommand pCommand1;
	printf("\n\nG'Day, Commander! What commmand would you like to run?\n");
	for(int i = 0;i < 3;i++){
		printf("%s",commands1[i]);
	}
	if(number_of_ncommands > 3){
		//printf("%s\n",(*(pCommands+3))->prompt);
		for(int i = 3;i < number_of_ncommands ;i++){
			//printf("here n = %d\n",number_of_ncommands);
			pCommand1 = *(pCommands + i);
			printf("\t%d %s %s  : %s\n",pCommand1->number -1,pCommand1->command,pCommand1->options,pCommand1->prompt);
		}
	}
	for(int j = 0;j < 5;j++){
		printf("%s",commands2[j]);
	}


}

char* askInput(){

	int number;
	//char str[5];
	char* str = malloc(sizeof(char));
	printf("Option?: ");
	fgets(str,128,stdin);
	str = strtok(str," \n");
	printf("\n");
	return str;

}

int takeAction(char* x,ppCommand pCommands,int* number_of_nCommands,ppbgCommand pbgCommands,int* number_of_bgcommands){
//	printf("M%sM\n",x);
	int i;
	if(strcmp(x,"0") == 0){
		command_whoami(pbgCommands,number_of_bgcommands);
	}else if(strcmp(x,"1") == 0){
		command_last(pbgCommands,number_of_bgcommands);
	}else if(strcmp(x,"2") == 0){
		command_ls(pbgCommands,number_of_bgcommands);
	}else if(strcmp(x,"a") == 0){
		command_add(pCommands,number_of_nCommands);
//		printf("option2.5 %s \n",(*(pCommands + *number_of_nCommands - 1))->options);
	}else if(strcmp(x,"c") == 0){
		command_change_directory();
	}else if(strcmp(x,"e") == 0){
		i = command_exit(*number_of_bgcommands);
		return i;
	}else if(strcmp(x,"p") == 0){
		command_print_working_directory();
		//return 0;
	}else if(strcmp(x,"r") == 0){
		command_print_running_bgProcesses(pbgCommands,number_of_bgcommands);
	}else {
		//printf("here1\n");
		//printf("is digit %d\n",isdigit(x));
		int l = strlen(x);
		for(int k = 0;k < l;k++){
			if((!isdigit(x[k])) && (x[k] != ' ') ){
				printf("error, command does not exist\n");
				return 0;
		
			}	
		}
		x = strtok(x," ");
		//printf("@%s@ %d\n",x,*number_of_nCommands);
		//printf("here 1\n");
		if((atoi(x) < *number_of_nCommands) && (2 < atoi(x) <= *number_of_nCommands)){
			//printf("hre 2\n");
			//printf("take action cmmc a\n");
			//printf("nbgcm %d\n",*number_of_bgcommands);
			command_added_command(pCommands[atoi(x)],pbgCommands,number_of_bgcommands);
		}else{
			printf("error, command does not exist\n");
		}
	}
	//printf("take action\n");
	return 0;
}


void command_whoami(ppbgCommand pbgCommands,int *number_of_bgcommands){

	printf("-- Who I am --\n");

	struct timeval start,end;
	struct rusage* rusage = malloc(sizeof(struct rusage));

	int pid = fork();
	if(pid < 0){
		printf("error,forking unsuccesfull\n");
	}

	gettimeofday(&start,NULL);
	char* args[2] = {"whoami",NULL};
	if(pid == 0){
		execvp(args[0],args);
		// exit(EXIT_SUCCESS);
	}
	if(pid > 0){
		process_handler(0,pbgCommands,pid,number_of_bgcommands,&start,&end);
	}
	//wait(NULL);
	//gettimeofday(&end,NULL);
	//getrusage(RUSAGE_CHILDREN,rusage);
	//printStatistics(start.tv_usec,end.tv_usec,rusage);
	//printf("here1\n");
	//free(rusage);
	return;
}

void command_last(ppbgCommand pbgCommands,int* number_of_bgcommands){
	printf("-- Last Login --\n");
	struct timeval start,end;
	struct rusage* rusage = malloc(sizeof(struct rusage));
	char* args[2] = {"last" ,NULL};
	int pid = fork();
	if(pid < 0){
		printf("error, forking unsuccesfull\n");
	}

	gettimeofday(&start,NULL);
	if(pid == 0){
		execvp(args[0],args);
	}
	
	if(pid > 0){
		process_handler(0,pbgCommands,pid,number_of_bgcommands,&start,&end);
	}


	//free(rusage);
	return;
}

void command_ls(ppbgCommand pbgCommands,int* number_of_bgcommands){
	printf("-- Directory Listing --\n");
	struct timeval start,end;
	struct rusage* rusage = malloc(sizeof(struct rusage));

	char arguments[128];
	char path[128];

	char* option = (char*)malloc(10*sizeof(char));
	char* path1 = (char*)malloc(128*sizeof(char));
	printf("Enter the arguments: ");
	fgets(arguments,128,stdin);
	printf("\nEnter the path: ");
	fgets(path,128,stdin);
	char* args[32];
//	char** args = (char**)malloc(32*sizeof(char*)); 
	args[0] = "ls";

	option = strtok(arguments,"\n ");

	int char_count = 0;
	int c = 1;
	while(option != NULL){
		args[c] = option;
		//char_count = char_count + strlen(option);
		option = strtok(NULL,"\n ");
		//char_count = char_count + strlen(option);
		//if(c >= 29 || char_count >= 127){
		//	printf("you entered more than 32 distinct arguments or more than 128 characters in one line of input\n");
		//c++;
		//break;
		//} 

		c++;
	}

	path1 = strtok(path,"\n ");
	while(path1 != NULL){

		args[c] = path1;
		path1 = strtok(NULL,"\n ");
		c++;
	}
	args[c] = NULL;


	int pid = fork();
	
	if(pid < 0){
		printf("error,forking unsuccesful\n");
	}

	gettimeofday(&start,NULL);
	if(pid == 0){
		if(execvp(args[0],args) < 0) {
			fprintf(stderr, "Exec Failes\n");
		}
	}
	if(pid > 0){
		process_handler(0,pbgCommands,pid,number_of_bgcommands,&start,&end);
	}


	

	//free(option);
	//free(path1);
	//free(rusage);
	return;

}

void command_add(ppCommand pCommands,int* number_of_nCommands){
	char* command_text = (char*)malloc(128*sizeof(char));
	char* options_text = (char*)malloc(128*sizeof(char));

	pCommand pNewCommand = (pCommand)malloc(sizeof(command));
	pNewCommand->command = (char*)malloc(128*sizeof(char));
	pNewCommand->options = (char*)malloc(128*sizeof(char));

	printf("-- Add a command --\n");
	printf("Command to add?: ");
	fgets(command_text,32,stdin);
	//printf("\nOptions for the command?: ");
	//fgets(options_text,32,stdin);

	pNewCommand->number = *number_of_nCommands + 1;
	strcpy(pNewCommand->command,strtok(command_text,"\n"));

	pNewCommand->prompt = "user added command";


	*(pCommands + *number_of_nCommands) = pNewCommand;
	*number_of_nCommands = *number_of_nCommands + 1;
}

void command_added_command(pCommand pCommand,ppbgCommand pbgCommands,int* number_of_bgcommands){
	struct timeval start,end;
	struct rusage* rusage = malloc(sizeof(struct rusage));
	
	char* option = (char*)malloc(128*sizeof(char));
	char arguments[128];

	int is_background = 0;



	pbgCommand newpbgcommand;
		

	strcpy(arguments,pCommand->command);

	char* args[32];// = (char**)malloc(32*sizeof(char*));

	option = strtok(arguments,"\n ");

	//int char_count = 0;
	int c = 0;
	while((option != NULL) && (strcmp("&",option) != 0)){
		args[c] = option;
		//fprintf(stderr,"args[i],c: @%s@ %d\n", args[c],c);
		option = strtok(NULL,"\n ");

		//char_count = char_count + strlen(option);
		//if(c >= 29 || char_count >= 127){
		//	printf("you entered more than 32 distinct arguments or more than 128 characters in one line of input\n");
		//c++;
		//break;
		//} 
		c++;

	}
	args[c] = NULL;
	//printf("here51 option %s\n",option);
	if((option != NULL) && (strcmp("&",option) == 0)){
	
		newpbgcommand = (pbgCommand)malloc(sizeof(bgCommand));
		//newpbgcommand->number = *number_of_bgcommands;
	 	is_background = 1;
	}

	//printf("here53\n");
	int parent_pid = fork();
	//printf("bgcmd %d\n",*number_of_bgcommands);

	//printf("here45\n");
	if(parent_pid < 0){
		printf("error,forking unsuccesful\n");
	}
	
	if(parent_pid > 0){
		if(is_background){
			*number_of_bgcommands = *number_of_bgcommands + 1;
			number_of_running_bgcommands = number_of_running_bgcommands + 1;
			newpbgcommand->pid = parent_pid;
			newpbgcommand->is_running = 1;
			(*(pbgCommands + *number_of_bgcommands - 1)) = newpbgcommand;
			printf("[%d] %d\n",*number_of_bgcommands,newpbgcommand->pid);
		} 
	}

	gettimeofday(&start,NULL);
	if(parent_pid == 0){
		if(execvp(args[0],args) < 0) {
			fprintf(stderr, "Exec Failes\n");
		}
	}

	if(parent_pid > 0){
		//FOREGROUND AND BACKGROUND HANDLER
		process_handler(is_background,pbgCommands,parent_pid,number_of_bgcommands,&start,&end);

	}

	//free(option);
	//free(rusage);

	return;


}

void command_change_directory(){
	char new_directory[128];
	printf("-- Change directory --\n");
	printf("Enter new directory: ");
	fgets(new_directory,32,stdin);
	//char* args[32] = (char**)malloc(32*sizeof(char*));
	//char** args = (char**)malloc(32*sizeof(char*));
	char* s = (char*)malloc(128*sizeof(char));
	s = strtok(new_directory,"\n");
	chdir(s);
	/*for(int b = 0;b<32;b++){
		free(args[b]);
	}
	free(args);*/
	//free(s);
	
	return;
}

int command_exit(int number_of_bgcommands){
	if(number_of_running_bgcommands > 0){
		printf("There are background processes running, can't exit\n");
		return 0;
	}else{
		printf("Logging you out, commander\n");
		return 1;
	}

}

void command_print_working_directory(){
	int is_backround = 0;	
	
	printf("-- Print Current Directory --\n");
	char cwd[1024];
	getcwd(cwd,sizeof(cwd));
	printf("current directory: %s\n",cwd);
	return;
}

void command_print_running_bgProcesses(ppbgCommand pbgCommands,int* number_of_bgCommands){\
	printf("-- Current Running Background porcesses --\n");
	int c;
	pbgCommand cmd;
	for(c = 0;c < *number_of_bgCommands;c++){
		cmd = pbgCommands[c];
		if(cmd->is_running){
			printf("[%d] %d\n",c+1,cmd->pid);
		}
	}
	return;

}

void process_handler(int is_background,ppbgCommand pbgCommands,int child_pid,int* number_of_bgcommands,struct timeval* start,struct timeval* end){
		//int is_background = 0;
		int stay_loop = 1;
		int immediate_entry = 0;
		struct rusage* rusage = malloc(sizeof(struct rusage));

		//printf("pid of child %d:\n",child_pid);
		while(stay_loop){
			//printf("in outter loop\n");
			if(is_background || immediate_entry){
				int pid1 = wait3(NULL,WNOHANG,rusage);
				//printf("after 1stwait before 1st loop pid ppid %d %d\n",pid1,child_pid);


				while(pid1 && (pid1 != -1)){

					show_done_bgProcess(pbgCommands,pid1,number_of_bgcommands);
					getrusage(RUSAGE_CHILDREN,rusage);
					gettimeofday(end,NULL);
					printStatistics(start->tv_usec,end->tv_usec,rusage,rusage);
					pid1 = wait3(NULL,WNOHANG,rusage);
				}
				if(pid1 == -1){
					printf("error in getting pid\n");
				}else if(pid1 == 0){
					printf("\nno other processes have finished\n");
					
				}

				stay_loop = 0;
				immediate_entry = 0;
				return;
				//prompt command start_commander();
			
			
			}else{
				int pid = wait3(NULL,0,rusage);
				//printf("after 1stwait before 2nd loop pid ppid %d %d\n",pid,child_pid);

				while(((pid != child_pid) && (pid != 1))){
					//printf("in second loop\n");
					//printf("%d %d\n",child_pid,pid);
					show_done_bgProcess(pbgCommands,pid,number_of_bgcommands);
					getrusage(RUSAGE_CHILDREN,rusage);
					gettimeofday(end,NULL);
					printStatistics(start->tv_usec,end->tv_usec,rusage,rusage);
					pid = wait3(NULL,0,rusage);
				}
				//printf("after 2n loop,waited for parent pid\n");
				getrusage(RUSAGE_CHILDREN,rusage);
				gettimeofday(end,NULL);
				printStatistics(start->tv_usec,end->tv_usec,rusage,rusage);
				stay_loop = 1;
				immediate_entry = 1;

			
			}


		}



}
void show_done_bgProcess(ppbgCommand pbgCommands,int pid,int* number_of_bgcommands){
	int c;
	printf("-- Background process that ended --\n");
	//printf("n of bg,pid: %d %d\n",*number_of_bgcommands,pid);
	
	for(c = 0;c < *number_of_bgcommands;c++){
	//	printf("[%d] %d]",c+1,pbgCommands[c]->pid);
		if(pid == pbgCommands[c]->pid){
			printf("[%d] %d",c+1,pbgCommands[c]->pid);
			pbgCommands[c]->is_running = 0;	
			number_of_running_bgcommands = number_of_running_bgcommands -1; 
		}
	}


}

void printStatistics(suseconds_t start,suseconds_t end,struct rusage* rusage){
	//  printf("here2\n");
	printf("\n-- Statistics --\n");
	printf("time(miliseconds): %0.4f\n",(float)(end - start)/(float)1000);
	printf("page faults: %ld\n",rusage->ru_majflt);
	printf("page faults(reclaimed): %ld\n",rusage->ru_minflt);
}
