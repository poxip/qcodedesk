/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jun 26, 2014
  * @brief  Main application file.
  */

#include <QApplication>
#include "MainWindow.h"

#include "config.h"

void setMetadata()
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APP_NAME);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    a.installEventFilter(&w);
    w.show();

    return a.exec();
}
