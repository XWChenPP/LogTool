#ifndef WIDGET_H
#define WIDGET_H

#include <QDebug>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSerialPortInfo>
#include <QtSerialPort/QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QFile           *logFile;
    bool            displayDebugLog = true;
    bool            displayInfoLog = true;
    bool            displayWarningLog = true;
    bool            displayErrorLog = true;
    QString         displayLogSource = "";
    QStringList     logSourceList;
    quint32         rxDataCounter = 0;
    quint32         txDataCounter = 0;
    QSerialPort     *serialPort = NULL;

private slots:
    bool eventFilter(QObject *obj, QEvent *event);

    void on_serialPort_readyRead();

    void on_pushButtonSerialCtrl_clicked();

    void on_checkBoxDebug_stateChanged(int arg1);

    void on_checkBoxInfo_stateChanged(int arg1);

    void on_checkBoxWarning_stateChanged(int arg1);

    void on_checkBoxError_stateChanged(int arg1);

    void on_comboBoxLogSourceList_currentIndexChanged(int index);

    void usr_textBrowserLogDisplay_printLine(QString str2Print);

    void usr_textBrowserLogDisplay_refresh();

    void on_pushButtonFileSelect_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
