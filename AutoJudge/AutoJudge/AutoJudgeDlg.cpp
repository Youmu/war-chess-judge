
// AutoJudgeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoJudge.h"
#include "AutoJudgeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAutoJudgeDlg 对话框



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


// CAutoJudgeDlg 消息处理程序

BOOL CAutoJudgeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	_targetDC = GetDlgItem(IDC_STATIC_IMG1)->GetDC();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAutoJudgeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
	_image -> Create(width, height,8*channels ); //默认图像像素单通道占用1个字节

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
