#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tv_quart.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionConnect_toggled(bool arg1);
    void getInfo();
    void serial_connection_event(TV_qUart::TV_UART_CONNECT_EVENT_TypeDef event);

    void on_pushButton_clicked();

    void on_actionClearLog_triggered();

    void on_lineEdit_Send_returnPressed();

    void on_pushButton_send_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_send_cmd_clicked();

    void on_pushButton_demo_2_clicked();

    void on_pushButton_demo_1_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    unsigned char calcCheckSum(QByteArray data);
    void packetBuilder(uint8_t target_id, uint8_t packet_type, QByteArray payload);
    QByteArray payloadFormatter(QByteArray str);
    void transmit_data(QByteArray data);
    Ui::MainWindow *ui;
    TV_qUart *serial;

};

#endif // MAINWINDOW_H
