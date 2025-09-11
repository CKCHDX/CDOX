#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QFrame>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onQuickActionTriggered(const QString &actionName);

private:
    void setupHeader();
    void setupDashboard();
    void setupConnections();

    QWidget *centralWidget_;
    QLabel *clockLabel_;
    QLabel *sysLed_;
    QPushButton *themeButton_;
    QPushButton *exportLogsButton_;

    QFrame *quickActionsPanel_;
    QFrame *networkPanel_;
    QFrame *alertsPanel_;

    QTextEdit *networkMap_;
    QTextEdit *alerts_;

    // other UI elements could be declared here
};
