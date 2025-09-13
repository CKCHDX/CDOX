#pragma once
#include <QString>

namespace FirewallUtils {
    bool checkSystemIntegrity();
    bool blockConnection(const QString &target);
    bool allowConnection(const QString &target);
    // more helpers: parse logs, etc.
}
