//
// Created by Peter Janků on 02.11.2022.
//

#include "serialconnection.h"

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
} // KilobotOhcLib