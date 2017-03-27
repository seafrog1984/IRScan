// CaptureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IRScan.h"
#include "CaptureDlg.h"
#include "afxdialogex.h"

#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

// CCaptureDlg �Ի���

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


// CCaptureDlg ��Ϣ�������


void CCaptureDlg::OnBnClickedCon()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL);
	dlg.m_ofn.lpstrTitle = _T("��ͼ���ļ�"); //�Ի������
	dlg.m_ofn.lpstrInitialDir = "E:\\work\\ʵ����\\����ɨ��\\ICI��ȡ��ͼ��"; //Ĭ�ϴ�·��
	dlg.m_ofn.lpstrFilter = "bmp (*.bmp)\0*.bmp\0 jpg (*.jpg)\0*.jpg\0 All Files (*.*) \0*.*\0\0"; //���ļ�����

	if (dlg.DoModal() != IDOK)             // �ж��Ƿ���ͼƬ         
		return;
	m_path = dlg.GetPathName();

	Mat src;

	//���ֽ��ַ����� CString ת char*  (LPSTR)(LPCSTR)
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
	Mat img;  //�������ź��ͼ��
	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect); //��ȡͼ����ʾ��

	resize(dst, img, Size(rect.Width(), rect.Height()), 0, 0);

	imshow("view", img);

	/*CDC *pDC = GetDlgItem(IDC_PIC)->GetDC();//��ʱ���õ���CWnd�ĳ�Ա����GetDC
	CPoint m_ptOrigin(0, rect.Width() / 2), m_ptEnd(rect.Height(), rect.Width() / 2);
	pDC->MoveTo(m_ptOrigin);
	pDC->LineTo(m_ptEnd);
	ReleaseDC(pDC);
	*/

}


BOOL CCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
