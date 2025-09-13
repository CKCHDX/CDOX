#include "FirewallModel.h"

FirewallModel::FirewallModel(QObject *parent) : QObject(parent) {}

const QVector<FirewallRule>& FirewallModel::rules() const { return m_rules; }

void FirewallModel::addRule(const FirewallRule &rule) {
    m_rules.append(rule);
    emit rulesChanged();
}

void FirewallModel::removeRule(int index) {
    if (index >=0 && index < m_rules.size()) {
        m_rules.removeAt(index);
        emit rulesChanged();
    }
}

void FirewallModel::clearRules() {
    m_rules.clear();
    emit rulesChanged();
}

QString FirewallModel::status() const { return m_status; }
