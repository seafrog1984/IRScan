// CaptureDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IRScan.h"
#include "CaptureDlg.h"
#include "afxdialogex.h"
#include <time.h>
#include<fstream>

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
	m_FG.SetCameraFocusSpeed(0, 0);  // stop!
	m_FG.DestroyCamera();
	//free(m_ImageData8);
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
	SetTimer(1, 0, NULL);

	// set the timer to interrogate the focus position
	// interrogating the focus position takes time away
	// from image transfers so should not be used excessively
	SetTimer(2, 300, NULL);

	m_CameraType = ICI_CAMERA_7000;
	// Initialize camera
	m_FG.InitializeCamera(m_CameraType);
	//m_FG.SetCalibrationFormula(3);    // will auto-set based on camera internal settings
//	m_ImageData8 = NULL;


	// Allocate later.. after image size is determined
	/*
	CString tmp;
	tmp.Format(_T("%d"), sta);
	AfxMessageBox(tmp);
	*/
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
	
*/

}


BOOL CCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	/*
	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_PIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	*/

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
//	16bitͼ�񱣴�

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

		ofstream fout("b.dat");

		for (int i = 0; i < width*height; i++)
		{
			fout<<*tmp++<<' ';
		}

	
		fout.close();

	//	Mat src(height, width, CV_16UC1, tmp);
	//	//	img.create(height, width, CV_16UC1, tmp);
	////	src.copyTo(img);
	//	vector<int> compression_params;
	//	compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	//	compression_params.push_back(0);    // ��ѹ��png.
	//	imwrite("1.png", src, compression_params);
	}
		
	/*
	time_t nowTime;
	nowTime = time(NULL);

	struct tm *systime = localtime(&nowTime);
	int year = 1900 + systime->tm_year;
	int month = 1 + systime->tm_mon;
	int day = systime->tm_mday;
	*/
//	imwrite("1.bmp", img);


}

void CCaptureDlg::InterrogateFocus(void)
{
	long rc = m_FG.UpdateFocusReadings();
	if (rc == 0) {
		// Error reading the focus settings...
		// Either the camera doesn't have a focus motor
		// or the command timed out
	//	SetDlgItemText(IDC_FOCUS_POSITION, "N/A");
		return;
	}
	long curpos;    // current focus position
	rc = m_FG.GetFocusPosition(&curpos);
	if (rc == 0) {
		// error reading the position
		// this should not happen if UpdateFocusReadings 
		// is successful
	//	SetDlgItemText(IDC_FOCUS_POSITION, "N/A");
		return;
	}

//	SetDlgItemInt(IDC_FOCUS_POSITION, curpos, false);
}

/*
void CCaptureDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);

	// my brain doesn't work well in degrees C
#define F_TO_C(x)  ((((x)-32.0) * 5.0) / 9.0)
#define C_TO_F(x)    ((32.0) + ((x) * (9.0/5.0)))
	if (nIDEvent == 2) {
		InterrogateFocus();
	}
	if (nIDEvent == 1) {
		// The first couple images coming out may not have good temperature data...
		// This is because the camera supervisor routine (in the API library)
		// may not have completed the camera setup procedures.
		int ImageReady = m_FG.GetImageReady();
		if (ImageReady) {
			unsigned short *tmp;
			long width, height;


			if (m_FG.GetImageSize(&width, &height) == 0) {
				// error... no camera or dll not loaded
				return;
			}

			tmp = m_FG.GetNextImage();
			if (tmp == NULL)
				return;

			if (m_ImageData8 == NULL) {
				// allocate now that we know the size of the image
				m_ImageData8 = calloc(width*height, sizeof(char));
			}





			// Use m_FG.GetNextImageFloat() to get a floating-point version of the image

			if (tmp) {
				float fpa, lens;
				m_FG.GetLastSensorReadings(&lens, &fpa);

				char buf[50];
				sprintf_s(buf, "Fpa, Lens = %.2f, %.2f", fpa, lens);
		//		SetDlgItemText(IDC_FPA_LENS, buf);


				sprintf_s(buf, "%.1f FPS", m_FG.GetAverageFPS());
//				SetDlgItemText(IDC_FPS, buf);


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

//				SetDlgItemText(IDC_CENTER_TEMP, buf);

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


				if (auto_range) {
					bottomvalue = 99999;
					topvalue = -99999;
					for (i = 0; i < end; i++) {
						value = src[i];
						bottomvalue = min(bottomvalue, value);
						topvalue = max(topvalue, value);
					}
				}

				float range = topvalue - bottomvalue;

				if (range != 0) {

					for (i = 0; i < end; i++) {
						value = *src++;
						// Limit upper + lower values
						if (value < bottomvalue) value = bottomvalue;
						if (value > topvalue)    value = topvalue;
						// Scale to 0..255 for display
						displayValue = ((value - bottomvalue) * 255) / range;
						*dest++ = displayValue;
					}

					CWnd *w = GetDlgItem(IDC_PIC);
					if (w) {
						// invalidate the screen area of the image....
						CRect r;
						w->GetClientRect(&r);
						w->MapWindowPoints(this, (POINT*)&r, 2);

						InvalidateRect(r);
					}
				}
			}
		}
	}
}

*/

void CCaptureDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnTimer(nIDEvent);

#define F_TO_C(x)  ((((x)-32.0) * 5.0) / 9.0)
#define C_TO_F(x)    ((32.0) + ((x) * (9.0/5.0)))
	if (nIDEvent == 2)
	{
		InterrogateFocus();
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

			namedWindow("src");
			imshow("src", src);

			//	img.create(height, width, CV_16UC1, tmp);
			src.copyTo(img);
		//	cvtColor(src,img , CV_BGR2GRAY);

			Mat dst, g_src(img),g_dst;
			dst.create(img.size(), CV_8UC3);
			g_dst.create(img.size(), CV_8UC1);
			//cvtColor(img, g_src, CV_BGR2GRAY);

			if (tmp)
			{
				float fpa, lens;
				m_FG.GetLastSensorReadings(&lens, &fpa);

				char buf[50];
				sprintf_s(buf, "Fpa, Lens = %.2f, %.2f", fpa, lens);
				//	SetDlgItemText(IDC_FPA_LENS, buf);


				sprintf_s(buf, "%.1f FPS", m_FG.GetAverageFPS());
				//	SetDlgItemText(IDC_FPS, buf);


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

				//	SetDlgItemText(IDC_CENTER_TEMP, buf);

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


				int i, end, col, row;
				float value;
				unsigned short *src = (unsigned short *)tmp;
				//unsigned char  *dest = (unsigned char  *)m_ImageData8;
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

						row = i / width;
						col = i%width;
						g_dst.at<uchar>(row, col) = displayValue;
					}
				}
			}
			//int i, j;
			//for (i = 0; i < dst.rows; i++)
			//{
			//	for (j = 0; j < dst.cols; j++)
			//	{
			//		int tmp = g_src.at<uchar>(i, j);

			//		dst.at<Vec3b>(i, j)[0] = 101.2 - 116.2*cos(tmp*0.08655) + 91.93*sin(tmp*0.08592);
			//		dst.at<Vec3b>(i, j)[1] = 150.9 - 110.9*cos(tmp*0.08457) - 97.33*sin(tmp*0.08457);
			//		dst.at<Vec3b>(i, j)[2] = 125.3 + 59.93*cos(tmp*0.04896) - 130.2*sin(tmp*0.04896);


			//	}
			//}

			namedWindow("test");

			imshow("test", g_dst);


			//Mat img_show;  //�������ź��ͼ��
			//CRect rect;
			//GetDlgItem(IDC_PIC)->GetClientRect(&rect); //��ȡͼ����ʾ��

			//resize(dst, img_show, Size(rect.Width(), rect.Height()), 0, 0);

			//imshow("view", img_show);
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
	m_CtrlCom.put_Output(COleVariant("#1100\r\n")); //��������
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
