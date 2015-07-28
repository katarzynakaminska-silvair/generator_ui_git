#ifndef GENERATOR_H
#define GENERATOR_H

#include <QMainWindow>
#include "protocol.h"

namespace Ui {
class Generator;
}

class Generator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Generator(QWidget *parent = 0);
    ~Generator();

private:
    Ui::Generator *ui;
    protocol *protocol_inst;


public slots:
    void portStart(void);
    void protStop(void);
    void protRefresh(void);
    void send_freq(void);
    void setInfo(QString infoTxt);
    void setError(QString infoTxt);
};

#endif // GENERATOR_H
