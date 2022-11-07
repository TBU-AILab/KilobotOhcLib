//
// Created by Peter Janků on 02.11.2022.
//

#include "serialconnection.h"
#include <QtSerialPort/QSerialPortInfo>

namespace KilobotOhcLib {



    SerialConnection::SerialConnection(QObject *parent) : QObject(parent) {
        port = new QSerialPort;
    }

    /**
     * @brief Send one command by using already opened connection.
     *
     * According to a "wait" parameter, the function will wait till the data will be written.
     *
     * @param data the command to be sent
     * @param wait true/false if the sending should block following execution
     */
    void SerialConnection::sendCommand(const QByteArray &data, bool wait ) {
        port->write(data);
        if ((wait) && (!port->waitForBytesWritten(m_waitTimeout))) {
            emit error(SerialConnectionStatus::SC_Status_Timeout, "Timeout when user command sending!");
        }
    }

    void SerialConnection::open() {
        if (!port) return;
        if (port->isOpen())
            close();
        if (!port->portName().isEmpty()){
            port->setBaudRate(38400);
            port->setDataBits(QSerialPort::Data8);
            port->setParity(QSerialPort::NoParity);
            port->setStopBits(QSerialPort::OneStop);
            port->setFlowControl(QSerialPort::NoFlowControl);
            if (!port->open(QIODevice::ReadWrite)){

                emit status(SerialConnectionStatus::SC_Status_CannotOpenPort);
            }else{
                mode = SerialConnectionTransferMode::MODE_NORMAL;
            }
        }
    }

    /**
     * @brief Provide set of available ports
     * @return List of available ports names
     */
    QVector<QString> SerialConnection::enumerate() {
        auto list = QVector<QString>(QSerialPortInfo::availablePorts().size());

        for (auto pi: QSerialPortInfo::availablePorts()){
            list.push_back(pi.portName());
        }

        return list;
    }

    void SerialConnection::close() {
        if (!port) return;
        if (port->isOpen())
            port->close();
    }

    void SerialConnection::setPort(const QString portName) {
        close();
        port->setPortName(portName);
        open();
    }
} // KilobotOhcLib