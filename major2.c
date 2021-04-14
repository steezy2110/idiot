

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

//swaps array elements
//used if "exit" is entered, moves exit to the end so it is executed last
void swapElements(char** arr, int i, int j)
{
    char* temp;
    temp = strdup(arr[i]);
    arr[i] = strdup(arr[j]);
    arr[j] = strdup(temp);
}

//this will return the number of spaces in a string
//useful when executing the parsed commands
int getNumSpaces(char* input)
{
    if (input == NULL)
    {
        return -1;
    }
    int spaces = 0;
    char* space = " ";
    for (int i = 0; i < strlen(input); i++)
    {
        if (input[i] == ' ')
        {
            spaces++;
        }
    }
    return spaces;
}

//this will separate any lines with a ;
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
    while (input[ind] != NULL)
    {
        ind++;
    }

    int n = 0;
    for (int i = 0; i < ind; i++)
    {
        parsed = strtok(input[i], space);
        while (parsed != NULL)
        {
            command[n] = parsed;
            n++;
            parsed = strtok(NULL, space);
        }
    }

    command[n] = NULL;
    return command;
}
    
int main(int argc, char* argv[])
{
    int go = 1;
    printf("go value changed\n");

    //parse command line arguments, if they exist
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
            //reads the line;
            char input[515];

            int s;

            //parsed string
            char** first;

            //separated commands
            char** command;

            pid_t pid;

            //read until the end of the file is reached
            while (fgets(input, 515, f))
            {
                //get rid of the newline character bc it messes up the input and the command wont execute
                char *ptr = strchr(input, '\n');
                if (ptr)
                {
                    *ptr = '\0';
                }
                //parse the strings
                first = parseLine(input);
                //have to make a copy of first because parseSpaces modifies first
                char** firstCopy;
                int firstSize = 0;
                while (first[firstSize] != NULL)
                {
                    firstSize++;
                }
                firstCopy = malloc(firstSize * sizeof(*firstCopy));
                for (int i = 0; i < firstSize; i++)
                {
                    firstCopy[i] = strdup(first[i]);
                }

                //firstCopy[firstSize] = NULL;
                printf("first size = %d\n", firstSize);

                //test
                int n = 0;
                int spaces;
                while (first[n] != NULL)
                {
                    printf("First[%d]: %s\n", n, first[n]);
                    printf("Spaces in First[%d]: %d\n", n, getNumSpaces(first[n]));
                    printf("firstCopy[%d]: %s\n", n, firstCopy[n]);
                    n++;
                }

                command = parseSpaces(first);

                if (!command[0])
                {
                    printf("No command entered\n");
                    continue;
                }

                int o = 0;
                while (command[o] != NULL)
                {
                    printf("Command[%d]: %s\n", o, command[o]);
                    o++;
                }

                printf("\n\n\n");

                pid = fork();
                //child process

                //stores each command, resets every iteration
                char** comm = malloc(firstSize * sizeof(*firstCopy));
                
                if (pid == 0)
                {
                    int ind = 0;
                    for (int i = 0; i < firstSize; i++)
                    {
                        if (firstCopy[i] == NULL)
                        {
                            break;
                        }
                        int spaces = getNumSpaces(firstCopy[i]);
                        int j = 0;
                        for(int j = 0; j < spaces + 1; j++)
                        {
                            comm[j] = strdup(command[ind]);
                            printf("comm[%d] = %s\n", j, comm[j]);
                            ind++;
                        }
                        //execute the command
                        execvp(comm[0], comm);
                        //the following lines wont execute if execvp fails
                        printf("%s: command not found\n", comm[0]);
                    }
                }
                //parent process
                else
                {
                    pid = wait(&s);
                }
                
                free(first);
                free(firstCopy);
                free(comm);
            }
        }
    }
    else if (argc > 2)
    {
        //error, too many arguments
        //exit program

    }
    
    //interactive mode happens with or without batch mode

    //reads the line;
    char input[515];

    int s;

    //parsed string
    char** first;

    //separated commands
    char** command;

    pid_t pid;

    //ask for prompt:
    printf("Enter a custom prompt: ");
    char prompt[15];
    scanf("%[^\n]%*c", prompt);

    while (go == 1)
    {
        //prompt for input
        printf("%s> ", prompt);
        scanf("%[^\n]%*c", input);

        //parse the strings
        first = parseLine(input);
        //have to make a copy of first because parseSpaces modifies first

        //check if "exit" was entered
        int exitCheck = 0;
        //get the size of the array
        int firstSize = 0;
        while (first[firstSize] != NULL)
        {
            firstSize++;
        }
        while (first[exitCheck] != NULL)
        {
            if (strcmp(first[exitCheck], "exit") == 0)
            {
                swapElements(first, exitCheck, firstSize - 1);
            }
            exitCheck++;
        }

        char** firstCopy;
        firstCopy = malloc(firstSize * sizeof(*firstCopy));
        for (int i = 0; i < firstSize; i++)
        {
            firstCopy[i] = strdup(first[i]);
        }

        //firstCopy[firstSize] = NULL;
        printf("first size = %d\n", firstSize);

        //test
        int n = 0;
        int spaces;
        while (first[n] != NULL)
        {
            printf("First[%d]: %s\n", n, first[n]);
            printf("Spaces in First[%d]: %d\n", n, getNumSpaces(first[n]));
            printf("firstCopy[%d]: %s\n", n, firstCopy[n]);
            n++;
        }

        command = parseSpaces(first);

        if (!command[0])
        {
            printf("No command entered\n");
            continue;
        }

        int o = 0;
        while (command[o] != NULL)
        {
            printf("Command[%d]: %s\n", o, command[o]);
            o++;
        }

        printf("\n\n\n");

        pid = fork();
        //child process

        //stores each command, resets every iteration
        char** comm = malloc(firstSize * sizeof(*firstCopy));
        
        if (pid == 0)
        {
            int ind = 0;
            for (int i = 0; i < firstSize; i++)
            {
                if (firstCopy[i] == NULL)
                {
                    break;
                }
                int spaces = getNumSpaces(firstCopy[i]);
                int j = 0;
                for(int j = 0; j < spaces + 1; j++)
                {
                    comm[j] = strdup(command[ind]);
                    printf("comm[%d] = %s\n", j, comm[j]);
                    ind++;
                    if (j == spaces)
                    {
                        if (strcmp(comm[0], "cd") == 0)
                        {
                            //cd code
                            //processcdCommand(comm[1])
                        }
                        if (strcmp(comm[0], "exit") == 0)
                        {
                            printf("exit entered, goodbye\n");
                            go = 0;
                            printf("go = %d, break\n", go);
                            //break;
                        }
                        else
                        {
                            //execute the commands
                            execvp(comm[0], comm);
                            //the following lines wont execute if execvp fails
                            printf("%s: command not found\n", comm[0]);
                            exit(1);
                        }
                    }
                }
            }
        }
        //parent process
        else
        {
            wait(&s);
        }
        
        free(first);
        free(firstCopy);
        free(comm);

        printf("before we go...\n");

        printf("go = %d\n", go);
        if (go == 0)
        {
            break;
        }
    }
    printf("out of while loop\n");
    return 0;
    
}
