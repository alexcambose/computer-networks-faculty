#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"


void getCommands () {
    FILE *fp = fopen(CONFIG_FILE, "r");
    char buff[2505];
    int index = 0;

    while ( fgets ( buff, sizeof buff, fp ) != NULL ) {
        char *title = strtok(buff, "|");
        char *command = strtok(NULL, "|");
        char *commandArgs = strtok(NULL, "|");

        strcpy(commands[index].title, title);
        strcpy(commands[index].command, command);
        strcpy(commands[index].commandArgs, commandArgs);

        printf("%s\n",commands[index].commandArgs);
        index++;
    }
    fclose ( fp );
}