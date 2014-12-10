
// AutoJudgeDlg.h : ͷ�ļ�
//

#pragma once
#include "opencvcapture.h"
#include "Recognize.h"
#include "afxwin.h"

// CAutoJudgeDlg �Ի���
class CAutoJudgeDlg : public CDialogEx
{
// ����
public:
	CAutoJudgeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AUTOJUDGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
