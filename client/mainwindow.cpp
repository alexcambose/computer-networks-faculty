#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("IoT Simulator");
    msgBox.setInformativeText("Client with an attractive graphical interface that will play the role of remote (remote control) for more 'smart devices'.\n\nContributors: Alexandru Cambose\n\nRepository: github.com/alexcambose/tema-retele-s11\n\n\n ©Alexandru Cambose 2020");
    msgBox.setWindowTitle("About me");
    msgBox.setStyleSheet("QLabel{min-width: 700px;}");
    msgBox.exec();
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_ipaAddressInput_textChanged(const QString &arg1)
{
    cout << "asd";
}

void MainWindow::on_connectButton_clicked()
{

}
