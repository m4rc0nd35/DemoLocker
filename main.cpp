#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <serialmcu.h>
#include <QDebug>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

//    int checkSum = 0;
//    char bytes[] = { 0x02, 0x00, 0x00, 0x51, 0x03};

//    qDebug() << bytes<< endl;
//    for(unsigned char b : bytes){
//        checkSum += b;
//        qDebug() << "checkSum" << checkSum<< endl;
//    }

//    qDebug() << char(checkSum & 0xFF) << endl;

    SerialMCU *control = new SerialMCU();
    return app.exec();
}
