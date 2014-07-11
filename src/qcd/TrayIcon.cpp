/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  TrayIcon implementation.
  */
#include "qcd/TrayIcon.h"

qcd::TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon(parent)
{
    // if __linux__ initialize libnotify
#ifdef __linux__
    notify_init(APP_NAME);
#endif
}

qcd::TrayIcon::~TrayIcon()
{
    notify_uninit();
}

/** \copydoc setIconImages */
void qcd::TrayIcon::setIconImages(const QString &normalImagePath,
                                  const QString &notifyImagePath)
{
    icons[State::Normal].addFile(normalImagePath);
    icons[State::Notify].addFile(notifyImagePath);

    setState(State::Normal);
}

/** \copydoc notify */
void qcd::TrayIcon::notify(const QString& title, const QString& message)
{
#ifdef __linux__
    NotifyNotification *popup = notify_notification_new(
                CONST_CHAR(title),
                CONST_CHAR(message),
                "default");
    notify_notification_show(popup, NULL);
    g_object_unref(G_OBJECT(popup));
#else
    showMessage(title, message);
#endif
}
