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

#include "qcd/TrayIcon.h"
#include "News.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    // Can be used as slot but even to simply toggle window
    void toggleWindow() { setVisible(!isVisible()); }
    // News update timeout slot
    void updateNewsView();

private:
    enum TopicViewColumn
    {
        Title,
        PostCount,
        LastUpdate
    };

    void createActions();
    void createTrayIcon();

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

    QTimer *news_timer;
    // Data
    News news;
};

#endif // MAINWINDOW_H
