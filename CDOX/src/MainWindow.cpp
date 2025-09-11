#include "MainWindow.h"
#include <QFile>
#include <QTextStream>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QFrame>
#include <QKeyEvent>
#include <QApplication>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("CDOX Operations Dashboard");

    // Start fullscreen without window decorations
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    showFullScreen();

    // Load and apply dark neon QSS
    QFile f(":/styles/cdox.qss");
    if (f.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QGridLayout *grid = new QGridLayout(central);
    grid->setContentsMargins(10, 10, 10, 10);
    grid->setHorizontalSpacing(10);
    grid->setVerticalSpacing(10);
    grid->setRowMinimumHeight(0, 65);

    // Header (mimics <header> tag)
    QLabel *header = new QLabel("CDOX Operations Dashboard", central);
    QFont headerFont("Orbitron", 32, QFont::Bold);
    header->setFont(headerFont);
    header->setStyleSheet(
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
    header->setMaximumHeight(65);
    grid->addWidget(header, 0, 0, 1, 3);

    // Left panel - Quick Actions
    QFrame *quickPanel = new QFrame(central);
    quickPanel->setObjectName("panel");
    quickPanel->setMinimumWidth(280);
    QVBoxLayout *quickLayout = new QVBoxLayout(quickPanel);
    quickLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *quickTitle = new QLabel("Quick Actions", quickPanel);
    QFont quickTitleFont("Orbitron", 16, QFont::Bold);
    quickTitle->setFont(quickTitleFont);
    quickTitle->setStyleSheet(
        "color: #00ffd5;"
        "text-transform: uppercase;"
        "border-bottom: 1px solid #00b7fdaa;"
        "padding-bottom: 6px;"
        "margin-bottom: 10px;"
    );
    quickLayout->addWidget(quickTitle);

    QStringList actions = {
        "Network Sweep",
        "Host IP Scan",
        "Cyber Lockdown",
        "Deploy Firewall",
        "Virus Scan"
    };
    for (const QString &action : actions) {
        QPushButton *btn = new QPushButton(action, quickPanel);
        btn->setObjectName("action-btn");
        btn->setCursor(Qt::PointingHandCursor);
        quickLayout->addWidget(btn);
        // Hook signals as needed
        connect(btn, &QPushButton::clicked, this, [=]() {
            qDebug("Quick action triggered: %s", qPrintable(action));
        });
    }
    quickLayout->addStretch();
    grid->addWidget(quickPanel, 1, 0);

    // Center panel - Network Topology
    QFrame *networkPanel = new QFrame(central);
    networkPanel->setObjectName("panel");
    networkPanel->setMinimumWidth(700);
    QVBoxLayout *netLayout = new QVBoxLayout(networkPanel);
    netLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *netTitle = new QLabel("Network Topology", networkPanel);
    netTitle->setFont(quickTitleFont);
    netTitle->setStyleSheet(quickTitle->styleSheet());
    netLayout->addWidget(netTitle);

    QTextEdit *networkText = new QTextEdit(networkPanel);
    networkText->setObjectName("network-map");
    networkText->setReadOnly(true);
    networkText->setText(
        "  [192.168.1.1]───┐\n"
        "     │            ├─[192.168.1.101] (Online)\n"
        "     │            ├─[192.168.1.102] (Online)\n"
        "  [192.168.1.254] │\n"
        "     │            └─[192.168.1.150] (Offline)\n"
        "  ... Network Graph ...\n"
    );
    netLayout->addWidget(networkText);
    grid->addWidget(networkPanel, 1, 1);

    // Right panel - Alerts & Logs
    QFrame *alertsPanel = new QFrame(central);
    alertsPanel->setObjectName("panel");
    alertsPanel->setMinimumWidth(350);
    QVBoxLayout *alertsLayout = new QVBoxLayout(alertsPanel);
    alertsLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *alertsTitle = new QLabel("Alerts & Logs", alertsPanel);
    alertsTitle->setFont(quickTitleFont);
    alertsTitle->setStyleSheet(quickTitle->styleSheet());
    alertsLayout->addWidget(alertsTitle);

    QTextEdit *alertsText = new QTextEdit(alertsPanel);
    alertsText->setObjectName("alerts");
    alertsText->setReadOnly(true);
    alertsText->setText(
        "14:36:01 | DETECTED unusual login from IP 198.51.100.23\n"
        "14:35:55 | Virus signature matched on host 192.168.1.102\n"
        "14:35:50 | Network sweep completed - no anomalies\n"
        "14:35:10 | User 'agent24' elevated privileges\n"
        "14:34:59 | Firewall rules updated successfully\n"
        "14:34:48 | Suspicious port scanning activity flagged\n"
        "...\n"
    );
    alertsLayout->addWidget(alertsText);

    grid->addWidget(alertsPanel, 1, 2);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Escape) {
        close(); // allow exit with ESC key
    } else {
        QMainWindow::keyPressEvent(event);
    }
}
MainWindow::~MainWindow() {
    // Destructor
}