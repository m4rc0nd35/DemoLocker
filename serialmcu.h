#ifndef SERIALMCU_H
#define SERIALMCU_H

#include <QObject>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

#define STX 0x02
#define ETX 0x03

class SerialMCU: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<QList<int>> modelStatusPort MEMBER _status_port  NOTIFY modelStatusPortChanged)

public:
    enum COMMAND{
        STATUS16 = 0x30,
        OPENCU16,
        STATUSCU16,
        STATUSCU48  = 0x50,
        OPENCU48
    };

    enum MODULE{
        SLAVE1 = 0x00, SLAVE2, SLAVE3, SLAVE4, SLAVE5,
        SLAVE6, SLAVE7, SLAVE8, SLAVE9, SLAVE10, SLAVEALL
    };

    Q_INVOKABLE bool connectSerial();
    void sendCommand(COMMAND, int=1, MODULE module=SLAVE1);
    void send();
    char sum(const char[]);
    QList<int> convert(unsigned int);

public slots:
    void readyRead();

signals:
    void modelStatusPortChanged();

private:
    QSerialPort *m_serial;
    QByteArray _buffer;
    QList<QList<int>> _status_port;
    QList<QList<int>> _status_sensor;
    QList<int> status_port = {1, 2};
};

#endif // SERIALMCU_H
