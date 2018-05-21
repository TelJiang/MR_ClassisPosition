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
// ������:   	CameraThread::searchCamera
// ���ʿ���:   	public static 
// ����ֵ:   	QT_NAMESPACE::QStringList
// ���ߣ�		���Ͽ�����
// ˵����		��������ͷ����ȡ����ͷ��Ϣ��unique��
// ���ڣ�		2018/4/10 08:33:51
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
// ������:   	CameraThread::openCameraThread
// ���ʿ���:   	public 
// ����ֵ:   	bool
// �β�: 		QString cameraInfo
// ���ߣ�		���Ͽ�����
// ˵����		������ͷ�߳�
// ���ڣ�		2018/4/10 08:45:37
//************************************
bool CameraThread::openCameraThread(QString cameraInfo)
{
	usedCameraName = cameraInfo;
	index = cameraName.indexOf(cameraInfo);
	if (index < 0) {
		showWarningInfo(tr("�����¼���ϵͳ�е�����ͷ�豸"));
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
// ������:   	CameraThread::closeCameraThread
// ���ʿ���:   	public 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�ر�����ͷ�߳�
// ���ڣ�		2018/4/10 16:19:22
//************************************
void CameraThread::closeCameraThread()
{
	isRun = false;
	delayMsec(1000);
}

//************************************
// ������:   	CameraThread::showCameraData
// ���ʿ���:   	public 
// ����ֵ:   	QT_NAMESPACE::QByteArray
// �β�: 		bool isShow
// ���ߣ�		���Ͽ�����
// ˵����		����ȥ��ʾͼ����Ľ��
// ���ڣ�		2018/4/10 12:58:27
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
// ������:   	CameraThread::transferCameraData
// ���ʿ���:   	public 
// ����ֵ:   	QT_NAMESPACE::QByteArray
// �β�: 		bool isTransfer
// ���ߣ�		���Ͽ�����
// ˵����		���ڴ���ͼ����Ľ��
// ���ڣ�		2018/4/10 12:59:10
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
// ������:   	CameraThread::getParameter
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		float par
// ���ߣ�		���Ͽ�����
// ˵����		һ���ⲿ�ӿڣ���ȡ����
// ���ڣ�		2018/4/11 15:27:14
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
// ������:   	CameraThread::getParameter
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		QByteArray par
// ���ߣ�		���Ͽ�����
// ˵����		һ���ⲿ�ӿڣ���ȡ����
// ���ڣ�		2018/4/11 15:28:04
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
// ������:   	CameraThread::getParameter
// ���ʿ���:   	public 
// ����ֵ:   	void
// �β�: 		bool par
// ���ߣ�		���Ͽ�����
// ˵����		һ���ⲿ�ӿڣ���ȡ����
// ���ڣ�		2018/4/11 15:28:06
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
// ������:   	CameraThread::dealWithData
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		vector<float> & data
// ���ߣ�		���Ͽ�����
// ˵����		��ͼ�����������Ľ������һЩ����
// ���ڣ�		2018/4/10 12:55:30
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
// ������:   	CameraThread::dealWithData
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		QByteArray data
// ���ߣ�		���Ͽ�����
// ˵����		��ͼ�����������Ľ������һЩ����
// ���ڣ�		2018/4/12 18:32:25
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
// ������:   	CameraThread::run
// ���ʿ���:   	protected 
// ����ֵ:   	void
// ���ߣ�		���Ͽ�����
// ˵����		�̵߳��¼�ѭ��
// ���ڣ�		2018/4/10 09:34:59
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

		//�¼�ѭ��
		while (isRun) {
			time = QTime::currentTime();
			dealWithData(line_MR.IFM());
		}

		//�ر�����ͷ
		line_MR.CloseOrOpenCamera(mv::MV_CLOSE, index);
	}
	else {
		//����ͼ���������������ĸ�����������ID�ĸ���
		dataID << MyProtocol::Check << MyProtocol::Check << MyProtocol::Check;

		//������ͷ�Լ���ʼ������
		testCamera.openCamera(index);
		/******************************************************/
		
		//�¼�ѭ��
		while (isRun) {
			time = QTime::currentTime();
			//ͼ�������
			testCamera.showPicture();
			dealWithData(testCamera.outputData());
		}

		//�ر�����ͷ
		testCamera.closeCamera(index);
		qDebug() << "finished";
	}
}

//************************************
// ������:   	CameraThread::showWarningInfo
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		QString info
// ���ߣ�		���Ͽ�����
// ˵����		������ʾ
// ���ڣ�		2018/4/10 09:56:33
//************************************
void CameraThread::showWarningInfo(QString info)
{
	QMessageBox::warning(new QDialog, "warning", info, QMessageBox::Ok);
}

//************************************
// ������:   	CameraThread::delayMsec
// ���ʿ���:   	private 
// ����ֵ:   	void
// �β�: 		int msecs
// ���ߣ�		���Ͽ�����
// ˵����		ʵ�ֺ��뼶��ʱ
// ���ڣ�		2018/4/14 19:12:49
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
