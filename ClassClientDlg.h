
// ClassClientDlg.h : 头文件
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

//提取SIFT特征
bool ExtractSift(CString strImgDir, CString strResultDir);

//通过解析字符串构建标签列表
void CreateLabel(CString strResultDir);
//导入标签
Image_Label* LoadLabel(CString strResultDir, int& nLabelNum);

//KMeans聚类
bool KMeansCluster(int nClusterNum, CString strResultDir);

//统计词频
bool CalcWordFreq(svm_problem& prob, int nClusterNum, CString strResultDir);

void SVMTrain(svm_problem prob, CString strResultDir);

void SVMPredict(CString strImg, CString strResultDir);

#endif

// CClassClientDlg 对话框
class CClassClientDlg : public CDialogEx
{
// 构造
public:
	CClassClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLASSCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
};
