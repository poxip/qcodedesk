/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  Modificated QSystemTrayIcon\
  *         to be able to switching states (normal, notify).
  */

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QtCore>
#include <QDesktopServices>
#include <QIcon>
#include <QDebug>

#include "config.h"

#ifdef __linux__
#include <libnotify/notify.h>
#endif

#define QSTRING_TO_CHAR(qstring) qstring.toUtf8().constData()

namespace qcd {
class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    enum State
    {
        Normal,
        Notify
    };

    explicit TrayIcon(QObject *parent = 0);
    ~TrayIcon();

    /**
     * @brief Sets icons, which will be used to\
     *        represent actual state of TrayIcon.
     * @param[in] normalImagePath Normal TrayIcon image filename.
     * @param[in] notifyImagePath Notify TrayIcon image filename.
     */
    void setIconImages(const QString &normalImagePath,
                       const QString &notifyImagePath);

    /**
     * @brief Sets the TrayIcon state (normal, notify)
     * @param[in] state qcd::TrayIcon::State value
     */
    inline void setState(const State state)
    {
        setIcon(icons[state]);
    }

    /**
     * @brief Crossplatform notification, uses libnotify for Linux\
     *        and default Qt showMessage() for Windows and others.
     * @param[in] title Notify message title
     * @param[in] message Notify message
     * @param[in] url Optional url to open in browser on user click
     */
    void notify(const QString& title, const QString& message, const char* url = nullptr);

private Q_SLOTS:
    /**
     * @brief A callback for default notifications - Qt's showMessage()
     */
    void messageClicked()
    {
        QDesktopServices::openUrl(url_to_open);
    }

private:
    QIcon icons[2];
    // For default Qt showMessage() urls
    QUrl url_to_open;
};
}

#endif // TRAYICON_H
