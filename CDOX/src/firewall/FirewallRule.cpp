#include "FirewallRule.h"
FirewallRule::FirewallRule(const QString &name, const QString &action, const QString &target)
    : name(name), action(action), target(target) {}
