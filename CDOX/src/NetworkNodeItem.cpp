#include "NetworkNodeItem.h"
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPen>
#include <QBrush>

NetworkNodeItem::NetworkNodeItem(const QString &id, const QString &label, QGraphicsItem *parent)
    : QObject(), QGraphicsEllipseItem(0, 0, 50, 50, parent),
      m_id(id),
      labelItem(new QGraphicsTextItem(label, this))
{
    setPen(QPen(Qt::blue, 2));
    setBrush(QBrush(Qt::green));  // Default green for healthy
    setAcceptHoverEvents(true);

    labelItem->setDefaultTextColor(Qt::white);
    labelItem->setFont(QFont("Orbitron", 9, QFont::Bold));
    labelItem->setPos(0, 55);
    labelItem->setTextWidth(50);
    labelItem->setTextInteractionFlags(Qt::NoTextInteraction);

    connect(&animationTimer, &QTimer::timeout, this, &NetworkNodeItem::animateStatus);
}

void NetworkNodeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event) {
    QMenu menu;
    QAction *pingAction = menu.addAction("Ping");
    QAction *traceRouteAction = menu.addAction("Trace Route");

    QAction *selected = menu.exec(event->screenPos());
    if (!selected)
        return;

    if (selected == pingAction) {
        emit pingRequested(m_id);
    } else if (selected == traceRouteAction) {
        emit traceRouteRequested(m_id);
    }
}

void NetworkNodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event);
    setPen(QPen(Qt::yellow, 3));
    startAnimation();
}

void NetworkNodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event);
    setPen(QPen(Qt::blue, 2));
    stopAnimation();
}

void NetworkNodeItem::startAnimation() {
    if (!animationOn) {
        animationOn = true;
        animationStep = 0;
        animationTimer.start(100);
    }
}

void NetworkNodeItem::stopAnimation() {
    animationOn = false;
    animationTimer.stop();
    setBrush(QBrush(Qt::green));
}

void NetworkNodeItem::animateStatus() {
    const int steps = 10;
    animationStep = (animationStep + 1) % (2 * steps);
    int val = animationStep < steps ? animationStep : 2 * steps - animationStep;
    int colorVal = 100 + val * 10;
    setBrush(QBrush(QColor(0, colorVal, 0)));
}
