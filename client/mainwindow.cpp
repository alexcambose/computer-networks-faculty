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
    this->ui->initiateAction->setDisabled(false);
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
    struct sockaddr_in serverAddr;

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
    printf("[+]Client Socket is created.\n");

   // memset(&clientSocket, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(4444);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int ret = ::connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        printf("[-]Error in connection. %d\n", clientSocket);
        exit(1);
    }
    printf("[+]Connected to Server.\n");
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

    clientsNum++;
    QStringList leftList, rightList;
    char b[100];
    for(int i = 0; i < clientsNum; i++) {
        for(int j = 0; j < clients[i].commandsNum; j++) {
            rightList << clients[i].commands[j].title;
        }
        strcpy(b, clients[i].address);
        strcat(b, ":");
        char port[100];
        sprintf(port,"%d", clients[i].port);
        strcat(b, port);
        leftList << b;
    }
    QStringListModel *model = new QStringListModel();
    model->setStringList(leftList);
    this->ui->ipListView->setModel(model);
    QStringListModel *model1 = new QStringListModel();
    model1->setStringList(rightList);
    this->ui->actionsListView->setModel(model1);
    // empty inputs
    this->ui->ipaAddressInput->setText("");
    this->ui->portInput->setText("");
    this->ui->connectButton->setDisabled(true);
}


void MainWindow::on_initiateAction_clicked()
{
    int currentSocketIndex =  this->ui->ipListView->currentIndex().row();
    int currentActionIndex =  this->ui->actionsListView->currentIndex().row();
    if(currentSocketIndex != -1 && currentActionIndex != -1) {
        char payload[100];
        bzero(payload,100);
        sprintf(payload,"%d", clients[currentSocketIndex].commands[currentActionIndex].code);
        strcat(payload, "|END ");
        send( clients[currentSocketIndex].socket, payload, strlen(payload), 0);
        qDebug() <<  currentSocketIndex << currentActionIndex;
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    int currentSocketIndex =  this->ui->ipListView->currentIndex().row();
    if(currentSocketIndex != -1) {
        char payload[100];
        strcpy(payload, "DISCONNECT");
        send( clients[currentSocketIndex].socket, "DISCONNECT", strlen("DISCONNECT"), 0);
        ::close(clients[currentSocketIndex].socket);
    }
}
