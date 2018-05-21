#include "serialportthread.h"

QStringList SerialPort::serialPortName = {};
QStringList SerialPort::serialPortDescription = {};

//************************************
// 函数名:   	SerialPort
// 访问控制:   	public 
// 返回值:   	
// 形参: 		QObject * parent
// 作者：		码上看世界
// 说明：		构造函数
// 日期：		2018/4/4 21:52:53
//************************************
SerialPort::SerialPort(QObject *parent)
{
	receivedData = nullptr;
	readBytesAmounts = 0;
	writeBytesAmounts = 0;
	isOpenSerialPort = false;
}

//************************************
// 函数名:   	~SerialPort
// 访问控制:   	public 
// 返回值:   	
// 作者：		码上看世界
// 说明：		析构函数，如果当对象被销毁时，串口没有关闭就在这里直接关闭
// 日期：		2018/4/4 21:53:10
//************************************
SerialPort::~SerialPort()
{
	if (mySerialPort.isOpen()) {
		mySerialPort.close();
	}
}

//************************************
// 函数名:   	openSerialPort
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QString serialPortNameOrDescription
// 形参: 		int baudRateIndex
// 形参: 		int dataBitsIndex
// 形参: 		int stopBitsIndex
// 形参: 		int parityIndex
// 形参: 		int flowControlIndex
// 作者：		码上看世界
// 说明：		根据串口的配置参数设置串口参数
// 日期：		2018/4/4 21:54:11
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
// 函数名:   	closeSerialPort
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		关闭串口
// 日期：		2018/4/4 21:54:58
//************************************
void SerialPort::closeSerialPort()
{
	mySerialPort.close();
}

//************************************
// 函数名:   	serarchSerialPort
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		搜索串口
// 日期：		2018/4/4 21:55:27
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
// 函数名:   	writeData
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QByteArray data
// 作者：		码上看世界
// 说明：		向串口写数据
// 日期：		2018/4/4 21:55:42
//************************************
void SerialPort::writeData(QByteArray data)
{
	mySerialPort.write(data);
}

//************************************
// 函数名:   	SerialPortThread
// 访问控制:   	public 
// 返回值:   	
// 作者：		码上看世界
// 说明：		构造函数
// 日期：		2018/4/4 21:56:01
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
		wait(10);//保证串口对象在次线程中已被创建
	}
}

//************************************
// 函数名:   	~SerialPortThread
// 访问控制:   	public 
// 返回值:   	
// 作者：		码上看世界
// 说明：		析构函数
// 日期：		2018/4/4 21:56:15
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
// 函数名:   	SerialPortThread::openSerialPortThread
// 访问控制:   	public 
// 返回值:   	bool
// 形参: 		QString serialPortNameOrDescription
// 形参: 		int baudRateIndex
// 形参: 		int dataBitsIndex
// 形参: 		int stopBitsIndex
// 形参: 		int parityIndex
// 形参: 		int flowControlIndex
// 作者：		码上看世界
// 说明：		打开串口线程
// 日期：		2018/4/5 12:50:19
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
// 函数名:   	SerialPortThread::closeSerialPortThread
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		关闭串口线程
// 日期：		2018/4/4 22:04:54
//************************************
void SerialPortThread::closeSerialPortThread()
{
	emit closeSerialPort();
	delayMsec(100);
	
}

//************************************
// 函数名:   	SerialPortThread::searchSerialPort
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		搜索串口
// 日期：		2018/4/4 22:04:16
//************************************
void SerialPortThread::searchSerialPort()
{
	wait(10);
	emit seekSerialPort();
}

//************************************
// 函数名:   	SerialPortThread::writeData
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QByteArray data
// 作者：		码上看世界
// 说明：		向串口写数据
// 日期：		2018/4/4 22:03:28
//************************************
void SerialPortThread::writeData(QByteArray data)
{
	writeBytesAmounts += data.length();
	emit sendData(data);
}

//************************************
// 函数名:   	SerialPortThread::readBytes
// 访问控制:   	public 
// 返回值:   	int
// 形参: 		bool isClear
// 作者：		码上看世界
// 说明：		获取读取的数据字节数，形参用于重新计数
// 日期：		2018/4/4 22:03:02
//************************************
int SerialPortThread::readBytes(bool isClear)
{
	if (isClear) {
		readBytesAmounts = 0;
	}
	return readBytesAmounts;
}

//************************************
// 函数名:   	SerialPortThread::writeBytes
// 访问控制:   	public 
// 返回值:   	int
// 形参: 		bool isClear
// 作者：		码上看世界
// 说明：		获取写入的数据字节数，形参用于重新计数
// 日期：		2018/4/5 12:51:14
//************************************
int SerialPortThread::writeBytes(bool isClear)
{
	if (isClear) {
		writeBytesAmounts = 0;
	}
	return writeBytesAmounts;
}

//************************************
// 函数名:   	SerialPortThread::isWrapOrHexShowOrTimeStamp
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		bool isWrap
// 形参: 		bool isHexShow
// 形参: 		bool isTimeStamp
// 作者：		码上看世界
// 说明：		用于格式化输出读入的数据
// 日期：		2018/4/5 12:51:59
//************************************
void SerialPortThread::isWrapOrHexShowOrTimeStamp(bool isWrap, bool isHexShow, bool isTimeStamp)
{
	this->isWrap = isWrap;
	this->isHexShow = isHexShow;
	this->isTimeStamp = isTimeStamp;
}

//************************************
// 函数名:   	SerialPortThread::getCurrentSystemTime
// 访问控制:   	public 
// 返回值:   	QT_NAMESPACE::QString
// 作者：		码上看世界
// 说明：		获取系统当前时间
// 日期：		2018/4/5 12:52:22
//************************************
QString SerialPortThread::getCurrentSystemTime()
{
	return QTime::currentTime().toString("hh:mm:ss:zzz");
}

//************************************
// 函数名:   	SerialPortThread::showData
// 访问控制:   	public 
// 返回值:   	QT_NAMESPACE::QByteArray
// 形参: 		bool isShow
// 作者：		码上看世界
// 说明：		用于显示接收到的数据
// 日期：		2018/4/5 12:52:35
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
// 函数名:   	SerialPortThread::transferData
// 访问控制:   	public 
// 返回值:   	QT_NAMESPACE::QByteArray
// 形参: 		bool isTransfer
// 作者：		码上看世界
// 说明：		用于传输接收到的数据
// 日期：		2018/4/5 12:52:50
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
// 函数名:   	SerialPortThread::getSerialPortName
// 访问控制:   	public 
// 返回值:   	QT_NAMESPACE::QStringList
// 作者：		码上看世界
// 说明：		获取串口的名称（COMx)
// 日期：		2018/4/5 12:53:14
//************************************
QStringList SerialPortThread::getSerialPortName()
{
	return SerialPort::serialPortName;
}

//************************************
// 函数名:   	SerialPortThread::getSerialPortDescription
// 访问控制:   	public 
// 返回值:   	QT_NAMESPACE::QStringList
// 作者：		码上看世界
// 说明：		获取串口的描述（xxxx(COMx))
// 日期：		2018/4/5 12:53:30
//************************************
QStringList SerialPortThread::getSerialPortDescription()
{
	return SerialPort::serialPortDescription;
}

//************************************
// 函数名:   	SerialPortThread::delayMsec
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		int msecs
// 作者：		码上看世界
// 说明：		实现毫秒级延时
// 日期：		2018/4/14 19:15:40
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
// 函数名:   	SerialPortThread::run
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		次线程入口
// 日期：		2018/4/5 12:53:55
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
// 函数名:   	SerialPortThread::readData
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		读取串口读取缓存区的数据
// 日期：		2018/4/5 12:54:10
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
