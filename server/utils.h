#ifndef UTILS_H 
#define UTILS_H

#define CONFIG_FILE "./config.txt"

struct CommandStruct {
    char title[255];
    int code;
    char command[255];
    char commandArgs[255];
};

int getCommands(struct CommandStruct commands[20]);

void commandsInfoPayload(struct CommandStruct commands[20], char *payload);
char *composeResponse(char data[]);
void executePaylaod(char buffer[]);
#endif