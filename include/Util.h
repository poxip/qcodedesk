/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Aug 24, 2014
  * @brief  Contains more and less useful stuff
  */

#ifndef UTIL_H
#define UTIL_H

#include <QtCore>

#define SECONDS(a) a*1000
#define MINUTES(a) SECONDS(a*60)
#define HOURS(a)   MINUTES(a*60)
#define DAYS(a)    HOURS(a*24)

#define MS_TO_SECONDS(a) a/1000
#define MS_TO_MINUTES(a) MS_TO_SECONDS(a)/60
#define MS_TO_HOURS(a)   MS_TO_MINUTES(a)/60
#define MS_TO_DAYS(a)    MS_TO_HOURS(a)/24

namespace ut {
class DateTimeDelta;
}

/**
 * @brief The DateTimeDelta class
 */
class ut::DateTimeDelta
{
public:
    quint64 days;
    quint64 hours;
    quint64 minutes;
    quint64 seconds;

    QMap <QString, QDateTime> date;

    DateTimeDelta()
        : days(0), hours(0), minutes(0), seconds(0)
    {}

    /**
     * @brief       Overloaded default constructor.
     *              Calculates time delta.
     * @param[i]    base Base QDateTime object
     * @param[i]    compared QDateTime object, that will be compared
     *              to \param base object
     */
    DateTimeDelta(const QDateTime base, const QDateTime compared);

    /**
     * @brief       Returns diffrence in cpp0x.pl format from forum
     * @return      QString formated in cpp0x.pl format
     */
    QString toString();

    /**
     * @brief       Calculates TimeDelta between
     *              \param base DateTime object and \param compared
     * @param[i]    base QDateTime object
     * @param[i]    compared QDateTime object,
     *              that will be compared to \param base object
     * @return      A reference to the DateTimeDelta object
     */
    DateTimeDelta& calculate(const QDateTime base, const QDateTime compared);
};

#endif // UTIL_H
