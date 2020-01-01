#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


int main() {
    char payload[100];
    struct CommandStruct commands[20];
    getCommands(commands);
    commandsInfoPayload(commands, payload);
    printf("%s",composeResponse(payload));
}