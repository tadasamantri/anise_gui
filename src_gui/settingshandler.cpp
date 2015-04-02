#include "settingshandler.h"
#include "anisecommunicator.h"
#include "data.h"
#include <QDebug>

QString SettingsHandler::settingsFilePath;
QMap<QString, QString> SettingsHandler::tempSettings = QMap<QString, QString>();

// checks if setting with given key exists
bool SettingsHandler::contains(QString key) {
    return tempSettings.contains(key);
}

// Stores setting in .ini file. When storing setting ensure loading in function
// initializeSettings()
QString SettingsHandler::getSetting(QString key)
{
   return tempSettings.value(key);
}

void SettingsHandler::storeSetting(QString SettingsKey, QString SettingsValue) {

    tempSettings.insert(SettingsKey, SettingsValue);
    storeAll();

}

void SettingsHandler::loadAllSettings() {

    QSettings settings(settingsFilePath, QSettings::IniFormat);

    QStringList settingKeys = settings.allKeys();
    QString val;

    foreach (QString key, settingKeys) {
        val = QString(settings.value(key, "").toString());
        tempSettings.insert(key, val);
    }


}

void SettingsHandler::storeAll(){

    QSettings settings(settingsFilePath, QSettings::IniFormat);

    for(QString key : tempSettings.keys())
        settings.setValue(key, tempSettings.value(key));

    settings.sync();

}

/* To ensure initalization when GUI starts please copy if-case for your
 * implemented Setting.
 * Make sure your stored Setting is loaded by adding if-cause and internal
 * variable is set!
 */
void SettingsHandler::initializeSettings() {

    loadAllSettings();


    QMap<QString, QString> allSettings(tempSettings);

    QString value;

    if (allSettings.contains("frameworkpath")) {
        value = allSettings.take("frameworkpath");
        qDebug() << "initializing framework path: " << value;
        AniseCommunicator::setFrameworkPath(value);
    }

    if (allSettings.contains("autosave_interval")) {
        value = allSettings.take("autosave_interval");
        qDebug() << "initializing autosav_interval: " << value;
        Data::instance()->setAutosave_interval(value.toInt());
    }

    if (!allSettings.isEmpty())
        qWarning() << "Not All Settings have been loaded from .ini-data\n\n "
                      "Missing: ";  //+ allSettings; TO-DO SHOW MISSING PARAMETERS
}

void SettingsHandler::setSettingsPath(QString path) {
    SettingsHandler::settingsFilePath = path;
}
