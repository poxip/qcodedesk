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

    createIcons();
    createActions();
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
    updateNewsView();
    createNewsThread();
}

void MainWindow::createIcons()
{
    topicNormalIcon.addFile(":/main/res/topic/normal.png");
}

void MainWindow::createActions()
{
    windowToggleAction = new QAction(tr("Pokaż/Schowaj okno"), this);
    connect(windowToggleAction, SIGNAL(triggered()), this, SLOT(toggleWindow()));

    updateAction = new QAction(tr("Sprawdź aktualności"), this);
    connect(updateAction, SIGNAL(triggered()), this, SLOT(updateNewsView()));

    quitAction = new QAction(tr("Zamknij"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(customQuit()));
}

void MainWindow::createNewsThread()
{
    news_timer = new QTimer();
    news_timer->setInterval(News::UPDATE_INTERVAL);
    news_timer->start();
    connect(news_timer, SIGNAL(timeout()), SLOT(updateNewsView()));

    news_thread = new QThread(this);
    news_timer->moveToThread(news_thread);
    news_thread->start();
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

void MainWindow::customQuit()
{
    // Perform some neccessary tasks
    news_thread->exit();
    QApplication::quit();
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
    trayIcon->setState(qcd::TrayIcon::State::Normal);
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

void MainWindow::topicItemDoubleClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column);

    QUrl url(item->data(TopicViewColumn::UrlData, Qt::UserRole).toString());
    QDesktopServices::openUrl(url);
}

void MainWindow::updateNewsView()
{
    if (!news.update())
    {
        std::cerr << "Cannot fetch or parse XML news document" << '\n';
        ui->statusBar->showMessage(tr("Wystąpił błąd podczas pobierania "\
                                  "lub parsowania listy tematów"));

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
}
