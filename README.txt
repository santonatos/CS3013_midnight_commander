How to run
------------------------
run 'make' to make the executable ./midnight_commander  . or 'make clean' to clean the directory from the executable



Program Function
----------------------------

The user can run one of the existing commands or add his own.
The user can add a command in the list of commands of the midinight commander program that will either run in the background or in the foregound. If the user wishes the new command to be a background command he should enter '&' (without the quotes) after the enters the new command at the prompt. E.g : @ubuntu: new_command: sleep 5 &   ;; instead of 'sleep 5'
When he chooses to runa background command, while the command is being executed in the background he can choose for a another command at the same time.
Thats is a fine demonstration of forking processes and the use of pids.



Brief program explanation/summary
-------------------------------
 
My commander runs a loop, in the main function. Inside the while loop, there is one function call:
start_commander() that takes a few arguments and returns an integer (determing to exit the loop and therefore the commander)

my start_commander() function runs three function calls:
printIntro()
askInput()
takeAction()

My takeAction() function takes the input from askInput and runs the given command. 
I have seperate functions for every command listed, except for the ones that the user added.
Those are called from one function, which takes as argument an element from the array of added functions

I have a seperate processhandling function to handle background and foreground processes which is called by the functions that fork, namely the 'number function' from 0 to X (x the number of the last command the user added)





Data structures and types I use to keep track of things
------------------------------------------------



I use a struct create a command(I only use a field of the struct,the rest I am not using although they are defined)
I use a strcut to create a background process too (often seen in my code as bgcommand).
My background process struct has two integer fields, one for its pid and one for whether it is running
or not(0 or 1)
I have 3 integers,one for my number of number commands (0,1,2 commands plus the user added commads)
one integer for the total number of background process and one for the number of running background processes.

I use an array for my user added commands and an array for my background processes
When I need to print the running background processes, I go through the array and if the is_running
integer is 1, it prints it.
If I want to end a background process, I decrease the number of running commands and I set its is_running flag to 0.

I use an array of pointers to my user-added command structs.

I DON'T USE GLOBAL VARIABLES for my arrays and for my 2 integers. Instead I am passing them 'by reference' in the functions that I need, by passing a pointer to them. I only use one global variable, an integer for my number of running background commands, although I could do the same for that number as i did for the above.

MAX CAPACITIES
-------------------------------------------------

i have defines as macros the max number of added commands and background porcesses someone can do.
Currently it is 25 number commands(including the first three 0,1,2) and 50 for background processes.
Each line reads up to 128 characters.





