/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  Modificated QSystemTrayIcon\
  *         to be able to switching states (normal, notify).
  */

#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>
#include <QIcon>
#include <QDebug>

#include "config.h"

#ifdef __linux__
#include <libnotify/notify.h>
#endif

#define CONST_CHAR(qstring) qstring.toUtf8().constData()

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
     */
    void notify(const QString& title, const QString& message);

private:
    QIcon icons[2];
    // libnotify popup icon
    GdkPixbuf* notify_icon;
};
}

#endif // TRAYICON_H
