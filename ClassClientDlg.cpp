
// ClassClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClassClient.h"
#include "ClassClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClassClientDlg 对话框




CClassClientDlg::CClassClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClassClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClassClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClassClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CClassClientDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CClassClientDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// CClassClientDlg 消息处理程序

BOOL CClassClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClassClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClassClientDlg::OnPaint()
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
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClassClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

string GetCategory(string strFileName)
{
	int nStart = 0;
	int nEnd;
	for (int i = strFileName.length() - 1; i > 0; i--)
	{
		if (strFileName[i] == '\\')
		{
			nStart = i + 1;
			break;
		}
	}
	for (int i = nStart; i < strFileName.length(); i++)
	{
		if (strFileName[i] == '_' && strFileName[i + 1] == '#' && strFileName[i + 2] == '_')
		{
			nEnd = i - 1;
			break;
		}
	}
	string strCategory = strFileName.substr(nStart, nEnd - nStart + 1);
	return strCategory;
}


//提取SIFT特征
bool ExtractSift(CString strImgDir, CString strResultDir)
{
	CFileFind finder;
	
	CString strSiftDrawDir = strResultDir + ("\\SiftDraw");
	CString strFeatureDir = strResultDir + ("\\Feature");
	CString strInfoFile = strResultDir + ("\\Info.ini");
	CString strListFile = strResultDir + ("\\ImgPath.list");
	CString strFileName;

	BOOL haveNext = finder.FindFile(strImgDir + "\\*.*");

	//检查文件夹路径
	if(haveNext)
	{
		if(!PathIsDirectory(strImgDir))
		{
			printf("Error: 图片源目录路径不正确！");
			return false;
		}
		if(!PathIsDirectory(strResultDir) && !CreateDirectory(strResultDir, NULL))
		{
			printf("Error: 结果保存文件夹创建失败！");
			return false;
		}
		if(!PathIsDirectory(strSiftDrawDir) && !CreateDirectory(strSiftDrawDir, NULL))
		{
			printf("Error: 图片特征绘制文件夹创建失败！");
			return false;
		}
		if(!PathIsDirectory(strFeatureDir) && !CreateDirectory(strFeatureDir, NULL))
		{
			printf("Error: 特征文件夹创建失败！");
			return false;
		}
	}
	else
	{
		printf("Error: 没有发现图片文件！");
		return false;
	}
	
	//Do sift
	printf("-----------------%s文件夹 特征提取开始-------------\n", strImgDir);
	CStdioFile file;
	file.Open(strImgDir, CStdioFile::modeCreate | CStdioFile::modeWrite);
	int featureNum = 0;
	ofstream foutList(strListFile, ios::out | ios::app);
	while(haveNext) 
	{ 
		haveNext = finder.FindNextFile(); 
		if(finder.IsDirectory()) 
			continue;

		strFileName = finder.GetFileName();

		//相关文件的路径
		CString temp1 =strImgDir + ("\\") + strFileName; 
		char* img_file_name = (char*)(temp1.GetBuffer(0));
		CString temp2 =strSiftDrawDir + ("\\") + strImgDir + ("_#_") + strFileName;
		char* out_img_name = (char*)(temp2.GetBuffer(0));
		CString temp3 =strFeatureDir + ("\\") + strImgDir + ("_#_") + strFileName + (".feature");
		char* out_file_name  = (char*)(temp3.GetBuffer(0));

		foutList << out_file_name <<endl;

		IplImage* img;
		struct feature* features;
		int n = 0;

		img = cvLoadImage(img_file_name, 1);
		if(!img)
		{
			return false;
		}
		n = _sift_features(img, &features, SIFT_INTVLS/*3*/, SIFT_SIGMA/*1.6*/, SIFT_CONTR_THR/*0.04*/,
			SIFT_CURV_THR/*10*/, SIFT_IMG_DBL/*1*/, SIFT_DESCR_WIDTH/*4*/, SIFT_DESCR_HIST_BINS/*8*/); //SIFTfeature提取
		featureNum += n;
		if( out_file_name != NULL )
			export_features(out_file_name, features, n); //将features导出为文件
		if(out_img_name != NULL)
		{
			draw_features(img, features, n); //在img图片上标记出features
			cvSaveImage(out_img_name, img, NULL); //将标记后的图片保存
		}
		printf("-----  %s: Sift特征提取完成!  -----\n", strFileName);
	}
	foutList.close();

	int featureSum = 0;
	if (access(strInfoFile.GetBuffer(0), 0) == 0)
	{
		featureSum = GetPrivateProfileInt(_T("FEATURE"), _T("FeatureSum"), NULL, strInfoFile);
	}
	else
	{
		file.Open(strInfoFile, CStdioFile::modeCreate | CStdioFile::modeWrite);
		file.Close();
	}

	CString temp;
	temp.Format(_T("%d"), featureSum + featureNum);
	WritePrivateProfileString(_T("FEATURE"), _T("FeatureSum"), temp, strInfoFile);

	CreateLabel(strResultDir);

	printf("-----------------%s文件夹 特征提取完成-------------\n\n", strImgDir);
	return true;
}

//KMeans聚类
bool KMeansCluster(int nClusterNum, CString strResultDir)
{
	CString strInfoFile = strResultDir + ("\\Info.ini");
	CString strListFile = strResultDir + ("\\ImgPath.list");
	CString strWordFile = strResultDir + ("\\WordList.xml");
	CString strFileName;
	CStdioFile file;
	
	if(!file.Open(strListFile, CStdioFile::modeRead))
	{
		printf("Error: 原始图片文件名路径列表文件打开失败\n");
		return false;
	}

	CString temp1;
	temp1.Format(_T("%d"), nClusterNum);
	WritePrivateProfileString(_T("KMeans"), _T("ClusterNum"), temp1, strInfoFile);

	int featureNum = GetPrivateProfileInt(_T("FEATURE"), _T("FeatureSum"), NULL, strInfoFile);

	printf("-----------------KMeans导入数据-------------\n");
	struct feature* features;
	int dims = 128;
	int n = 0;
	int k = nClusterNum;
	int temp = 0;
	CvMat *samples=cvCreateMat(featureNum, dims, CV_32FC1); //包含所有图片的所有feature信息的矩阵，featureNum个feature，每个feature为dims（128）维向量，每一维的元素类型为32位浮点数
	CvMat *clusters=cvCreateMat(featureNum, 1, CV_32SC1); //每个feature所在“质心”的指针（实际上本例程中没有用到该信息）
	CvMat *centers=cvCreateMat(k, dims, CV_32FC1); //“质心”信息的数组，k个“质心”每个质心都是dims（128）维向量，每一维的元素类型为32位浮点数
	cvSetZero(clusters); //将矩阵初始化为0
	cvSetZero(centers); //将矩阵初始化为0
	while(file.ReadString(strFileName))
	{
		n = import_features((char*)(strFileName.GetBuffer(0)), FEATURE_LOWE, &features); //导入feature文件，n为导入的feature个数
		if(-1 == n)
		{
			printf("Error: 导入特征文件失败!\n");
			return false;
		}
		else
		{
			printf("-----  %s: Sift特征导入完成!  -----\n", strFileName);
		}
		//将feature文件内所有feature信息存入samples矩阵结构内
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < dims; j++)
			{
				samples->data.fl[temp++] = features[i].descr[j];
			}
		}
	}
	file.Close();

	printf("-----------------KMeans开始计算-------------\n");
	cvKMeans2(samples, k, clusters,cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,100, 1.0), 3, (CvRNG *)0, 1, centers); //Kmeans聚类

	cvSave(strWordFile, centers);
	printf("-----------------KMeans计算结束-------------\n\n");
	
	return true;
}

void CreateLabel(CString strResultDir)
{
	CString strListFile = strResultDir + ("\\ImgPath.list");
	CString strImageLabel = strResultDir + ("\\ImageLabel.data");
	ifstream fin;
	fin.open(strListFile, ios::in);
	int nLabelNum = 0;
	Image_Label imgLabel[50];
	while (!fin.eof())
	{
		string strLine;
		fin>>strLine;
		if (strLine == "")
		{
			break;
		}
		string strCategory = GetCategory(strLine);
		bool bFlag = false;
		for (int i = 0; i < nLabelNum; i++)
		{
			if (imgLabel[i].strCategory == strCategory)
			{
				bFlag = true;
				break;
			}
		}
		if (!bFlag)
		{
			imgLabel[nLabelNum].nLabel = nLabelNum;
			imgLabel[nLabelNum].strCategory = strCategory.c_str();
			nLabelNum++;
		}
	}
	fin.close();
	ofstream fout;
	fout.open(strImageLabel, ios::out);
	fout<<nLabelNum<<endl;
	for (int i = 0; i < nLabelNum; i++)
	{
		fout<<imgLabel[i].strCategory<<" "<<imgLabel[i].nLabel<<endl;
	}
	fout.close();
}

//导入标签
Image_Label* LoadLabel(CString strResultDir, int& nLabelNum)
{
	CString strImageLabel = strResultDir + ("\\ImageLabel.data");
	ifstream fin;
	fin.open(strImageLabel, ios::in);
	fin>>nLabelNum;
	Image_Label* imgLabel = new Image_Label[nLabelNum];
	for (int i = 0; i < nLabelNum; i++)
	{
		fin>>imgLabel[i].strCategory>>imgLabel[i].nLabel;
	}
	return imgLabel;
}

//统计词频
bool CalcWordFreq(svm_problem& prob, int nClusterNum, CString strResultDir)
{
	CString strFreqFile = strResultDir + ("\\WordFreq.data");
	CString strListFile = strResultDir + ("\\ImgPath.list");
	CString strWordFile = strResultDir + ("\\WordList.xml");
	ofstream outFile;
	ifstream finList;
	outFile.open(strFreqFile, ios::out);
	finList.open(strListFile, ios::in);

	CvMat* wordList = (CvMat*)cvLoad(strWordFile);

	CString info;
	CString resultDir = strResultDir;

	int nLabelNum = 0;
	Image_Label* imgLabel = LoadLabel(strResultDir, nLabelNum);

	CString fileName;
	int wordNum = nClusterNum;
	int* wordTimes = new int[wordNum];
	struct feature* features;
	int dims = 128;
	float* pa = new float[dims];
	float* pb = NULL;
	printf("-----------------词频统计开始-------------\n");

	int nImgNum = 0;
	while(!finList.eof())
	{
		string strFeatureName;
		finList>>strFeatureName;
		if (strFeatureName == "")
		{
			continue;
		}
		else
		{
			nImgNum++;
		}
	}
	finList.close();
	finList.open(strListFile, ios::in);

	prob.l = nImgNum;
	prob.y = (double*)malloc(sizeof(double) * nImgNum);
	prob.x = (svm_node**)malloc(sizeof(svm_node*) * nImgNum);

	int nNum = 0;
	while(!finList.eof())
	{
		string strFeatureName;
		finList>>strFeatureName;
		if (strFeatureName == "")
		{
			continue;
		}
		string strCategory = GetCategory(strFeatureName);

		printf("-----  %s: 词频统计处理！  -----\n", strFeatureName.data());

		int n = import_features((char*)(strFeatureName.c_str()), FEATURE_LOWE, &features);

		//初始化词频数组
		for(int i = 0; i < wordNum; i++)
		{
			wordTimes[i] = 0;
		}

		for(int i = 0; i < n; i++)
		{
			float dist = 0, distMin = 0;
			int wordPos = 0;
			for(int j = 0; j < dims; j++) //把第i个feature的128个double浮点数复制到pa指向的128个float数
			{
				pa[j] = features[i].descr[j];
			}
			pb = (float*)(wordList->data.ptr);
			distMin = normL2Sqr_(pa, pb, dims); //计算欧式距离
			for(int j = 1; j < wordNum; j++)
			{
				pb += dims;
				dist = normL2Sqr_(pa, pb, dims); //计算欧式距离
				if(dist < distMin)
				{
					distMin = dist;
					wordPos = j;
				}
			}
			wordTimes[wordPos]++;
		}

		//写词频文件
		CString strLine("");
		int nSum = 0;
		for(int i = 0; i < wordNum; i++)
		{
			nSum += wordTimes[i];
		}
		prob.x[nNum] = (svm_node*)malloc(sizeof(svm_node) * (wordNum + 1));
		for(int i = 0; i < wordNum; i++)
		{
			strLine.AppendFormat(_T(" %d:%f"), i + 1, wordTimes[i] * 1.0 / nSum);
			prob.x[nNum][i].index = i + 1;
			prob.x[nNum][i].value = wordTimes[i] * 1.0 / nSum;
		}
		prob.x[nNum][wordNum].index = -1;
		CString preContent;
		for (int i = 0; i < nLabelNum; i++)
		{
			if (imgLabel[i].strCategory == strCategory)
			{
				preContent.Format(_T("%d"), i);
				prob.y[nNum] = i;
				break;
			}
		}
		strLine.Append(_T("\n"));
		outFile << preContent.GetBuffer(0) << strLine.GetBuffer(0);

		nNum++;
	}
	delete[] pa;
	delete[] wordTimes;
	finList.close();
	outFile.close();

	printf("-----  词频文件保存成功!  -----\n");
	printf("-----------------词频统计结束-------------\n\n");
	return true;
}


void SVMTrain(svm_problem prob, CString strResultDir)
{
	CString strModelFile = strResultDir + ("\\SvmTrain.model");

	svm_parameter param;
	svm_model* model;

	//SVM参数初始化,默认参数
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;


	param.C = 4;//需调整的参数
	param.gamma = 16.0;//需调整的参数

	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	svm_set_print_string_function(NULL);

	//SVM参数以及训练数据检查
	const char* error_msg = svm_check_parameter(&prob, &param);
	if (error_msg)
	{
		printf("SVM ERROR: %s\n",error_msg);
		return;
	}

	//SVM训练
	model = svm_train(&prob, &param);
	if (svm_save_model(strModelFile.GetBuffer(0), model))
	{
		printf("SVM ERROR: Cannot Save Model File!\n");
	}

	//数据内存释放
	svm_free_and_destroy_model(&model);
	svm_destroy_param(&param);
	free(prob.y);
	free(prob.x);
}



void CClassClientDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strResultDir = ("TrainResult");//结果目录
	CString strTestDir=  ("TestImage");//测试图片目录
	int nKmeanCulsterNum = 50;//K均值聚类类别数目 原设是330
	svm_problem prob;

	// 训练阶段
	// 使用前将结果目录里的文件删除
	ExtractSift("ai", strResultDir);
	ExtractSift("bang", strResultDir);
	ExtractSift("bei", strResultDir);
	//ExtractSift("beii", strResultDir);
	ExtractSift("bo", strResultDir);

	ExtractSift("bu", strResultDir);
	ExtractSift("cheng", strResultDir);
	ExtractSift("chi", strResultDir);
	//ExtractSift("da", strResultDir);
	ExtractSift("de", strResultDir);

	KMeansCluster(nKmeanCulsterNum, strResultDir);
	
	CalcWordFreq(prob, nKmeanCulsterNum, strResultDir);
	
	SVMTrain(prob, strResultDir);


}


void CClassClientDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码


}
