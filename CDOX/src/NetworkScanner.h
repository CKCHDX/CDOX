#pragma once

#include <QObject>
#include <QProcess>
#include <QMap>
#include <QString>
#include <QVector>
#include <QPair>

class NetworkScanner : public QObject {
    Q_OBJECT
public:
    explicit NetworkScanner(QObject *parent = nullptr);

    void scanSubnet(const QString &subnet);

signals:
    // Emitted with list of discovered IPs and their hostnames if any
    void scanCompleted(QMap<QString, QString> hosts);

private slots:
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void processReadyRead();

private:
    QProcess *proc;
    QString rawOutput;
};
