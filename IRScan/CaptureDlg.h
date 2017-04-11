#pragma once

#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
// CCaptureDlg �Ի���

#include "FGInterface.h"
#include "mscomm1.h"
using namespace cv;

class CCaptureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCaptureDlg)

public:
	CCaptureDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCaptureDlg();

// �Ի�������
	enum { IDD = IDD_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCon();

public:
	CString m_path;
	FGInterface m_FG;
	Mat img;
	int m_CameraType;
	CString m_sRXDATA;
    CString m_sTXDATA;


	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedInit();
	afx_msg void OnBnClickedRot();
	CMscomm1 m_CtrlCom;
	DECLARE_EVENTSINK_MAP()
	void OnOncommMscomm1();
};
