/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  Polls cpp0x.pl for rss changes and produces news info.
  */

#ifndef NEWS_H
#define NEWS_H

#include <QtNetwork>
#include <QUrl>
#include <QByteArray>
#include <QEventLoop>
#include <QString>
#include <QDebug>
#include <QApplication>
#include <vector>

#include "Topic.h"

#define XML_URL "http://cpp0x.pl/xml/"

class News : public QObject
{
public:
    News();

    /**
     * @brief polls cpp0x.pl/xml for XML file
     * @return A QString object filled with XML data (plain text)
     */
    QString fetchXml() const;
    /**
     * @brief fetches cpp0x.pl/xml and updates news data
     * @return \btrue if update was complete successfull, otherwise \bfalse
     */
    bool update();

    std::vector <Topic> topics;

private:
    QNetworkAccessManager *network_manager;
};

#endif // NEWS_H
