#include "Topic.h"

// Optional initializer
Topic::Topic(const Id id,
      const Id forum_section,
      const QString &title,
      const QString &last_user,
      const QString &update_date,
      const uint post_count)
{
    // Initialization list looked really awful here, so:
    this->id            = id;
    this->forum_section = forum_section;
    this->title         = title;
    this->last_user     = last_user;
    this->update_date   = update_date;
    this->post_count    = post_count;
}
