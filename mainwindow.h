#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QMessageBox>
#include <QColorDialog>
#include <QRgb>
#include "customgraphicsview.h"
#include "secretwordswindow.h"
#include "server.h"
#include "client.h"
#include "connectionwindow.h"
#include "presentationlayerprotocol.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

signals:

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void graphicsMouseMove(QPointF pos);
    void graphicsMousePress(QPointF pos);
    void graphicsMouseRelease(QPointF pos);
    void messageAvail(QByteArray message);
    void on_action_startServer_triggered();
    void on_action_connectToHost_triggered();
    void on_lineEdit_returnPressed();
    void setUserName(QString userName);
    void slot_connectRequested(QString HostInfo);
    void slot_clientConnected(QString hostAddress);
    void on_action_startGame_triggered();
    void slot_connectedToHost();
    void slot_clientDisconnected(QString hostAddress);
    void on_action_palette_triggered();
    void secretWordChoosed(QString secretWord);

private:
    Ui::MainWindow *ui;
    AbstractConnection *_connection;
    QMessageBox *_msgBox;
    SecretWordsWindow *_secretWordsWindow;
    QString _userName, secretWord;
    ConnectionWindow *_connectionWindow;
    unsigned int lastGameMaster;

    void parseMessage(Message message);
    void chooseGameMaster();
};

#endif // MAINWINDOW_H
