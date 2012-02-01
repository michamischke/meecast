/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c MeecastIf -a dbusadaptor.h:dbusadaptor.cpp com.meecast.applet.xml
 *
 * qdbusxml2cpp is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DBUSADAPTOR_H_1328122973
#define DBUSADAPTOR_H_1328122973

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.meecast.applet
 */
class MeecastIf: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.meecast.applet")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.meecast.applet\">\n"
"    <method name=\"SetCurrentData\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"station\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"temperature\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"temperature_hi\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"temperature_low\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"icon\"/>\n"
"      <arg direction=\"in\" type=\"u\" name=\"until_valid_time\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"current\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"lockscreen\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"last_update\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    MeecastIf(QObject *parent);
    virtual ~MeecastIf();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void SetCurrentData(const QString &station, const QString &temperature, const QString &temperature_hi, const QString &temperature_low, const QString &icon, uint until_valid_time, bool current, bool lockscreen, const QString &last_update);
Q_SIGNALS: // SIGNALS
};

#endif
