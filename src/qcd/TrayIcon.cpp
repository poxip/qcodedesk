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
#else
    // set default Qt's messageClicked() callback
    connect(this, SIGNAL(messageClicked()), this, SLOT(messageClicked()));
#endif
}

qcd::TrayIcon::~TrayIcon()
{
#ifdef __linux__
    notify_uninit();
#endif
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
void qcd::TrayIcon::notify(const QString& title, const QString& message, const char* url)
{
#ifdef __linux__
    NotifyNotification *popup = notify_notification_new(
                QSTRING_TO_CHAR(title),
                QSTRING_TO_CHAR(message),
                "mail-message-new");

    if (url)
    {
        // We need copy of user_data
        char* url_copy = strdup(url);

        notify_notification_add_action(popup, "default", "default", onNotifyClick, (gpointer)url_copy, NULL);
        notify_notification_add_action(popup, "open-url", "Otwórz", onNotifyClick, (gpointer)url_copy, NULL);
    }

    notify_notification_show(popup, NULL);
    if (!url)
        g_object_unref(G_OBJECT(popup));
#else
    url_to_open = QUrl(url);
    showMessage(title, message);
#endif

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

#ifdef __linux__
static void onNotifyClick(NotifyNotification* n, char* action, gpointer user_data)
{
    Q_UNUSED(action);

    if (user_data)
    {
        QUrl url((char*)user_data);
        bool success = QDesktopServices::openUrl(url);
        if (!success)
            std::cerr << "Unable to open url in browser. Maybe the url is corrupted. "\
                        "Specified url: " << QSTRING_TO_CHAR(url.url().toString());
    }

    // free user data
    if (user_data)
        free(user_data);

    notify_notification_close (n, NULL);
    g_object_unref(G_OBJECT(n));

    setState(State::Normal);
}
#endif
