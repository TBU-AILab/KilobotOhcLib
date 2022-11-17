//
// Created by Peter Janků on 02.11.2022.
//

#include "serialconnection.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

namespace KilobotOhcLib {

    static unsigned char buf[4096];
    static QByteArray packet(PACKET_SIZE, 0);

    SerialConnection::SerialConnection(QObject *parent) : QObject(parent) {
        port = new QSerialPort(this);
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
        if (port->isOpen()) {
            port->write(data);
            if ((wait) && (!port->waitForBytesWritten(m_waitTimeout))) {
                emit error(SerialConnectionStatus::SC_Status_Timeout, "Timeout when user command sending!");
            }
        } else {
            emit error(SerialConnectionStatus::SC_Status_CannotOpenPort, "Port is not opened");
            mode = SerialConnectionTransferMode::MODE_NORMAL;
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
            if (!port->open(QIODevice::ReadWrite)) {
                emit error(SerialConnectionStatus::SC_Status_CannotOpenPort,
                           tr("Cannot open %1 port").arg(port->portName()));
            }else {
                mode = SerialConnectionTransferMode::MODE_NORMAL;
                emit status(SerialConnectionStatus::SC_Status_CannotOpenPort,
                            tr("Port %1 opened.").arg(port->portName()));
            }
        }
    }

    /**
     * @brief Provide set of available ports
     * @return List of available ports names
     */
    QVector<QString> SerialConnection::enumerate() {
        auto list = QVector<QString>();

        for (const auto &pi: QSerialPortInfo::availablePorts()) {
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
        bool openned = false;
        if (port->isOpen()) {
            close();
            openned = true;
        }
        port->setPortName(portName);
        if (openned)
            open();
    }


    //TODO: Check if it's necessery to controll the connection
    void SerialConnection::sendProgram(QString file) {
        data.load(file.toStdString());
        page_total = data.size() / PAGE_SIZE + 1;
        if (page_total > 220)
            page_total = 220;
        page = page_total;
        qDebug() << "Total pages..." << page_total;
        qDebug() << "Packet size .. " << packet.size();
        if (mode != SerialConnectionTransferMode::MODE_UPLOAD) {
            mode = SerialConnectionTransferMode::MODE_UPLOAD;
            delay.setSingleShot(true);
            delay.start(130);
            QMetaObject::invokeMethod(this, "programLoop", Qt::QueuedConnection);
        }

    }

    void SerialConnection::programLoop() {
        if (delay.remainingTime() == 0) {
            if (page >= page_total) {
                page = 0;
                packet.fill(0);
                packet[0] = PACKET_HEADER;
                packet[1] = PACKET_FORWARDMSG;
                packet[2] = page_total;
                packet[11] = BOOTPGM_SIZE;
                packet[PACKET_SIZE - 1] = PACKET_HEADER ^ PACKET_FORWARDMSG ^ page_total ^ BOOTPGM_SIZE;
                qDebug() << packet;

                //TODO: Sent packet and check aoumt of bytes already sent

            } else {
                packet[0] = PACKET_HEADER;
                packet[1] = PACKET_BOOTPAGE;
                packet[2] = page;
                uint8_t checksum = PACKET_HEADER ^ PACKET_BOOTPAGE ^ page;
                uint8_t data_byte;
                for (int i = 0; i < PAGE_SIZE; i++) {
                    data_byte = data.get(page * PAGE_SIZE + i);
                    packet[i + 3] = data_byte;
                    checksum ^= data_byte;
                }
                packet[PACKET_SIZE - 1] = checksum;
                //TODO: Sent packet and check aoumt of bytes already sent
                page++;
            }

            sendCommand(packet, true);

            emit status(SerialConnectionStatus::SC_Status_OK, QString("FW Page sent: %1").arg(page));

            delay.start(300);
        }

        if (mode == SerialConnectionTransferMode::MODE_UPLOAD) {
            QMetaObject::invokeMethod(this, "programLoop", Qt::QueuedConnection);
        }
    }


} // KilobotOhcLib
void KilobotOhcLib::SerialConnection::sendMessage(unsigned char type) {
    QByteArray packet(PACKET_SIZE, 0);
    if (type == COMMAND_STOP) {
        mode = SerialConnectionTransferMode::MODE_NORMAL;
        packet[0] = PACKET_HEADER;
        packet[1] = PACKET_STOP;
        packet[PACKET_SIZE - 1] = PACKET_HEADER ^ PACKET_STOP;
    } else {
        if (mode != SerialConnectionTransferMode::MODE_NORMAL) {
            sendMessage(COMMAND_STOP);
            return;
            //return;
        }
        if (type == COMMAND_LEDTOGGLE) {
            mode = SerialConnectionTransferMode::MODE_NORMAL;
            packet[0] = PACKET_HEADER;
            packet[1] = PACKET_LEDTOGGLE;
            packet[PACKET_SIZE - 1] = PACKET_HEADER ^ PACKET_LEDTOGGLE;
        } else {
            mode = SerialConnectionTransferMode::MODE_COMMAND;
            packet[0] = PACKET_HEADER;
            packet[1] = PACKET_FORWARDMSG;
            packet[11] = type;
            packet[PACKET_SIZE - 1] = PACKET_HEADER ^ PACKET_FORWARDMSG ^ type;
        }
    }

    //serial_conn->resetDelay();
    sendCommand(packet);
}
