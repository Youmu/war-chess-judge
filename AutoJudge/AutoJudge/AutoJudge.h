
// AutoJudge.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAutoJudgeApp:
// �йش����ʵ�֣������ AutoJudge.cpp
//

class CAutoJudgeApp : public CWinApp
{
public:
	CAutoJudgeApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAutoJudgeApp theApp;