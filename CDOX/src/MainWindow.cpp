#include "MainWindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QFrame>
#include <QKeyEvent>
#include <QSplitter>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include "backend/SystemLogWatcher.h"
#include "NetworkTopologyWidget.h"
#include "NetworkScanner.h"
#include <QNetworkInterface>
#include <QHostAddress>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("CDOX Operations Dashboard");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    showFullScreen();

    QFile f(":/styles/cdox.qss");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    logWatcher = new SystemLogWatcher(this);

    connect(logWatcher, &SystemLogWatcher::newLogEntry, this, [this](const QString &log) {
        if (systemLogsTextEdit) {
            systemLogsTextEdit->moveCursor(QTextCursor::End);
            systemLogsTextEdit->insertPlainText(log + "\n");
            systemLogsTextEdit->moveCursor(QTextCursor::End);
        }
        qDebug() << "Log: " << log;
    });

    logWatcher->startWatching();


    setupUI();
}

MainWindow::~MainWindow() = default;

QWidget* createSystemStatusWidget(QWidget *parent) {
    // System info e.g. CPU usage, Battery, Time - placeholder static for now
    QWidget *widget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(15);

    auto createStatusLabel = [](const QString &iconText, const QString &desc) -> QLabel* {
        QLabel *label = new QLabel(iconText);
        label->setToolTip(desc);
        label->setStyleSheet("color: #00e8ff; font-weight: bold; font-family: 'Orbitron';");
        return label;
    };

    QLabel *cpuLabel = createStatusLabel("CPU: 23%", "CPU Usage");
    QLabel *batteryLabel = createStatusLabel("Battery: 92%", "Battery Level");
    QLabel *timeLabel = createStatusLabel(QDateTime::currentDateTime().toString("HH:mm:ss"), "Current Time");

    layout->addWidget(cpuLabel);
    layout->addWidget(batteryLabel);
    layout->addWidget(timeLabel);

    // Timer to update time label every second
    QTimer *timer = new QTimer(widget);
    QObject::connect(timer, &QTimer::timeout, [timeLabel]() {
        timeLabel->setText(QDateTime::currentDateTime().toString("HH:mm:ss"));
    });
    timer->start(1000);

    return widget;
}

QWidget* createCyberStatusWidget(QWidget *parent) {
    // Cybersecurity statuses e.g. Firewall, Integrity - placeholder static values
    QWidget *widget = new QWidget(parent);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(5, 0, 5, 0);
    layout->setSpacing(15);

    auto createStatusLabel = [](const QString &iconText, const QString &desc) -> QLabel* {
        QLabel *label = new QLabel(iconText);
        label->setToolTip(desc);
        label->setStyleSheet("color: #00e8ff; font-weight: bold; font-family: 'Orbitron';");
        return label;
    };

    QLabel *firewallLabel = createStatusLabel("FW: Active", "Firewall Status");
    QLabel *integrityLabel = createStatusLabel("Integrity: OK", "System Integrity Status");
    QLabel *threatDetectLabel = createStatusLabel("Threats: 0", "Active Threat Detections");

    layout->addWidget(firewallLabel);
    layout->addWidget(integrityLabel);
    layout->addWidget(threatDetectLabel);

    return widget;
}

void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Create vertical splitter to hold header and main splitter
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, central);
    verticalSplitter->setHandleWidth(8);
    verticalSplitter->setChildrenCollapsible(false);

    // Header widget with three sections: left cyber status, center title, right system status
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setMinimumHeight(65);
    headerWidget->setStyleSheet(
        "background-color: #001f3f;"
        "border-bottom: 2px solid #00697d;"
    );

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(10, 10, 10, 10);

    QWidget *cyberStatus = createCyberStatusWidget(headerWidget);
    QLabel *titleLabel = new QLabel("CDOX Operations Dashboard", headerWidget);
    titleLabel->setStyleSheet(
        "color: #00fefe;"
        "font-family: 'Orbitron';"
        "font-weight: bold;"
        "font-size: 32pt;"
        "text-transform: uppercase;"
        "text-align: center;"
        "text-shadow: 0 0 5px #00dbff, 0 0 10px #00dbff, 0 0 20px #00dbff, 0 0 30px #00dbff;"
    );
    QWidget *systemStatus = createSystemStatusWidget(headerWidget);

    // Use stretch to keep title center aligned
    headerLayout->addWidget(cyberStatus);
    headerLayout->addStretch(1);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch(1);
    headerLayout->addWidget(systemStatus);

    verticalSplitter->addWidget(headerWidget);
    verticalSplitter->setCollapsible(0, false);

    // Horizontal splitter for main panels: Quick actions | center tabs | Alerts
    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal, verticalSplitter);
    horizontalSplitter->setHandleWidth(10);
    horizontalSplitter->setChildrenCollapsible(false);

    quickActionsPanel = createQuickActionsPanel();
    centerTabWidget = createCenterTabWidget();
    alertsPanel = createAlertsPanel();

    horizontalSplitter->addWidget(quickActionsPanel);
    horizontalSplitter->addWidget(centerTabWidget);
    horizontalSplitter->addWidget(alertsPanel);

    horizontalSplitter->setSizes({250, 700, 350});

    verticalSplitter->addWidget(horizontalSplitter);

    verticalSplitter->setSizes({65, 1000});

    layout->addWidget(verticalSplitter);
}

QWidget* MainWindow::createHeader() {
    QLabel *headerLabel = new QLabel("CDOX Operations Dashboard", this);
    QFont headerFont("Orbitron", 32, QFont::Bold);
    headerLabel->setFont(headerFont);
    headerLabel->setStyleSheet(
        "color: #00fefe;"
        "background-color: #001f3f;"
        "padding: 16px;"
        "border-bottom: 2px solid #00697d;"
        "letter-spacing: 2px;"
        "text-align: center;"
        "qproperty-alignment: 'AlignCenter';"
        "text-transform: uppercase;"
        "text-shadow: 0 0 5px #00dbff, 0 0 10px #00dbff, 0 0 20px #00dbff, 0 0 30px #00dbff;"
    );
    return headerLabel;
}

QWidget* MainWindow::createQuickActionsPanel() {
    QFrame *panel = new QFrame(this);
    panel->setObjectName("panel");
    panel->setMinimumWidth(280);
    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(10, 10, 10, 10);

    QLabel *title = new QLabel("Quick Actions", panel);
    QFont font("Orbitron", 16, QFont::Bold);
    title->setFont(font);
    title->setStyleSheet(
        "color: #00ffd5;"
        "text-transform: uppercase;"
        "border-bottom: 1px solid #00b7fdaa;"
        "padding-bottom: 6px;"
        "margin-bottom: 10px;"
    );
    layout->addWidget(title);

    QStringList actions {"Network Sweep", "Host IP Scan", "Cyber Lockdown", "Deploy Firewall", "Virus Scan"};
    for (const QString &action : actions) {
        QPushButton *btn = new QPushButton(action, panel);
        btn->setObjectName("action-btn");
        btn->setCursor(Qt::PointingHandCursor);
        layout->addWidget(btn);
        connect(btn, &QPushButton::clicked, this, [=]() {
            qDebug() << "Quick action triggered:" << action;
        });
    }
    layout->addStretch();

    return panel;
}



QString getLocalIPv4Address()
{
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : interfaces) {
        if (!(iface.flags() & QNetworkInterface::IsUp) || 
            (iface.flags() & QNetworkInterface::IsLoopBack))
            continue;

        const auto entries = iface.addressEntries();
        for (const QNetworkAddressEntry &entry : entries) {
            QHostAddress ip = entry.ip();
            if (ip.protocol() == QAbstractSocket::IPv4Protocol && !ip.isLoopback()) {
                return ip.toString();
            }
        }
    }
    return QString();
}

QTabWidget* MainWindow::createCenterTabWidget() {
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setObjectName("panel");
    tabWidget->setMinimumWidth(700);
    tabWidget->setDocumentMode(true);
    centerTabWidget = tabWidget;

    setupTabs();

    networkScanner = new NetworkScanner(this);

    connect(networkScanner, &NetworkScanner::scanCompleted, this, [this](QMap<QString, QString> hosts){
        QMap<QString, QString> nodes;
        QVector<QPair<QString, QString>> edges;

        nodes["router"] = "Router\n" + getLocalIPv4Address();

        // Add your local device as distinct black node
        QString localIp = getLocalIPv4Address();
        nodes["localDevice"] = "This Device\n" + localIp;

        edges.append(qMakePair(QString("router"), QString("localDevice")));

        int clientId = 0;
        for (auto it = hosts.begin(); it != hosts.end(); ++it) {
            QString key = "host" + QString::number(clientId++);
            nodes[key] = it.value();
            edges.append(qMakePair(QString("router"), key));
        }

        QMap<QString, QPointF> layout = {
            {"router", QPointF(150, 50)},
            {"localDevice", QPointF(150, 150)}
        };

        if (topologyWidget) {
            topologyWidget->loadNetworkData(nodes, edges, layout);
        }
    });

    return tabWidget;
}


void MainWindow::setupTabs() {
    if (!centerTabWidget)
        return;

    while(centerTabWidget->count() > 0)
        centerTabWidget->removeTab(0);

    // Network Topology Tab
    QWidget *networkTab = new QWidget();
    QVBoxLayout *netLayout = new QVBoxLayout(networkTab);
    netLayout->setContentsMargins(0, 0, 0, 0);
    netLayout->setSpacing(0);

    QLabel *netTitle = new QLabel("Network Topology", networkTab);
    QFont font("Orbitron", 16, QFont::Bold);
    netTitle->setFont(font);
    netTitle->setStyleSheet(
        "color: #00ffd5;"
        "text-transform: uppercase;"
        "border-bottom: 1px solid #00b7fdaa;"
        "padding: 6px 10px;"
        "margin-bottom: 0;"
    );
    netLayout->addWidget(netTitle);

    // Your custom network topology widget
    topologyWidget = new NetworkTopologyWidget(networkTab);
    topologyWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    netLayout->addWidget(topologyWidget);
    

    centerTabWidget->addTab(networkTab, "Network Topology");

    // Logs Tab
    QWidget *logsTab = new QWidget();
    QVBoxLayout *logsLayout = new QVBoxLayout(logsTab);
    logsLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *logsTitle = new QLabel("System Logs", logsTab);
    logsTitle->setFont(font);
    logsTitle->setStyleSheet(netTitle->styleSheet());
    logsLayout->addWidget(logsTitle);

    QTextEdit *logsText = new QTextEdit(logsTab);
    logsText->setObjectName("alerts");
    logsText->setReadOnly(true);
    systemLogsTextEdit = logsText;
    logsLayout->addWidget(logsText);

    centerTabWidget->addTab(logsTab, "System Logs");

    // Threat Intelligence Tab
    QWidget *threatTab = new QWidget();
    QVBoxLayout *threatLayout = new QVBoxLayout(threatTab);
    threatLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *threatTitle = new QLabel("Threat Intelligence", threatTab);
    threatTitle->setFont(font);
    threatTitle->setStyleSheet(netTitle->styleSheet());
    threatLayout->addWidget(threatTitle);

    QTextEdit *threatText = new QTextEdit(threatTab);
    threatText->setReadOnly(true);
    threatText->setText("Threat intelligence data will appear here...");
    threatLayout->addWidget(threatText);

    centerTabWidget->addTab(threatTab, "Threat Intelligence");
}


QWidget* MainWindow::createAlertsPanel() {
    QFrame *panel = new QFrame(this);
    panel->setObjectName("panel");
    panel->setMinimumWidth(350);
    QVBoxLayout *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(10, 10, 10, 10);

    QLabel *title = new QLabel("Alerts & Logs", panel);
    QFont font("Orbitron", 16, QFont::Bold);
    title->setFont(font);
    title->setStyleSheet(
        "color: #00ffd5;"
        "text-transform: uppercase;"
        "border-bottom: 1px solid #00b7fdaa;"
        "padding-bottom: 6px;"
        "margin-bottom: 10px;"
    );
    layout->addWidget(title);

    QTextEdit *alerts = new QTextEdit(panel);
    alerts->setObjectName("alerts");
    alerts->setReadOnly(true);
    alerts->setText(
        "14:36:01 | DETECTED unusual login from IP 198.51.100.23\n"
        "14:35:55 | Virus signature matched on host 192.168.1.102\n"
        "14:35:50 | Network sweep completed - no anomalies\n"
        "14:35:10 | User 'agent24' elevated privileges\n"
        "14:34:59 | Firewall rules updated successfully\n"
        "14:34:48 | Suspicious port scanning activity flagged\n"
        "...\n"
    );
    layout->addWidget(alerts);

    return panel;
}



void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Escape) {
        close();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}
