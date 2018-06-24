#include "mainwindow.h"
#include "ui_mainwindow.h"
#define appName "GPWatcher"
#include <QCompleter>
#include <QScrollBar>
typedef enum{
    a=0,
    b=1
}BOOL;

BOOL c;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
    serial = new TV_qUart();
    ui->setupUi(this);
    connect(serial,SIGNAL(serialConnectionEvent(TV_qUart::TV_UART_CONNECT_EVENT_TypeDef)),this,SLOT(serial_connection_event    (TV_qUart::TV_UART_CONNECT_EVENT_TypeDef)));
    connect(serial,SIGNAL(serialInfoIn         ()),                             this,SLOT(getInfo     ()));
    setWindowTitle(QString(appName)+QString("  disconnected"));
    serial->moveToThread(serial->thread());
    QStringList wordList = {"aaa","bbb"};
    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit_Send->setCompleter(completer);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::transmit_data(QByteArray data){
    serial->serialDataOut(data);
}

void MainWindow::getInfo(){
    QString str_orig;
    QString str_hex;
    QScrollBar *bar1 = ui->plainTextEdit_raw_orig->verticalScrollBar();
    QScrollBar *bar2 = ui->plainTextEdit_raw_hex->verticalScrollBar();
    int b1 = bar1->maximum() - bar1->value();
    int b2 = bar2->maximum() - bar2->value();
    while(1){
        TV_qUart::PENDING_RAW_DATA_TypeDef data = serial->popPendingInfo();
        if(data.time == 0)break;
        str_orig.append(data.string_orig);
        str_hex.append(data.string_hex);
    }
    ui->plainTextEdit_raw_orig->insertPlainText(str_orig);
    ui->plainTextEdit_raw_hex->insertPlainText(str_hex);
    if (b1<10)
        bar1->setValue(bar1->maximum());
    if (b2<10)
        bar2->setValue(bar2->maximum());
    ui->plainTextEdit_raw_orig->repaint();
    ui->plainTextEdit_raw_hex->repaint();
}

void MainWindow::serial_connection_event(TV_qUart::TV_UART_CONNECT_EVENT_TypeDef event){
    switch(event){
    case(TV_qUart::UART_CONNECT_SUCCESS):{
        ui->actionConnect->setText("DisConnect");
          qDebug() << "UART_CONNECT_SUCCESS";
          break;
    }
    case(TV_qUart::UART_CONNECT_FAIL):{
          qDebug() << "UART_CONNECT_FAIL";
          break;
    }
    case(TV_qUart::UART_CONNECT_CANCEL):{
          qDebug() << "UART_CONNECT_CANCEL";
          break;
    }
    case(TV_qUart::UART_CONNECT_ERROR_VALUE):{
          qDebug() << "UART_CONNECT_ERROR_VALUE";
          break;
    }
    case(TV_qUart::UART_DISCONNECTED):{
          qDebug() << "UART_DISCONNECTED";
          break;
    }
    case(TV_qUart::UART_DISCONNECTED_MANUALLY):{
          qDebug() << "UART_DISCONNECTED_MANUALLY";
          break;
    }
    case(TV_qUart::UART_DISCONNECT_DUE_TO_RESOURCE_ERROR):{
        qDebug() << "UART_DISCONNECT_DUE_TO_RESOURCE_ERROR";
        break;
    }
    }
    if(serial->isOpen()){
        ui->actionConnect->setChecked(true);
        const char parity[6] = {'N','E','O','S','M','U'};
        QString stopBits;
        switch(serial->stopBits()){
        case(1):{ stopBits = "1"; break; }
        case(2):{ stopBits = "2"; break; }
        case(3):{ stopBits = "1.5"; break; }
        default:{ stopBits = "U"; break; }
        }

        QString connectionInfo =
                QString("%1 %2 %3,%4,%5")
                .arg(serial->portName())
                .arg(serial->baudRate())
                .arg(parity[(unsigned char)serial->parity()])
                .arg(serial->dataBits())
                .arg(stopBits);
        ui->actionConnect->setToolTip(QString("Click to disconnect from %1").arg(connectionInfo) );
            setWindowTitle(QString(appName)+QString("  connected : %1").arg(connectionInfo));
    }else{
        ui->actionConnect->setChecked(false);
        ui->actionConnect->setToolTip(QString("Connect to ...."));
        setWindowTitle(QString(appName)+QString("  disconnected"));
    }
}

void MainWindow::on_actionConnect_toggled(bool arg1){
    serial->connectDialog(arg1);
}

void MainWindow::on_pushButton_clicked()
{
    QByteArray arr;
    arr.append(0x30);
    arr.append(0x31);
    arr.append(0x32);
    arr.append(0x33);
    serial->serialDataOut(arr);
}

void MainWindow::on_actionClearLog_triggered()
{
    ui->plainTextEdit_raw_orig->clear();
    ui->plainTextEdit_raw_hex->clear();
}
QByteArray MainWindow::payloadFormatter(QByteArray str){
    QByteArray arr;
    str = str.replace(" ","");
    str = str.replace(",","");
    str = str.replace("\r","");
    str = str.replace("\n","");
    for (int i = 0; i < str.length(); i += 2){
        QString sub = str.mid(i,2);
        bool ok;
        unsigned char num = sub.toInt(&ok,16);
        if (ok)
        arr.append((char)num);
    }
    return arr;
}
void MainWindow::on_pushButton_send_clicked()
{
    if(ui->checkBox_hex->isChecked()){
        QString str = ui->lineEdit_Send->text();
        transmit_data(payloadFormatter(str.toLatin1()));
    }else{
        transmit_data(ui->lineEdit_Send->text().toLatin1());
    }
    ui->lineEdit_Send->clear();
}


void MainWindow::on_lineEdit_Send_returnPressed()
{
    on_pushButton_send_clicked();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if(index>=3){
        ui->plainTextEdit_send_payload->setDisabled(true);
    }else{
        ui->plainTextEdit_send_payload->setDisabled(false);
    }
}

#define protocol_header_char 0x55
#define protocol_footer_char 0xAA
#define master_id 1
#define payloadBufferSize ((uint16_t)2048)
unsigned char MainWindow::calcCheckSum(QByteArray data){
    unsigned char res = 0;
    for(int i=0;i<data.length();i++){
        res += data.at(i);
    }
    return res;
}
void MainWindow::packetBuilder(uint8_t target_id, uint8_t packet_type, QByteArray payload){
    payload = payloadFormatter(payload);
    if(payload.length()>payloadBufferSize)
        return;

    QByteArray packet;
    if (payload.length()<128){
        packet.append(payload.length());
    }else{
        packet.append((payload.length()&0x7F)|0x80);
        packet.append((payload.length()>>7)&0x7F);
    }
    packet.push_front((char)master_id);
    packet.push_front((char)packet_type);
    packet.push_front((char)target_id);
    packet.append(payload);

    packet.append((char)calcCheckSum(packet));
    packet.append((char)protocol_footer_char);
    packet.push_front((char)protocol_header_char);

    transmit_data(packet);


}

void MainWindow::on_pushButton_send_cmd_clicked()
{
    QByteArray rawPayload;
    if(ui->comboBox->currentIndex()<3){
        rawPayload = ui->plainTextEdit_send_payload->toPlainText().toLatin1();
    }
    packetBuilder((unsigned char)ui->spinBox_target->value(),(unsigned char)ui->comboBox->currentIndex(),rawPayload);
}

void MainWindow::on_pushButton_demo_2_clicked()
{
    ui->comboBox->setCurrentIndex(0);
    ui->spinBox_target->setValue(2);
    ui->plainTextEdit_send_payload->setPlainText(QString("02, 00,00, 01, 01, 01, A7, F9, 11, 41"));
    ui->plainTextEdit_send_payload->repaint();
}

void MainWindow::on_pushButton_demo_1_clicked()
{
    ui->comboBox->setCurrentIndex(2);
    ui->spinBox_target->setValue(2);
    ui->plainTextEdit_send_payload->setPlainText(QString("03, 06, 07,\n\
0B, 00,\n\
00, 00,\n\
09, 00,\n\
0C, 02, 00, 00, 00, 01, 01, 00,\n\
03, 02, 00, 03, 01, 01, 03, 02,\n\
0D, 02, 00, 00, 00, 01, 04, 00,\n\
10, 03, 00, 00, 00, 01, 05, 01, 03, 02, 00,\n\
0F, 02, 00, 00, 00, 01, 06, 01,\n\
0E, 02, 00, 00, 00, 01, 07, 01,\n\
01, 01, 00, 03, 01,\n\
01, 01, 00, 03, 02,\n\
01, 01, 00, 04, 00,\n\
01, 01, 00, 05, 01,\n\
01, 01, 00, 06, 01,\n\
01, 01, 00, 07, 01,\n\
01, 01, 00, 08, 01"));
    ui->plainTextEdit_send_payload->repaint();
}
typedef union{ //最長應限制在4byte
  float value_real;
  unsigned char value_bytes[4];
}union_value_t;
void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    union_value_t v;
    v.value_real = arg1;
    QString res = QString("%1, %2, %3, %4")
            .arg(v.value_bytes[0],2,16,QChar('0'))
            .arg(v.value_bytes[1],2,16,QChar('0'))
            .arg(v.value_bytes[2],2,16,QChar('0'))
            .arg(v.value_bytes[3],2,16,QChar('0'));
    ui->lineEdit_to_hex->setText(res.toUpper());
}

//(unsigned char)data.data.at(i),2,16,QChar('0')
