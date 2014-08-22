/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  Application MainWindow.
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QDateTime>
#include <QtGui>
#include <QtWidgets>

#include <functional>
#include <iostream>

#include "config.h"
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

    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    // Can be used as slot but even to simply toggle window
    void toggleWindow() { setVisible(!isVisible()); }
    void topicItemDoubleClicked(QTreeWidgetItem* item, int column);
    // News update slot
    void releaseRefreshAction();
    void performNewsViewUpdate();
    void updateNewsView(bool success, const QString &error_desc);


private:
    enum TopicViewColumn
    {
        Title,
        PostCount,
        LastUpdate,
        // Hidden
        UrlData
    };

    void loadSettings();
    void saveSettings();

    void createIcons();
    void createTrayIcon();
    void configureActions();
    void configureGui();
    void configureStatusBar();
    void configureNews();

    inline void showStatusMessage(const QString& message)
    {
        status_message->setText(message);
    }

    bool eventFilter(QObject* object, QEvent* event);

    Ui::MainWindow *ui;
    QLabel *status_message;

    // Tray
    qcd::TrayIcon *tray_icon;
    QMenu *tray_icon_menu;
        // Icons
    QIcon state_icon[2];

    QTimer *news_timer;
    // Data
    News news;

    // Misc
    QIcon topic_normal_icon;
};

#endif // MAINWINDOW_H
