// ConfigManager.h
#pragma once
#include <QObject>
#include <QSettings>
#include <QProcess>

class ConfigManager : public QObject {
    Q_OBJECT
public:
    explicit ConfigManager(const QString &fileName, QObject *parent = nullptr) 
        : QObject(parent), m_settings(new QSettings(fileName, QSettings::IniFormat, this)) {}

    Q_INVOKABLE QString getValue(const QString &group, const QString &key, const QString &defaultValue = "") {
        return m_settings->value(group + "/" + key, defaultValue).toString();
    }

    Q_INVOKABLE void setValue(const QString &group, const QString &key, const QString &value) {
        m_settings->setValue(group + "/" + key, value);
    }

    // Executes system shell triggers defined in settings 
    void triggerShellCommand(const QString &settingKey, const QString &callerId) {
        QString cmdPattern = getValue("System", settingKey, "");
        if (cmdPattern.isEmpty()) return;
        
        // Replaces variables in configuration scripts, such as caller ID parameters 
        QString finalCommand = cmdPattern.replace("%caller%", callerId);
        
        #if defined(Q_OS_WIN)
            QProcess::startDetached("cmd.exe", QStringList() << "/c" << finalCommand);
        #else
            QProcess::startDetached("/bin/sh", QStringList() << "-c" << finalCommand);
        #endif
    }

private:
    QSettings *m_settings;
};