#pragma once


#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
// CCaptureDlg 对话框

#include "FGInterface.h"
using namespace cv;

class CCaptureDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCaptureDlg)

public:
	CCaptureDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCaptureDlg();

// 对话框数据
	enum { IDD = IDD_CAPTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCon();

public:
	CString m_path;
	FGInterface m_FG;
	Mat img;
	int m_CameraType;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSave();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedInit();
};
