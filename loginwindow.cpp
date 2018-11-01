#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    msg = new QMessageBox(this);
    mainWindow = new MainWindow();
    connect(this, SIGNAL(userNameValid(QString)), mainWindow, SLOT(setUserName(QString)));
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_pushButton_clicked()
{
    if(ui->lineEdit_username->text().isEmpty())
    {
        msg->setWindowTitle("dolboeb?");
        msg->setText("Кого ты там сломать пытаешься, ало");
        msg->show();
        return;
    }

    emit userNameValid(ui->lineEdit_username->text());
    mainWindow->show();
    this->close();
}
