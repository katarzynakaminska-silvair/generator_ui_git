#include "generator.h"
#include "ui_generator.h"
#include <QSerialPortInfo>
#include <QRegExp>
#include <QRegExpValidator>

Generator::Generator(QWidget *parent) :
    QMainWindow(parent),
    protocol_inst(new protocol()),
    ui(new Ui::Generator)
{
    ui->setupUi(this);

    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->CB_comList->addItem(info.portName());
        qDebug() <<info.portName() << info.description() << info.manufacturer() << info.serialNumber();
    }
    foreach (const quint32 baud, QSerialPortInfo::standardBaudRates()) {
        ui->CB_baudList->addItem(QString::number(baud));
        ui->CB_baudList->setCurrentText("57600");
    }

    QStringList SL_ext_atten;
    SL_ext_atten    << "0.0" << "0.5" << "1.0" << "1.5" << "2.0" << "2.5" << "3.0" << "3.5" << "4.0" << "3.5"<< "5.0" << "5.5" << "6.0" << "6.5" << "7.0" << "7.5" << "8.0" << "8.5" << "9.0"
                    << "9.5" << "10.0" << "10.5" << "11.0" << "11.5" << "12.0" << "12.5" << "13.0" << "13.5"<< "14.0" << "14.5" << "15.0" << "15.5" << "16.0" << "16.5" << "17.0" << "17.5" << "18.0"
                    << "18.5" << "19.0" << "19.5" << "20.0" << "20.5" << "21.0" << "21.5" << "22.0" << "22.5"<< "23.0" << "23.5" << "24.0" << "24.5" << "25.0" << "25.5" << "26.0" << "26.5" << "27.0"
                    << "27.5" << "28.0" << "28.5" << "29.0" << "29.5" << "30.0" << "30.5" << "31.0" << "31.5";
    ui->CB_extAtten->addItems(SL_ext_atten);
    SL_ext_atten.clear();
    SL_ext_atten << "0.0" << "3.0";
    ui->CB_int2Atten->addItems(SL_ext_atten);
    SL_ext_atten.clear();
    SL_ext_atten << "0.0" << "3.0" << "6.0" << "9.0";
    ui->CB_int1Atten->addItems(SL_ext_atten);

    QRegExp fRegex ("^[1]{0,1}[0-9]{4,4}[.]{1,1}[0-9]{0,6}$");
    QRegExpValidator *fValidator = new QRegExpValidator(fRegex, this);
    ui->LE_freq->setValidator(fValidator);


    connect(ui->PB_start,SIGNAL(clicked()),this,SLOT(portStart()));
    connect(ui->PB_stop,SIGNAL(clicked()),this,SLOT(protStop()));
    connect(ui->PB_refresh,SIGNAL(clicked()),this,SLOT(protRefresh()));
    connect(ui->PB_freq, SIGNAL(clicked()), this, SLOT(send_freq()));
    connect(protocol_inst, SIGNAL(portInfo(QString)), this, SLOT(setInfo(QString)));
    connect(protocol_inst, SIGNAL(portError(QString)), this, SLOT(setError(QString)));
}

Generator::~Generator()
{
    delete ui;
}

void Generator::portStart(void)
{
    if(protocol_inst->start(ui->CB_comList->currentText(),ui->CB_baudList->currentText().toUInt()))
    {
        setInfo("Device started.");
    }
    else
    {
        setError("Device not started.");
    }
}

void Generator::protStop(void)
{
    if(protocol_inst->stop())
    {
        setInfo("Device stopped.");
    }
    else
    {
        setError("Device is already stopped.");
    }
}

void Generator::protRefresh(void)
{
    if(!protocol_inst->isStarted())
    {
        ui->CB_comList->clear();
        ui->CB_baudList->clear();
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            ui->CB_comList->addItem(info.portName());
        }
        foreach (const quint32 baud, QSerialPortInfo::standardBaudRates()) {
            ui->CB_baudList->addItem(QString::number(baud));
        }
        ui->CB_baudList->setCurrentText("57600");
        setInfo("Device list refreshed.");
    }
    else
    {
        setError("Can't refresh device list.");
    }
}

void Generator::send_freq()
{
    bool valid_f = true;
    QByteArray BA_dataToSend;
    float f = 0.0f;
    double d = 0.0;
//    f= ui->LE_freq->text().toFloat();
    d= ui->LE_freq->text().toDouble();
    if( d> 10000.0 | d<1500.0)
    {
        valid_f = false;
        setError("Frequency out of range. Please choose frequency between 1500.0 to 10000.0 MHz.");
    }
    if(valid_f == true)
    {
//      QByteArray array(reinterpret_cast<const char*>(&f), sizeof(f));
        QByteArray array1(reinterpret_cast<const char*>(&d), sizeof(d));
//      BA_dataToSend.append(array);
        BA_dataToSend.append(array1);
//      BA_dataToSend.append(ui->CB_int1Atten->currentIndex());
        BA_dataToSend.append(ui->CB_int1Atten->currentIndex());
        BA_dataToSend.append(ui->CB_int2Atten->currentIndex());
        f= ui->CB_extAtten->currentText().toFloat();
        QByteArray array3(reinterpret_cast<const char*>(&f), sizeof(f));
        BA_dataToSend.append(array3);

        protocol_inst->sendData(CMD_GENERATE_FREQ, BA_dataToSend);
    }
}

void Generator::setInfo(QString infoTxt)
{
       ui->L_logs->setStyleSheet("QLabel {color : green; }");
       ui->L_logs->setText(infoTxt);
}

void Generator::setError(QString infoTxt)
{
       ui->L_logs->setStyleSheet("QLabel {color : red; }");
       ui->L_logs->setText(infoTxt);
}

