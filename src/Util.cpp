#include "Util.h"

using namespace ut;

DateTimeDelta::DateTimeDelta(const QDateTime base, const QDateTime compared)
{
    calculate(base, compared);
}

/** \copydoc toString */
QString DateTimeDelta::toString()
{
    if (date.size() < 2)
        return "";

    QString result;
    switch(days) {
    case 0:
        result += QObject::tr("dzisiaj:") + " ";
        if (hours > 1)
            result += QString::number(hours) + "h" + " ";

        result += QString::number(minutes) + " min "; break;
        break;
    case 1:
        result += QObject::tr("wczoraj") + " ";
        result += QObject::tr("o godz.") + " " + date["base"].toString("hh:mm");
        break;
    default:
        result += QString::number(days) + " " + QObject::tr("dni") + " ";
    }

    (days != 1) ? result += QObject::tr("temu") : "";

    return result;
}

/** \copydoc calculate */
DateTimeDelta& DateTimeDelta::calculate(const QDateTime base, const QDateTime compared)
{
    date["base"]     = base;
    date["compared"] = compared;

    auto duration = (
         base.daysTo(compared)*HOURS(24) +
         base.time().msecsTo(compared.time())
    );
         duration /= 1000;

    this->seconds = duration % 60; duration /= 60;
    this->minutes = duration % 60; duration /= 60;
    this->hours   = duration % 24;
    this->days    = duration / 24;

    return *(this);
}
