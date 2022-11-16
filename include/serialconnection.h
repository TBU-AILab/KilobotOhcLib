//
// Created by Peter Janků on 02.11.2022.
//

#ifndef KILOBOTOHCLIB_SERIALCONNECTION_H
#define KILOBOTOHCLIB_SERIALCONNECTION_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QVector>
#include <QTimer>

#include "intelhex.h"
#include "packet.h"


namespace KilobotOhcLib {

    enum class SerialConnectionStatus {
        SC_Status_OK,
        SC_Status_Timeout,
        SC_Status_CannotOpenPort
    };

    enum class  SerialConnectionTransferMode{
        MODE_NORMAL = 0,
        MODE_UPLOAD = 0x01,
        MODE_DOWNLOAD = 0x02
    };

    /**
     * @brief Class representing serial connection to kilobots by using OHC Controller.
     *
     */
    class SerialConnection :public QObject {
        Q_OBJECT
    public:
        explicit SerialConnection(QObject *parent = nullptr);
        static QVector<QString> enumerate();
    signals:
        void readText(QString);
        void status(SerialConnectionStatus status, QString msg = "");

        void error(SerialConnectionStatus status, QString msg = "");

        void SendMsgsQueueState(bool);

    public slots:

        void setPort(const QString portName);

        void sendCommand(const QByteArray &data, bool wait = true);

        void open();

        void close();

        void sendProgram(QString file);

        void programLoop();

    protected:
    private:

        QSerialPort *port;
        int m_waitTimeout = 20;
        SerialConnectionTransferMode mode = SerialConnectionTransferMode::MODE_NORMAL;
        intelhex::hex_data data;    /**< Loaded firmware in hexa format */
        int page_total = 0;         /**< Amount of pages to be send when FW is uploading. */
        int page = 0;               /**< Current page */
        QTimer delay;
    };

} // KilobotOhcLib

#endif //KILOBOTOHCLIB_SERIALCONNECTION_H
