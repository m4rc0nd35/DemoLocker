#include "singleton.h"
Singleton *Singleton::m_instance=nullptr;

Singleton::Singleton(QObject *parent) : QObject(parent)
{
    qDebug() << "Singleton::Singleton" << endl;

}
QObject* Singleton::createSingletonInstance(QQmlEngine *engine, QJSEngine *scriptEngine){
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    if(m_instance==nullptr)
        m_instance = new Singleton;

    return m_instance;
}
QString Singleton::getInfo(){
    emit callme();
    return "Hi, i am from SingletonClass";
}
