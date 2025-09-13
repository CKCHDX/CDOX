#pragma once
#include <QObject>
#include <QVector>
#include "FirewallRule.h"

class FirewallModel : public QObject {
    Q_OBJECT
public:
    explicit FirewallModel(QObject *parent = nullptr);

    const QVector<FirewallRule>& rules() const;
    void addRule(const FirewallRule &rule);
    void removeRule(int index);
    void clearRules();

    // For status/alerts
    QString status() const;
signals:
    void rulesChanged();
    void threatDetected(const QString &description);

private:
    QVector<FirewallRule> m_rules;
    QString m_status{"Operational"};
};
