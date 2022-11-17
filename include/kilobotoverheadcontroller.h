//
// Created by Peter Janků on 02.11.2022.
//

#ifndef KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H
#define KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H

#include <QObject>
#include <QThread>
#include "serialconnection.h"
#include "packet.h"

namespace KilobotOhcLib {
    #define COMMAND_STOP 250
    #define COMMAND_LEDTOGGLE 251

    class KilobotOverheadController : public QObject {
    Q_OBJECT
    public:
        explicit KilobotOverheadController(QObject *parent = 0);

        ~KilobotOverheadController();

        void sendMessage(unsigned char type);

    public slots:

        void openConnection();

        void closeConnection();

        void setPort(QString portName);

        void sendProgram(QString fileName);

    signals:

        void open();

        void close();

        void sendCommad(QByteArray cmd, bool wait);

        void port(QString portName);

        void sendFirmware(QString fileName);

        void command(unsigned char type);

        void status(SerialConnectionStatus status, QString msg = "");

        void error(SerialConnectionStatus status, QString msg = "");

    private:
        void stopSending();

        bool sending = false;
        QThread thread;
        SerialConnection *serial_conn;
    };

} // KilobotOhcLib

#endif //KILOBOTOHCLIB_KILOBOTOVERHEADCONTROLLER_H
