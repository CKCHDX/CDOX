#pragma once
#include <QObject>
#include <QProcess>

class SystemLogWatcher : public QObject {
    Q_OBJECT
public:
    explicit SystemLogWatcher(QObject *parent = nullptr);
    ~SystemLogWatcher();

    void startWatching();

signals:
    void newLogEntry(const QString &log);

private slots:
    void handleProcessReadyRead();

private:
    QProcess *journalProcess = nullptr;
};
