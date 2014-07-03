/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 28, 2014
  * @brief  Topic data structure (Basing on cpp0x.pl news xml).
  */

#ifndef TOPIC_H
#define TOPIC_H

#include <QString>

typedef std::size_t Id;

struct Topic
{
    Id id;
    Id forum_section;

    QString update_date;
    std::size_t post_count;

    QString title;

    Topic()
        : id(0),
          forum_section(0),
          post_count(0)
    {}

    Topic(const Id id,
          const Id forum_section,
          const QString &title,
          const QString &update_date,
          const std::size_t post_count);
};

#endif // TOPIC_H
