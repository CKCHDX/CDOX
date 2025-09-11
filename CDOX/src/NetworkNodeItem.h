#pragma once

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QObject>
#include <QTimer>

class NetworkNodeItem : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    explicit NetworkNodeItem(const QString &id, const QString &label, QGraphicsItem *parent = nullptr);
    ~NetworkNodeItem() override = default;

    QString id() const { return m_id; }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private slots:
    void animateStatus();

signals:
    void pingRequested(const QString &nodeId);
    void traceRouteRequested(const QString &nodeId);

private:
    QString m_id;
    QGraphicsTextItem *labelItem;
    QTimer animationTimer;
    bool animationOn = false;
    int animationStep = 0;

    void startAnimation();
    void stopAnimation();
};
