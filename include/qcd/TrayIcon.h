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

private:
    QIcon icons[2];
};
}

#endif // TRAYICON_H
