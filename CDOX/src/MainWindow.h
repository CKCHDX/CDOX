#pragma once
#include <QMainWindow>
#include <QKeyEvent>
#include <QTextEdit>  
#include "backend/SystemLogWatcher.h"
#include "NetworkTopologyWidget.h"
#include "NetworkScanner.h"
class QTabWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupUI();
    QWidget* createHeader();
    QWidget* createQuickActionsPanel();
    QTabWidget* createCenterTabWidget();
    QWidget* createAlertsPanel();

    void setupTabs();
    NetworkScanner *networkScanner = nullptr;
    NetworkTopologyWidget *topologyWidget = nullptr;
    QTabWidget *centerTabWidget = nullptr;
    SystemLogWatcher *logWatcher = nullptr;
    QTextEdit *systemLogsTextEdit = nullptr; 
    QWidget *header = nullptr;
    QWidget *quickActionsPanel = nullptr;
    QWidget *alertsPanel = nullptr;
};
