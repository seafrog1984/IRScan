// CaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IRScan.h"
#include "CaptureDlg.h"
#include "afxdialogex.h"


// CCaptureDlg 对话框

IMPLEMENT_DYNAMIC(CCaptureDlg, CDialogEx)

CCaptureDlg::CCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCaptureDlg::IDD, pParent)
{

}

CCaptureDlg::~CCaptureDlg()
{
}

void CCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCaptureDlg, CDialogEx)
END_MESSAGE_MAP()


// CCaptureDlg 消息处理程序
