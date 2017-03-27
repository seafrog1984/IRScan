// CaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IRScan.h"
#include "CaptureDlg.h"
#include "afxdialogex.h"

#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

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
	ON_BN_CLICKED(IDC_CON, &CCaptureDlg::OnBnClickedCon)
END_MESSAGE_MAP()


// CCaptureDlg 消息处理程序


void CCaptureDlg::OnBnClickedCon()
{
	// TODO:  在此添加控件通知处理程序代码

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
	dlg.m_ofn.lpstrTitle = _T("打开图像文件"); //对话框标题
	dlg.m_ofn.lpstrInitialDir = "E:\\work\\实验室\\红外扫描\\ICI获取的图像"; //默认打开路径
	dlg.m_ofn.lpstrFilter = "bmp (*.bmp)\0*.bmp\0 jpg (*.jpg)\0*.jpg\0 All Files (*.*) \0*.*\0\0"; //打开文件类型

	if (dlg.DoModal() != IDOK)             // 判断是否获得图片         
		return;
	m_path = dlg.GetPathName();

	Mat src;

	//多字节字符集下 CString 转 char*  (LPSTR)(LPCSTR)
	src = imread((LPSTR)(LPCSTR)m_path);

	Mat dst,g_src;
	dst.create(src.size(), CV_8UC3);
	cvtColor(src, g_src, CV_BGR2GRAY);

	int i, j;
	for (i = 0; i < dst.rows; i++)
	{
		for (j = 0; j < dst.cols; j++)
		{
			int tmp = g_src.at<uchar>(i, j);
	/*
			dst.at<Vec3b>(i, j)[0] = 0;
			dst.at<Vec3b>(i, j)[1] = 0;
			dst.at<Vec3b>(i, j)[2] = tmp;
	*/
			dst.at<Vec3b>(i, j)[0] = 101.2 - 116.2*cos(tmp*0.08655) + 91.93*sin(tmp*0.08592);
			dst.at<Vec3b>(i, j)[1] = 150.9 - 110.9*cos(tmp*0.08457) -97.33*sin(tmp*0.08457);
			dst.at<Vec3b>(i, j)[2] = 125.3 +59.93*cos(tmp*0.04896) -130.2*sin(tmp*0.04896);
	
		/*
			if (tmp < 64)
			{
				dst.at<Vec3b>(i, j)[1] = 255;
				dst.at<Vec3b>(i, j)[0] = tmp*4;
				dst.at<Vec3b>(i, j)[2] = 0;
			}
			else if (tmp<128)
			{
				dst.at<Vec3b>(i, j)[1] = 255-(tmp-64)*4;
				dst.at<Vec3b>(i, j)[0] = 255;
				dst.at<Vec3b>(i, j)[2] = 0;
			}
			else if (tmp<192)
			{
				dst.at<Vec3b>(i, j)[1] = 0;
				dst.at<Vec3b>(i, j)[0] = 255;
				dst.at<Vec3b>(i, j)[2] = (tmp-128)*4;
			}
			else
			{
				dst.at<Vec3b>(i, j)[1] = 0;
				dst.at<Vec3b>(i, j)[0] = 255-(tmp-192)*4;
				dst.at<Vec3b>(i, j)[2] = 255;
			}
			*/
		}
	}

	namedWindow("test");

	imshow("test", g_src);
	Mat img;  //保存缩放后的图像
	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect); //获取图像显示区

	resize(dst, img, Size(rect.Width(), rect.Height()), 0, 0);

	imshow("view", img);

	/*CDC *pDC = GetDlgItem(IDC_PIC)->GetDC();//此时利用的是CWnd的成员函数GetDC
	CPoint m_ptOrigin(0, rect.Width() / 2), m_ptEnd(rect.Height(), rect.Width() / 2);
	pDC->MoveTo(m_ptOrigin);
	pDC->LineTo(m_ptEnd);
	ReleaseDC(pDC);
	*/

}


BOOL CCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
