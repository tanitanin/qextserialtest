#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new Serial(ui->listWidget);
    enumerator = new QextSerialEnumerator();
    connect(enumerator, SIGNAL(deviceDiscovered(const QextPortInfo &)),
               serial, SLOT(onDeviceDiscovered(const QextPortInfo &)));
    connect(enumerator, SIGNAL(deviceRemoved(const QextPortInfo &)),
               serial, SLOT(onDeviceRemoved(const QextPortInfo &)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete serial;
    delete enumerator;
}


void MainWindow::on_checkBox_toggled(bool checked)
{
    if(checked) {
        ui->lineEdit->text() = "";
        ui->lineEdit->setEnabled(true);
        ui->checkBoxLF->setEnabled(true);
    } else {
        ui->lineEdit->text() = "";
        ui->lineEdit->setEnabled(false);
        ui->checkBoxLF->setEnabled(false);
    }
}
//void MainWindow::on_pushButton_clicked() {
//}

void MainWindow::on_pushButtonConnect_clicked()
{
    if(ui->listWidget->selectedItems().isEmpty()) {
        return;
    }
    QString portName = ui->listWidget->selectedItems().first()->text();
    port = new QextSerialPort(portName);
    BaudRateType baud = BAUD9600;
    QString selectedBaud = ui->comboBoxBaud->currentText();
    if(selectedBaud=="9600")        baud = BAUD9600;
    else if(selectedBaud=="115200") baud = BAUD115200;

    DataBitsType dtype = DATA_8;
    QString selectedData = ui->comboBoxDataBit->currentText();
    if(selectedData=="8")      dtype = DATA_8;
    else if(selectedData=="7") dtype = DATA_7;

    ParityType parity = PAR_NONE;
    QString selectedParity = ui->comboBoxParity->currentText();
    if(selectedParity=="None")      parity = PAR_NONE;
    else if(selectedParity=="Even") parity = PAR_EVEN;
    else if(selectedParity=="Odd")  parity = PAR_ODD;
    StopBitsType stopbit = STOP_1;
    QString selectedStopbit = ui->comboBoxStopBit->currentText();
    if(selectedStopbit=="1")      stopbit = STOP_1;
    else if(selectedStopbit=="2") stopbit = STOP_2;

    port->setBaudRate(baud);
    port->setDataBits(dtype);
    port->setParity(parity);
    port->setStopBits(stopbit);

    port->open(QextSerialPort::ReadWrite);

    connect(port,SIGNAL(readyRead()),this,SLOT(on_serialPort_readyRead()));
    connect(port,SIGNAL(bytesWritten(qint64)),this,SLOT(on_serialPort_bytesWritten(qint64)));
    connect(port,SIGNAL(readChannelFinished()),this,SLOT(on_serialPort_readChannelFinished()));

    ui->comboBoxBaud->setEnabled(false);
    ui->comboBoxDataBit->setEnabled(false);
    ui->comboBoxParity->setEnabled(false);
    ui->comboBoxStopBit->setEnabled(false);
    ui->pushButtonStop->setEnabled(true);
    ui->pushButtonConnect->setEnabled(false);
}

void MainWindow::on_pushButtonStop_clicked()
{
    disconnect(port,SIGNAL(readyRead()),this,SLOT(on_serialPort_readyRead()));
    disconnect(port,SIGNAL(bytesWritten(qint64)),this,SLOT(on_serialPort_bytesWritten(qint64)));
    disconnect(port,SIGNAL(readChannelFinished()),this,SLOT(on_serialPort_readChannelFinished()));

    delete port;
    ui->comboBoxBaud->setEnabled(true);
    ui->comboBoxDataBit->setEnabled(true);
    ui->comboBoxParity->setEnabled(true);
    ui->comboBoxStopBit->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonConnect->setEnabled(true);
}

void MainWindow::on_serialPort_readChannelFinished() {
    disconnect(port,SIGNAL(readyRead()),this,SLOT(on_serialPort_readyRead()));
    disconnect(port,SIGNAL(bytesWritten(qint64)),this,SLOT(on_serialPort_bytesWritten(qint64)));
    disconnect(port,SIGNAL(readChannelFinished()),this,SLOT(on_serialPort_readChannelFinished()));

    delete port;
    ui->comboBoxBaud->setEnabled(true);
    ui->comboBoxDataBit->setEnabled(true);
    ui->comboBoxParity->setEnabled(true);
    ui->comboBoxStopBit->setEnabled(true);
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonConnect->setEnabled(true);
}

void MainWindow::on_serialPort_readyRead() {
    QByteArray data = port->readAll();
    QByteArray text = QByteArray::fromHex(data);
    QByteArray hex = data.toHex();

    ui->textEditCharRecv->moveCursor(QTextCursor::End);
    ui->textEditCharRecv->insertPlainText(text);
    ui->textEditCharRecv->moveCursor(QTextCursor::End);

    ui->textEditHexRecv->moveCursor(QTextCursor::End);
    ui->textEditHexRecv->insertPlainText(hex);
    ui->textEditHexRecv->moveCursor(QTextCursor::End);

    foreach(char byte, data) {
        ui->textEditDecRecv->moveCursor(QTextCursor::End);
        ui->textEditDecRecv->insertPlainText(QString::number(byte).append(" "));
        ui->textEditDecRecv->moveCursor(QTextCursor::End);
    }
}

void MainWindow::on_serialPort_bytesWritten(qint64 bytes) {
    const char byte = (qint8)bytes;

    ui->textEditHexSend->moveCursor(QTextCursor::End);
    ui->textEditHexSend->insertPlainText(QString(byte));
    ui->textEditHexSend->moveCursor(QTextCursor::End);

    ui->textEditDecSend->moveCursor(QTextCursor::End);
    ui->textEditDecSend->insertPlainText(QString::number(byte).append(" "));
    ui->textEditDecSend->moveCursor(QTextCursor::End);

    ui->textEditCharSend->moveCursor(QTextCursor::End);
    ui->textEditCharSend->insertPlainText(QString(byte));
    ui->textEditCharSend->moveCursor(QTextCursor::End);

}


void MainWindow::on_lineEdit_returnPressed()
{
    if(!ui->lineEdit->text().isNull()) {
        QString str = ui->lineEdit->text();
        if(ui->checkBoxLF->isChecked()) str.append('\n');
        qDebug() << "Input line: " << str.toStdString().c_str();
        port->write(str.toStdString().c_str());
        ui->lineEdit->setText("");
    }
}
