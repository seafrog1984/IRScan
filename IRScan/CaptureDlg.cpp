// CaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "IRScan.h"
#include "CaptureDlg.h"
#include "afxdialogex.h"
#include <time.h>

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
	ON_BN_CLICKED(IDC_SAVE, &CCaptureDlg::OnBnClickedSave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_INIT, &CCaptureDlg::OnBnClickedInit)
END_MESSAGE_MAP()


// CCaptureDlg 消息处理程序


void CCaptureDlg::OnBnClickedCon()
{
	// TODO:  在此添加控件通知处理程序代码

	m_CameraType = ICI_CAMERA_7000;
	int sta = m_FG.InitializeCamera(m_CameraType);

	CString tmp;
	tmp.Format(_T("%d"), sta);
	AfxMessageBox(tmp);

	/*
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

	Mat dst, g_src;
	dst.create(src.size(), CV_8UC3);
	cvtColor(src, g_src, CV_BGR2GRAY);

	int i, j;
	for (i = 0; i < dst.rows; i++)
	{
	for (j = 0; j < dst.cols; j++)
	{
	int tmp = g_src.at<uchar>(i, j);

	dst.at<Vec3b>(i, j)[0] = 101.2 - 116.2*cos(tmp*0.08655) + 91.93*sin(tmp*0.08592);
	dst.at<Vec3b>(i, j)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
	dst.at<Vec3b>(i, j)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);


	}
	}

	namedWindow("test");

	imshow("test", g_src);
	Mat img_show;  //保存缩放后的图像
	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect); //获取图像显示区

	resize(dst, img_show, Size(rect.Width(), rect.Height()), 0, 0);

	imshow("view", img_show);
	*/
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


void CCaptureDlg::OnBnClickedSave()
{
	// TODO:  在此添加控件通知处理程序代码
	/*16bit图像保存
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(0);    // 无压缩png.

	imwrite("1.png", img, compression_params);
	namedWindow("test");
	imshow("test", img);
	*/
	/*
	time_t nowTime;
	nowTime = time(NULL);

	struct tm *systime = localtime(&nowTime);
	int year = 1900 + systime->tm_year;
	int month = 1 + systime->tm_mon;
	int day = systime->tm_mday;
	*/
	imwrite("1.bmp", img);


}


void CCaptureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);

#define F_TO_C(x)  ((((x)-32.0) * 5.0) / 9.0)
#define C_TO_F(x)    ((32.0) + ((x) * (9.0/5.0)))
	if (nIDEvent == 2)
	{
		//InterrogateFocus();
	}
	if (nIDEvent == 1)
	{
		// The first couple images coming out may not have good temperature data...
		// This is because the camera supervisor routine (in the API library)
		// may not have completed the camera setup procedures.
		int ImageReady = m_FG.GetImageReady();
		if (ImageReady)
		{
			unsigned short *tmp;
			long width, height;


			if (m_FG.GetImageSize(&width, &height) == 0)
			{
				// error... no camera or dll not loaded
				return;
			}

			tmp = m_FG.GetNextImage();
			if (tmp == NULL)
				return;

			Mat src(height, width, CV_8UC1, tmp);
			//	img.create(height, width, CV_16UC1, tmp);
			src.copyTo(img);


			/*
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
			*/
			Mat dst, g_src(img);
			dst.create(img.size(), CV_8UC3);
			//cvtColor(img, g_src, CV_BGR2GRAY);

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
					dst.at<Vec3b>(i, j)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
					dst.at<Vec3b>(i, j)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);


				}
			}

			namedWindow("test");

			imshow("test", g_src);
			Mat img_show;  //保存缩放后的图像
			CRect rect;
			GetDlgItem(IDC_PIC)->GetClientRect(&rect); //获取图像显示区

			resize(dst, img_show, Size(rect.Width(), rect.Height()), 0, 0);

			imshow("view", img_show);

			/*CDC *pDC = GetDlgItem(IDC_PIC)->GetDC();//此时利用的是CWnd的成员函数GetDC
			CPoint m_ptOrigin(0, rect.Width() / 2), m_ptEnd(rect.Height(), rect.Width() / 2);
			pDC->MoveTo(m_ptOrigin);
			pDC->LineTo(m_ptEnd);
			ReleaseDC(pDC);
			*/
		}
	}
}



void CCaptureDlg::OnBnClickedInit()
{
	// TODO:  在此添加控件通知处理程序代码
}
