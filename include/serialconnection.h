//
// Created by Peter Janků on 02.11.2022.
//

#ifndef KILOBOTOHCLIB_SERIALCONNECTION_H
#define KILOBOTOHCLIB_SERIALCONNECTION_H
#include <QObject>
#include <QtSerialPort/QSerialPort>

namespace KilobotOhcLib {

    enum class SerialConnectionStatus{
        SC_Status_OK,
        SC_Status_Timeout
    };

    /**
     * @brief Class representing serial connection to kilobots by using OHC Controller.
     *
     */
    class SerialConnection :public QObject {
        Q_OBJECT
    public:
        explicit SerialConnection(QObject *parent = nullptr);

    signals:
        void readText(QString);
        void status(SerialConnectionStatus status, QString msg = "");
        void error(SerialConnectionStatus status, QString msg = "");
        void SendMsgsQueueState(bool);

    public slots:
        void sendCommand(const QByteArray &data);
        void open();

    private:
        QSerialPort *port;
        int m_waitTimeout = 20;
    };

} // KilobotOhcLib

#endif //KILOBOTOHCLIB_SERIALCONNECTION_H
