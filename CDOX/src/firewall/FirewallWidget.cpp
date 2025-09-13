// src/firewall/FirewallWidget.cpp

#include "FirewallWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QFrame>
#include <QProgressBar>
#include <QTimer>
#include <QMovie>
#include <QPainter>
#include <QHeaderView>

FirewallWidget::FirewallWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #f4f6f8; color: #2f3e46; font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(20);

    // Title (Clean and modern)
    QLabel *title = new QLabel("Firewall Control Panel", this);
    title->setAlignment(Qt::AlignLeft);
    QFont titleFont("Segoe UI", 28, QFont::DemiBold);
    title->setFont(titleFont);
    title->setStyleSheet("color: #1f2937;");
    mainLayout->addWidget(title);

    // Top Panel (Status + Controls) section with subtle styling
    QHBoxLayout *topPanel = new QHBoxLayout();
    topPanel->setSpacing(30);

    // Security Status Frame — styled clean card with progress bar
    QFrame *statusFrame = new QFrame(this);
    statusFrame->setMinimumWidth(350);
    statusFrame->setStyleSheet(
        "background-color: #ffffff;"
        "border: 1px solid #d1d5db;"
        "border-radius: 8px;"
        "padding: 15px;"
        "box-shadow: 0 2px 6px rgba(0,0,0,0.05);"
    );
    QVBoxLayout *statusLayout = new QVBoxLayout(statusFrame);
    statusLayout->setContentsMargins(10, 10, 10, 10);
    statusLayout->setSpacing(10);

    QLabel *secStatusLabel = new QLabel("System Security Status", statusFrame);
    secStatusLabel->setFont(QFont("Segoe UI", 12, QFont::Bold));
    secStatusLabel->setStyleSheet("color: #374151;");
    statusLayout->addWidget(secStatusLabel);

    QLabel *secStatusValue = new QLabel("SAFE", statusFrame);
    secStatusValue->setFont(QFont("Segoe UI", 20, QFont::Bold));
    secStatusValue->setStyleSheet("color: #10b981;");  // green color
    statusLayout->addWidget(secStatusValue);

    QProgressBar *secProgress = new QProgressBar(statusFrame);
    secProgress->setRange(0, 100);
    secProgress->setValue(85);  // example security rating percentage
    secProgress->setTextVisible(false);
    secProgress->setStyleSheet(
        "QProgressBar {"
        "  border: 1px solid #9ca3af;"
        "  border-radius: 6px;"
        "  background-color: #e5e7eb;"
        "  height: 14px;"
        "}"
        "QProgressBar::chunk {"
        "  background-color: #10b981;"
        "  border-radius: 6px;"
        "}"
    );
    statusLayout->addWidget(secProgress);

    mainLayout->addWidget(statusFrame, 0);

    // Control Buttons Frame — refined with layout and subtle colors
    QFrame *controlFrame = new QFrame(this);
    controlFrame->setMinimumWidth(350);
    controlFrame->setStyleSheet(
        "background-color: #ffffff;"
        "border: 1px solid #d1d5db;"
        "border-radius: 8px;"
        "padding: 15px;"
        "box-shadow: 0 2px 6px rgba(0,0,0,0.05);"
    );
    QVBoxLayout *controlLayout = new QVBoxLayout(controlFrame);
    controlLayout->setContentsMargins(10, 10, 10, 10);
    controlLayout->setSpacing(15);

    QPushButton *btnLockdown = new QPushButton("Activate Lockdown", controlFrame);
    QPushButton *btnQuarantine = new QPushButton("Isolate Threats", controlFrame);
    QPushButton *btnReset = new QPushButton("Reset Firewall Rules", controlFrame);

    auto styleButton = [](QPushButton *btn) {
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(40);
        btn->setFont(QFont("Segoe UI", 11, QFont::DemiBold));
        btn->setStyleSheet(
            "QPushButton {"
            "  background-color: #3b82f6;"
            "  color: white;"
            "  border: none;"
            "  border-radius: 6px;"
            "  padding: 10px;"
            "  transition: background-color 0.3s;"
            "}"
            "QPushButton:hover {"
            "  background-color: #2563eb;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #1d4ed8;"
            "}"
        );
    };

    styleButton(btnLockdown);
    styleButton(btnQuarantine);
    styleButton(btnReset);

    controlLayout->addWidget(btnLockdown);
    controlLayout->addWidget(btnQuarantine);
    controlLayout->addWidget(btnReset);

    mainLayout->addWidget(controlFrame, 0);

    // Horizontal separator
    QFrame *separator = new QFrame(this);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setStyleSheet("color: #d1d5db;");
    mainLayout->addWidget(separator);

    // Content Area: Rules Table and Threat Alerts
    QHBoxLayout *contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(25);

    // Rules Table (Advanced styling & interactivity)
    QTableWidget *rulesTable = new QTableWidget(this);
    rulesTable->setColumnCount(4);
    rulesTable->setHorizontalHeaderLabels(QStringList{"Rule Name", "Action", "Target", "Enabled"});
    rulesTable->horizontalHeader()->setStretchLastSection(true);
    rulesTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    rulesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    rulesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    rulesTable->setAlternatingRowColors(true);
    rulesTable->setStyleSheet(
        "QTableWidget {"
        "  background-color: #ffffff;"
        "  alternate-background-color: #f9fafb;"
        "  color: #1f2937;"
        "  border: 1px solid #d1d5db;"
        "  border-radius: 6px;"
        "}"
        "QHeaderView::section {"
        "  background-color: #e5e7eb;"
        "  padding: 8px;"
        "  border: 1px solid #d1d5db;"
        "  font-weight: 600;"
        "  color: #374151;"
        "}"
        "QTableWidget::item:selected {"
        "  background-color:#bfdbfe;"
        "  color: #1e40af;"
        "}"
    );
    rulesTable->setMinimumWidth(650);
    rulesTable->setMinimumHeight(350);
    contentLayout->addWidget(rulesTable, 3);

    // Threat Alerts Panel
    QFrame *alertsFrame = new QFrame(this);
    alertsFrame->setMinimumWidth(350);
    alertsFrame->setMinimumHeight(350);
    alertsFrame->setStyleSheet(
        "background-color: #ffffff;"
        "border: 1px solid #d1d5db;"
        "border-radius: 8px;"
        "padding: 15px;"
        "box-shadow: 0 2px 6px rgba(0,0,0,0.05);"
    );
    QVBoxLayout *alertsLayout = new QVBoxLayout(alertsFrame);
    alertsLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *alertsTitle = new QLabel("Active Threats", alertsFrame);
    alertsTitle->setFont(QFont("Segoe UI", 16, QFont::Bold));
    alertsTitle->setStyleSheet("color: #dc2626;");
    alertsLayout->addWidget(alertsTitle);

    QLabel *activeThreatsText = new QLabel("No active threats detected.", alertsFrame);
    activeThreatsText->setWordWrap(true);
    activeThreatsText->setFont(QFont("Segoe UI", 12));
    activeThreatsText->setStyleSheet("color: #b91c1c;");
    alertsLayout->addWidget(activeThreatsText);

    // Animated loading gif / icon for simulated activity (Optional)
    QLabel *loadingIcon = new QLabel(alertsFrame);
    QMovie *movie = new QMovie(":/resources/animated_fire_icon.gif");
    if (movie->isValid()) {
        loadingIcon->setMovie(movie);
        movie->start();
        alertsLayout->addWidget(loadingIcon);
    } else {
        loadingIcon->setText("[Activity visualization not available]");
        loadingIcon->setAlignment(Qt::AlignCenter);
        alertsLayout->addWidget(loadingIcon);
    }

    contentLayout->addWidget(alertsFrame, 1);

    mainLayout->addLayout(contentLayout);

    // Example: simulate some dynamic updates for security status and threats
    QTimer *updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [=]() {
        static bool toggle = false;
        toggle = !toggle;
        secStatusValue->setText(toggle ? "SAFE" : "UNDER ATTACK");
        secStatusValue->setStyleSheet(toggle ? "color: #10b981;" : "color: #ef4444;");
        activeThreatsText->setText(toggle ? "No active threats detected." :
            "Malicious packet detected from IP 192.168.1.34.");
    });
    updateTimer->start(4000);

    // Connect button placeholders for future logic
    connect(btnLockdown, &QPushButton::clicked, this, [=]() {
        activeThreatsText->setText("Lockdown activated: All suspicious network activity blocked.");
        secStatusValue->setText("LOCKDOWN");
        secStatusValue->setStyleSheet("color: #f59e0b;");
    });
    connect(btnQuarantine, &QPushButton::clicked, this, [=]() {
        activeThreatsText->setText("Quarantine mode: Isolating detected threats for analysis.");
        secStatusValue->setText("QUARANTINE");
        secStatusValue->setStyleSheet("color: #f43f5e;");
    });
    connect(btnReset, &QPushButton::clicked, this, [=]() {
        activeThreatsText->setText("Firewall rules reset to defaults.");
        secStatusValue->setText("SAFE");
        secStatusValue->setStyleSheet("color: #10b981;");
    });

    // Populate rules table with example data
    rulesTable->setRowCount(3);
    QStringList names = {"Allow HTTP", "Block FTP", "Allow DNS"};
    QStringList actions = {"Allow", "Block", "Allow"};
    QStringList targets = {"Port 80", "Port 21", "Port 53"};
    QStringList statuses = {"Enabled", "Disabled", "Enabled"};
    for (int i = 0; i < 3; ++i) {
        rulesTable->setItem(i, 0, new QTableWidgetItem(names[i]));
        rulesTable->setItem(i, 1, new QTableWidgetItem(actions[i]));
        rulesTable->setItem(i, 2, new QTableWidgetItem(targets[i]));
        QTableWidgetItem *statusItem = new QTableWidgetItem(statuses[i]);
        if (statuses[i] == "Enabled") {
            statusItem->setForeground(QColor("#059669"));
        } else {
            statusItem->setForeground(QColor("#b91c1c"));
        }
        rulesTable->setItem(i, 3, statusItem);
    }
}
