#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"

int commandsNum = 0;

void getCommands (struct CommandStruct *commands) {

    FILE *fp = fopen(CONFIG_FILE, "r");
    char buff[2505];
    

    while ( fgets ( buff, sizeof buff, fp ) != NULL ) {
        char *title = strtok(buff, "|");
        char *command = strtok(NULL, "|");
        char *commandArgs = strtok(NULL, "|");

        strcpy(commands[commandsNum].title, title);
        strcpy(commands[commandsNum].command, command);
        strcpy(commands[commandsNum].commandArgs, commandArgs);
        commandsNum++;
    }
    fclose (fp);
}

void commandsInfoPayload(struct CommandStruct *commands, char *payload) {
    
    for(int i = 0; i < commandsNum; i++) {
        strcat(payload, commands[i].title);
        strcat(payload, "|");

        char code[100];
        sprintf(code,"%d", i);

        strcat(payload, code);
        strcat(payload, "\n");
    }
}

char *composeResponse(char data[]) {
    strcat(data, "END");
    return data;
}