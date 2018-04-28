/*================================================================
*   Copyright (C) 2018 Vertiv Tech Co., Ltd. All rights reserved.
*   
*   文件名称：dialog.cpp
*   创 建 者：yunweilian
*   创建日期：2018年02月26日16时
*   描    述：对话框类的实现
*
================================================================*/

#include <QtWidgets/QAction>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QScrollBar>
#include <QtCore/QDebug>  
#include "widgets.h"
#include <ctype.h>

MyQWidget::MyQWidget(QWidget *parent):QWidget(parent)
{
   // 初始化组件
   init_component();
   //设置无边框窗口，且置顶 
   //this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
   
   init_port();

   //启动鼠标跟踪事件
   this->setMouseTracking(true);

   /**
    * @name 设置右上角图标
    * @{ */ 
    QPixmap pixmap("../image/pic/program_icon.jpg");

   /**  @} */

    /**
     * @name 布局
     * @{ */ 
    
   set_layout(); 

    /**  @} */


    set_connect();
}

MyQWidget::~MyQWidget()
{
    
}

void MyQWidget::init_component()
{
    label_serialport= new QLabel("串口" , this);
    label_baudRate = new QLabel("波特率" , this);
    label_parity = new QLabel("校验位" , this);
    label_stopbits = new QLabel("停止位" , this);
    label_data = new QLabel("数据位" , this);
    label_send = new QLabel("发送区:" , this);
    label_recv = new QLabel("接收区:" , this);

    send_bt = new QPushButton("发送" , this);
    switch_bt = new QPushButton("打开串口" , this);
    clear_send_bt = new QPushButton("清除" , this);
    clear_recv_bt = new QPushButton("清除" , this);
    comb_port = new QComboBox(this);

    comb_baudRate = new QComboBox(this);
    comb_dataBits = new QComboBox(this);
    comb_parity = new QComboBox(this);
    comb_stopbits = new QComboBox(this);

    h_layout1 = new QHBoxLayout();
    h_layout2 = new QHBoxLayout();
    h_layout3 = new QHBoxLayout();
    h_layout4 = new QHBoxLayout();
    v_layout = new QVBoxLayout();

    send_edit = new QTextEdit(this);
    recv_edit = new QPlainTextEdit(this);

    port = new QSerialPort(this);
    timer = new QTimer(this);

    port_open = false;

}

void MyQWidget::switch_serialport()
{
    if(port_open)
    {
        port->close();
        switch_bt->setText(tr("打开串口"));
        port_open = false;

        /* Enable all QComboBox.*/
        comb_port->setEnabled(true);
        comb_baudRate->setEnabled(true);
        comb_dataBits->setEnabled(true);
        comb_parity->setEnabled(true);
        comb_stopbits->setEnabled(true);
    }
    else
    {
        /* If serial port is closed,open it. */

        /* List and QComboBox are in same order,so they can use the same index. */
        port->setPort(infolist[comb_port->currentIndex()]);
        port->setBaudRate(baudRate[comb_baudRate->currentIndex()]);
        port->setDataBits(dataBits[comb_dataBits->currentIndex()]);
        port->setParity(parity[comb_parity->currentIndex()]);
        port->setStopBits(stopBits[comb_stopbits->currentIndex()]);
        if(port->open(QSerialPort::ReadWrite))
        {
            switch_bt->setText(tr("关闭串口"));
            port_open = true;
            /* Disable all QComboBox.*/
            comb_port->setEnabled(true);
            comb_baudRate->setEnabled(true);
            comb_dataBits->setEnabled(true);
            comb_parity->setEnabled(true);
            comb_stopbits->setEnabled(true);
        }
        else
        {
            QString message = "Connect to " + comb_port->currentText() + " failed : " + port->errorString();
            printf("%s\n" , message.toStdString().c_str()); //@note linux下 一定要以管理员的权限打开串口
            //this->statusBar()->showMessage(message,5000);
        }
    }
    printf("switch_serialport\n");
}

void MyQWidget::insertDataDisplay(const QString& text)
{
    QTextCharFormat fmt;
    recv_edit->setCurrentCharFormat(fmt);
    recv_edit->insertPlainText(text);
    QScrollBar* scroll = recv_edit->verticalScrollBar();
    scroll->setSliderPosition(scroll->maximum());
}


void MyQWidget::send()
{

    /* Check if the data is splited by withspace every 2 characters. */
    QString data = send_edit->toPlainText();
   qDebug() << "data:" << data;
    QRegExp regExp(" *([0-9A-Fa-f]{2} +)+[0-9A-Fa-f]{2} *");
    if(regExp.exactMatch(data))
    {
        /* Convert every 2 characters to hexadecimal. */
        QStringList dataList = data.split(QRegExp(" +"));
        QString newData,showData;
        foreach(const QString& i, dataList)
        {
            showData += i + " ";
            int n = i.toInt(0,16);
            newData += data.sprintf("%c",static_cast<char>(n));
        }

        /* Transmit data. */
        if(port->write(newData.toStdString().c_str()) == -1)
       // if(port->write("7E 32 30 30 31 45 31 45 42 41 30 30 36 30 31 30 30 30 30 46 43 34 38 0D") == -1)
            //~2001E1EBA006010000FC48
        {
            //statusBar()->showMessage("Send data failed : "+ port->errorString());
            return;
        }
        printf("str:%s\n" , newData.toStdString().c_str());

        for(int i = 0; i< newData.size(); ++i)
            qDebug() << "|" << newData[i] <<"|";
        /* Show data. */
        insertDataDisplay(showData);
    }else{
        data += '\r';
        port->write(data.toStdString().c_str());
    }

    #if 0
    const char *ptr = send_edit->toPlainText().toStdString().c_str();
    QByteArray newData;
    for(int i = 0; ptr[i];++i){
        int a = toascii(*ptr);
        newData[i] = a;
    }
    if(port->write(newData) == -1)
       // if(port->write("7E 32 30 30 31 45 31 45 42 41 30 30 36 30 31 30 30 30 30 46 43 34 38 0D") == -1)
        {
            //statusBar()->showMessage("Send data failed : "+ port->errorString());
            return;
        }


        /* Show data. */
        insertDataDisplay(ptr);
        #endif
}


/* Receive data from serial port. */
void MyQWidget::receive(void)
{
    /* Receive data. */
    QByteArray readdata = port->readAll();
    while(port->waitForReadyRead(100))
        readdata.append(port->readAll());
    //QString data = port->readAll(); ///@note linux下不能这样读取，因为一次只能读取一个字节
#if 0
    /* Convert data to hexadecimal. */
    QString temp;
    const char* str = data.toStdString().c_str();
    for(int i=0;i < (int)qstrlen(str);i++){
            temp.sprintf("%s %02X",temp.toStdString().c_str(),str[i]);
        data = temp;
    }

    /* Show the data received. */
    insertDataDisplay(data + ' ');
#endif
    insertDataDisplay(readdata);
}

void MyQWidget::clear_recv()
{
    recv_edit->clear();
}

void MyQWidget::clear_send()
{
    send_edit->clear();
}

void MyQWidget::set_connect()
{
    this->connect(switch_bt , SIGNAL(clicked()) , this , SLOT(switch_serialport()));
    this->connect(send_bt , SIGNAL(clicked()) , this , SLOT(send()));
    this->connect(port , SIGNAL(readyRead()) , this , SLOT(receive()));
    this->connect(clear_send_bt , SIGNAL(clicked()) , this , SLOT(clear_send()));
    this->connect(clear_recv_bt , SIGNAL(clicked()) , this , SLOT(clear_recv()));
}

void MyQWidget::set_layout()
{
    // 第一行
   h_layout1->addWidget(label_serialport); 
   h_layout1->addWidget(comb_port);
   h_layout1->addWidget(label_baudRate); 
   h_layout1->addWidget(comb_baudRate);
   h_layout1->addWidget(label_parity); 
   h_layout1->addWidget(comb_parity);
   h_layout1->addWidget(label_stopbits); 
   h_layout1->addWidget(comb_stopbits);
   h_layout1->addWidget(label_data);
   h_layout1->addWidget(comb_dataBits);

   // 第二行
    h_layout2->addWidget(label_recv);
    h_layout2->addWidget(recv_edit);
    h_layout2->addWidget(clear_recv_bt);

   // 第三行
   h_layout3->addWidget(label_send);
   h_layout3->addWidget(send_edit);
   h_layout3->addWidget(clear_send_bt);

   //第四行
   h_layout4->addWidget(switch_bt);
   h_layout4->addWidget(send_bt);

#if 1
   //总布局
   v_layout->addLayout(h_layout1);
   v_layout->addLayout(h_layout2);
   v_layout->addLayout(h_layout3);
   v_layout->addLayout(h_layout4);
#endif
   this->setLayout(v_layout);
}

void MyQWidget::init_port()
{
    infolist = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo& info , infolist)
    {
        comb_port->addItem(info.portName());
    }
    if(comb_port->count() == 0)
    {
        comb_port->addItem(tr("没有可用串口"));
        send_bt->setEnabled(false);
    }

    baudRate << QSerialPort::Baud115200 << QSerialPort::Baud57600 << QSerialPort::Baud38400
                   << QSerialPort::Baud19200 << QSerialPort::Baud9600 << QSerialPort::Baud4800
                   << QSerialPort::Baud2400 << QSerialPort::Baud1200;
    QStringList s_baudRate;
    s_baudRate << tr("115200") << tr("57600") << tr("38400")
             << tr("19200") << tr("9600") << tr("4800")
             << tr("2400") << tr("1200");
    comb_baudRate->addItems(s_baudRate);

    dataBits << QSerialPort::Data8 << QSerialPort::Data7
                   << QSerialPort::Data6 << QSerialPort::Data5;
    QStringList d_dataBits;
    d_dataBits << tr("8") << tr("7") << tr("6") << tr("5");
    comb_dataBits->addItems(d_dataBits);

    parity << QSerialPort::NoParity << QSerialPort::OddParity << QSerialPort::EvenParity
                 << QSerialPort::SpaceParity << QSerialPort::MarkParity;
    QStringList p_parity;
    p_parity << tr("无校验") << tr("奇校验") << tr("偶校验")
           << tr("空校验") << tr("标记校验");
    comb_parity->addItems(p_parity);

    stopBits << QSerialPort::OneStop << QSerialPort::OneAndHalfStop << QSerialPort::TwoStop;
    QStringList s_stopBits;
    s_stopBits << tr("1") << tr("1.5") << tr("2");
    comb_stopbits->addItems(s_stopBits); 
}
