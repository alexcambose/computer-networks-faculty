#include "mainwindow.h"
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <QApplication>
#include <QDebug>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
