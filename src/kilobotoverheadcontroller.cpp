//
// Created by Peter Janků on 02.11.2022.
//

#include "kilobotoverheadcontroller.h"
#include <QThread>


namespace KilobotOhcLib {


    KilobotOverheadController::KilobotOverheadController(QObject *parent) : QObject(parent) {

        serial_conn = new SerialConnection();





        // Create thread
        QThread *thread = new QThread();
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        // Move connection to thread
        //serial_conn->moveToThread(thread);



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
                this->stopSending();
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
} // KilobotOhcLib