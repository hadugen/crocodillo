#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QDebug>
#include <QTime>
#include <QPointF>

class CustomGraphicsView : public QGraphicsView
{
    Q_OBJECT

signals:
    void signal_mouseMoveEvent(QPointF pos);
    void signal_mousePressEvent(QPointF pos);
    void signal_mouseReleaseEvent(QPointF pos);

public:
    CustomGraphicsView(QWidget *parent);
    void resizeEvent(QResizeEvent *event);
    void gameMasterMouseMoveEvent(qreal x, qreal y);
    void gameMasterMousePressEvent(qreal x, qreal y);
    void gameMasterMouseReleaseEvent(qreal x, qreal y);
    bool isGameMasterMode();
    void setGameMasterMode(bool gm);
    void setColor(QRgb color);
    void clear();

private:
    QGraphicsScene *_scene;
    QPointF _lastPoint;
    bool _isButtonPressed, _isGameMasterMode;
    QColor _color;

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:

};

#endif // CustomGraphicsView_H
