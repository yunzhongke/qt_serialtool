/*================================================================
*   Copyright (C) 2018 Vertiv Tech Co., Ltd. All rights reserved.
*   
*   文件名称：dialog.h
*   创 建 者：yunweilian
*   创建日期：2018年02月26日16时
*   描    述：登录对话框
*
================================================================*/


#ifndef _DIALOG_H
#define _DIALOG_H

#include <QtWidgets/QtWidgets>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGroupBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore/QTimer>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPlainTextEdit>

/**
 * @brief 对话框类
 */
class MyQWidget : public QWidget
{
    Q_OBJECT ///< QT的宏，实现信号和槽机制
    public:
        explicit MyQWidget(QWidget *parent = 0);
        ~MyQWidget();

        // 初始化所有组件
        void init_component();

        void init_port();

        void set_connect();

        void set_layout();


        void insertDataDisplay(const QString& text);


    public slots:
        void switch_serialport();
        void clear_send();
        void clear_recv();
        void receive();
        void send();


    private:
        QLabel *label_serialport; ///< 串口
        QLabel *label_baudRate; ///< 波特率 
        QLabel *label_parity; ///< 校验位
        QLabel *label_stopbits; ///< 停止位
        QLabel *label_data;

        QLabel *label_send; 
        QLabel *label_recv;

        QPushButton *send_bt; 
        QPushButton *clear_send_bt;
        QPushButton *clear_recv_bt;
        QPushButton *switch_bt;

        QTextEdit *send_edit; 
        QPlainTextEdit *recv_edit;

        QList<QSerialPort::BaudRate> baudRate;
        QList<QSerialPort::DataBits> dataBits;
        QList<QSerialPort::Parity> parity;
        QList<QSerialPort::StopBits> stopBits;

        QComboBox *comb_baudRate;
        QComboBox *comb_dataBits;
        QComboBox *comb_parity;
        QComboBox *comb_stopbits;
        QComboBox *comb_port;
    
        QSerialPort* port;
        QTimer* timer;
        QList<QSerialPortInfo> infolist;

        bool port_open;

        QHBoxLayout *h_layout1;
        QHBoxLayout *h_layout2;
        QHBoxLayout *h_layout3;
        QHBoxLayout *h_layout4;
        QVBoxLayout *v_layout;
};

#endif //DIALOG_H
