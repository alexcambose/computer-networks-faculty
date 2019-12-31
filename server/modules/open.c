#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BROWSER "browser"
#define EXPLORER "explorer"
#define BRIGHTNESS_FILE "/sys/class/backlight/intel_backlight/brightness"


void main(int argc, char *argv[]) {
    if(!argv[1]) {
        printf("Usage: open <%s|%s> \n", BROWSER, EXPLORER);
        exit(1);
    }

    if(strcmp(argv[1], BROWSER) == 0) {
        int ret = execl ("/usr/bin/sensible-browser", "sensible-browser", "--no-sandbox", (char *)0);
    } else if (strcmp(argv[1], EXPLORER) == 0) {
       int ret = execl ("/usr/bin/xdg-open", "xdg-open", ".", (char *)0);
    } else {
        printf("Argument %s not recognized", argv[1]);
    } 
}