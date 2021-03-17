#include "serialmcu.h"

SerialMCU::SerialMCU(QObject *parent) : QObject(parent)
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
        const char _size[] = {byte[0], byte[1], byte[2], byte[3], byte[4], byte[5], byte[6], byte[7], byte[8]};
        qDebug() << sum(byte) << byte.toHex() <<  endl;

    }
//    for(int i=0; byte.count() > i; i++)
//        qDebug() << "DEC RETURN" << byte[i]<< endl;
//        convert(int(byte[i]));
}
void SerialMCU::convert(unsigned int val)
{
    QString bits;
    unsigned int mask = 1 << 7;

    for(int i = 0; i <  8; i++)
    {
        if( (val & mask) == 0 )
            bits.append('0');
        else
            bits.append('1');

        mask  >>= 1;
    }
    qDebug() << bits << endl ;
}
