#include "camerathread.h"
#pragma execution_character_set("utf-8")

QStringList CameraThread::cameraName = {};

CameraThread::CameraThread(QObject * parent)
{
	AR_chasisCameraName = "";
	AR_RelativeCameraName = "";
	MR_BallRockCameraName = "";
	MR_chasisCameraName = "USB_Camera[29a4ead]";
	index = -1;
}

CameraThread::~CameraThread()
{
	if (isRunning()) {
		isRun = false;
		wait(10);
	}
}

//************************************
// 函数名:   	CameraThread::searchCamera
// 访问控制:   	public static 
// 返回值:   	QT_NAMESPACE::QStringList
// 作者：		码上看世界
// 说明：		搜索摄像头，获取摄像头信息（unique）
// 日期：		2018/4/10 08:33:51
//************************************
QStringList CameraThread::searchCamera()
{
	cameraName.clear();
	foreach(const QCameraInfo &info, QCameraInfo::availableCameras()) {
		cameraName.append(info.description() + "[" + info.deviceName().mid(46, 7) + "]");
	}
	return cameraName;
}

//************************************
// 函数名:   	CameraThread::openCameraThread
// 访问控制:   	public 
// 返回值:   	bool
// 形参: 		QString cameraInfo
// 作者：		码上看世界
// 说明：		打开摄像头线程
// 日期：		2018/4/10 08:45:37
//************************************
bool CameraThread::openCameraThread(QString cameraInfo)
{
	usedCameraName = cameraInfo;
	index = cameraName.indexOf(cameraInfo);
	if (index < 0) {
		showWarningInfo(tr("请重新检索系统中的摄像头设备"));
		return false;
	}
	else {
		if (!isRunning()) {
			isRun = true;
			start();
		}
		return true;
	}
}

//************************************
// 函数名:   	CameraThread::closeCameraThread
// 访问控制:   	public 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		关闭摄像头线程
// 日期：		2018/4/10 16:19:22
//************************************
void CameraThread::closeCameraThread()
{
	isRun = false;
	delayMsec(1000);
}

//************************************
// 函数名:   	CameraThread::showCameraData
// 访问控制:   	public 
// 返回值:   	QT_NAMESPACE::QByteArray
// 形参: 		bool isShow
// 作者：		码上看世界
// 说明：		用于去显示图像处理的结果
// 日期：		2018/4/10 12:58:27
//************************************
QByteArray CameraThread::showCameraData(bool isShow)
{
	QByteArray tmp = nullptr;
	if (isShow) {
		if (mutex.tryLock()) {
			tmp = dataShowed;
			dataShowed.clear();
			mutex.unlock();
		}
	}
	return tmp;
}

//************************************
// 函数名:   	CameraThread::transferCameraData
// 访问控制:   	public 
// 返回值:   	QT_NAMESPACE::QByteArray
// 形参: 		bool isTransfer
// 作者：		码上看世界
// 说明：		用于传输图像处理的结果
// 日期：		2018/4/10 12:59:10
//************************************
QByteArray CameraThread::transferCameraData(bool isTransfer)
{
	QByteArray tmp = nullptr;
	if (isTransfer) {
		if (mutex.tryLock()) {
			tmp = dataTransfered;
			dataTransfered.clear();
			mutex.unlock();
		}	
	}
	return tmp;
}

//************************************
// 函数名:   	CameraThread::getParameter
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		float par
// 作者：		码上看世界
// 说明：		一个外部接口，获取参数
// 日期：		2018/4/11 15:27:14
//************************************
void CameraThread::getParameter(float par)
{
	if (AR_chasisCameraName == usedCameraName) {

	}
	else if (AR_RelativeCameraName == usedCameraName) {

	}
	else if (MR_BallRockCameraName == usedCameraName) {

	}
	else if (MR_chasisCameraName == usedCameraName) {

	}
	else {
		qDebug() << par;
	}
}

//************************************
// 函数名:   	CameraThread::getParameter
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		QByteArray par
// 作者：		码上看世界
// 说明：		一个外部接口，获取参数
// 日期：		2018/4/11 15:28:04
//************************************
void CameraThread::getParameter(QByteArray par)
{
	if (AR_chasisCameraName == usedCameraName) {

	}
	else if (AR_RelativeCameraName == usedCameraName) {

	}
	else if (MR_BallRockCameraName == usedCameraName) {

	}
	else if (MR_chasisCameraName == usedCameraName) {

	}
	else {
		qDebug() << par;
	}
}

//************************************
// 函数名:   	CameraThread::getParameter
// 访问控制:   	public 
// 返回值:   	void
// 形参: 		bool par
// 作者：		码上看世界
// 说明：		一个外部接口，获取参数
// 日期：		2018/4/11 15:28:06
//************************************
void CameraThread::getParameter(bool par)
{
	if (AR_chasisCameraName == usedCameraName) {

	}
	else if (AR_RelativeCameraName == usedCameraName) {

	}
	else if (MR_BallRockCameraName == usedCameraName) {

	}
	else if (MR_chasisCameraName == usedCameraName) {

	}
	else {
		qDebug() << par;
	}
}

//************************************
// 函数名:   	CameraThread::dealWithData
// 访问控制:   	private 
// 返回值:   	void
// 形参: 		vector<float> & data
// 作者：		码上看世界
// 说明：		对图像处理程序输出的结果进行一些处理
// 日期：		2018/4/10 12:55:30
//************************************
void CameraThread::dealWithData(vector<float>& data)
{
	float fps = 1000 / time.msecsTo(QTime::currentTime());
	mutex.lock();
	int length = data.size();
	for (int i = 0; i < length - 1; i++) {
		dataShowed.append(tr("%1-----  ").arg(data.at(i)).toLocal8Bit());
		dataTransfered.append(myPro.createProtocolPacket(MyProtocol::PC, dataID.at(i), data.at(i)));
	}
	dataShowed.append(tr("%1  %2 FPS\n").arg(data.at(length-1)).arg(fps).toLocal8Bit());
	dataTransfered.append(myPro.createProtocolPacket(MyProtocol::PC, dataID.at(length - 1), data.at(length - 1)));
	mutex.unlock();
}

//************************************
// 函数名:   	CameraThread::dealWithData
// 访问控制:   	private 
// 返回值:   	void
// 形参: 		QByteArray data
// 作者：		码上看世界
// 说明：		对图像处理程序输出的结果进行一些处理
// 日期：		2018/4/12 18:32:25
//************************************
void CameraThread::dealWithData(QByteArray data)
{
	float fps = 1000 / time.msecsTo(QTime::currentTime());
	mutex.lock();
	int length = data.size();
	for (int i = 0; i < length - 1; i++) {
		dataShowed.append(tr("%1-----  ").arg(data.at(i)));
		dataTransfered.append(data.at(i));
	}
	dataShowed.append(tr("%1  %2 FPS\n").arg(data.at(length - 1)).arg(fps));
	dataTransfered.append(data.at(length - 1));
	mutex.unlock();
}

//************************************
// 函数名:   	CameraThread::run
// 访问控制:   	protected 
// 返回值:   	void
// 作者：		码上看世界
// 说明：		线程的事件循环
// 日期：		2018/4/10 09:34:59
//************************************
void CameraThread::run()
{
	if (AR_chasisCameraName == usedCameraName) {

	}
	else if (AR_RelativeCameraName == usedCameraName) {

	}
	else if(MR_BallRockCameraName == usedCameraName) {

	}
	else if (1/*MR_chasisCameraName == usedCameraName*/) {
		dataID << MyProtocol::MR_ChasisCamera_dx << MyProtocol::MR_ChasisCamera_dy << MyProtocol::MR_ChasisCamera_angle;

		line_MR.CloseOrOpenCamera(mv::MV_OPEN, index);
		/******************************************************/

		//事件循环
		while (isRun) {
			time = QTime::currentTime();
			dealWithData(line_MR.IFM());
		}

		//关闭摄像头
		line_MR.CloseOrOpenCamera(mv::MV_CLOSE, index);
	}
	else {
		//根据图像处理程序输出参数的个数设置数据ID的个数
		dataID << MyProtocol::Check << MyProtocol::Check << MyProtocol::Check;

		//打开摄像头以及初始化操作
		testCamera.openCamera(index);
		/******************************************************/
		
		//事件循环
		while (isRun) {
			time = QTime::currentTime();
			//图像处理程序
			testCamera.showPicture();
			dealWithData(testCamera.outputData());
		}

		//关闭摄像头
		testCamera.closeCamera(index);
		qDebug() << "finished";
	}
}

//************************************
// 函数名:   	CameraThread::showWarningInfo
// 访问控制:   	private 
// 返回值:   	void
// 形参: 		QString info
// 作者：		码上看世界
// 说明：		警告提示
// 日期：		2018/4/10 09:56:33
//************************************
void CameraThread::showWarningInfo(QString info)
{
	QMessageBox::warning(new QDialog, "warning", info, QMessageBox::Ok);
}

//************************************
// 函数名:   	CameraThread::delayMsec
// 访问控制:   	private 
// 返回值:   	void
// 形参: 		int msecs
// 作者：		码上看世界
// 说明：		实现毫秒级延时
// 日期：		2018/4/14 19:12:49
//************************************
void CameraThread::delayMsec(int msecs)
{
	QTime currentTime, lastTime;
	lastTime = QTime::currentTime();
	currentTime = QTime::currentTime();
	while (lastTime.msecsTo(currentTime) < msecs) {
		currentTime = QTime::currentTime();
	}
}

TestCamera::TestCamera()
{
}

TestCamera::~TestCamera()
{
}

bool TestCamera::openCamera(int index)
{
	if (testCamera.open(index)) {
		return true;
	}
	else {
		return false;
	}
}

void TestCamera::closeCamera(int index)
{
	testCamera.release();
}

void TestCamera::showPicture()
{
	Mat src;
	testCamera >> src;
	imshow("Test", src);
	waitKey(1);
}

vector<float> TestCamera::outputData()
{
	vector<float> test;
	test.push_back(66.66);
	test.push_back(-11.11);
	test.push_back(0.2345);
	return  test;
}
