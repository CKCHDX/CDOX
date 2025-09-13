#pragma once
#include <QWidget>

class ThreatIndicatorWidget : public QWidget {
    Q_OBJECT
public:
    explicit ThreatIndicatorWidget(QWidget *parent = nullptr);
    void showThreat(const QString &msg);

private:
    QString lastThreat;
};
