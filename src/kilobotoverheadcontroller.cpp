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
        connect(this, &KilobotOverheadController::command, serial_conn, &SerialConnection::sendMessage);

        connect(serial_conn, &SerialConnection::error, this, &KilobotOverheadController::error);
        connect(serial_conn, &SerialConnection::status, this, &KilobotOverheadController::status);


        // Move connection to thread
        //serial_conn->moveToThread(&thread);
        //thread.start();


    }

    KilobotOverheadController::~KilobotOverheadController() {

    }

    void KilobotOverheadController::sendMessage(unsigned char type) {
        emit command(type);
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