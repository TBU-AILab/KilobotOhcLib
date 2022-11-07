//
// Created by Peter Janků on 02.11.2022.
//

#ifndef KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H
#define KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H

#include <QObject>
#include "serialconnection.h"
#include "packet.h"

namespace KilobotOhcLib {
    #define COMMAND_STOP 250
    #define COMMAND_LEDTOGGLE 251
    class KilobotOverheadController: public QObject {
        Q_OBJECT
        public:
            explicit KilobotOverheadController(QObject *parent = 0);
            ~KilobotOverheadController();
            SerialConnection *serial_conn;
            void sendMessage(unsigned char type);

        private:
            void stopSending();
            bool sending = false;
    };

} // KilobotOhcLib

#endif //KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H
