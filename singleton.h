#ifndef SINGLETON_H
#define SINGLETON_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QDebug>
class Singleton : public QObject
{
    Q_OBJECT

public:
    explicit  Singleton(QObject *parent = nullptr);
    static QObject* createSingletonInstance(QQmlEngine *engine,  QJSEngine *scriptEngine);
    Q_INVOKABLE QString getInfo();

signals:
    void callme();

private:
   static Singleton *m_instance;
};

#endif // SINGLETON_H
