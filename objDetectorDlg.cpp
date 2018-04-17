
// objDetectorDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "objDetector.h"
#include "objDetectorDlg.h"
#include "afxdialogex.h"
#include "IPCConfigDlg.h"
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EDGE 0

labelMap g_map;

// ��ͼ����Ӿ��β���ע����
void AddRectanges(cv::Mat &m, const std::vector<Tips> &tips)
{
	for (std::vector<Tips>::const_iterator p = tips.begin(); 
		p != tips.end(); ++p)
		p->AddTips(m);
}


// ����ͼ��
void CobjDetectorDlg::Save(const cv::Mat &m, int class_id)
{
	time_t timep = time(NULL);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H%M%S.jpg",localtime(&timep));
	char path[_MAX_PATH];
	sprintf_s(path, "%s\\object detection\\%s\\%s", m_strPath, g_map.getItemName(class_id), tmp);
	cv::imwrite(path, m);
}


// ��ʼ��python���û���
void CobjDetectorDlg::InitPyCaller(LPVOID param)
{
	CobjDetectorDlg *pThis = (CobjDetectorDlg*)param;
	pThis->m_nThreadState[_InitPyCaller] = Thread_Start;
	if (NULL == pThis->m_py)
		pThis->m_py = new pyCaller("detect");
	pThis->m_bOK = true;
	pThis->m_nThreadState[_InitPyCaller] = Thread_Stop;
}


// �����Ƶ
void CobjDetectorDlg::DetectVideo(LPVOID param)
{
	OutputDebugStringA("======> DetectVideo Start.\n");
	CobjDetectorDlg *pThis = (CobjDetectorDlg*)param;
	pThis->m_nThreadState[_DetectVideo] = Thread_Start;
	timeBeginPeriod(1);
	// �ӳٲ���
	_beginthread(&PlayVideo, 0, param);
	std::vector<Tips> tips;
	int count = 0, flag = 0;
	const int K = min(DETECT_RATE, 4);
	do 
	{
		cv::Mat m = pThis->m_reader.PlayVideo();
		if (m.empty()) // ��1֡δ���
			break;
		++count;
		switch (pThis->m_nMediaState)
		{
		case STATE_DETECTING:
			if ( 0 == (count % DETECT_RATE) )
			{
				flag = 0;
				tips = pThis->DoDetect(m);
			}
			else if (++flag < K)// ʹ���ο�ͣ�� K * 40 ms
				AddRectanges(m, tips);
			break;
		case STATE_PAUSE:
			while(STATE_PAUSE == pThis->m_nMediaState)
				Sleep(10);
			break;
		case STATE_DONOTHING:
			break;
		default:
			break;
		}
		if (STATE_DONOTHING == pThis->m_nMediaState)
			break;
		while (!pThis->m_reader.PushImage(m))
			Sleep(10);
	} while (!pThis->m_bExit);
	timeEndPeriod(1);
	pThis->m_nThreadState[_DetectVideo] = Thread_Stop;
	OutputDebugStringA("======> DetectVideo Stop.\n");
}


void CobjDetectorDlg::PlayVideo(LPVOID param)
{
	OutputDebugStringA("======> PlayVideo Start.\n");
	CobjDetectorDlg *pThis = (CobjDetectorDlg*)param;
	pThis->m_nThreadState[_PlayVideo] = Thread_Start;
	timeBeginPeriod(1);
	while(pThis->m_nThreadState[_DetectVideo] && pThis->m_reader.IsBuffering())
		Sleep(10);
	do
	{
		clock_t t = clock();
		switch (pThis->m_nMediaState)
		{
		case STATE_DETECTING:
			break;
		case STATE_PAUSE:
			while(STATE_PAUSE == pThis->m_nMediaState)
				Sleep(10);
			break;
		case STATE_DONOTHING:
			break;
		default:
			break;
		}
		if (STATE_DONOTHING == pThis->m_nMediaState)
			break;
		while (pThis->m_reader.PopImage() == 0)
		{
			if (pThis->m_nThreadState[_DetectVideo] == Thread_Stop)
				break;
			Sleep(10);
		}
		if (pThis->m_nThreadState[_DetectVideo] == Thread_Stop)
			break;
		pThis->Paint();
		int nTime = 40 - (clock() - t);
		Sleep(nTime > 0 ? nTime : 40);
	} while (!pThis->m_bExit);
	timeEndPeriod(1);
	pThis->m_reader.Open(pThis->m_strFile);
	pThis->Paint();
	pThis->m_nMediaState = STATE_DONOTHING;
	pThis->m_nThreadState[_PlayVideo] = Thread_Stop;
	OutputDebugStringA("======> PlayVideo Stop.\n");
}


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


// CobjDetectorDlg �Ի���



CobjDetectorDlg::CobjDetectorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CobjDetectorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_strFile, 0, _MAX_PATH);
	memset(m_strPath, 0, _MAX_PATH);
	m_fThresh = 0.8;
	m_bOK = false;
	m_nMediaState = STATE_DONOTHING;
	m_bExit = false;
	m_nThreadState[_InitPyCaller] = m_nThreadState[_DetectVideo] = 
		m_nThreadState[_PlayVideo] = Thread_Stop;
#if USING_TENSORFLOW
	_beginthread(&InitPyCaller, 0, this);
#endif
}

void CobjDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picCtrl);
}

BEGIN_MESSAGE_MAP(CobjDetectorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &CobjDetectorDlg::OnFileOpen)
	ON_WM_DESTROY()
	ON_COMMAND(ID_OBJ_DETECT, &CobjDetectorDlg::OnObjDetect)
	ON_COMMAND(ID_FILE_CLOSE, &CobjDetectorDlg::OnFileClose)
	ON_WM_INITMENUPOPUP()
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, &CobjDetectorDlg::OnUpdateFileOpen)
	ON_UPDATE_COMMAND_UI(ID_OBJ_DETECT, &CobjDetectorDlg::OnUpdateObjDetect)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, &CobjDetectorDlg::OnUpdateFileClose)
	ON_COMMAND(ID_FILE_QUIT, &CobjDetectorDlg::OnFileQuit)
	ON_COMMAND(ID_EDIT_PLAY, &CobjDetectorDlg::OnEditPlay)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PLAY, &CobjDetectorDlg::OnUpdateEditPlay)
	ON_COMMAND(ID_EDIT_PAUSE, &CobjDetectorDlg::OnEditPause)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PAUSE, &CobjDetectorDlg::OnUpdateEditPause)
	ON_COMMAND(ID_EDIT_STOP, &CobjDetectorDlg::OnEditStop)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STOP, &CobjDetectorDlg::OnUpdateEditStop)
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_IPC, &CobjDetectorDlg::OnFileIpc)
	ON_UPDATE_COMMAND_UI(ID_FILE_IPC, &CobjDetectorDlg::OnUpdateFileIpc)
END_MESSAGE_MAP()


// CobjDetectorDlg ��Ϣ�������

BOOL CobjDetectorDlg::OnInitDialog()
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

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CDC* pDC = m_picCtrl.GetDC();
	m_picCtrl.GetClientRect(&m_rtPaint);
	m_hPaintDC = pDC->GetSafeHdc();

	// ��ȡ�����Ϣ
	char path[_MAX_PATH], *p = path, obj[_MAX_PATH];;
	GetModuleFileNameA(NULL, path, _MAX_PATH);
	while('\0' != *p) ++p;
	while('\\' != *p) --p;
	*p = '\0';
	strcpy_s(m_strPath, path);
	sprintf_s(obj, "%s\\object detection", path);
	strcpy(p, "\\label_map.ini");
	int n = GetPrivateProfileIntA("item", "class_num", 1, path);
	g_map.Create(n);
	if (-1 == _access(obj, 0))
		_mkdir(obj);
	for (int i = 0; i < n; ++i)
	{
		char temp[_MAX_PATH], name[_MAX_PATH];
		sprintf_s(temp, "class%d", i+1);
		GetPrivateProfileStringA("item", temp, "unknown", name, _MAX_PATH, path);
		sprintf_s(temp, "class%d_id", i+1);
		int id = GetPrivateProfileIntA("item", temp, 1, path);
		g_map.InsertItem(Item(name, id));
		sprintf_s(temp, "%s\\%s", obj, name);
		if (-1 == _access(temp, 0))
			_mkdir(temp);
	}
	// ��ȡsettings
	sprintf_s(path, "%s\\settings.ini", m_strPath);
	GetPrivateProfileStringA("settings", "threshold", "1.0", obj, _MAX_PATH, path);
	m_fThresh = atof(obj);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CobjDetectorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CobjDetectorDlg::OnPaint()
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
		// �˺�������Ƶ��ʹ�ó������ʱCPUҲ�ܸߣ���˼���Sleep
		Sleep(40);
		Paint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CobjDetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CobjDetectorDlg::OnFileOpen()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(TRUE);
	if (IDOK == dlg.DoModal())
	{
		m_nMediaState =STATE_DONOTHING;
		while(m_nThreadState[_PlayVideo] || m_nThreadState[_DetectVideo])
			Sleep(10);
		CString strFile = dlg.GetPathName();
		USES_CONVERSION;
		strcpy_s(m_strFile, W2A(strFile));
		if (!m_reader.Open(m_strFile))
		{
			TRACE("======> ���ļ�ʧ��.\n");
		}
		Invalidate(TRUE);
	}
}


void CobjDetectorDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_bExit = true;
	m_nMediaState = STATE_DONOTHING;
	while (m_nThreadState[_InitPyCaller] || m_nThreadState[_DetectVideo] || 
		m_nThreadState[_PlayVideo] || IsDetecting())
		Sleep(100);

	if (m_py)
		delete m_py;

	m_reader.Destroy();
}


void CobjDetectorDlg::OnObjDetect()
{
	if (m_reader.IsEmpty())
		return;
	
	if(m_py && !m_bOK)
		return;

	if (m_py)
		m_py->ActivateFunc("test_src");

	if (m_reader.IsImage())
	{
		cv::Mat m = m_reader.Front();
		DoDetect(m);
		Paint();
	}else if (m_reader.IsVideo())
	{
		if (STATE_DONOTHING == m_nMediaState)
		{
			m_nMediaState = STATE_DETECTING;
			_beginthread(&DetectVideo, 0, this);
		}
		else if(STATE_PLAYING == m_nMediaState)
			m_nMediaState = STATE_DETECTING;
		else if(STATE_PAUSE == m_nMediaState)
			m_nMediaState = STATE_DETECTING;
	}
}


void CobjDetectorDlg::OnFileClose()
{
	memset(m_strFile, 0, _MAX_PATH);
	m_reader.Clear();
	Invalidate(TRUE);
}


// ʹ�˵����״̬���Ը���
void CobjDetectorDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogEx::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

	ASSERT(pPopupMenu != NULL);
	// Check the enabled state of various menu items.

	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	ASSERT(state.m_pOther == NULL);
	ASSERT(state.m_pParentMenu == NULL);

	// Determine if menu is popup in top-level menu and set m_pOther to
	// it if so (m_pParentMenu == NULL indicates that it is secondary popup).
	HMENU hParentMenu;
	if (AfxGetThreadState()->m_hTrackingMenu == pPopupMenu->m_hMenu)
		state.m_pParentMenu = pPopupMenu;    // Parent == child for tracking popup.
	else if ((hParentMenu = ::GetMenu(m_hWnd)) != NULL)
	{
		CWnd* pParent = this;
		// Child windows don't have menus--need to go to the top!
		if (pParent != NULL &&
			(hParentMenu = ::GetMenu(pParent->m_hWnd)) != NULL)
		{
			int nIndexMax = ::GetMenuItemCount(hParentMenu);
			for (int nIndex = 0; nIndex < nIndexMax; nIndex++)
			{
				if (::GetSubMenu(hParentMenu, nIndex) == pPopupMenu->m_hMenu)
				{
					// When popup is found, m_pParentMenu is containing menu.
					state.m_pParentMenu = CMenu::FromHandle(hParentMenu);
					break;
				}
			}
		}
	}

	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
		state.m_nIndex++)
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
		if (state.m_nID == 0)
			continue; // Menu separator or invalid cmd - ignore it.

		ASSERT(state.m_pOther == NULL);
		ASSERT(state.m_pMenu != NULL);
		if (state.m_nID == (UINT)-1)
		{
			// Possibly a popup menu, route to first item of that popup.
			state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
			if (state.m_pSubMenu == NULL ||
				(state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				state.m_nID == (UINT)-1)
			{
				continue;       // First item of popup can't be routed to.
			}
			state.DoUpdate(this, TRUE);   // Popups are never auto disabled.
		}
		else
		{
			// Normal menu item.
			// Auto enable/disable if frame window has m_bAutoMenuEnable
			// set and command is _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(this, FALSE);
		}

		// Adjust for menu deletions and additions.
		UINT nCount = pPopupMenu->GetMenuItemCount();
		if (nCount < state.m_nIndexMax)
		{
			state.m_nIndex -= (state.m_nIndexMax - nCount);
			while (state.m_nIndex < nCount &&
				pPopupMenu->GetMenuItemID(state.m_nIndex) == state.m_nID)
			{
				state.m_nIndex++;
			}
		}
		state.m_nIndexMax = nCount;
	}
}


void CobjDetectorDlg::OnUpdateFileOpen(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!IsDetecting());
}


void CobjDetectorDlg::OnUpdateObjDetect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bOK && !m_reader.IsEmpty() && !IsDetecting());
}


void CobjDetectorDlg::OnUpdateFileClose(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!IsDetecting() && !m_reader.IsEmpty());
}


void CobjDetectorDlg::OnFileQuit()
{
	SendMessage(WM_CLOSE, 0, 0);
}


void CobjDetectorDlg::OnEditPlay()
{
	if (m_reader.IsVideo())
	{
		if (STATE_DONOTHING == m_nMediaState)
		{
			m_nMediaState = STATE_PLAYING;
			_beginthread(&DetectVideo, 0, this);
		}
		else if (STATE_PAUSE == m_nMediaState)
			m_nMediaState = STATE_PLAYING;
		else if (STATE_DETECTING == m_nMediaState)
			m_nMediaState = STATE_PLAYING;
	}
}


void CobjDetectorDlg::OnUpdateEditPlay(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_reader.IsVideo() && !IsBusy());
}


void CobjDetectorDlg::OnEditPause()
{
	m_nMediaState = STATE_PAUSE;
}


void CobjDetectorDlg::OnUpdateEditPause(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_reader.IsVideo() && IsBusy());
}


void CobjDetectorDlg::OnEditStop()
{
	m_nMediaState = STATE_DONOTHING;
}


void CobjDetectorDlg::OnUpdateEditStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_reader.IsFile() && STATE_DONOTHING != m_nMediaState);
}


// ����Ŀ����
std::vector<Tips> CobjDetectorDlg::DoDetect(cv::Mat &m)
{
	std::vector<Tips> tips;
	if (m_py)
	{ 
		npy_intp dims[] = {m_reader.dims(0), m_reader.dims(1), m_reader.dims(2)}; // ����ά����Ϣ
		// ���ɰ��������ά�����PyObject����ʹ��PyArray_SimpleNewFromData����
		// ��һ������2��ʾά�ȣ��ڶ���Ϊά������Dims,����������ָ����������ͣ����ĸ�����Ϊ����
		PyObject *PyArray  = PyArray_SimpleNewFromData(3, dims, NPY_UBYTE, m.data);
		// ͬ�������С��Python������������һ�µ�PyTuple����
		PyObject *ArgArray = PyTuple_New(1);
		PyTuple_SetItem(ArgArray, 0, PyArray); 
		tfOutput output = m_py->CallFunction("test_src", ArgArray);

		//output.PrintBoxes();
		if (0 == output.n)
			return tips;
		const float c = m.cols; // ����
		const float r = m.rows; // ����
		for (int i = 0; i < output.counts[0]; ++i)
		{
			float x1 = output.p(i, 1), x2 = output.p(i, 0), 
				x3 = output.p(i, 3), x4 = output.p(i, 2);

			cv::Rect rect(CvPoint(c * x1, r * x2), CvPoint(c * x3, r * x4));
			Tips tip(rect, output.scores[i], output.classes[i]);
			tip.AddTips(m);
			if (output.scores[i] > m_fThresh)
				Save(m, output.classes[i]);// ����
			tips.push_back(tip);
		}
	}
	return tips;
}


BOOL CobjDetectorDlg::PreTranslateMessage(MSG* pMsg)
{
	// ���� ESC/Enter �رմ���
	if(pMsg->message == WM_KEYDOWN && 
		(pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) )
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CobjDetectorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_picCtrl.GetSafeHwnd())
	{
		CRect rt;
		GetClientRect(&rt);
		m_picCtrl.MoveWindow(CRect(rt.left + EDGE, rt.top + EDGE, 
			rt.right - EDGE, rt.bottom - EDGE), TRUE);
		m_picCtrl.GetClientRect(&m_rtPaint);
	}
}


void CobjDetectorDlg::OnFileIpc()
{
	CIPCConfigDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		IPCamInfo info;
		USES_CONVERSION;
		strcpy_s(info.ip, W2A(dlg.m_strAddress));
		info.port = dlg.m_nPort;
		strcpy_s(info.user, W2A(dlg.m_strUser));
		strcpy_s(info.pwd, W2A(dlg.m_strPassword));
		HWND hWnd = GetDlgItem(IDC_IPC_CAPTURE)->GetSafeHwnd();
		if(!m_reader.OpenIPCamera(info, hWnd))
		{
			TRACE("======> ��IPCʧ��.\n");
		}
		else{
			m_strFile[0] = '\0';
			OnEditPlay();
		}
		Invalidate(TRUE);
	}
}


void CobjDetectorDlg::OnUpdateFileIpc(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!IsDetecting());
}
