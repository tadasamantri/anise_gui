#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QSettings>
#include <QMap>

class SettingsHandler : public QObject{

    Q_OBJECT
private:

    static QString settingsFilePath;
    static QMap<QString, QString> tempSettings;

public:
    static QString getSetting(QString key);
    static void storeSetting(QString SettingsKey, QString SettingsValue);
    static void initializeSettings();
    static bool contains(QString key);
    static void setSettingsPath(QString path);
    static void loadAllSettings();
    static void storeAll();
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
