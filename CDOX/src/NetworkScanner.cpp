#include "NetworkScanner.h"
#include <QRegularExpression>

NetworkScanner::NetworkScanner(QObject *parent)
    : QObject(parent), proc(new QProcess(this))
{
    connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &NetworkScanner::processFinished);
    connect(proc, &QProcess::readyReadStandardOutput,
            this, &NetworkScanner::processReadyRead);
}

void NetworkScanner::scanSubnet(const QString &subnet) {
    rawOutput.clear();
    QStringList args = {"-sn", subnet, "-oG", "-"};
    proc->start("nmap", args);
}

void NetworkScanner::processReadyRead() {
    rawOutput += proc->readAllStandardOutput();
}

void NetworkScanner::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    QMap<QString, QString> hosts; // ip -> hostname

    // Parse grepable output
    QRegularExpression re(R"(Host: (\S+) \(([^)]*)\))");
    auto lines = rawOutput.split('\n');
    for (const QString &line : lines) {
        auto match = re.match(line);
        if (match.hasMatch()) {
            QString ip = match.captured(1);
            QString hostname = match.captured(2);
            if (hostname == "")
                hostname = ip;
            hosts[ip] = hostname;
        }
    }

    emit scanCompleted(hosts);
}
