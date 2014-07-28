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

#include <iostream>
#include <vector>

#include "Topic.h"

#define XML_URL "http://cpp0x.pl/xml/"
#define SECONDS(a) a*1000

class News : public QObject
{
public:
    News();

    /**
     * @brief fetches cpp0x.pl/xml and updates news data
     * @return \b true if update was complete successfull, otherwise \b false
     */
    bool update();
    /**
     * @brief Checks for new posts
     * @return \b true if new posts otherwise \b false
     */
    inline bool newPosts()
    {
        return (document_has_changes && document_has_new_posts);
    }

    // Milliseconds
    static const unsigned int UPDATE_INTERVAL;
    std::vector <Topic> topics;

    // How many times news were updated
    std::size_t updates_count;

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

    // Notifications data
    // flags
    bool document_has_changes;
    bool document_has_new_posts;

    QString last_update_date;
    Topic last_first_topic;
};

#endif // NEWS_H
