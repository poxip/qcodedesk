/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  MainWindow implementation.
  */

#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->header()->setMaximumSectionSize(256);

    createActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    news_timer = new QTimer(this);
    connect(news_timer, SIGNAL(timeout()), SLOT(updateNewsView()));
    news_timer->start(News::UPDATE_INTERVAL);
    // And trigger
    updateNewsView();

    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createActions()
{
    windowToggleAction = new QAction(tr("Pokaż/Schowaj okno"), this);
    connect(windowToggleAction, SIGNAL(triggered()), this, SLOT(toggleWindow()));

    updateAction = new QAction(tr("Sprawdź aktualności"), this);
    connect(updateAction, SIGNAL(triggered()), this, SLOT(toggleWindow()));

    quitAction = new QAction(tr("Zamknij"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon()
{
    // Tray icon context menu
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(windowToggleAction);
    trayIconMenu->addAction(updateAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new qcd::TrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    // Tray icon
    trayIcon->setIconImages(":/main/res/tray-icon/normal.png",
                            ":/main/res/tray-icon/notify.png");
    trayIcon->setState(qcd::TrayIcon::State::Notify);
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::ActivationReason::Trigger:
    case QSystemTrayIcon::ActivationReason::DoubleClick:
        toggleWindow();
        break;
    default:
        ;
    }
}

void MainWindow::updateNewsView()
{
    if (!news.update())
    {
        trayIcon->showMessage("Wystąpił błąd",
                              "Nie można pobrać aktualnej listy tematów lub jest ona błędna.");
        return;
    }

    ui->treeWidget->clear();
    for (auto topic : news.topics)
    {
        // Create temporary item
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(TopicViewColumn::Title, topic.title);
        item->setText(TopicViewColumn::PostCount, QString::number(topic.post_count));
        item->setText(TopicViewColumn::LastUpdate, topic.update_date);

        ui->treeWidget->addTopLevelItem(item);
    }

    ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);
}
