#include "secretwordswindow.h"
#include "ui_secretwordswindow.h"

SecretWordsWindow::SecretWordsWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecretWordsWindow)
{
    ui->setupUi(this);
}

SecretWordsWindow::~SecretWordsWindow()
{
    delete ui;
}

void SecretWordsWindow::closeEvent(QCloseEvent *event)
{
    emit accepted(ui->listWidget->currentItem()->text());
    event->accept();
}

void SecretWordsWindow::on_pushButton_clicked()
{
    close();
}
