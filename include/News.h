/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  Polls cpp0x.pl for rss changes and produces news info.
  */

#ifndef NEWS_H
#define NEWS_H

#include <QString>
#include <vector>

class News
{
public:
    News();

    /**
     * @brief fetches cpp0x.pl/xml and updates news data
     * @return \btrue if update was complete successfull, otherwise \bfalse
     */
    bool update();

    std::vector <Topic> topics;
};

#endif // NEWS_H
