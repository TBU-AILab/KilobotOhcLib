//
// Created by Peter Janků on 02.11.2022.
//

#ifndef KILOBOTOHCLIB_MAINWINDOW_H
#define KILOBOTOHCLIB_MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow: public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_cbDevice_currentTextChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_cbDevice_activated(int index);

private:
    Ui::MainWindow *ui;
    QSerialPort *currentPort = nullptr;
};


#endif //KILOBOTOHCLIB_MAINWINDOW_H
