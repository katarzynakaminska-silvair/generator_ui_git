#include "protocol.h"
#include <QThread>

protocol::protocol(QObject *parent)
{
        port = new QSerialPort();
        frame = NULL;

        connect(port,SIGNAL(readyRead()),this,SLOT(newDataAvailable()));
}

protocol::~protocol()
{
    delete port;
}

bool protocol::start(QString name, quint32 baud)
{
#ifdef __APPLE__
    port->setPortName("/dev/tty." + name);
#else
    port->setPortName(name);
#endif
    port->setBaudRate(baud);
    port->setParity(QSerialPort::NoParity);
    port->setDataBits(QSerialPort::Data8);
    port->setFlowControl(QSerialPort::NoFlowControl);
    port->setStopBits(QSerialPort::OneStop);
    if(port->open(QIODevice::ReadWrite))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool protocol::stop()
{
    if(port->isOpen())
    {
        port->close();
        return true;
    }
    else
    {
        return false;
    }
}

bool protocol::isStarted()
{
    return port->isOpen();
}

void protocol::newDataAvailable()
{

}

void protocol::sendData(enum commands command, QByteArray additional_data)
{
    QByteArray command_frame;
    command_frame.append(command).append(additional_data);
    if(port->isOpen())
    {
        port->write(command_frame);
        emit portInfo("Command sent");
    }
    else
    {
        emit portError("Device is stopped!");
    }
}
