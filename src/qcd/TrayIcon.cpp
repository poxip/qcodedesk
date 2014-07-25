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

    setState(State::Notify);
}

static void onNotifyClick(NotifyNotification* n, char* action, gpointer user_data)
{
    if (strcmp(action, "open-url") && user_data)
    {
        QUrl url((char*)user_data);
        bool success = QDesktopServices::openUrl(url);
        if (!success)
            qDebug() << "Unable to open url in browser. Maybe the url is corrupted. \
                        Specified url: " << url.url();
    }

    notify_notification_close (n, NULL);
    g_object_unref(G_OBJECT(n));
}

/** \copydoc notify */
void qcd::TrayIcon::notify(const QString& title, const QString& message, const char* url)
{
#ifdef __linux__
    NotifyNotification *popup = notify_notification_new(
                QSTRING_TO_CHAR(title),
                QSTRING_TO_CHAR(message),
                "mail-message-new");
    if (url)
    {
        notify_notification_add_action(popup, "default", "default", onNotifyClick, (gpointer)url, NULL);
        notify_notification_add_action(popup, "open-url", "Otwórz", onNotifyClick, (gpointer)url, NULL);
    }

    notify_notification_show(popup, NULL);
    if (!url)
        g_object_unref(G_OBJECT(popup));
#else
    showMessage(title, message);
#endif
}
