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
	ON_BN_CLICKED(IDC_SAVE, &CCaptureDlg::OnBnClickedSave)
	ON_WM_TIMER()
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

	Mat dst, g_src;
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
			dst.at<Vec3b>(i, j)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
			dst.at<Vec3b>(i, j)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);

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


void CCaptureDlg::OnBnClickedSave()
{
	// TODO:  在此添加控件通知处理程序代码

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

			if (m_ImageData8 == NULL)
			{
				// allocate now that we know the size of the image
				m_ImageData8 = calloc(width*height, sizeof(char));
			}





			// Use m_FG.GetNextImageFloat() to get a floating-point version of the image
			/*
			if (tmp)
			{
				float fpa, lens;
				m_FG.GetLastSensorReadings(&lens, &fpa);

				char buf[50];
				sprintf(buf, "Fpa, Lens = %.2f, %.2f", fpa, lens);
				SetDlgItemText(IDC_FPA_LENS, buf);


				sprintf(buf, "%.1f FPS", m_FG.GetAverageFPS());
				SetDlgItemText(IDC_FPS, buf);


				//
				// Sample of picking off a temperature from image data
				//
				int center_x, center_y;
				center_x = width / 2;
				center_y = height / 2;

				float temperature;
				if (m_CameraType != ICI_CAMERA_SWIR)
					temperature = (float)tmp[center_x + center_y * width] / 100.0;
				else
					temperature = (float)tmp[center_x + center_y * width];
				sprintf_s(buf, sizeof(buf), "%.2f", temperature);

				SetDlgItemText(IDC_CENTER_TEMP, buf);

				// highly non-optimal conversion from 16-bit to 8-bit

				// Values are in degrees C * 100.0
				// So, 1 deg C would == 100
				// My mind doesn't work well in deg C so I have a F_TO_C conversion
				//
				// topvalue represents the highest temperature value we want to display
				// bottomvalue represents the lowest temperature value we want to display
				//

				bool auto_range = true; // set to false to use manual range...

				// Manual range values
				float topvalue = (int)(F_TO_C(130) * 100.0);
				float bottomvalue = (int)(F_TO_C(50) * 100.0);


				int i, end;
				float value;
				unsigned short *src = (unsigned short *)tmp;
				unsigned char  *dest = (unsigned char  *)m_ImageData8;
				unsigned char displayValue;
				end = width * height;  // image size...


				if (auto_range)
				{
					bottomvalue = 99999;
					topvalue = -99999;
					for (i = 0; i < end; i++)
					{
						value = src[i];
						bottomvalue = min(bottomvalue, value);
						topvalue = max(topvalue, value);
					}
				}

				float range = topvalue - bottomvalue;

				if (range != 0)
				{

					for (i = 0; i < end; i++)
					{
						value = *src++;
						// Limit upper + lower values
						if (value < bottomvalue) value = bottomvalue;
						if (value > topvalue)    value = topvalue;
						// Scale to 0..255 for display
						displayValue = ((value - bottomvalue) * 255) / range;
						*dest++ = displayValue;
					}

					CWnd *w = GetDlgItem(IDC_IMAGE);
					if (w)
					{
						// invalidate the screen area of the image....
						CRect r;
						w->GetClientRect(&r);
						w->MapWindowPoints(this, (POINT*)&r, 2);

						InvalidateRect(r);
					}
				}
				
			}*/
		}
	}
}
