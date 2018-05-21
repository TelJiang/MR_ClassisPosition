//////////////////////////////////////////////////////////////////////
// Video Capture using DirectShow
// Author: Shiqi Yu (shiqi.yu@gmail.com)
// Thanks to:
//		HardyAI@OpenCV China
//		flymanbox@OpenCV China (for his contribution to function CameraName, and frame width/height setting)
// Last modification: April 9, 2009
//
// 使用说明：
//   1. 将CameraDS.h CameraDS.cpp以及目录DirectShow复制到你的项目中
//   2. 菜单 Project->Settings->Settings for:(All configurations)->C/C++->Category(Preprocessor)->Additional include directories
//      设置为 DirectShow/Include
//   3. 菜单 Project->Settings->Settings for:(All configurations)->Link->Category(Input)->Additional library directories
//      设置为 DirectShow/Lib
//////////////////////////////////////////////////////////////////////

#ifndef CCAMERA_H
#define CCAMERA_H

#define WIN32_LEAN_AND_MEAN

#include <atlbase.h>

#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__
#include "qedit.h"
#include <dshow.h>
#include <windows.h>
#include <cxcore.h>
#include <iostream>

#define MYFREEMEDIATYPE(mt)	{if ((mt).cbFormat != 0)		\
					{CoTaskMemFree((PVOID)(mt).pbFormat);	\
					(mt).cbFormat = 0;						\
					(mt).pbFormat = NULL;					\
				}											\
				if ((mt).pUnk != NULL)						\
				{											\
					(mt).pUnk->Release();					\
					(mt).pUnk = NULL;						\
				}}									




#if 1

class CSampleGrabberCB : public ISampleGrabberCB
{
public:
	long       lWidth;
	long       lHeight;
	BOOL       bGrabVideo;

	BOOL       bReadyGO;
	double     dblSampleTime;
	BYTE	   *pBuffer;
	long       lBufferSize;
public:
	CSampleGrabberCB() {
		lWidth = 0;
		lHeight = 0;
		lBufferSize = 0;
		dblSampleTime = 0.0;
		bGrabVideo = FALSE;
		bReadyGO = FALSE;
		pBuffer = NULL;
	}
	~CSampleGrabberCB() {
		if (pBuffer) {
			delete[]pBuffer;
			pBuffer = NULL;
		}
	}
	STDMETHODIMP_(ULONG) AddRef() { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv) {
		if (riid == IID_ISampleGrabberCB || riid == IID_IUnknown) {
			*ppv = static_cast<void *>(this);
			return NOERROR;
		}
		return E_NOINTERFACE;
	}
	STDMETHODIMP SampleCB(double SampleTime, IMediaSample * pSample) {
		return 0;
	}
	STDMETHODIMP BufferCB(double dblSampleTime, BYTE * pBuffer, long lBufferSize) {
		if (!pBuffer) return E_POINTER;
		if (!this->pBuffer) this->pBuffer = new BYTE[lBufferSize];
		this->dblSampleTime = dblSampleTime;
		this->lBufferSize = lBufferSize;
		if (!bGrabVideo) {
			bReadyGO = FALSE;  //确保原子操作  
			memcpy(this->pBuffer, pBuffer, lBufferSize);
			bReadyGO = TRUE;
		}
		// 只使用标志位的话，你没法保证一个线程在对标志位做判断和修改  
		// 标志位这两个操作之间不会有其它的线程修改标志位。所以需要有  
		// 一个原子操作去一次性的互斥的完成判断和修改这两个操作。但这  
		// 里只有一个线程会执行该段代码，所以标志位足矣  
		return 0;
	}
};

class CCameraDS
{
private:

	bool m_bConnected, m_bLock, m_bChanged;

	int m_nWidth, m_nHeight;

	long m_nBufferSize;

	IplImage *m_pFrame;

	CComPtr<IGraphBuilder> m_pGraph;

	CComPtr<ISampleGrabber> m_pSampleGrabber;

	CComPtr<IMediaControl> m_pMediaControl;

	CComPtr<IMediaEvent> m_pMediaEvent;

	CComPtr<IBaseFilter> m_pSampleGrabberFilter;
	CComPtr<IBaseFilter> m_pDeviceFilter;
	CComPtr<IBaseFilter> m_pNullFilter;

	CComPtr<IPin> m_pGrabberInput;
	CComPtr<IPin> m_pGrabberOutput;
	CComPtr<IPin> m_pCameraOutput;
	CComPtr<IPin> m_pNullInputPin;

	bool BindFilter(int nCamIDX, IBaseFilter **pFilter);

	void SetCrossBar();

	CSampleGrabberCB *m_pCB = new CSampleGrabberCB;

public:

	CCameraDS();
	virtual ~CCameraDS();

	//打开摄像头，nCamID指定打开哪个摄像头，取值可以为0,1,2,...
	//bDisplayProperties指示是否自动弹出摄像头属性页
	//nWidth和nHeight设置的摄像头的宽和高，如果摄像头不支持所设定的宽度和高度，则返回false
	bool OpenCamera(int nCamID, bool bDisplayProperties = true, int nWidth = 320, int nHeight = 240);

	//关闭摄像头，析构函数会自动调用这个函数
	void CloseCamera();

	//返回摄像头的数目
	//可以不用创建CCameraDS实例，采用int c=CCameraDS::CameraCount();得到结果。
	static int CameraCount();

	//根据摄像头的编号返回摄像头的名字
	//nCamID: 摄像头编号
	//sName: 用于存放摄像头名字的数组
	//nBufferSize: sName的大小
	//可以不用创建CCameraDS实例，采用CCameraDS::CameraName();得到结果。
	static int CameraName(int nCamID, char* sName, int nBufferSize);

	//返回图像宽度
	int GetWidth() { return m_nWidth; }

	//返回图像高度
	int GetHeight() { return m_nHeight; }

	//抓取一帧，返回的IplImage不可手动释放！
	//返回图像数据的为RGB模式的Top-down(第一个字节为左上角像素)，即IplImage::origin=0(IPL_ORIGIN_TL)
	IplImage * QueryFrame();
};


#endif

#if 0
class CCameraDS
{
private:

	bool m_bConnected, m_bLock, m_bChanged;

	int m_nWidth, m_nHeight;

	long m_nBufferSize;

	IplImage *m_pFrame;

	CComPtr<IGraphBuilder> m_pGraph;

	CComPtr<ISampleGrabber> m_pSampleGrabber;

	CComPtr<IMediaControl> m_pMediaControl;

	CComPtr<IMediaEvent> m_pMediaEvent;

	CComPtr<IBaseFilter> m_pSampleGrabberFilter;
	CComPtr<IBaseFilter> m_pDeviceFilter;
	CComPtr<IBaseFilter> m_pNullFilter;

	CComPtr<IPin> m_pGrabberInput;
	CComPtr<IPin> m_pGrabberOutput;
	CComPtr<IPin> m_pCameraOutput;
	CComPtr<IPin> m_pNullInputPin;

	bool BindFilter(int nCamIDX, IBaseFilter **pFilter);

	void SetCrossBar();

public:

	CCameraDS();
	virtual ~CCameraDS();

	//打开摄像头，nCamID指定打开哪个摄像头，取值可以为0,1,2,...
	//bDisplayProperties指示是否自动弹出摄像头属性页
	//nWidth和nHeight设置的摄像头的宽和高，如果摄像头不支持所设定的宽度和高度，则返回false
	bool OpenCamera(int nCamID, bool bDisplayProperties = true, int nWidth = 320, int nHeight = 240);

	//关闭摄像头，析构函数会自动调用这个函数
	void CloseCamera();

	//返回摄像头的数目
	//可以不用创建CCameraDS实例，采用int c=CCameraDS::CameraCount();得到结果。
	static int CameraCount();

	//根据摄像头的编号返回摄像头的名字
	//nCamID: 摄像头编号
	//sName: 用于存放摄像头名字的数组
	//nBufferSize: sName的大小
	//可以不用创建CCameraDS实例，采用CCameraDS::CameraName();得到结果。
	static int CameraName(int nCamID, char* sName, int nBufferSize);

	//返回图像宽度
	int GetWidth() { return m_nWidth; }

	//返回图像高度
	int GetHeight() { return m_nHeight; }

	//抓取一帧，返回的IplImage不可手动释放！
	//返回图像数据的为RGB模式的Top-down(第一个字节为左上角像素)，即IplImage::origin=0(IPL_ORIGIN_TL)
	IplImage * QueryFrame();
};

#endif // 0

#endif 
