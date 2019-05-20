
// ClassClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClassClient.h"
#include "ClassClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CClassClientDlg �Ի���




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


// CClassClientDlg ��Ϣ�������

BOOL CClassClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClassClientDlg::OnPaint()
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
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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


//��ȡSIFT����
bool ExtractSift(CString strImgDir, CString strResultDir)
{
	CFileFind finder;
	
	CString strSiftDrawDir = strResultDir + ("\\SiftDraw");
	CString strFeatureDir = strResultDir + ("\\Feature");
	CString strInfoFile = strResultDir + ("\\Info.ini");
	CString strListFile = strResultDir + ("\\ImgPath.list");
	CString strFileName;

	BOOL haveNext = finder.FindFile(strImgDir + "\\*.*");

	//����ļ���·��
	if(haveNext)
	{
		if(!PathIsDirectory(strImgDir))
		{
			printf("Error: ͼƬԴĿ¼·������ȷ��");
			return false;
		}
		if(!PathIsDirectory(strResultDir) && !CreateDirectory(strResultDir, NULL))
		{
			printf("Error: ��������ļ��д���ʧ�ܣ�");
			return false;
		}
		if(!PathIsDirectory(strSiftDrawDir) && !CreateDirectory(strSiftDrawDir, NULL))
		{
			printf("Error: ͼƬ���������ļ��д���ʧ�ܣ�");
			return false;
		}
		if(!PathIsDirectory(strFeatureDir) && !CreateDirectory(strFeatureDir, NULL))
		{
			printf("Error: �����ļ��д���ʧ�ܣ�");
			return false;
		}
	}
	else
	{
		printf("Error: û�з���ͼƬ�ļ���");
		return false;
	}
	
	//Do sift
	printf("-----------------%s�ļ��� ������ȡ��ʼ-------------\n", strImgDir);
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

		//����ļ���·��
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
			SIFT_CURV_THR/*10*/, SIFT_IMG_DBL/*1*/, SIFT_DESCR_WIDTH/*4*/, SIFT_DESCR_HIST_BINS/*8*/); //SIFTfeature��ȡ
		featureNum += n;
		if( out_file_name != NULL )
			export_features(out_file_name, features, n); //��features����Ϊ�ļ�
		if(out_img_name != NULL)
		{
			draw_features(img, features, n); //��imgͼƬ�ϱ�ǳ�features
			cvSaveImage(out_img_name, img, NULL); //����Ǻ��ͼƬ����
		}
		printf("-----  %s: Sift������ȡ���!  -----\n", strFileName);
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

	printf("-----------------%s�ļ��� ������ȡ���-------------\n\n", strImgDir);
	return true;
}

//KMeans����
bool KMeansCluster(int nClusterNum, CString strResultDir)
{
	CString strInfoFile = strResultDir + ("\\Info.ini");
	CString strListFile = strResultDir + ("\\ImgPath.list");
	CString strWordFile = strResultDir + ("\\WordList.xml");
	CString strFileName;
	CStdioFile file;
	
	if(!file.Open(strListFile, CStdioFile::modeRead))
	{
		printf("Error: ԭʼͼƬ�ļ���·���б��ļ���ʧ��\n");
		return false;
	}

	CString temp1;
	temp1.Format(_T("%d"), nClusterNum);
	WritePrivateProfileString(_T("KMeans"), _T("ClusterNum"), temp1, strInfoFile);

	int featureNum = GetPrivateProfileInt(_T("FEATURE"), _T("FeatureSum"), NULL, strInfoFile);

	printf("-----------------KMeans��������-------------\n");
	struct feature* features;
	int dims = 128;
	int n = 0;
	int k = nClusterNum;
	int temp = 0;
	CvMat *samples=cvCreateMat(featureNum, dims, CV_32FC1); //��������ͼƬ������feature��Ϣ�ľ���featureNum��feature��ÿ��featureΪdims��128��ά������ÿһά��Ԫ������Ϊ32λ������
	CvMat *clusters=cvCreateMat(featureNum, 1, CV_32SC1); //ÿ��feature���ڡ����ġ���ָ�루ʵ���ϱ�������û���õ�����Ϣ��
	CvMat *centers=cvCreateMat(k, dims, CV_32FC1); //�����ġ���Ϣ�����飬k�������ġ�ÿ�����Ķ���dims��128��ά������ÿһά��Ԫ������Ϊ32λ������
	cvSetZero(clusters); //�������ʼ��Ϊ0
	cvSetZero(centers); //�������ʼ��Ϊ0
	while(file.ReadString(strFileName))
	{
		n = import_features((char*)(strFileName.GetBuffer(0)), FEATURE_LOWE, &features); //����feature�ļ���nΪ�����feature����
		if(-1 == n)
		{
			printf("Error: ���������ļ�ʧ��!\n");
			return false;
		}
		else
		{
			printf("-----  %s: Sift�����������!  -----\n", strFileName);
		}
		//��feature�ļ�������feature��Ϣ����samples����ṹ��
		for(int i = 0; i < n; i++)
		{
			for(int j = 0; j < dims; j++)
			{
				samples->data.fl[temp++] = features[i].descr[j];
			}
		}
	}
	file.Close();

	printf("-----------------KMeans��ʼ����-------------\n");
	cvKMeans2(samples, k, clusters,cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,100, 1.0), 3, (CvRNG *)0, 1, centers); //Kmeans����

	cvSave(strWordFile, centers);
	printf("-----------------KMeans�������-------------\n\n");
	
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

//�����ǩ
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

//ͳ�ƴ�Ƶ
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
	printf("-----------------��Ƶͳ�ƿ�ʼ-------------\n");

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

		printf("-----  %s: ��Ƶͳ�ƴ���  -----\n", strFeatureName.data());

		int n = import_features((char*)(strFeatureName.c_str()), FEATURE_LOWE, &features);

		//��ʼ����Ƶ����
		for(int i = 0; i < wordNum; i++)
		{
			wordTimes[i] = 0;
		}

		for(int i = 0; i < n; i++)
		{
			float dist = 0, distMin = 0;
			int wordPos = 0;
			for(int j = 0; j < dims; j++) //�ѵ�i��feature��128��double���������Ƶ�paָ���128��float��
			{
				pa[j] = features[i].descr[j];
			}
			pb = (float*)(wordList->data.ptr);
			distMin = normL2Sqr_(pa, pb, dims); //����ŷʽ����
			for(int j = 1; j < wordNum; j++)
			{
				pb += dims;
				dist = normL2Sqr_(pa, pb, dims); //����ŷʽ����
				if(dist < distMin)
				{
					distMin = dist;
					wordPos = j;
				}
			}
			wordTimes[wordPos]++;
		}

		//д��Ƶ�ļ�
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

	printf("-----  ��Ƶ�ļ�����ɹ�!  -----\n");
	printf("-----------------��Ƶͳ�ƽ���-------------\n\n");
	return true;
}


void SVMTrain(svm_problem prob, CString strResultDir)
{
	CString strModelFile = strResultDir + ("\\SvmTrain.model");

	svm_parameter param;
	svm_model* model;

	//SVM������ʼ��,Ĭ�ϲ���
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;


	param.C = 4;//������Ĳ���
	param.gamma = 16.0;//������Ĳ���

	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;
	svm_set_print_string_function(NULL);

	//SVM�����Լ�ѵ�����ݼ��
	const char* error_msg = svm_check_parameter(&prob, &param);
	if (error_msg)
	{
		printf("SVM ERROR: %s\n",error_msg);
		return;
	}

	//SVMѵ��
	model = svm_train(&prob, &param);
	if (svm_save_model(strModelFile.GetBuffer(0), model))
	{
		printf("SVM ERROR: Cannot Save Model File!\n");
	}

	//�����ڴ��ͷ�
	svm_free_and_destroy_model(&model);
	svm_destroy_param(&param);
	free(prob.y);
	free(prob.x);
}



void CClassClientDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strResultDir = ("TrainResult");//���Ŀ¼
	CString strTestDir=  ("TestImage");//����ͼƬĿ¼
	int nKmeanCulsterNum = 50;//K��ֵ���������Ŀ ԭ����330
	svm_problem prob;

	// ѵ���׶�
	// ʹ��ǰ�����Ŀ¼����ļ�ɾ��
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������


}
