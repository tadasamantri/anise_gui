#include "settingshandler.h"
#include "anisecommunicator.h"
#include <QDebug>

QString SettingsHandler::settingsFilePath;

//checks if setting with given key exists
bool SettingsHandler::contains(QString key){

    QSettings settings(settingsFilePath, QSettings::IniFormat);

    return settings.contains(key);

}

//loads a setting from .ini file with given key
QString SettingsHandler::loadSetting(QString key){

    QSettings settings(settingsFilePath, QSettings::IniFormat);

    if(settings.contains(key))
        return settings.value(key, "").toString();
    return NULL;


}

//Stores setting in .ini file. When storing setting ensure loading in function initializeSettings()
void SettingsHandler::storeSetting(QString SettingsKey, QString SettingsValue){

    QSettings settings(settingsFilePath, QSettings::IniFormat);
    settings.setValue(SettingsKey, SettingsValue);

}


QMap<QString, QString> SettingsHandler::loadAllSettings(){

    QMap<QString, QString> values = QMap<QString, QString>();
    QSettings settings(settingsFilePath, QSettings::IniFormat);

    QStringList settingKeys = settings.allKeys();
    QString val;

    foreach(QString key , settingKeys){

        val = QString(settings.value(key,"").toString());
        values.insert(key, val);

    }

    return values;
}


/* To ensure initalization when GUI starts please copy if-case for your implemented Setting.
 * Make sure your stored Setting is loaded by adding if-cause and internal variable is set!
 */
void SettingsHandler::initializeSettings(){

    QMap<QString, QString> allSettings = SettingsHandler::loadAllSettings();

    QString value;


    if(allSettings.contains("frameworkpath")){

        value = allSettings.take("frameworkpath");
        qDebug() << "initializing framework path: " << value;
        AniseCommunicator::setFrameworkPath(value);


    }



    if(!allSettings.isEmpty())
        qWarning() << "Not All Settings have been loaded from .ini-data\n\n Missing: "; //+ allSettings; TO-DO SHOW MISSING PARAMETERS

}


void SettingsHandler::setSettingsPath(QString path){

    SettingsHandler::settingsFilePath = path;

}

