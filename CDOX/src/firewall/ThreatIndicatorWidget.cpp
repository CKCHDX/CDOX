#include "ThreatIndicatorWidget.h"
#include <QLabel>
#include <QVBoxLayout>

ThreatIndicatorWidget::ThreatIndicatorWidget(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    auto *label = new QLabel("No Active Threats", this);
    layout->addWidget(label);
}

void ThreatIndicatorWidget::showThreat(const QString &msg) {
    lastThreat = msg;
    // update UI - omitted for brevity
}
