
// ClassClientDlg.h : ͷ�ļ�
//

#pragma once
#ifndef PROCESSING_H
#define PROCESSING_H

#include <iostream>
#include <io.h>
#include "sift.h"
#include "imgfeatures.h"
#include "utils.h"
#include "svm.h"

#include <afx.h>
#include <string>
#include "highgui.h"
#include "cv.h"
#include "svm.h"
#include <fstream>

#include <iostream>
#include <stdlib.h>
#include <afx.h>
using namespace std;
using namespace cv;

struct Image_Label
{
	string strCategory;
	int nLabel;
};

//��ȡSIFT����
bool ExtractSift(CString strImgDir, CString strResultDir);

//ͨ�������ַ���������ǩ�б�
void CreateLabel(CString strResultDir);
//�����ǩ
Image_Label* LoadLabel(CString strResultDir, int& nLabelNum);

//KMeans����
bool KMeansCluster(int nClusterNum, CString strResultDir);

//ͳ�ƴ�Ƶ
bool CalcWordFreq(svm_problem& prob, int nClusterNum, CString strResultDir);

void SVMTrain(svm_problem prob, CString strResultDir);

void SVMPredict(CString strImg, CString strResultDir);

#endif

// CClassClientDlg �Ի���
class CClassClientDlg : public CDialogEx
{
// ����
public:
	CClassClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLASSCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
};
