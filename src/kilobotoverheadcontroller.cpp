//
// Created by Peter Janků on 02.11.2022.
//

#include "kilobotoverheadcontroller.h"
#include <QThread>


namespace KilobotOhcLib {


    KilobotOverheadController::KilobotOverheadController(QObject *parent) : QObject(parent) {

        serial_conn = new SerialConnection();

        connect(this, &KilobotOverheadController::open, serial_conn, &SerialConnection::open);
        connect(this, &KilobotOverheadController::sendCommad, serial_conn, &SerialConnection::sendCommand);
        connect(this, &KilobotOverheadController::close, serial_conn, &SerialConnection::close);
        connect(this, &KilobotOverheadController::port, serial_conn, &SerialConnection::setPort);
        connect(this, &KilobotOverheadController::sendFirmware, serial_conn, &SerialConnection::sendProgram);

        // Move connection to thread
        //serial_conn->moveToThread(&thread);
        //thread.start();


    }

    KilobotOverheadController::~KilobotOverheadController() {

    }

    void KilobotOverheadController::sendMessage(unsigned char type) {
        QByteArray packet(PACKET_SIZE, 0);
        if (type == COMMAND_STOP) {
            sending = false;
            packet[0] = PACKET_HEADER;
            packet[1] = PACKET_STOP;
            packet[PACKET_SIZE-1]=PACKET_HEADER^PACKET_STOP;
        } else  {
            if (sending) {
                sendMessage(COMMAND_STOP);
                return;
                //return;
            }

            if (type == COMMAND_LEDTOGGLE) {
                sending = false;
                packet[0] = PACKET_HEADER;
                packet[1] = PACKET_LEDTOGGLE;
                packet[PACKET_SIZE-1]=PACKET_HEADER^PACKET_LEDTOGGLE;
            } else {
                sending = true;
                packet[0] = PACKET_HEADER;
                packet[1] = PACKET_FORWARDMSG;
                packet[11] = type;
                packet[PACKET_SIZE-1]=PACKET_HEADER^PACKET_FORWARDMSG^type;
            }
        }

        //serial_conn->resetDelay();
        serial_conn->sendCommand(packet);

    }

    void KilobotOverheadController::stopSending() {
        //TODO: append implementation
    }

    void KilobotOverheadController::openConnection() {
        emit open();
    }

    void KilobotOverheadController::closeConnection() {
        emit close();
    }

    void KilobotOverheadController::setPort(QString portName) {
        emit port(portName);
    }

    void KilobotOverheadController::sendProgram(QString fileName) {
        emit sendFirmware(fileName);
    }
} // KilobotOhcLib