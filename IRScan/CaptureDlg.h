#pragma once


// CCaptureDlg �Ի���

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

	virtual BOOL OnInitDialog();
};
