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
public:
    enum COMMAND{
        STATUS16 = 0x30,
        OPENCU16,
        STATUSCU16,
        STATUSCU48  = 0x50,
        OPENCU48
    };

    enum MODULE{
        SLAVE1 = 0x00,
        SLAVE2,
        SLAVE3,
        SLAVE4,
        SLAVE5,
        SLAVE6,
        SLAVE7,
        SLAVE8,
        SLAVE9,
        SLAVE10,
        SLAVEALL
    };

    explicit SerialMCU(QObject *parent = nullptr);
    void sendCommand(COMMAND, int=1, MODULE module=SLAVE1);
    void send();
    char sum(const char[]);
    void convert(unsigned int palindrome);

public slots:
    void readyRead();

private:
    QSerialPort *m_serial;
    QByteArray _buffer;
    int m_timeoutRead = 10000;
};

#endif // SERIALMCU_H
