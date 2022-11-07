//
// Created by Peter Janků on 02.11.2022.
//

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>
#include "kilobotoverheadcontroller.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    for(const auto &pInfo:QSerialPortInfo::availablePorts()){
        ui->cbDevice->addItem(pInfo.portName());
    }
    ui->cbDevice->setCurrentText("");
    for(const auto &pBaudRate: QSerialPortInfo::standardBaudRates()){
        ui->cbBaudRate->addItem(QString("%1").arg(pBaudRate));
    }
    ui->teLog->setVisible(false);
    connect(ui->pbLedToggle, &QPushButton::clicked, this, &MainWindow::btnLedToggle);
}

MainWindow::~MainWindow() {
 delete ui;
}

void MainWindow::on_cbDevice_currentTextChanged(const QString &arg1)
{

}

void MainWindow::on_pushButton_clicked()
{
    ui->cbDevice->clear();
    for(const auto &pInfo:QSerialPortInfo::availablePorts()){
        ui->cbDevice->addItem(pInfo.portName());
    }
    //TODO: Refactor immediately
    on_cbDevice_activated(0);

}


void MainWindow::on_cbDevice_activated(int index)
{
    delete currentPort;
    QString selectedPortName = ui->cbDevice->itemText(index);
    for(const auto &pInfo:QSerialPortInfo::availablePorts()){
        if (pInfo.portName() == selectedPortName){
            currentPort = new QSerialPort(selectedPortName);
        }
    }
    if (currentPort){
        ui->pbBootLoader->setEnabled(true);
        ui->pbUpload->setEnabled(true);
        ui->teLog->append(QString("Port changed: %1").arg(currentPort->portName()));
    }
}

void MainWindow::btnLedToggle() {
    auto controller = new KilobotOhcLib::KilobotOverheadController(this);
    controller->serial_conn->setPort(ui->cbDevice->currentText());
    controller->serial_conn->open();
    controller->sendMessage(COMMAND_LEDTOGGLE);
    controller->serial_conn->close();
    qDebug() << "done";

}

