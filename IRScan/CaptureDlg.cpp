// CaptureDlg.cpp : ʵ���ļ�
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
	DDX_Control(pDX, IDC_MSCOMM1, m_CtrlCom);
}


BEGIN_MESSAGE_MAP(CCaptureDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CON, &CCaptureDlg::OnBnClickedCon)
	ON_BN_CLICKED(IDC_SAVE, &CCaptureDlg::OnBnClickedSave)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_INIT, &CCaptureDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDC_ROT, &CCaptureDlg::OnBnClickedRot)
END_MESSAGE_MAP()


// CCaptureDlg ��Ϣ�������


void CCaptureDlg::OnBnClickedCon()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*
	m_CameraType = ICI_CAMERA_7000;
	int sta = m_FG.InitializeCamera(m_CameraType);

	CString tmp;
	tmp.Format(_T("%d"), sta);
	AfxMessageBox(tmp);
*/
	
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
	Mat img_show;  //�������ź��ͼ��
	CRect rect;
	GetDlgItem(IDC_PIC)->GetClientRect(&rect); //��ȡͼ����ʾ��

	resize(dst, img_show, Size(rect.Width(), rect.Height()), 0, 0);

	imshow("view", img_show);
	
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

	if (m_CtrlCom.get_PortOpen())
	{
		m_CtrlCom.put_PortOpen(FALSE);
	}
	m_CtrlCom.put_CommPort(3);                //ѡ��com3�����Ը��ݾ����������  
	m_CtrlCom.put_InBufferSize(1024);         //�������뻺�����Ĵ�С��Bytes  
	m_CtrlCom.put_OutBufferSize(1024);        //��������������Ĵ�С��Bytes  
	m_CtrlCom.put_Settings(_T("9600,n,8,1")); //������9600����У�飬8������λ��ֹͣλ1  
	m_CtrlCom.put_InputMode(1);               //1:��ʾ�Զ����Ʒ�ʽ��������  
	m_CtrlCom.put_RThreshold(1);              //����1��ʾÿ�����ڽ��ջ��������ж��ڻ����1���ַ�ʱ������һ���������ݵ�OnComm�¼�  
	m_CtrlCom.put_InputLen(0);                //���õ�ǰ������������0  
	if (!m_CtrlCom.get_PortOpen())
	{
		m_CtrlCom.put_PortOpen(TRUE);
	}
	else
	{
		AfxMessageBox(_T("Can not open serial port!"));
	}
	m_CtrlCom.get_Input();                    //��Ԥ���������������������  
	UpdateData(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CCaptureDlg::OnBnClickedSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*16bitͼ�񱣴�
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	compression_params.push_back(0);    // ��ѹ��png.

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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
			dlg.m_ofn.lpstrTitle = _T("��ͼ���ļ�"); //�Ի������
			dlg.m_ofn.lpstrInitialDir = "E:\\work\\ʵ����\\����ɨ��\\ICI��ȡ��ͼ��"; //Ĭ�ϴ�·��
			dlg.m_ofn.lpstrFilter = "bmp (*.bmp)\0*.bmp\0 jpg (*.jpg)\0*.jpg\0 All Files (*.*) \0*.*\0\0"; //���ļ�����

			if (dlg.DoModal() != IDOK)             // �ж��Ƿ���ͼƬ
			return;
			m_path = dlg.GetPathName();

			Mat src;

			//���ֽ��ַ����� CString ת char*  (LPSTR)(LPCSTR)
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
			Mat img_show;  //�������ź��ͼ��
			CRect rect;
			GetDlgItem(IDC_PIC)->GetClientRect(&rect); //��ȡͼ����ʾ��

			resize(dst, img_show, Size(rect.Width(), rect.Height()), 0, 0);

			imshow("view", img_show);

			/*CDC *pDC = GetDlgItem(IDC_PIC)->GetDC();//��ʱ���õ���CWnd�ĳ�Ա����GetDC
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CCaptureDlg::OnBnClickedRot()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_CtrlCom.put_Output(COleVariant("1111")); //��������
}
BEGIN_EVENTSINK_MAP(CCaptureDlg, CDialogEx)
	ON_EVENT(CCaptureDlg, IDC_MSCOMM1, 1, CCaptureDlg::OnOncommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CCaptureDlg::OnOncommMscomm1()
{
	// TODO:  �ڴ˴������Ϣ����������
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len, k;
	BYTE rxdata[2048];
	CString strtemp;
	if (m_CtrlCom.get_CommEvent() == 2)       //�¼�ֵΪ2��ʾ�����������ַ�  
	{
		variant_inp = m_CtrlCom.get_Input();  //��������  
		safearray_inp = variant_inp;        //VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���  
		len = safearray_inp.GetDim();       //�õ���Ч���ݳ���  
		for (k = 0; k < len; k++)
		{
			safearray_inp.GetElement(&k, rxdata + k);//ת��ΪBYTE������  
		}
		for (k = 0; k < len; k++)            //������ת��ΪCString�ͱ���  
		{
			BYTE bt = *(char*)(rxdata + k); //�ַ���  
			strtemp.Format(_T("%c"), bt);   //���ַ�������ʱ����strtemp���  
			m_sRXDATA += strtemp;           //���յ������ݷŵ��༭���Ӧ�ı�����  
		}
	}
	SetDlgItemText(IDC_EDIT_RXDATA, m_sRXDATA);
}
