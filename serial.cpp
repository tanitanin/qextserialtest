#include "serial.h"

Serial::Serial()
{
}

Serial::Serial(QListWidget *lw) : lwidget(lw) {
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    foreach (QextPortInfo port, ports) {
        if(port.portName!="") {
            lw->addItem(port.portName);
        }
    }
}

void Serial::onDeviceDiscovered(const QextPortInfo &port) {
    lwidget->addItem(port.portName);
}
void Serial::onDeviceRemoved(const QextPortInfo &port) {
    delete (lwidget->findItems(port.portName,Qt::MatchExactly).first());
}
