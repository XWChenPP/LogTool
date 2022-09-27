#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    serialPort = new QSerialPort(this);

    // 扫描添加串口列表
    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        ui->comboBoxPortList->addItem(info.portName());
    }
    // 为端口下拉列表注册事件过滤
    ui->comboBoxPortList->installEventFilter(this);

    // 在源文件列表中添加所有文件
    logSourceList.append("所有文件");
    // 为日志源选择下拉列表注册事件过滤
    ui->comboBoxLogSourceList->installEventFilter(this);
}

Widget::~Widget()
{
    logFile->close();
    delete ui;
}


bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (obj == ui->comboBoxPortList) {
            ui->comboBoxPortList->clear();
            for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
                ui->comboBoxPortList->addItem(info.portName());
            }
        } else if (obj == ui->comboBoxLogSourceList) {
            ui->comboBoxLogSourceList->clear();
            ui->comboBoxLogSourceList->addItems(logSourceList);
        } else {
            // TODO: 其余事件触发，不做处理
        }
    }

    return QWidget::eventFilter(obj, event);
}


void Widget::on_serialPort_readyRead()
{
    // 保存接收到的日志数据到数据库文件中
    // 遍历 logSourceList 不在里面就加进去
    // 增加 rxDataCounter
}


void Widget::on_pushButtonSerialCtrl_clicked()
{
    if (ui->pushButtonSerialCtrl->text() == "打开串口") {
        serialPort->setPortName(ui->comboBoxPortList->currentText());
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setBaudRate(QSerialPort::Baud115200);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        // 链接信号槽
        connect(serialPort, SIGNAL(readyRead()), this, SLOT(on_serialPort_readyRead()));

        if (serialPort->open(QIODevice::ReadWrite)) {
            QMessageBox::information(this, "提示", "串口链接成功！");
            ui->pushButtonSerialCtrl->setText("关闭串口");
        } else {
            QMessageBox::warning(this, "警告", "串口链接失败！");
        }

    } else if (ui->pushButtonSerialCtrl->text() == "关闭串口") {
        serialPort->close();
        ui->pushButtonSerialCtrl->setText("打开串口");
    } else {
        QMessageBox::critical(this, "错误", "按钮字段错误");
        if (serialPort->isOpen()) {
            ui->pushButtonSerialCtrl->setText("关闭串口");
        } else {
            ui->pushButtonSerialCtrl->setText("打开串口");
        }
    }
}


void Widget::on_checkBoxDebug_stateChanged(int arg1)
{
    displayDebugLog = arg1;
    usr_textBrowserLogDisplay_refresh();
}


void Widget::on_checkBoxInfo_stateChanged(int arg1)
{
    displayInfoLog = arg1;
    usr_textBrowserLogDisplay_refresh();
}


void Widget::on_checkBoxWarning_stateChanged(int arg1)
{
    displayWarningLog = arg1;
    usr_textBrowserLogDisplay_refresh();
}


void Widget::on_checkBoxError_stateChanged(int arg1)
{
    displayErrorLog = arg1;
    usr_textBrowserLogDisplay_refresh();
}


void Widget::on_comboBoxLogSourceList_currentIndexChanged(int index)
{
    displayLogSource = ui->comboBoxLogSourceList->currentText();
    usr_textBrowserLogDisplay_refresh();
}


void Widget::on_pushButtonFileSelect_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择日志文件",  "", "TXT(*.txt)");
    if (fileName.isEmpty()) {
        return;
    }
    logFile = new QFile(fileName);
    if (!logFile->open(QIODevice::ReadWrite | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "文件打开失败！");
        return;
    }
    ui->pushButtonSerialCtrl->setEnabled(true);
}


void Widget::usr_textBrowserLogDisplay_printLine(QString str2Print)
{
    // 打印一行信息
    // 根据日志过滤器信息判断是否需要打印
    // 1. 提取日志等级 [D][I][W][E]
    // 2. 提取日志来源 [source file]
    // 3. 判断是否需要打印
    // 4. 打印
    ui->textBrowserLogDisplay->append(str2Print);
}


void Widget::usr_textBrowserLogDisplay_refresh()
{
    // 清空原有内容
    ui->textBrowserLogDisplay->clear();

    // 遍历日志文件
    // 每读取一行，调用usr_textBrowserLogDisplay_printLine()打印一行
}

