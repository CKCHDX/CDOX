#pragma once
#include <QWidget>

class SecurityStatusWidget : public QWidget {
    Q_OBJECT
public:
    explicit SecurityStatusWidget(QWidget *parent = nullptr);
    void setStatus(const QString &status);

private:
    QString m_status;
};
