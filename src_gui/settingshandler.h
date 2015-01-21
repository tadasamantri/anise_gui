#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QSettings>
#include <QString>
#include <QMap>

class SettingsHandler {
private:
    static QString settingsFilePath;

public:
    static QString loadSetting(QString key);
    static void storeSetting(QString SettingsKey, QString SettingsValue);
    static void initializeSettings();
    static bool contains(QString key);
    static void setSettingsPath(QString path);
    static QMap<QString, QString> loadAllSettings();
};

#endif  // SETTINGSHANDLER_H

/*-----------ALL EXISTING KEYS (please keep it up to
 *date)---------------------------------
 *
 * #1 frameworkpath
 *
 *
 *
 */
