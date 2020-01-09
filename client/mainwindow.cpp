#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QStringListModel>
#include <QDebug>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <QCloseEvent>

using namespace std;
ConnectedClients clients[20];

int clientsNum=0;

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
    this->ui->connectButton->setDisabled(false);
}
void refreshList(Ui::MainWindow *ui) {

}
void parseInitialPayload(int clientIndex, char *payload) {
    std::istringstream stream(payload);
    std::string line;
    char buff[200], title[100];
    int code, commandsIndex = 0;
    while(std::getline(stream, line)) {
        if(line == "END") break;
        strcpy(buff, line.c_str());
        strtok(buff, "|");
        strcpy(title, buff);
        char *a = strtok(NULL, "|");

        code = atoi(a);
        qDebug() << a << code;

        clients[clientIndex].commands[commandsIndex].code = code;
        strcpy(clients[clientIndex].commands[commandsIndex].title, title);
        commandsIndex++;
    }
    commandsIndex++;
    clients[clientIndex].commandsNum = commandsIndex-1;
}

void MainWindow::on_connectButton_clicked()
{
    char address[200], port[200];
    strcpy(address, this->ui->ipaAddressInput->text().toUtf8().constData());
    strcpy(port, this->ui->portInput->text().toUtf8().constData());

    for(int i = 0; i < clientsNum; i++) {
        if(strcmp(clients[i].address, address) == 0 && clients[i].port == atoi(port)) {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","You are already connected !");
            messageBox.setFixedSize(500,200);
            return;
        }
    }
    struct sockaddr_in serverAddr;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Client Socket is created.\n");

   // memset(&clientSocket, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(port));
    serverAddr.sin_addr.s_addr = inet_addr(address);

    int ret = ::connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("[-]Error in connection. %d\n", clientSocket);
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Cannnot connect to server !");
        messageBox.setFixedSize(500,200);
        return;
    }
    qDebug() << "Connected to server" << address << ":" << port;
    char buffer[1024];
    bzero (buffer, 1024);
    if(recv(clientSocket, buffer, 1024, 0) < 0){
        printf("[-]Error in receiving data.\n");
    }else{
        QMessageBox msgBox;
        msgBox.setText("IoT Device added successfully!");
        msgBox.exec();
        parseInitialPayload(clientsNum, buffer);
        printf("Server: \t%s\n", buffer);
    }

    clients[clientsNum].socket = clientSocket;
    strcpy(clients[clientsNum].address, address);
    clients[clientsNum].port = atoi(port);
    for(int i = 0; i < clientsNum; i++) {
        clients[i].isSelected = 0;
    }
    clients[clientsNum].isSelected = 1;

    clientsNum++;
    MainWindow::updateList();
    // empty inputs
    this->ui->ipaAddressInput->setText("");
    this->ui->portInput->setText("");
    this->ui->connectButton->setDisabled(true);
}


void MainWindow::on_initiateAction_clicked()
{
    int currentSocketIndex = 0;
    for (int i = 0; i < clientsNum; ++i) {
        if(clients[i].isSelected) {
            currentSocketIndex = i;
        }
    }
    int currentActionIndex =  this->ui->actionsListView->currentIndex().row();
    qDebug() << "Trying to execute from " << currentSocketIndex << "" << currentActionIndex;
    if(currentSocketIndex != -1 && currentActionIndex != -1) {
        char payload[100];
        bzero(payload,100);
        sprintf(payload,"%d", clients[currentSocketIndex].commands[currentActionIndex].code);
        strcat(payload, "|END ");
        int res = send( clients[currentSocketIndex].socket, payload, strlen(payload), 0);
        if(res < 0) {
            QMessageBox messageBox;
            messageBox.critical(0,"Error","Cannnot send command to server !");
            messageBox.setFixedSize(500,200);
        }
        qDebug() << "Executing command code" << clients[currentSocketIndex].commands[currentActionIndex].code << "on" <<  clients[currentSocketIndex].address << ":" <<  clients[currentSocketIndex].port << "RESULT CODE:" << res << "\n";
    }
}

void disconnectFromServer(int currentSocketIndex) {
    send(clients[currentSocketIndex].socket, "DISCONNECT", strlen("DISCONNECT"), 0);
    ::close(clients[currentSocketIndex].socket);
    qDebug() << "Disconnected from" << clients[currentSocketIndex].address << clients[currentSocketIndex].port << "\n";
    // remove from array
    for (int i = currentSocketIndex; i < clientsNum; ++i)
        clients[i] = clients[i + 1];
    clientsNum--;
}
void disconnectFromServer() {
    for (int i = 0; i < clientsNum; ++i)
        if(clients[i].isSelected) disconnectFromServer(i);
}
void MainWindow::on_disconnectButton_clicked()
{
    disconnectFromServer();
    MainWindow::updateList();
}

void MainWindow::updateList() {
    QStringList leftList, rightList;
    char b[100];
    for(int i = 0; i < clientsNum; i++) {
        if(clients[i].isSelected) {
            for(int j = 0; j < clients[i].commandsNum; j++) {
                rightList << clients[i].commands[j].title;
            }
        }
        strcpy(b, clients[i].address);
        strcat(b, ":");
        char port[100];
        sprintf(port,"%d", clients[i].port);
        strcat(b, port);
        if(clients[i].isSelected) {
            strcat(b, "*");
        }
        leftList << b;
    }
    QStringListModel *model = new QStringListModel();
    model->setStringList(leftList);
    this->ui->ipListView->setModel(model);
    QStringListModel *model1 = new QStringListModel();
    model1->setStringList(rightList);
    this->ui->actionsListView->setModel(model1);
    if(clientsNum > 0) {
        this->ui->initiateAction->setDisabled(false);
        this->ui->connectButton->setDisabled(false);
        this->ui->disconnectButton->setDisabled(false);
    } else {
        this->ui->connectButton->setDisabled(true);
        this->ui->initiateAction->setDisabled(true);
        this->ui->disconnectButton->setDisabled(true);
    }
}


void MainWindow::closeEvent (QCloseEvent *event){
//        QMessageBox::StandardButton resBtn = QMessageBox::question( this, "APP_NAME",
//                                                                    tr("Are you sure?\nThis action will close all the opened connections!"),
//                                                                    QMessageBox::No | QMessageBox::Yes,
//                                                                    QMessageBox::No);
//        if (resBtn != QMessageBox::Yes) {
//            event->ignore();
//        } else {
            event->accept();
            for(int i = 0; i <= clientsNum; i++) {
                disconnectFromServer(0);
            }
//        }
}


void MainWindow::on_actionsListView_clicked(const QModelIndex &index)
{
   this->ui->initiateAction->setEnabled(true);
}

void MainWindow::on_ipListView_clicked(const QModelIndex &index)
{
    this->ui->ipListView->setCurrentIndex(index);
    int currentSocketIndex =  this->ui->ipListView->currentIndex().row();
    for(int i = 0; i < clientsNum; i++) {
        clients[i].isSelected = 0;
    }
    clients[currentSocketIndex].isSelected = 1;
    MainWindow::updateList();
    qDebug() << "Selected" << clients[currentSocketIndex].address << " " << clients[currentSocketIndex].port;

}


