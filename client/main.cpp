#include "mainwindow.h"
#include <string>
#include <cstring>
#include <QApplication>

using namespace std;
struct Command {
    string label;
    int code;
    void (*func) ();
};
Command availableCommands[10];

void incBrightness() {

}

int main(int argc, char *argv[])
{
    availableCommands[0].label = "Marire luminozitate";
    availableCommands[0].code = 41;
    availableCommands[0].func = incBrightness;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
