/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  MainWindow implementation.
  */

#include "News.h"

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

/** \copydoc update */
bool News::update()
{

}
