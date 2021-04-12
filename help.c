#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*//this will separate any lines with a ;
char** parseLine(char* input)
{
    //this will be the parsed string
    char **command = malloc(8 * sizeof(char*));
    //separator
    char *semiColon = ";";
    //will contain the tokens after calling strtok()
    char *parsed;
    int ind = 0;

    //break the string into a series of tokens using the delimiter ";"
    //assign to command
    parsed = strtok(input, semiColon);
    while (parsed != NULL)
    {
        command[ind] = parsed;
        ind++;
        parsed = strtok(NULL, semiColon);
    }

    command[ind] = NULL;
    return command;
}

//this will parse a line with each space to separate each command
//this happens after parsing the line with any ;
char** parseSpaces(char** input)
{
    //this will be the parsed string
    char **command = malloc(8 * sizeof(char*));
    //separator
    char *space = " ";
    //will contain the tokens after calling strtok()
    char *parsed;
    int ind = 0;

    //break the string into a series of tokens using the delimiter " "
    //assign to command
    parsed = strtok(input, space);
    while (parsed != NULL)
    {
        command[ind] = parsed;
        ind++;
        parsed = strtok(NULL, space);
    }

    command[ind] = NULL;
    return command;
}*/

char** parseCommand(char* input)
{
    //this will be the parsed string
    char **command = malloc(8 * sizeof(char*));
    //separator
    char *space = " ";
    //will contain the tokens after calling strtok()
    char *parsed;
    int ind = 0;

    //break the string into a series of tokens using the delimiter " "
    //assign to command
    parsed = strtok(input, space);
    while (parsed != NULL)
    {
        command[ind] = parsed;
        ind++;
        parsed = strtok(NULL, space);
    }

    command[ind] = NULL;
    return command;
}


int main(/*int argc, char* argv[]*/)
{
    /*//parse command line arguments, if they exist
    //if there are arguments present...
    if (argc == 2)
    {
        //batch mode
        printf("BATCH MODE\n");

        //open file
        FILE *f = fopen(argv[1], "r");

        //if file cant be opened
        if (f == 0)
        {
            printf("Could not open file\n\n");
        }
        else
        {
            //this will be the parsed input string
            //may contain a line with multiple commands separated by ;
            char** parsedLine;

            //this will store all separated commmands
            //if parsedLine contains any lines with a ;
            //this will store each separate command
            char** parsedCommands;

            //reads each line
            char* line;

            //variable that ends the program
            int go = 1;

            pid_t pid;

            while (go != 0)
            {
                //read input
                //fscanf("%[^\n]%*c", line);


            }
        }
    }
    else if (argc > 2)
    {
        //error, too many arguments
        //exit program

    }
    else if (argc == 1)
    {
        //do nothing?
        int nothing = 0;
    }
    else
    {
        //do nothing again??
        int nothingAgain = 0;
    }*/
    
    
    //interactive mode happens with or without batch mode
    
    //input cstring;
    char *input;
    char quit[] = "quit";   //conditional string
    int in = 1;             //checks if input is "quit"

    int s;

    //this will be the parsed input string
    char **command;

    //pid var
    pid_t pid;
    
    //while input != "quit"
    while (in != 0)
    {
        //prompt and read command/arguments
        printf("minor3> ");
        scanf("%[^\n]%*c", input);

        //parse the input string
        command = parseCommand(input);

        //handle empty commands to avoid seg fault
        if (!command[0])
        {
            printf("No command entered\n");
            continue;
        } 

        //check if the input is "quit"
        in = strcmp(command[0], quit);
        //printf("%d\n", in);
        if (in == 0)
        {
            return 0;
        }

        //call fork()
        pid = fork();

        printf("pid: %d\n", pid);
        
        //child process
        if (pid == 0)
        {
            //call exec
            //printf("%s %s\n", command[0], *command);
            int n = 0;
            while (command[n] != NULL)
            {
                printf("Command[%d]: %s\n", n, command[n]);
                n++;
            }
            execvp(command[0], command);
            //the following lines wont execute if execvp doesnt fail
            //these handle errors
            //print a message
            printf("%s: command not found\n", command[0]);
            //exit child
            exit(1);           
        }
        //if parent process, wait for child to finish
        else
        {
            //waitpid(pid, &s, WUNTRACED);
            pid = wait(&s);
            //printf("next command incoming...\n");
        }
    }
    return 0;
}
