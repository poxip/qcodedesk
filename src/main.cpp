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
    QCoreApplication::setApplicationVersion(APP_VERSION);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    setMetadata();

    QCommandLineParser cargs;
    cargs.setApplicationDescription(APP_DESCRIPTION);
    cargs.addHelpOption();cargs.addVersionOption();
    QCommandLineOption minimalized_option(
        "minimalized",
        "Starts application minimalized to the tray"
    );
    cargs.addOption(minimalized_option);
    cargs.process(app);

    MainWindow window;
    app.installEventFilter(&window);
    // Prevent showing window if app've been launched on autorun
    if (!cargs.isSet(minimalized_option))
        window.show();

    return app.exec();
}
