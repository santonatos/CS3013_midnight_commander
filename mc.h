//authos: Spyridon Antonatos

#ifndef phase_1_h
#define phase_1_h

typedef struct{
	int  number;
	char* command;
	char* options;
	char* prompt;
}command,*pCommand,**ppCommand;

typedef struct{
	int  pid;
	int is_running;
}bgCommand,*pbgCommand,**ppbgCommand;


//typedef (*)pCommand ppCommand;

int start_commander(int* number_of_nCommands,ppCommand pCommands,ppbgCommand pbgCommands,int* number_of_bgcommands);

void printIntro(char** commands1,char** commands2,int number_of_ncommads,ppCommand pCommands);
char* askInput();
int takeAction(char* x,ppCommand pCommands,int* number_of_nCommands,ppbgCommand pbgCommands,int* number_of_bgcommands);

void command_whoami(ppbgCommand pbgCommands,int*number_of_bgcommands);
void command_ls(ppbgCommand pbgCommands,int*number_of_bgcommands);
void command_last(ppbgCommand pbgCommands,int*number_of_bgcommands);
void command_add(ppCommand pCommands,int* number_of_nCommands);
void command_added_command(pCommand pCommand,ppbgCommand pbgCommands,int* number_of_bgcommands);
void command_change_directory();
int command_exit(int number_of_bgcommands);
void command_print_working_directory();
void command_print_running_bgProcesses(ppbgCommand pbgCommands,int* number_of_bgCommands);

void process_handler(int is_background,ppbgCommand pbgCommands,int pid,int* number_of_bgcommands,struct timeval* start,struct timeval* end);
void show_done_bgProcess(ppbgCommand pbgCommands,int pid,int* number_of_bgcommands);
void printStatistics();


#endif 
