#include "NetworkTopologyWidget.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QPen>
#include <QBrush>
#include "NetworkNodeItem.h"
#include <QColor> 

// Simple Edge representing connections
class NetworkEdgeItem : public QGraphicsLineItem {
public:
    NetworkEdgeItem(QGraphicsItem *parent = nullptr) : QGraphicsLineItem(parent) {
        setPen(QPen(Qt::gray, 2, Qt::DashLine));
    }

    void updatePosition(const QPointF &start, const QPointF &end) {
        setLine(QLineF(start, end));
    }
};

NetworkTopologyWidget::NetworkTopologyWidget(QWidget *parent)
    : QGraphicsView(parent),
      scene(new QGraphicsScene(this))
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
}

// Public API to load network data
void NetworkTopologyWidget::loadNetworkData(const QMap<QString, QString> &nodesInfo,
                                            QVector<QPair<QString, QString>> &edges,
                                            const QMap<QString, QPointF> &layout)

{
    clearNetwork();

    for (auto it = nodesInfo.begin(); it != nodesInfo.end(); ++it) {
        QPointF pos = layout.contains(it.key()) ? layout[it.key()] : QPointF(0,0);
        addNode(it.key(), it.value(), pos, it.key() == "localDevice" ? Qt::black : Qt::green);
    }



    for (const auto &edge : edges) {
        addEdge(edge.first, edge.second);
    }
}

void NetworkTopologyWidget::clearNetwork() {
    scene->clear();
    nodesMap.clear();
}

void NetworkTopologyWidget::addNode(const QString &id, const QString &label, const QPointF &pos, const QColor &color) {
    NetworkNodeItem *node = new NetworkNodeItem(id, label);
    node->setPos(pos);
    node->setBrush(QBrush(color));  // Set node color here
    scene->addItem(node);
    nodesMap[id] = node;

    connect(node, &NetworkNodeItem::pingRequested, this, [=](const QString &id){
        emit pingRequested(id);
    });
    connect(node, &NetworkNodeItem::traceRouteRequested, this, [=](const QString &id){
        emit traceRouteRequested(id);
    });
}


void NetworkTopologyWidget::addEdge(const QString &fromId, const QString &toId) {
    if (!nodesMap.contains(fromId) || !nodesMap.contains(toId))
        return;

    auto fromNode = nodesMap[fromId];
    auto toNode = nodesMap[toId];

    // Connect center of nodes
    QPointF start = fromNode->sceneBoundingRect().center();
    QPointF end = toNode->sceneBoundingRect().center();

    NetworkEdgeItem *edge = new NetworkEdgeItem();
    edge->updatePosition(start, end);
    scene->addItem(edge);
}

void NetworkTopologyWidget::wheelEvent(QWheelEvent *event) {
    constexpr qreal scaleFactor = 1.15;
    if (event->angleDelta().y() > 0) {
        scale(scaleFactor, scaleFactor);
    } else {
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}
