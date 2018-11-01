#ifndef SECRETWORDSWINDOW_H
#define SECRETWORDSWINDOW_H

#include <QWidget>
#include <QCloseEvent>
#include <QDebug>

namespace Ui {
class SecretWordsWindow;
}

class SecretWordsWindow : public QWidget
{
    Q_OBJECT

signals:
    void accepted(QString secretWord);

public:
    explicit SecretWordsWindow(QWidget *parent = 0);
    ~SecretWordsWindow();

private slots:
    void on_pushButton_clicked();
    void closeEvent(QCloseEvent *event);

private:
    Ui::SecretWordsWindow *ui;
};

#endif // SECRETWORDSWINDOW_H
