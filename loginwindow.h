#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <mainwindow.h>
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

signals:
    void userNameValid(QString userName);

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginWindow *ui;
    QMessageBox *msg;
    MainWindow *mainWindow;
};

#endif // LOGINWINDOW_H
