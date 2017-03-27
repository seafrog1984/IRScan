#pragma once


// CCaptureDlg 对话框

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

	virtual BOOL OnInitDialog();
};
