#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStringListModel>
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
    QStringListModel *model = new QStringListModel();
    QStringList list;
    list << "127.0.0.1";
    model->setStringList(list);

    this->ui->ipListView->setModel(model);

    QStringListModel *model1 = new QStringListModel();
    QStringList list1;
    list1 << "Marirre luminozitate" << "Micsorare luminozitate" << "Oprire ecran";
    model1->setStringList(list1);
    this->ui->actionsListView->setModel(model1);
    this->ui->initiateAction->setDisabled(false);
    this->ui->connectButton->setDisabled(false);
}

void MainWindow::on_connectButton_clicked()
{

}
