
// AutoJudgeDlg.h : 头文件
//

#pragma once
#include "opencvcapture.h"
#include "Recognize.h"
#include "afxwin.h"

// CAutoJudgeDlg 对话框
class CAutoJudgeDlg : public CDialogEx
{
// 构造
public:
	CAutoJudgeDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_AUTOJUDGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonCapture();
private:
	OpenCVCapture _capture;
	CDC *_targetDC;
	CImage *_image;
	Recognize _recognize;
	
public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	CEdit ResultBox;
};
