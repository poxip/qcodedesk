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
    connect(ui->treeWidget, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(topicItemDoubleClicked(QTreeWidgetItem*, int)));
    connect(ui->refreshButton, SIGNAL(clicked()), this, SLOT(performNewsViewUpdate()));

    createIcons();
    configureActions();
    createTrayIcon();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/** \copydoc setupData */
void MainWindow::setupData()
{
    performNewsViewUpdate();
    configureNews();
}

void MainWindow::createIcons()
{
    topicNormalIcon.addFile(":/main/res/topic/normal.png");
}

void MainWindow::configureActions()
{
    ui->refreshButton->setDefaultAction(ui->actionRefresh);

    connect(ui->actionToggleWindow, SIGNAL(triggered()), this, SLOT(toggleWindow()));
    connect(ui->actionRefresh, SIGNAL(triggered()), this, SLOT(performNewsViewUpdate()));
    connect(ui->actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::configureNews()
{
    news_timer = new QTimer(this);
    news_timer->setInterval(News::UPDATE_INTERVAL);
    news_timer->start();
    connect(news_timer, SIGNAL(timeout()), SLOT(performNewsViewUpdate()));

    connect(&news, SIGNAL(updateFinished(bool)), this, SLOT(updateNewsView(bool)));
}

void MainWindow::createTrayIcon()
{
    // Tray icon context menu
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(ui->actionToggleWindow);
    trayIconMenu->addAction(ui->actionRefresh);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(ui->actionQuit);

    trayIcon = new qcd::TrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);

    // Tray icon
    trayIcon->setIconImages(":/main/res/tray-icon/normal.png",
                            ":/main/res/tray-icon/notify.png");
    trayIcon->setState(qcd::TrayIcon::State::Normal);
}


bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    // Is it main window?
    if (object->objectName().contains(this->objectName()))
    {
        if (event->type() == QEvent::FocusIn)
            trayIcon->setState(qcd::TrayIcon::State::Normal);
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

void MainWindow::updateNewsView(bool success)
{
    if (!success)
    {
        std::cerr << "Cannot fetch or parse XML news document" << '\n';
        ui->statusBar->showMessage(tr("Wystąpił błąd podczas pobierania "\
                                  "lub parsowania listy tematów"));

        ui->refreshButton->setDisabled(false);
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
        item->setIcon(0, topicNormalIcon);

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

            trayIcon->notify(title, first_topic.title, QUrl(url));
        }
        catch (const std::out_of_range e) {
            std::cerr << "Out of Range Exception "\
                         "while getting data from parsed document: " << e.what() << '\n';

            return;
        }
    }

    ui->statusBar->showMessage(tr("Zaktualizowano"));

    qDebug() << "News: updated";
    ui->actionRefresh->setDisabled(false);
}

void MainWindow::performNewsViewUpdate()
{
    ui->actionRefresh->setDisabled(true);
    news.update();
}
