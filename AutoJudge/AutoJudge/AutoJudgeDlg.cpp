
// AutoJudgeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoJudge.h"
#include "AutoJudgeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoJudgeDlg �Ի���



CAutoJudgeDlg::CAutoJudgeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAutoJudgeDlg::IDD, pParent)
{
	_image = nullptr;
	_targetDC = nullptr;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	// FIXME: do not hard code.
	_recognize.InitClassifier("G:\\WarChess\\Serialization\\vocabulary.yml","G:\\WarChess\\Serialization\\SVM");
}

void CAutoJudgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoJudgeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CAutoJudgeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CAutoJudgeDlg::OnBnClickedButtonCapture)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAutoJudgeDlg ��Ϣ�������

BOOL CAutoJudgeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	_targetDC = GetDlgItem(IDC_STATIC_IMG1)->GetDC();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAutoJudgeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	if(_targetDC != nullptr)
	{
		CRect cr;
		GetDlgItem(IDC_STATIC_IMG1)->GetClientRect(&cr);
		_image ->Draw(_targetDC->m_hDC,cr, Gdiplus::InterpolationMode::InterpolationModeDefault);
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAutoJudgeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAutoJudgeDlg::OnBnClickedButtonStart()
{
	if(_capture.IsRunning())
		_capture.StopCapture();
	else
		_capture.StartCapture();
}


void CAutoJudgeDlg::OnBnClickedButtonCapture()
{
	if(!_capture.IsRunning()) return;
	cv::Mat frame = _capture.GetImage();
	int width    = frame.cols;
	int height   = frame.rows;
	int channels = frame.channels();
	if(_image != nullptr) delete _image;
	_image = new CImage();
	_image -> Create(width, height,8*channels ); //Ĭ��ͼ�����ص�ͨ��ռ��1���ֽ�

	//copy values
	uchar* ps;
	uchar* pimg = (uchar*)_image ->GetBits(); //A pointer to the bitmap buffer
	int step = _image ->GetPitch();

	for (int i = 0; i < height; ++i)
	{
		ps = (frame.ptr<uchar>(i));
		for ( int j = 0; j < width; ++j )
		{
			if ( channels == 1 ) //gray
			{
				*(pimg + i*step + j) = ps[j];
			}
			else if ( channels == 3 ) //color
			{
				for (int k = 0 ; k < 3; ++k )
				{
					*(pimg + i*step + j*3 + k ) = ps[j*3 + k];
				}			
			}
		}	
	}

	
	CRect cr;
	GetDlgItem(IDC_STATIC_IMG1)->GetClientRect(&cr);
	_image ->Draw(_targetDC->m_hDC,cr, Gdiplus::InterpolationMode::InterpolationModeDefault);
}


void CAutoJudgeDlg::OnClose()
{
	CDialogEx::OnClose();
}


void CAutoJudgeDlg::OnDestroy()
{
	_capture.StopCapture();
	if(_targetDC != nullptr) GetDlgItem(IDC_STATIC_IMG1)->ReleaseDC(_targetDC);
	if(_image != nullptr) delete _image;
	CDialogEx::OnDestroy();
}
