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


    QString update_date;
    std::size_t post_count;

    QString title;
    QString last_user;

    Topic()
        : id(0),
          forum_section(0),
          post_count(0)
    {}

    // Optional initializer
    Topic(const Id id,
          const Id forum_section,
          const QString &title,
          const QString &last_user,
          const QString &update_date,
          const uint post_count);
};

#endif // TOPIC_H
