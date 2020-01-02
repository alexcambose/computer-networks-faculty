#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <string>
#include <cstring>
#include <QMainWindow>
#include <QModelIndexList>
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateList();
private slots:
    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_ipaAddressInput_textChanged(const QString &arg1);

    void on_connectButton_clicked();

    void on_initiateAction_clicked();

    void on_disconnectButton_clicked();

    void on_actionsListView_clicked(const QModelIndex &index);

    void closeEvent(QCloseEvent *event);

    void on_ipListView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
};
struct Command {
    char title[100];
    int code;
};
struct ConnectedClients {
    Command commands[20];
    char address[100];
    int port;
    int socket;
    bool isSelected;
    int commandsNum;
};
#endif // MAINWINDOW_H
