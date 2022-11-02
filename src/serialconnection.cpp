//
// Created by Peter Janků on 02.11.2022.
//

#include "serialconnection.h"
#include <QtSerialPort/QSerialPortInfo>

namespace KilobotOhcLib {
    SerialConnection::SerialConnection(QObject *parent) : QObject(parent) {

    }

    void SerialConnection::sendCommand(const QByteArray &data) {
        port->write(data);
        if (!port->waitForBytesWritten(m_waitTimeout)) {
            emit error(SerialConnectionStatus::SC_Status_Timeout, "Timeout when user command sending!");
        }
    }

    void SerialConnection::open() {

    }

    QVector<std::pair< QString,  QString>> SerialConnection::enumerate() {
        auto list = QVector<std::pair<QString, QString>>(QSerialPortInfo::availablePorts().size());

        for (auto pi: QSerialPortInfo::availablePorts()){
            list.emplace_back(pi.portName(), pi.systemLocation());
        }

        return list;
    }
} // KilobotOhcLib