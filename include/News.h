/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  Polls cpp0x.pl for rss changes and produces news info.
  */

#ifndef NEWS_H
#define NEWS_H

#include <QtCore>
#include <QtNetwork>
#include <QtXml>
#include <vector>

#include "Topic.h"

#define XML_URL "http://cpp0x.pl/xml/"

class News : public QObject
{
public:
    News();

    /**
     * @brief fetches cpp0x.pl/xml and updates news data
     * @return \b true if update was complete successfull, otherwise \b false
     */
    bool update();

    static const std::size_t UPDATE_INTERVAL;
    std::vector <Topic> topics;

private:
    /**
     * @brief polls cpp0x.pl/xml for XML file
     * @return A QString object filled with XML data (plain text)
     */
    QString fetchXml() const;
    /**
     * @brief Parses XML and updates topics list
     * @param[i] data A XML data
     * @return \b true if parsing was complete successfull, otherwise \b false
     */
    bool parse(const QString& data);

    QNetworkAccessManager *network_manager;
};

#endif // NEWS_H
