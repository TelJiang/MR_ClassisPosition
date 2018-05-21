#pragma once
#include <QObject>
#include <QThread>
#include <QCamera>
#include <QCameraInfo>
#include <QDebug>
#include <QMessageBox>
#include <QMutex>
#include <QTime>
#include <vector>
#include <opencv2/opencv.hpp>
#include "wust_vision/classis_mr.h"
#include "myprotocol.hpp"
using namespace cv;
using namespace std;

class TestCamera
{
public:
	TestCamera();
	~TestCamera();
	bool openCamera(int index);
	void closeCamera(int index);
	void showPicture();
	vector<float> outputData();
private:
	VideoCapture testCamera;
};

class CameraThread : public QThread
{
public:
	CameraThread(QObject *parent = 0);
	~CameraThread();
	static QStringList searchCamera();
	bool openCameraThread(QString cameraInfo);
	void closeCameraThread();
	QByteArray showCameraData(bool isShow = false);
	QByteArray transferCameraData(bool isTransfer = false);
	void getParameter(float par);
	void getParameter(QByteArray par);
	void getParameter(bool par);
protected:
	void run();
private:
	void dealWithData(vector <float> &data);
	void dealWithData(QByteArray data);
	void showWarningInfo(QString info);
	void delayMsec(int msecs);
	bool isRun;
	QByteArray dataTransfered;
	QByteArray dataShowed;
	static QStringList cameraName;
	QString usedCameraName;
	QString AR_chasisCameraName;
	QString AR_RelativeCameraName;
	QString MR_chasisCameraName;
	QString MR_BallRockCameraName;
	int index;
	MyProtocol myPro;
	QList<MyProtocol::DataID> dataID;
	QMutex mutex;
	QTime time;

	//图像处理程序对象申明
	TestCamera testCamera;
	mv::GetLineIFM_MR line_MR;
};

