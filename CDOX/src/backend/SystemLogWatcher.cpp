#include "SystemLogWatcher.h"
#include <QDebug>

SystemLogWatcher::SystemLogWatcher(QObject *parent)
    : QObject(parent),
      journalProcess(new QProcess(this))
{
    connect(journalProcess, &QProcess::readyReadStandardOutput, this, &SystemLogWatcher::handleProcessReadyRead);
    connect(journalProcess, &QProcess::errorOccurred, this, [=](QProcess::ProcessError err){
        qWarning() << "Journal process error:" << err;
    });
}

SystemLogWatcher::~SystemLogWatcher() {
    if (journalProcess->state() == QProcess::Running)
        journalProcess->kill();
}

void SystemLogWatcher::startWatching() {
    // Start journalctl follow logs (requires privileges)
    journalProcess->start("journalctl", QStringList() << "-f" << "-o" << "short");
    if (!journalProcess->waitForStarted(3000)) {
        qWarning() << "Failed to start journalctl process";
    }
}

void SystemLogWatcher::handleProcessReadyRead() {
    QByteArray data = journalProcess->readAllStandardOutput();
    QStringList lines = QString::fromUtf8(data).split('\n', Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        emit newLogEntry(line);
    }
}
