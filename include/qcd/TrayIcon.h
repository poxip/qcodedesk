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
    void setIconImages(const QString &normalImagePath,
                       const QString &notifyImagePath);

    inline void setState(const State state)
    {
        setIcon(icons[state]);
    }

private:
    QIcon icons[2];
};
}

#endif // TRAYICON_H
