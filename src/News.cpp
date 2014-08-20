/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  MainWindow implementation.
  */

#include "News.h"

uint News::update_interval = MINUTES(1);

News::News()
{
    network_manager = new QNetworkAccessManager();
    updates_count = 0;
}


bool News::_parse(const QString& data)
{
    topics.clear();

    QDomDocument doc("codedesk");
    if(!doc.setContent(data))
        return false;

    QDomElement doc_elem = doc.documentElement();

    QString update_date;
    // read last update date
    try {
        auto info_node = doc_elem.elementsByTagName("Information").at(0);
        update_date = info_node.toElement().attribute("CreateDT");
    } catch (std::out_of_range e) {
        std::cerr << "Out of Range Exception "\
                     "while parsing XML news data: " << e.what() << '\n';

        return false;
    }
    document_has_changes = (update_date != last_update_date);
    last_update_date = update_date;

    // Parse all Topic elements
    QDomNodeList elements = doc_elem.elementsByTagName("Topic");
    for (int i = 0; i < elements.size(); ++i)
    {
        QDomNode node = elements.at(i);
        // Attribs
        QDomElement e = node.toElement();
        if (e.isNull())
            return false;

        // Parse topic
        Topic topic;
        topic.id            = e.attribute("Id").toUInt();
        topic.forum_section = e.attribute("ParentId").toUInt();
        topic.last_user     = e.attribute("LastUserNick");
        topic.update_date   = e.attribute("UpdateDT");
        topic.post_count    = e.attribute("PostCount").toUInt();

        // Title
        QDomElement title_element = node.firstChildElement();
        if (title_element.isNull())
            return false;

        topic.title = title_element.text();
        if (topic.title == "") topic.title = tr("Nienazwany");
        topics.push_back(topic);
    }

    Topic first_topic;
    try {
        first_topic = topics.at(0);
    }
    catch (const std::out_of_range e) {
        std::cerr << "Out of Range Exception "\
                     "while parsing XML news data: " << e.what() << '\n';

        return false;
    }
    document_has_new_posts = (first_topic.post_count != last_first_topic.post_count);
    last_first_topic = first_topic;

    // Successfull update
    ++updates_count;

    return true;
}

/** \copydoc parse */
void News::parse(const QString& data)
{
    Q_EMIT updateFinished(_parse(data));
}

void News::performParsing(QNetworkReply* reply)
{
    QByteArray data = reply->readAll();

    if (data.isNull())
        return;

    QtConcurrent::run(this, &News::parse, data);
}

/** \copydoc update */
void News::update()
{
    connect(network_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(performParsing(QNetworkReply*)));

    QNetworkRequest request(QUrl(XML_URL));
    request.setHeader(QNetworkRequest::UserAgentHeader, APP_NAME);
    network_manager->get(request);
}
