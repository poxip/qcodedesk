/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  Application MainWindow.
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include <iostream>

#include "qcd/TrayIcon.h"
#include "News.h"

#define FORUM_TOPIC_PAATTERN "http://cpp0x.pl/forum/temat/?id=%1"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    // Hide window to irritate the user!
    void closeEvent(QCloseEvent *e);
    // Modified Qt quit()
    void customQuit();

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    // Can be used as slot but even to simply toggle window
    void toggleWindow() { setVisible(!isVisible()); }
    void topicItemDoubleClicked(QTreeWidgetItem* item, int column);
    // News update timeout slot
    void updateNewsView();

private:
    enum TopicViewColumn
    {
        Title,
        PostCount,
        LastUpdate,
        // Hidden
        UrlData
    };

    void createIcons();
    void createActions();
    void createTrayIcon();
    void createNewsThread();

    Ui::MainWindow *ui;

    // Tray
    qcd::TrayIcon *trayIcon;
    QMenu *trayIconMenu;
        // Icons
    QIcon stateIcon[2];
        // Actions
    QAction *windowToggleAction;
    QAction *updateAction;
    QAction *quitAction;

    QThread *news_thread;
    QTimer *news_timer;
    // Data
    News news;

    // Misc
    QIcon topicNormalIcon;
};

#endif // MAINWINDOW_H
