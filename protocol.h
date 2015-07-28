#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QSerialPort>

enum commands
{
    CMD_GENERATE_FREQ = 0x0A,
};

class protocol: public QObject
{
    Q_OBJECT
public:
    protocol(QObject *parent = 0);
    ~protocol();

signals:
    void new_module_data(QByteArray frme);
    void new_mesh_data(QByteArray frme);
    void portInfo(QString Info);
    void portError(QString error);

public slots:
    bool start(QString name, quint32 baud);
    bool stop(void);
    bool isStarted(void);
    void newDataAvailable(void);
    void sendData(enum commands command, QByteArray additional_data);

private:
    QSerialPort *port;
    QByteArray *frame;

private slots:
};

#endif // PROTOCOL_H
