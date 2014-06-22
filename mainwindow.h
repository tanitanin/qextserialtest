#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qextserialport.h"
#include "qextserialenumerator.h"
#include "serial.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //void on_pushButton_clicked();
    void on_checkBox_toggled(bool checked);
    void on_pushButtonConnect_clicked();
    void on_pushButtonStop_clicked();
    void on_serialPort_readyRead();
    void on_serialPort_readChannelFinished();
    void on_serialPort_bytesWritten(qint64 bytes);
    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QextSerialEnumerator *enumerator;
    Serial *serial;
    QextSerialPort *port;
};

#endif // MAINWINDOW_H
