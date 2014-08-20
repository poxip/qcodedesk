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
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>

#include <iostream>
#include <vector>

#include "config.h"
#include "Topic.h"

#define XML_URL "http://cpp0x.pl/xml/"

#define SECONDS(a) a*1000
#define MINUTES(a) SECONDS(a*60)

#define MS_TO_SECONDS(a) a/1000
#define MS_TO_MINUTES(a) MS_TO_SECONDS(a)/60

class News : public QObject
{
    Q_OBJECT

public:
    News();

    /**
     * @brief fetches cpp0x.pl/xml and updates news data
     */
    void update();
    /**
     * @brief Checks for new posts
     * @return \b true if new posts otherwise \b false
     */
    inline bool newPosts()
    {
        return (document_has_changes && document_has_new_posts);
    }

    // Milliseconds
    static uint update_interval;
    std::vector <Topic> topics;

    // How many times news were updated
    std::size_t updates_count;

Q_SIGNALS:
    /**
     * @brief Emmited when the update triggered by update() has been finished
     * @param[in] success True if update was successfull, otherwise false
     */
    void updateFinished(bool success);

private:
    /**
     * @brief Parses XML and updates topics list
     * @param[i] data A XML data
     */
    inline void parse(const QString& data);
    bool _parse(const QString& data);

    QNetworkAccessManager *network_manager;

    // Notifications data
    // flags
    bool document_has_changes;
    bool document_has_new_posts;

    QString last_update_date;
    Topic last_first_topic;

private Q_SLOTS:
    void performParsing(QNetworkReply* reply);
};

#endif // NEWS_H
