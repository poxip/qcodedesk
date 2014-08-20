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
    loadSettings();

    createIcons();
    configureActions();
    createTrayIcon();
    connect(tray_icon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    configureGui();
    configureNews();

    tray_icon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Settings
void MainWindow::loadSettings()
{
    QSettings settings(SETTINGS_FILENAME, SETTINGS_FORMAT);
    qDebug() << "settings: loaded - keys: " << settings.allKeys();

    // load
    uint update_interval = settings.value(SKEY_UPDATE_INTERVAL, 1).toUInt();
    if (update_interval < 1) update_interval = 1;
    update_interval = MINUTES(update_interval); // convert to ms

    // set
    News::update_interval = update_interval;
    // Set interval only on settings reload
    if (news_timer)
        news_timer->setInterval(News::update_interval);

    ui->updateIntervalSpinBox->setValue(MS_TO_MINUTES(update_interval));
}

void MainWindow::saveSettings()
{
    QSettings settings(SETTINGS_FILENAME, SETTINGS_FORMAT);

    uint update_interval = ui->updateIntervalSpinBox->value();
    if (update_interval < 1) update_interval = 1;
    settings.setValue(SKEY_UPDATE_INTERVAL, update_interval);

    qDebug() << "settings: saved";
    ui->statusBar->showMessage("Zapisano", SECONDS(2));

    loadSettings();
}

void MainWindow::createIcons()
{
    topic_normal_icon.addFile(":/main/res/topic/normal.png");
}

void MainWindow::configureActions()
{
    ui->refreshButton->setDefaultAction(ui->actionRefresh);

    connect(ui->actionToggleWindow, SIGNAL(triggered()), this, SLOT(toggleWindow()));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(performNewsViewUpdate()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::configureGui()
{
    // Tree widget
    ui->treeWidget->header()->setMaximumSectionSize(256);
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(topicItemDoubleClicked(QTreeWidgetItem*, int)));

    // Save button
    connect(ui->saveSettingsButton, &QPushButton::clicked, [=]() {
        saveSettings();
    });

    configureStatusBar();
}

void MainWindow::configureStatusBar()
{
    status_message = new QLabel(tr("Gotowy"));
    ui->statusBar->insertWidget(0, status_message, 1);
}

void MainWindow::configureNews()
{
    performNewsViewUpdate();

    news_timer = new QTimer(this);
    news_timer->setInterval(News::update_interval);
    news_timer->start();
    connect(news_timer, SIGNAL(timeout()), SLOT(performNewsViewUpdate()));

    connect(&news, SIGNAL(updateFinished(bool)), this, SLOT(updateNewsView(bool)));
}

void MainWindow::createTrayIcon()
{
    // Tray icon context menu
    tray_icon_menu = new QMenu(this);
    tray_icon_menu->addAction(ui->actionToggleWindow);
    tray_icon_menu->addAction(ui->actionRefresh);
    tray_icon_menu->addSeparator();
    tray_icon_menu->addAction(ui->actionQuit);

    tray_icon = new qcd::TrayIcon(this);
    tray_icon->setContextMenu(tray_icon_menu);

    // Tray icon
    tray_icon->setIconImages(":/main/res/tray-icon/normal.png",
                            ":/main/res/tray-icon/notify.png");
    tray_icon->setState(qcd::TrayIcon::State::Normal);
}


bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    // Is it main window?
    if (object->objectName().contains(this->objectName()))
    {
        if (event->type() == QEvent::FocusIn)
            tray_icon->setState(qcd::TrayIcon::State::Normal);
    }

    return false;
}

// Slots
void MainWindow::closeEvent(QCloseEvent *e)
{
    this->hide();

    e->ignore();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason) {
    case QSystemTrayIcon::ActivationReason::DoubleClick:
        // BUGFIX issue #14
        if (isMinimized())
        {
            showNormal();
            activateWindow();

            break;
        }

        toggleWindow();
        break;
    default:
        ;
    }
}

void MainWindow::topicItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);

    QUrl url(item->data(TopicViewColumn::UrlData, Qt::UserRole).toString());
    QDesktopServices::openUrl(url);
}

void MainWindow::releaseRefreshAction()
{
    ui->actionRefresh->setEnabled(true);
}

void MainWindow::updateNewsView(bool success)
{
    if (!success)
    {
        std::cerr << "Cannot fetch or parse XML news document" << '\n';
        showStatusMessage(tr("Wystąpił błąd podczas pobierania "\
                                  "lub parsowania listy tematów"));

        ui->actionRefresh->setEnabled(true);
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

        QString url =  QString(FORUM_TOPIC_PAATTERN).arg(topic.id);
        item->setData(TopicViewColumn::UrlData, Qt::UserRole, url);
        item->setIcon(0, topic_normal_icon);

        ui->treeWidget->addTopLevelItem(item);
    }

    ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);

    // Notify about changes
    if (news.newPosts() && news.updates_count > 1)
    {
        try {
            Topic first_topic = news.topics.at(0);
            QString title = "#" + QString::number(first_topic.id);
            QString url = QString(FORUM_TOPIC_PAATTERN).arg(first_topic.id);

            tray_icon->notify(title, first_topic.title, QUrl(url));
        }
        catch (const std::out_of_range e) {
            std::cerr << "Out of Range Exception "\
                         "while getting data from parsed document: " << e.what() << '\n';

            return;
        }
    }

    qDebug() << "News: updated";

    QDateTime now = QDateTime::currentDateTime();
    showStatusMessage(tr("Ostatnia aktualizacja ") + now.toString("hh:mm:ss"));

    QTimer::singleShot(SECONDS(10), this, SLOT(releaseRefreshAction()));
}

void MainWindow::performNewsViewUpdate()
{
    ui->actionRefresh->setDisabled(true);
    showStatusMessage(tr("Aktualizuję.."));
    news.update();
}
