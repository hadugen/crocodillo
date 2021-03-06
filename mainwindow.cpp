#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _msgBox = new QMessageBox(this);
    _connectionWindow = new ConnectionWindow(this);
    _secretWordsWindow = new SecretWordsWindow;
    _connectionWindow->setWindowModality(Qt::WindowModal);
    connect(_connectionWindow, SIGNAL(connectRequested(QString)), this, SLOT(slot_connectRequested(QString)));
    connect(_secretWordsWindow, SIGNAL(accepted(QString)), this, SLOT(secretWordChoosed(QString)));
    connect(ui->cgv, SIGNAL(signal_mousePressEvent(QPointF)), this, SLOT(graphicsMousePress(QPointF)));
    connect(ui->cgv, SIGNAL(signal_mouseReleaseEvent(QPointF)), this, SLOT(graphicsMouseRelease(QPointF)));
    connect(ui->cgv, SIGNAL(signal_mouseMoveEvent(QPointF)), this, SLOT(graphicsMouseMove(QPointF)));
    ui->centralWidget->setEnabled(false);
}

void MainWindow::secretWordChoosed(QString secretWord)
{
    this->secretWord = secretWord;
    Message msg = PresentationLayerProtocol::incapsulate("SecretWord", "%cp", secretWord.toUtf8().constData());
    _connection->write(msg);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _secretWordsWindow;
    _connection->stop();
}

void MainWindow::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty())
        return;

    Message msg = PresentationLayerProtocol::incapsulate("Message", "%cp%cp", _userName.toUtf8().constData(),
                                                         ui->lineEdit->text().toUtf8().constData());
    _connection->write(msg);
    ui->lineEdit->clear();
}

void MainWindow::messageAvail(QByteArray message)
{
    while(!message.isEmpty())
    {
        Message msg;
        unsigned int messageBeginsAt = message.indexOf(msg.head),
                     packetSize = message.left(messageBeginsAt).toInt() + messageBeginsAt;
        QByteArray packet = message.left(packetSize);
        parseMessage(PresentationLayerProtocol::decapsulate(packet));
        message.remove(0, packetSize);
    }
}

void MainWindow::parseMessage(Message message)
{
    if(message.definer == "Message")
    {
        QString text = message.data.split(',').at(1);
        QString userName = message.data.split(',').at(0);
        ui->textEdit->append(userName + " >> " + text);
        if(text == secretWord)
        {
            ui->textEdit->append(userName + " угадал");
            chooseGameMaster();
        }
        return;
    }

    if(message.definer == "SecretWord")
    {
        secretWord = message.data;
        ui->label_secretWord->setText(QString().fill('_', secretWord.size()));
        return;
    }

    if(message.definer == "Color")
    {
        ui->cgv->setColor(message.data.toUInt());
        return;
    }

    if(message.definer == "GameMaster")
    {
        bool isGameMaster = message.data.toInt();
        ui->cgv->setGameMasterMode(isGameMaster);
        if(isGameMaster)
        {
            _secretWordsWindow->show();
            ui->action_palette->setEnabled(true);
        }
        else
        {
            ui->action_palette->setEnabled(false);
        }
        return;
    }

    if(message.definer == "Move" && !ui->cgv->isGameMasterMode())
    {
        double x = message.data.split(',').at(0).toFloat();     //накапливаются пакеты, добавить парсер
        double y = message.data.split(',').at(1).toFloat();
        ui->cgv->gameMasterMouseMoveEvent(x, y);
        return;
    }

    if(message.definer == "Press" && !ui->cgv->isGameMasterMode())
    {
        double x = message.data.split(',').at(0).toFloat();
        double y = message.data.split(',').at(1).toFloat();
        ui->cgv->gameMasterMousePressEvent(x, y);
        return;
    }

    if(message.definer == "Release" && !ui->cgv->isGameMasterMode())
    {
        double x = message.data.split(',').at(0).toFloat();
        double y = message.data.split(',').at(1).toFloat();
        ui->cgv->gameMasterMouseReleaseEvent(x, y);
        return;
    }
}

void MainWindow::graphicsMouseMove(QPointF pos)
{
    Message msg = PresentationLayerProtocol::incapsulate("Move", "%d%d", pos.x(), pos.y());
    _connection->write(msg);
}

void MainWindow::graphicsMousePress(QPointF pos)
{
    Message msg = PresentationLayerProtocol::incapsulate("Press", "%d%d", pos.x(), pos.y());
    _connection->write(msg);
}

void MainWindow::graphicsMouseRelease(QPointF pos)
{
    Message msg = PresentationLayerProtocol::incapsulate("Release", "%d%d", pos.x(), pos.y());
    _connection->write(msg);
}

void MainWindow::on_action_startServer_triggered()
{
    _connection = new Server(this);

    if(_connection->start())
    {
        ui->action_connectToHost->setEnabled(false);
        ui->action_startGame->setEnabled(true);
        ui->textEdit->append("Сервер запущен, порт " + QString::number(_connection->port()));
        connect(_connection, SIGNAL(readyRead(QByteArray)), this, SLOT(messageAvail(QByteArray)));
        connect(dynamic_cast<Server*>(_connection), SIGNAL(clientConnected(QString)), this, SLOT(slot_clientConnected(QString)));
        connect(dynamic_cast<Server*>(_connection), SIGNAL(clientDisconnected(QString)), this, SLOT(slot_clientDisconnected(QString)));
        ui->centralWidget->setEnabled(true);
        ui->cgv->clear();
    }
    else
    {
        _msgBox->setText("err");
        _msgBox->show();
    }
}

void MainWindow::slot_connectRequested(QString hostInfo)
{
    _connection = new Client(this);
    connect(_connection, SIGNAL(readyRead(QByteArray)), this, SLOT(messageAvail(QByteArray)));
    connect(dynamic_cast<Client*>(_connection), SIGNAL(connectedToHost()), this, SLOT(slot_connectedToHost()));
    QStringList remoteHostInfo = hostInfo.split(':');
    QString ip = remoteHostInfo[0];
    QString port = remoteHostInfo[1];
    _connection->start(ip, port.toUInt());
    ui->centralWidget->setEnabled(true);
}

void MainWindow::slot_clientConnected(QString hostAddress)
{
    ui->textEdit->append(hostAddress + " подключен");
}

void MainWindow::slot_clientDisconnected(QString hostAddress)
{
    ui->textEdit->append(hostAddress + " отключился");
}

void MainWindow::slot_connectedToHost()
{
    ui->action_startGame->setEnabled(false);
    ui->action_startServer->setEnabled(false);
    _msgBox->setText("Успешно подключен");
    _msgBox->setWindowTitle("Статус подключения");
    _msgBox->show();
}

void MainWindow::on_action_connectToHost_triggered()
{
    _connectionWindow->show();
}

void MainWindow::setUserName(QString userName)
{
    this->_userName = userName;
    this->setWindowTitle(userName);
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

void MainWindow::chooseGameMaster()
{
    Server *server = dynamic_cast<Server*>(_connection);
    if(server)
    {
        Message msgToNewGameMaster = PresentationLayerProtocol::incapsulate("GameMaster", "%i", 1);
        Message msgToOldGameMaster = PresentationLayerProtocol::incapsulate("GameMaster", "%i", 0);
        unsigned int who = rand() % server->clientCount();
        server->write(lastGameMaster, msgToOldGameMaster);
        server->write(who, msgToNewGameMaster);
        lastGameMaster = who;
    }
}

void MainWindow::on_action_startGame_triggered()
{
    chooseGameMaster();
}

void MainWindow::on_action_palette_triggered()
{
    bool ok;
    QRgb color = QColorDialog::getRgba(0xFFFFFF, &ok);
    if(ok)
    {
        ui->cgv->setColor(color);
        Message msg = PresentationLayerProtocol::incapsulate("Color", "%ui", (unsigned int)color);
        _connection->write(msg);
    }
    else
        qDebug() << "cancel";
}
