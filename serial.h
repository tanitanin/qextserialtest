#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QListWidget>

#include "qextserialport.h"
#include "qextserialenumerator.h"

class Serial;

class Serial : public QObject
{
    Q_OBJECT
public:
    Serial();
    Serial(QListWidget *lw);
    QListWidget *lwidget;
signals:

public slots:
    void onDeviceDiscovered(const QextPortInfo &port);
    void onDeviceRemoved(const QextPortInfo &port);
};

#endif // SERIAL_H
