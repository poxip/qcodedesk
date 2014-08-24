/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 28, 2014
  * @brief  Topic data structure (Basing on cpp0x.pl news xml).
  */

#ifndef TOPIC_H
#define TOPIC_H

#include <QtCore>

typedef uint Id;

struct Topic
{
    Id id;
    Id forum_section;


    QDateTime update_date;
    QString date_diff;
    std::size_t post_count;

    QString title;
    QString last_user;

    Topic()
        : id(0),
          forum_section(0),
          post_count(0)
    {}
};

#endif // TOPIC_H
