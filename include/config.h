/**
  * @author MrPoxipol <michal.proszek@gmail.com>
  * @date   Jul 11, 2014
  * @brief  App config/data file
  */

#ifndef CONFIG_H
#define CONFIG_H

#define APP_NAME "qCodeDesk"
#define APP_VERSION "0.9-stable"
#define APP_DESCRIPTION "Written in C++ and Qt5 \
cpp0x.pl/codedesk app for Linux and Windows"

#define ORGANIZATION_NAME "mrpoxipol-dev"
#define ORGANIZATION_DOMAIN "poxip.ml"

// Settings ini keys
#define SETTINGS_FILENAME "config/config.ini"
#define SETTINGS_FORMAT QSettings::IniFormat // Native or Ini
// SKEY = settings key
#define SKEY_UPDATE_INTERVAL "update-interval"

#endif // CONFIG_H
