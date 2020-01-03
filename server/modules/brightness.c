#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INCREMENT "inc"
#define DECREMENT "dec"
#define BRIGHTNESS_FILE "/sys/class/backlight/intel_backlight/brightness"


void main(int argc, char *argv[]) {
    if(!argv[1]) {
        printf("Usage: brightnes <%s|%s> \n", INCREMENT, DECREMENT);
        exit(1);
    }
    FILE *fp = fopen(BRIGHTNESS_FILE, "r");
    char buff[255];
    fscanf(fp, "%s", buff);
    int currentBrightness = atoi(buff);
    printf("Current brightness is %d\n", currentBrightness);

    if(strcmp(argv[1], INCREMENT) == 0) {
        currentBrightness+=100;
    } else if (strcmp(argv[1], DECREMENT) == 0) {
        currentBrightness-=100;
    } else {
        printf("Argument %s not recognized", argv[1]);
    }
    sprintf(buff, "%d", currentBrightness);

    printf("New brightness is %d\n", currentBrightness);
    FILE *fpr = fopen(BRIGHTNESS_FILE, "w");
    fputs(buff, fpr);
    fclose(fp);
}