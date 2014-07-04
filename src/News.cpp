/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  MainWindow implementation.
  */

#include "News.h"

const std::size_t News::UPDATE_INTERVAL = 10000;

News::News()
{
    network_manager = new QNetworkAccessManager();
}

/** \copydoc fetchXml */
QString News::fetchXml() const
{
    QEventLoop *event_loop = new QEventLoop();
    QObject::connect(network_manager, SIGNAL(finished(QNetworkReply*)), event_loop, SLOT(quit()));

    QNetworkRequest request(QUrl(XML_URL));
    request.setHeader(QNetworkRequest::UserAgentHeader, "qCodeDesk devel test");

    QNetworkReply *reply = network_manager->get(request);
    event_loop->exec(QEventLoop::ExcludeUserInputEvents);

    QByteArray reply_data = reply->readAll();
    return reply_data;
}

/** \copydoc parse */
bool News::parse(const QString& data)
{
    topics.clear();

    QDomDocument doc("codedesk");
    if(!doc.setContent(data))
        return false;

    QDomElement doc_elem = doc.documentElement();

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
        topic.update_date   = e.attribute("UpdateDT");
        topic.post_count    = e.attribute("PostCount").toUInt();

        // Title
        QDomElement title_element = node.firstChildElement();
        if (title_element.isNull())
            return false;

        topic.title = title_element.text();
        topics.push_back(topic);
    }

    return true;
}

/** \copydoc update */
bool News::update()
{

}
