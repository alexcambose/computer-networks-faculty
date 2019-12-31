#ifndef UTILS_H 
#define UTILS_H

#define CONFIG_FILE "./config.txt"

struct CommandStruct {
    char title[255];
    int code;
    char command[255];
    char commandArgs[255];
} commands[100];
 
void printHello();

#endif