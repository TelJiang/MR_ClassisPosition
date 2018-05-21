#pragma once
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDialog>
#include <QThread>
#include <QDebug>
#include <QTime>
#include <QMutex>
class SerialPort : public QObject
{
	Q_OBJECT
public:
	SerialPort(QObject *parent = 0);
	~SerialPort();
	QSerialPort mySerialPort;
	bool isOpenSerialPort;
	static QStringList serialPortName, serialPortDescription;

public slots:
	void openSerialPort(QString serialPortNameOrDescription, int baudRateIndex, int dataBitsIndex, int stopBitsIndex,
		int parityIndex, int flowControlIndex);
	void closeSerialPort();
	void serarchSerialPort();
	void writeData(QByteArray data);

private:

	QByteArray receivedData;
	int readBytesAmounts, writeBytesAmounts;
};

class SerialPortThread : public QThread
{
	Q_OBJECT
public:
	SerialPortThread();
	~SerialPortThread();
	bool openSerialPortThread(QString serialPortNameOrDescription, int baudRateIndex, int dataBitsIndex,
		int stopBitsIndex, int parityIndex, int flowControlIndex);
	void closeSerialPortThread();
	void searchSerialPort();
	void writeData(QByteArray data);
	int readBytes(bool isClear);
	int writeBytes(bool isClear);
	void isWrapOrHexShowOrTimeStamp(bool isWrap = false, bool isHexShow = false, bool isTimeStamp = false);
	QString getCurrentSystemTime();
	QByteArray showData(bool isShow = false);
	QByteArray transferData(bool isTransfer = false);
	QStringList getSerialPortName();
	QStringList getSerialPortDescription();
	void delayMsec(int msecs);
	void run();

public slots:
	void readData();
	void error(QSerialPort::SerialPortError error);

signals:
	void openSerialPort(QString serialPortNameOrDescription, int baudRateIndex, int dataBitsIndex, int stopBitsIndex,
		int parityIndex, int flowControlIndex);
	void closeSerialPort();
	void sendData(QByteArray data);
	void seekSerialPort();
	void emit_error(QString info);

private:
	SerialPort *serialPort;
	bool isWrap, isHexShow, isTimeStamp;
	int readBytesAmounts, writeBytesAmounts;
	QByteArray dataShow, dataTransfer;
	QString t_serialPortNameOrDescription;
	int t_baudRateIndex;
	int t_dataBitsIndex;
	int t_stopBitsIndex;
	int t_parityIndex;
	int t_flowControlIndex;

	QMutex mutex;
};
