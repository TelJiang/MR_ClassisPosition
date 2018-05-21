//////////////////////////////////////////////////////////////////////
// Video Capture using DirectShow
// Author: Shiqi Yu (shiqi.yu@gmail.com)
// Thanks to:
//		HardyAI@OpenCV China
//		flymanbox@OpenCV China (for his contribution to function CameraName, and frame width/height setting)
// Last modification: April 9, 2009
//
// ʹ��˵����
//   1. ��CameraDS.h CameraDS.cpp�Լ�Ŀ¼DirectShow���Ƶ������Ŀ��
//   2. �˵� Project->Settings->Settings for:(All configurations)->C/C++->Category(Preprocessor)->Additional include directories
//      ����Ϊ DirectShow/Include
//   3. �˵� Project->Settings->Settings for:(All configurations)->Link->Category(Input)->Additional library directories
//      ����Ϊ DirectShow/Lib
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
			bReadyGO = FALSE;  //ȷ��ԭ�Ӳ���  
			memcpy(this->pBuffer, pBuffer, lBufferSize);
			bReadyGO = TRUE;
		}
		// ֻʹ�ñ�־λ�Ļ�����û����֤һ���߳��ڶԱ�־λ���жϺ��޸�  
		// ��־λ����������֮�䲻�����������߳��޸ı�־λ��������Ҫ��  
		// һ��ԭ�Ӳ���ȥһ���ԵĻ��������жϺ��޸�����������������  
		// ��ֻ��һ���̻߳�ִ�иöδ��룬���Ա�־λ����  
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

	//������ͷ��nCamIDָ�����ĸ�����ͷ��ȡֵ����Ϊ0,1,2,...
	//bDisplayPropertiesָʾ�Ƿ��Զ���������ͷ����ҳ
	//nWidth��nHeight���õ�����ͷ�Ŀ�͸ߣ��������ͷ��֧�����趨�Ŀ�Ⱥ͸߶ȣ��򷵻�false
	bool OpenCamera(int nCamID, bool bDisplayProperties = true, int nWidth = 320, int nHeight = 240);

	//�ر�����ͷ�������������Զ������������
	void CloseCamera();

	//��������ͷ����Ŀ
	//���Բ��ô���CCameraDSʵ��������int c=CCameraDS::CameraCount();�õ������
	static int CameraCount();

	//��������ͷ�ı�ŷ�������ͷ������
	//nCamID: ����ͷ���
	//sName: ���ڴ������ͷ���ֵ�����
	//nBufferSize: sName�Ĵ�С
	//���Բ��ô���CCameraDSʵ��������CCameraDS::CameraName();�õ������
	static int CameraName(int nCamID, char* sName, int nBufferSize);

	//����ͼ����
	int GetWidth() { return m_nWidth; }

	//����ͼ��߶�
	int GetHeight() { return m_nHeight; }

	//ץȡһ֡�����ص�IplImage�����ֶ��ͷţ�
	//����ͼ�����ݵ�ΪRGBģʽ��Top-down(��һ���ֽ�Ϊ���Ͻ�����)����IplImage::origin=0(IPL_ORIGIN_TL)
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

	//������ͷ��nCamIDָ�����ĸ�����ͷ��ȡֵ����Ϊ0,1,2,...
	//bDisplayPropertiesָʾ�Ƿ��Զ���������ͷ����ҳ
	//nWidth��nHeight���õ�����ͷ�Ŀ�͸ߣ��������ͷ��֧�����趨�Ŀ�Ⱥ͸߶ȣ��򷵻�false
	bool OpenCamera(int nCamID, bool bDisplayProperties = true, int nWidth = 320, int nHeight = 240);

	//�ر�����ͷ�������������Զ������������
	void CloseCamera();

	//��������ͷ����Ŀ
	//���Բ��ô���CCameraDSʵ��������int c=CCameraDS::CameraCount();�õ������
	static int CameraCount();

	//��������ͷ�ı�ŷ�������ͷ������
	//nCamID: ����ͷ���
	//sName: ���ڴ������ͷ���ֵ�����
	//nBufferSize: sName�Ĵ�С
	//���Բ��ô���CCameraDSʵ��������CCameraDS::CameraName();�õ������
	static int CameraName(int nCamID, char* sName, int nBufferSize);

	//����ͼ����
	int GetWidth() { return m_nWidth; }

	//����ͼ��߶�
	int GetHeight() { return m_nHeight; }

	//ץȡһ֡�����ص�IplImage�����ֶ��ͷţ�
	//����ͼ�����ݵ�ΪRGBģʽ��Top-down(��һ���ֽ�Ϊ���Ͻ�����)����IplImage::origin=0(IPL_ORIGIN_TL)
	IplImage * QueryFrame();
};

#endif // 0

#endif 
