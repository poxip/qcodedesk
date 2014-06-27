#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDebug>

#include "qcd/TrayIcon.h"

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

private:
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
};

#endif // MAINWINDOW_H
