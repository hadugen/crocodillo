#include "connectionwindow.h"
#include "ui_connectionwindow.h"

ConnectionWindow::ConnectionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionWindow)
{
    ui->setupUi(this);
}

ConnectionWindow::~ConnectionWindow()
{
    delete ui;
}

void ConnectionWindow::on_buttonBox_accepted()
{
    emit connectRequested(ui->lineEdit_ip->text() + ":" + ui->lineEdit_port->text());
}
