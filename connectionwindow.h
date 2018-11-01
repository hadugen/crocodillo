#ifndef CONNECTIONWINDOW_H
#define CONNECTIONWINDOW_H

#include <QDialog>

namespace Ui {
class ConnectionWindow;
}

class ConnectionWindow : public QDialog
{
    Q_OBJECT

signals:
    void connectRequested(QString hostInfo);

public:
    explicit ConnectionWindow(QWidget *parent = 0);
    ~ConnectionWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ConnectionWindow *ui;
};

#endif // CONNECTIONWINDOW_H
