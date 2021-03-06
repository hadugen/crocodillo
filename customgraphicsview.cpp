#include "customgraphicsview.h"

CustomGraphicsView::CustomGraphicsView(QWidget *parent) : QGraphicsView(parent), _isGameMasterMode(false)
{
    _scene = new QGraphicsScene(0, 0, 300, 250, this);
    setScene(_scene);
    _isButtonPressed = false;
    fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
    this->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing |
                         QPainter::TextAntialiasing);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(!_isGameMasterMode) {
        return;
    }

    if(_isButtonPressed)
    {
        QPointF pos = mapToScene(event->pos());
        _scene->addLine(QLineF(_lastPoint, pos), QPen(_color));
        _lastPoint = pos;
        emit signal_mouseMoveEvent(pos);

    }

}

void CustomGraphicsView::setColor(QRgb color)
{
    this->_color = QColor(color);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(!_isGameMasterMode)
        return;

    switch(event->button())
    {

    case Qt::LeftButton:

        _isButtonPressed = true;
        _lastPoint = mapToScene(event->pos());
        emit signal_mousePressEvent(_lastPoint);

        break;

    case Qt::RightButton:
        break;

    default:
        break;
    }
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(!_isGameMasterMode) {
        return;
    }
    switch(event->button())
    {

    case Qt::LeftButton:

        emit signal_mouseReleaseEvent(_lastPoint);
        _isButtonPressed = false;

        break;

    case Qt::RightButton:
        break;

    default:
        break;
    }
}

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
}

void CustomGraphicsView::clear()
{
    _scene->clear();
}

bool CustomGraphicsView::isGameMasterMode()
{
    return _isGameMasterMode;
}

void CustomGraphicsView::setGameMasterMode(bool gm)
{
    _isGameMasterMode = gm;
}

void CustomGraphicsView::gameMasterMouseMoveEvent(qreal x, qreal y)
{
    if(_isButtonPressed)
    {
        QPointF pos(x, y);
        _scene->addLine(QLineF(_lastPoint, pos), QPen(_color));
        _lastPoint = pos;
    }
}

void CustomGraphicsView::gameMasterMousePressEvent(qreal x, qreal y)
{
    _isButtonPressed = true;
    _lastPoint = QPointF(x, y);
}

void CustomGraphicsView::gameMasterMouseReleaseEvent(qreal x, qreal y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    _isButtonPressed = false;
}
