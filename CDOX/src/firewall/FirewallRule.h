#pragma once
#include <QString>

class FirewallRule {
public:
    FirewallRule(const QString &name = "Rule", const QString &action = "Allow", const QString &target = "Any");

    QString name;
    QString action; // "Allow", "Block", etc.
    QString target; // Target IP/port/protocol etc.
};
