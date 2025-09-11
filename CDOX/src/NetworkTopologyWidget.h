#pragma once

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMap>
#include <QString>

class NetworkNodeItem;
class NetworkEdgeItem;

class NetworkTopologyWidget : public QGraphicsView {
    Q_OBJECT
public:
    explicit NetworkTopologyWidget(QWidget *parent = nullptr);

    // Load network data - pass map of device ID -> info and edges
    void loadNetworkData(const QMap<QString, QString> &nodesInfo,
                        QVector<QPair<QString, QString>> &edges,
                        const QMap<QString, QPointF> &layout = QMap<QString, QPointF>());


    void clearNetwork();

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsScene *scene;
    QMap<QString, NetworkNodeItem*> nodesMap;

    void addNode(const QString &id, const QString &label, const QPointF &pos, const QColor &color = Qt::green);

    
    void addEdge(const QString &fromId, const QString &toId);

signals:
    void pingRequested(const QString &nodeId);
    void traceRouteRequested(const QString &nodeId);
};