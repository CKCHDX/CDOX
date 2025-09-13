#include "SecurityStatusWidget.h"
#include <QLabel>
#include <QVBoxLayout>

SecurityStatusWidget::SecurityStatusWidget(QWidget *parent) : QWidget(parent) {
    auto *layout = new QVBoxLayout(this);
    auto *label = new QLabel("Overall Security: Classified", this);
    layout->addWidget(label);
}

void SecurityStatusWidget::setStatus(const QString &status) {
    m_status = status;
    // update UI - omitted for brevity
}
