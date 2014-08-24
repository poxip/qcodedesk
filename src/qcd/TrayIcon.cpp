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
#ifdef Q_OS_LINUX
    notify_init(APP_NAME);
#else
    // set default Qt's messageClicked() callback
    connect(this, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
#endif // Q_OS_LINUX
}

qcd::TrayIcon::~TrayIcon()
{
#ifdef Q_OS_LINUX
    notify_uninit();
#endif // Q_OS_LINUX
}

/** \copydoc setIconImages */
void qcd::TrayIcon::setIconImages(const QString &normalImagePath,
                                  const QString &notifyImagePath)
{
    icons[State::Normal].addFile(normalImagePath);
    icons[State::Notify].addFile(notifyImagePath);

    setState(State::Normal);
}

#ifdef Q_OS_LINUX
static void onNotifyClick(NotifyNotification* n, char* action, gpointer user_data)
{
    Q_UNUSED(action);

    qcd::TrayIcon::TrayNotifyData* tray_data = nullptr;

    if (user_data)
    {
        tray_data = static_cast<qcd::TrayIcon::TrayNotifyData*>(user_data);

        QUrl url(tray_data->url);
        bool success = QDesktopServices::openUrl(url);
        if (!success)
            qDebug() << "Unable to open url in browser. Maybe the url is corrupted. "\
                        "Specified url: " << url;

        tray_data->tray_icon->setState(qcd::TrayIcon::State::Normal);
    }

    // free user data
    if (user_data)
        free(user_data);

    notify_notification_close (n, NULL);
    g_object_unref(G_OBJECT(n));
}
#endif // Q_OS_LINUX

/** \copydoc notify */
void qcd::TrayIcon::notify(const QString& title, const QString& message, const QUrl url)
{
    if (!url.isEmpty() && !url.isValid())
    {
        qDebug() << "Url is invalid. Specified url: " << url;
        return;
    }

#ifdef Q_OS_LINUX
    NotifyNotification *popup = notify_notification_new(
                QSTRING_TO_CHAR(title),
                QSTRING_TO_CHAR(message),
                "mail-message-new");

    if (!url.isEmpty())
    {
        auto* tray_data = new TrayNotifyData(url, this);

        notify_notification_add_action(popup, "default", "default", onNotifyClick, (gpointer)tray_data, NULL);
        notify_notification_add_action(popup, "open-url", "Otwórz", onNotifyClick, (gpointer)tray_data, NULL);
    }

    notify_notification_show(popup, NULL);
    if (url.isEmpty())
        g_object_unref(G_OBJECT(popup));
#else
    url_to_open = QUrl(url);
    showMessage(title, message);
#endif // Q_OS_LINUX

    // Change icon to notify the user (if has not clicked the notify baloon)
    setState(State::Notify);
}

// Slots
/** \copydoc messageClicked */
void qcd::TrayIcon::messageClicked()
{
    QDesktopServices::openUrl(url_to_open);
    setState(State::Normal);
}
