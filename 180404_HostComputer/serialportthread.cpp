#include "serialportthread.h"

QStringList SerialPort::serialPortName = {};
QStringList SerialPort::serialPortDescription = {};

//************************************
// ������:   	SerialPort
// ���ʿ���:   	public 
// ����ֵ:   	
// �β�: 		QObject * parent
// ���ߣ�		���Ͽ�����
// ˵����		���캯��
// ���ڣ�		2018/4/4 21:52:53
//************************************
SerialPort::SerialPort(QObject *parent)
{
	receivedData = nullptr;
	readBytesAmounts = 0;
	writeBytesAmounts = 0;
	isOpenSerialPort = false;
}

//************************************
// ������:   	~SerialPort
// ���ʿ���:   	public 
// ����ֵ:   	
// ���ߣ�		���Ͽ�����
// ˵����		�����������������������ʱ������û�йرվ�������ֱ�ӹر�
// ���ڣ�		2018/4/4 21:53:10
//************************************
SerialPort::~SerialPort()
{
	if (mySerialPort.isOpen()) {
		mySerialPort.close();
	}
}

//************************************
// ������:   	openSerialPort
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		QString serialPortNameOrDescription
// �β�: 		int baudRateIndex
// �β�: 		int dataBitsIndex
// �β�: 		int stopBitsIndex
// �β�: 		int parityIndex
// �β�: 		int flowControlIndex
// ���ߣ�		���Ͽ�����
// ˵����		���ݴ��ڵ����ò������ô��ڲ���
// ���ڣ�		2018/4/4 21:54:11
//************************************
void SerialPort::openSerialPort(QString serialPortNameOrDescription, int baudRateIndex, int dataBitsIndex,
	int stopBitsIndex, int parityIndex, int flowControlIndex)
{
	switch (baudRateIndex)
	{
	case 0:
		mySerialPort.setBaudRate(QSerialPort::Baud115200, QSerialPort::AllDirections);
		break;
	case 1:
		mySerialPort.setBaudRate(QSerialPort::Baud57600, QSerialPort::AllDirections);
		break;
	case 2:
		mySerialPort.setBaudRate(QSerialPort::Baud38400, QSerialPort::AllDirections);
		break;
	case 3:
		mySerialPort.setBaudRate(QSerialPort::Baud19200, QSerialPort::AllDirections);
		break;
	case 4:
		mySerialPort.setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);
		break;
	case 5:
		mySerialPort.setBaudRate(QSerialPort::Baud4800, QSerialPort::AllDirections);
		break;
	case 6:
		mySerialPort.setBaudRate(QSerialPort::Baud2400, QSerialPort::AllDirections);
		break;
	case 7:
		mySerialPort.setBaudRate(QSerialPort::Baud1200, QSerialPort::AllDirections);
		break;
	default:
		break;
	}

	switch (dataBitsIndex)
	{
	case 0:
		mySerialPort.setDataBits(QSerialPort::Data8);
		break;
	case 1:
		mySerialPort.setDataBits(QSerialPort::Data7);
		break;
	case 2:
		mySerialPort.setDataBits(QSerialPort::Data6);
		break;
	case 3:
		mySerialPort.setDataBits(QSerialPort::Data5);
		break;
	default:
		break;
	}

	switch (stopBitsIndex)
	{
	case 0:
		mySerialPort.setStopBits(QSerialPort::OneStop);
		break;
	case 1:
		mySerialPort.setStopBits(QSerialPort::OneAndHalfStop);
		break;
	case 2:
		mySerialPort.setStopBits(QSerialPort::TwoStop);
		break;
	default:
		break;
	}

	switch (parityIndex)
	{
	case 0:
		mySerialPort.setParity(QSerialPort::NoParity);
		break;
	case 1:
		mySerialPort.setParity(QSerialPort::OddParity);
		break;
	case 2:
		mySerialPort.setParity(QSerialPort::EvenParity);
		break;
	default:
		break;
	}

	switch (flowControlIndex)
	{
	case 0:
		mySerialPort.setFlowControl(QSerialPort::NoFlowControl);
		break;
	case 1:
		mySerialPort.setFlowControl(QSerialPort::HardwareControl);
		break;
	case 2:
		mySerialPort.setFlowControl(QSerialPort::SoftwareControl);
		break;
	default:
		break;
	}

	if (serialPortNameOrDescription.contains("(")) {
		int i = serialPortNameOrDescription.indexOf("(");
		int j = serialPortNameOrDescription.indexOf(")");
		mySerialPort.setPortName(serialPortNameOrDescription.mid(i + 1, j - i - 1));
	}
	else {
		mySerialPort.setPortName(serialPortNameOrDescription);
	}
	if (!mySerialPort.open(QIODevice::ReadWrite)) {
		mySerialPort.close();
		if (!mySerialPort.open(QIODevice::ReadWrite)) {
			isOpenSerialPort = false;
		}
		else {
			isOpenSerialPort = true;
		}
	}
	else {
		isOpenSerialPort = true;
	}
}

//************************************
// ������:   	closeSerialPort
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�رմ���
// ���ڣ�		2018/4/4 21:54:58
//************************************
void SerialPort::closeSerialPort()
{
	mySerialPort.close();
}

//************************************
// ������:   	serarchSerialPort
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��������
// ���ڣ�		2018/4/4 21:55:27
//************************************
void SerialPort::serarchSerialPort()
{
	serialPortName.clear();
	serialPortDescription.clear();
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
		serialPortName.append(info.portName());
		serialPortDescription.append(info.description() + "(" + info.portName() + ")");
	}
}

//************************************
// ������:   	writeData
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		QByteArray data
// ���ߣ�		���Ͽ�����
// ˵����		�򴮿�д����
// ���ڣ�		2018/4/4 21:55:42
//************************************
void SerialPort::writeData(QByteArray data)
{
	mySerialPort.write(data);
}

//************************************
// ������:   	SerialPortThread
// ���ʿ���:   	public 
// ����ֵ:   	
// ���ߣ�		���Ͽ�����
// ˵����		���캯��
// ���ڣ�		2018/4/4 21:56:01
//************************************
SerialPortThread::SerialPortThread()
{
	isWrap = false;
	isHexShow = false;
	isTimeStamp = false;
	readBytesAmounts = 0;
	writeBytesAmounts = 0;
	t_serialPortNameOrDescription = "";
	t_baudRateIndex = 0;
	t_dataBitsIndex = 0;
	t_stopBitsIndex = 0;
	t_parityIndex = 0;
	t_flowControlIndex = 0;
	if (!isRunning()) {
		start();
		wait(10);//��֤���ڶ����ڴ��߳����ѱ�����
	}
}

//************************************
// ������:   	~SerialPortThread
// ���ʿ���:   	public 
// ����ֵ:   	
// ���ߣ�		���Ͽ�����
// ˵����		��������
// ���ڣ�		2018/4/4 21:56:15
//************************************
SerialPortThread::~SerialPortThread()
{
	delete serialPort;
	wait(10);
	if (isRunning()) {
		terminate();
		wait(10);
	}
}

//************************************
// ������:   	SerialPortThread::openSerialPortThread
// ���ʿ���:   	public 
// ����ֵ:   	bool
// �β�: 		QString serialPortNameOrDescription
// �β�: 		int baudRateIndex
// �β�: 		int dataBitsIndex
// �β�: 		int stopBitsIndex
// �β�: 		int parityIndex
// �β�: 		int flowControlIndex
// ���ߣ�		���Ͽ�����
// ˵����		�򿪴����߳�
// ���ڣ�		2018/4/5 12:50:19
//************************************
bool SerialPortThread::openSerialPortThread(QString serialPortNameOrDescription, int baudRateIndex, int dataBitsIndex,
	int stopBitsIndex, int parityIndex, int flowControlIndex)
{
	t_serialPortNameOrDescription = serialPortNameOrDescription;
	t_baudRateIndex = baudRateIndex;
	t_dataBitsIndex = dataBitsIndex;
	t_stopBitsIndex = stopBitsIndex;
	t_parityIndex = parityIndex;
	t_flowControlIndex = flowControlIndex;
	emit openSerialPort(serialPortNameOrDescription, baudRateIndex, dataBitsIndex, stopBitsIndex, parityIndex,
		flowControlIndex);
	wait(200);
	return serialPort->isOpenSerialPort;
}

//************************************
// ������:   	SerialPortThread::closeSerialPortThread
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�رմ����߳�
// ���ڣ�		2018/4/4 22:04:54
//************************************
void SerialPortThread::closeSerialPortThread()
{
	emit closeSerialPort();
	delayMsec(100);
	
}

//************************************
// ������:   	SerialPortThread::searchSerialPort
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��������
// ���ڣ�		2018/4/4 22:04:16
//************************************
void SerialPortThread::searchSerialPort()
{
	wait(10);
	emit seekSerialPort();
}

//************************************
// ������:   	SerialPortThread::writeData
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		QByteArray data
// ���ߣ�		���Ͽ�����
// ˵����		�򴮿�д����
// ���ڣ�		2018/4/4 22:03:28
//************************************
void SerialPortThread::writeData(QByteArray data)
{
	writeBytesAmounts += data.length();
	emit sendData(data);
}

//************************************
// ������:   	SerialPortThread::readBytes
// ���ʿ���:   	public 
// ����ֵ:   	int
// �β�: 		bool isClear
// ���ߣ�		���Ͽ�����
// ˵����		��ȡ��ȡ�������ֽ������β��������¼���
// ���ڣ�		2018/4/4 22:03:02
//************************************
int SerialPortThread::readBytes(bool isClear)
{
	if (isClear) {
		readBytesAmounts = 0;
	}
	return readBytesAmounts;
}

//************************************
// ������:   	SerialPortThread::writeBytes
// ���ʿ���:   	public 
// ����ֵ:   	int
// �β�: 		bool isClear
// ���ߣ�		���Ͽ�����
// ˵����		��ȡд��������ֽ������β��������¼���
// ���ڣ�		2018/4/5 12:51:14
//************************************
int SerialPortThread::writeBytes(bool isClear)
{
	if (isClear) {
		writeBytesAmounts = 0;
	}
	return writeBytesAmounts;
}

//************************************
// ������:   	SerialPortThread::isWrapOrHexShowOrTimeStamp
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		bool isWrap
// �β�: 		bool isHexShow
// �β�: 		bool isTimeStamp
// ���ߣ�		���Ͽ�����
// ˵����		���ڸ�ʽ��������������
// ���ڣ�		2018/4/5 12:51:59
//************************************
void SerialPortThread::isWrapOrHexShowOrTimeStamp(bool isWrap, bool isHexShow, bool isTimeStamp)
{
	this->isWrap = isWrap;
	this->isHexShow = isHexShow;
	this->isTimeStamp = isTimeStamp;
}

//************************************
// ������:   	SerialPortThread::getCurrentSystemTime
// ���ʿ���:   	public 
// ����ֵ:   	QT_NAMESPACE::QString
// ���ߣ�		���Ͽ�����
// ˵����		��ȡϵͳ��ǰʱ��
// ���ڣ�		2018/4/5 12:52:22
//************************************
QString SerialPortThread::getCurrentSystemTime()
{
	return QTime::currentTime().toString("hh:mm:ss:zzz");
}

//************************************
// ������:   	SerialPortThread::showData
// ���ʿ���:   	public 
// ����ֵ:   	QT_NAMESPACE::QByteArray
// �β�: 		bool isShow
// ���ߣ�		���Ͽ�����
// ˵����		������ʾ���յ�������
// ���ڣ�		2018/4/5 12:52:35
//************************************
QByteArray SerialPortThread::showData(bool isShow)
{
	QByteArray tmp = nullptr;
	if (isShow) {
		if (mutex.tryLock()) {
			tmp = dataShow;
			dataShow.clear();
			mutex.unlock();
		}
	}
	return tmp;
}

//************************************
// ������:   	SerialPortThread::transferData
// ���ʿ���:   	public 
// ����ֵ:   	QT_NAMESPACE::QByteArray
// �β�: 		bool isTransfer
// ���ߣ�		���Ͽ�����
// ˵����		���ڴ�����յ�������
// ���ڣ�		2018/4/5 12:52:50
//************************************
QByteArray SerialPortThread::transferData(bool isTransfer)
{
	QByteArray tmp = nullptr;
	if (isTransfer) {
		if (mutex.tryLock()) {
			tmp = dataTransfer;
			dataTransfer.clear();
			mutex.unlock();
		}	
	}
	return tmp;
}

//************************************
// ������:   	SerialPortThread::getSerialPortName
// ���ʿ���:   	public 
// ����ֵ:   	QT_NAMESPACE::QStringList
// ���ߣ�		���Ͽ�����
// ˵����		��ȡ���ڵ����ƣ�COMx)
// ���ڣ�		2018/4/5 12:53:14
//************************************
QStringList SerialPortThread::getSerialPortName()
{
	return SerialPort::serialPortName;
}

//************************************
// ������:   	SerialPortThread::getSerialPortDescription
// ���ʿ���:   	public 
// ����ֵ:   	QT_NAMESPACE::QStringList
// ���ߣ�		���Ͽ�����
// ˵����		��ȡ���ڵ�������xxxx(COMx))
// ���ڣ�		2018/4/5 12:53:30
//************************************
QStringList SerialPortThread::getSerialPortDescription()
{
	return SerialPort::serialPortDescription;
}

//************************************
// ������:   	SerialPortThread::delayMsec
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		int msecs
// ���ߣ�		���Ͽ�����
// ˵����		ʵ�ֺ��뼶��ʱ
// ���ڣ�		2018/4/14 19:15:40
//************************************
void SerialPortThread::delayMsec(int msecs)
{
	QTime currentTime, lastTime;
	lastTime = QTime::currentTime();
	currentTime = QTime::currentTime();
	while (lastTime.msecsTo(currentTime) < msecs) {
		currentTime = QTime::currentTime();
	}
}

//************************************
// ������:   	SerialPortThread::run
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		���߳����
// ���ڣ�		2018/4/5 12:53:55
//************************************
void SerialPortThread::run()
{
	serialPort = new SerialPort;
	connect(this, &SerialPortThread::openSerialPort, serialPort, &SerialPort::openSerialPort);
	connect(this, &SerialPortThread::closeSerialPort, serialPort, &SerialPort::closeSerialPort);
	connect(&serialPort->mySerialPort, &QSerialPort::readyRead, this, &SerialPortThread::readData, Qt::DirectConnection);
	connect(&serialPort->mySerialPort, &QSerialPort::errorOccurred, this, &SerialPortThread::error, Qt::DirectConnection);
	connect(this, &SerialPortThread::sendData, serialPort, &SerialPort::writeData);
	connect(this, &SerialPortThread::seekSerialPort, serialPort, &SerialPort::serarchSerialPort, Qt::DirectConnection);
	exec();
}

void SerialPortThread::error(QSerialPort::SerialPortError error)
{
	switch (error)
	{
	case QSerialPort::NoError:
		emit emit_error(t_serialPortNameOrDescription + " NoError!!!");
		break;
	case QSerialPort::DeviceNotFoundError:
		emit emit_error(t_serialPortNameOrDescription + " DeviceNotFoundError!!!");
		break;
	case QSerialPort::PermissionError:
		emit emit_error(t_serialPortNameOrDescription + " PermissionError!!!");
		break;
	case QSerialPort::OpenError:
		emit emit_error(t_serialPortNameOrDescription + " OpenError!!!");
		break;
	case QSerialPort::ParityError:
		emit emit_error(t_serialPortNameOrDescription + " ParityError!!!");
		break;
	case QSerialPort::FramingError:
		emit emit_error(t_serialPortNameOrDescription + " FramingError!!!");
		break;
	case QSerialPort::BreakConditionError:
		emit emit_error(t_serialPortNameOrDescription + " BreakConditionError!!!");
		break;
	case QSerialPort::WriteError:
		emit emit_error(t_serialPortNameOrDescription + " WriteError!!!");
		break;
	case QSerialPort::ReadError:
		emit emit_error(t_serialPortNameOrDescription + " ReadError!!!");
		break;
	case QSerialPort::ResourceError:
		emit emit_error(t_serialPortNameOrDescription + " ResourceError!!!");
		break;
	case QSerialPort::UnsupportedOperationError:
		emit emit_error(t_serialPortNameOrDescription + " UnsupportedOperationError!!!");
		break;
	case QSerialPort::UnknownError:
		emit emit_error(t_serialPortNameOrDescription + " UnknownError!!!");
		break;
	case QSerialPort::TimeoutError:
		emit emit_error(t_serialPortNameOrDescription + " TimeoutError!!!");
		break;
	case QSerialPort::NotOpenError:
		emit emit_error(t_serialPortNameOrDescription + " NotOpenError!!!");
		break;
	default:
		break;
	}
}

//************************************
// ������:   	SerialPortThread::readData
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		��ȡ���ڶ�ȡ������������
// ���ڣ�		2018/4/5 12:54:10
//************************************
void SerialPortThread::readData()
{
	QByteArray data = serialPort->mySerialPort.readAll();
	mutex.lock();
	readBytesAmounts += data.length();

	QByteArray tmp = data;
	dataTransfer.append(data);
	if (isHexShow) {
		tmp = data.toHex();
	}
	if (isWrap) {
		if (isTimeStamp) {
			tmp += "  " + getCurrentSystemTime().toLocal8Bit();
		}
		tmp += "\n";
	}
	dataShow.append(tmp);
	mutex.unlock();
}
