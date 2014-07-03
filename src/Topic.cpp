#include "Topic.h"

Topic::Topic(const Id id,
      const Id forum_section,
      const QString &title,
      const QString &update_date,
      const std::size_t post_count)
{
    // Initialization list looked really awful here, so:
    this->id            = id;
    this->forum_section = forum_section;
    this->title         = title;
    this->update_date   = update_date;
    this->post_count    = post_count;
}
