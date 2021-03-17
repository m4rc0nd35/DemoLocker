#include "serialmcu.h"

bool SerialMCU::connectSerial()
{
    m_serial = new QSerialPort();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(m_serial->open(QIODevice::ReadWrite))
            qDebug() << QString("[DEVICE] ERROR %1").arg(m_serial->errorString()) << endl;

        if(info.vendorIdentifier() == 0x0403 && info.productIdentifier() == 0x6001)
        {
            m_serial->setPortName(info.portName());
            m_serial->setBaudRate(QSerialPort::Baud19200);
            m_serial->setDataBits(QSerialPort::Data8);
            m_serial->setParity(QSerialPort::NoParity);
            m_serial->setStopBits(QSerialPort::OneStop);
            m_serial->setFlowControl(QSerialPort::NoFlowControl);

            if(m_serial->open(QIODevice::ReadWrite))
            {
                connect(m_serial, SIGNAL(readyRead()), this, SLOT(readyRead()));
                qDebug() << QString("[SUCCESS] CONNECTED DEVICE PORT: %1").arg(info.portName()) << endl;
            }else
                qDebug() << "[CRITICAL] DISCONNECTED" << endl;
        }
    }

    sendCommand(STATUSCU16, 8);
}
void SerialMCU::sendCommand(COMMAND command, int address, MODULE module)
{
    // OPENCU16 ingnora MODULE, 1~16, 17~32...
    if(command == OPENCU16)
    {
        const char _bytes[] = {STX, char(address - 1), char(command), ETX, 0x00};
        _buffer.append(_bytes, 4);
        _buffer.append(sum(_bytes));
    }
    else if (command == OPENCU48) {
        const char _bytes[] = {STX, char(module), char(address - 1), char(command), ETX};
        _buffer.append(_bytes, 5);
        _buffer.append(sum(_bytes));
    }
    else if (command == STATUSCU16) {
        char _bytes[] = { 0x02, 0xF0, 0x32, 0x03, 0x27 };
        _buffer.append(_bytes, 5);
    }
    qDebug() << "openPort" << _buffer << endl;
    send();
}
void SerialMCU::send()
{
    m_serial->write(_buffer);
    m_serial->flush();
    _buffer.clear();
}
char SerialMCU::sum(const char _bytes[])
{
    int checkSum = 0;
    for(int i=0; (sizeof (_bytes) + 1) > i; i++)
        checkSum +=  _bytes[i];

    return char(checkSum & 0xFF);
}
void SerialMCU::readyRead()
{
    QByteArray byte = m_serial->readAll();
    if(byte.count() == 9)
    {
        switch (int(byte[1])) {
            case 0:
                qDebug() << "" << endl;
                break;

        }
        QList<int> port;
        port.append(convert(int(byte[3])));
        port.append(convert(int(byte[4])));
        _status_port.append(port);

        QList<int> sensor;
        sensor.append(convert(int(byte[5])));
        sensor.append(convert(int(byte[6])));
        _status_sensor.append(sensor);
    }

    Q_EMIT modelStatusPortChanged();
    qDebug() << "STATUS PORT:" << _status_port << byte << endl;
    qDebug() << "STATUS SENSOR:" << _status_sensor << endl;
}
QList<int> SerialMCU::convert(unsigned int val)
{
    QList<int> bits;
    unsigned int mask = 1 << 7;

    for(int i = 0; i <  8; i++)
    {
        if( (val & mask) == 0 )
            bits.push_front(0);
        else
            bits.push_front(1);

        mask  >>= 1;
    }
    return bits;
}
