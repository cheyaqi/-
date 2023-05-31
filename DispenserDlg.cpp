
// DispenserDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Dispenser.h"
#include "DispenserDlg.h"
#include "afxdialogex.h"

#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDispenserDlg 对话框



CDispenserDlg::CDispenserDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DISPENSER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDispenserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_CameraList);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_SText);
}

BEGIN_MESSAGE_MAP(CDispenserDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON7, &CDispenserDlg::OnBnClickedButton7)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDispenserDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDispenserDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &CDispenserDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CDispenserDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON2, &CDispenserDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CDispenserDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDispenserDlg 消息处理程序

void CDispenserDlg::InitCame()
{
	int num=CCameraDS::CameraCount();
	if (num == 0)
	{
		return;
		MessageBox(L"没有检测到相机");
	}

	USES_CONVERSION;          //字符串转换宏(用于CString<--->char)
	char Name[32] = { 0 };    //用于保存相机名称 
	for (int i = 0; i < num; i++)
	{
		m_camera.CameraName(i, Name, sizeof(Name));
		m_CameraList.AddString(A2T(Name));
		m_SText.SetWindowTextW(L"相机初始化成功");
	}

}

BOOL CDispenserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
	
	GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON5)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON6)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON7)->EnableWindow(true);


	InitCame();

	CWnd* pWnd = GetDlgItem(IDC_STATIC_SHOW);
	m_halconWin.openWindow(pWnd);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDispenserDlg::OnPaint()
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
HCURSOR CDispenserDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//相机串口选择
void CDispenserDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	CameraID=m_CameraList.GetCurSel();
	GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
}
void CDispenserDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!OpenCame)
	{
			if (!m_camera.OpenCamera(CameraID))
			{
				m_SText.SetWindowTextW(L"相机连接失败");
				return;
			}
			m_SText.SetWindowTextW(L"相机连接成功");
			GetDlgItem(IDC_BUTTON1)->SetWindowTextW(L"断开相机");
			GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON5)->EnableWindow(true);
			OpenCame = true;
	}
	else
	{
		OpenCame = false;
		//m_camera.CloseCamera();
		m_SText.SetWindowTextW(L"相机断开");
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(L"连接相机");
		GetDlgItem(IDC_BUTTON2)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(false);
		
	}
}

//点胶机
void CDispenserDlg::OnBnClickedButton6()
{
	if (!mcOpen)
	{
		if (!mc.ConnectDispenser())
		{
			m_SText.SetWindowTextW(L"点胶机连接失败");
			return;
		}
		m_SText.SetWindowTextW(L"点胶机连接成功");
		GetDlgItem(IDC_BUTTON6)->SetWindowTextW(L"断开点胶机");
		mcOpen = true;
		mc.ResetDispenser();
	}
	else
	{
		mc.CloseDispenser();
		mcOpen = false;
		m_SText.SetWindowTextW(L"点胶机断开成功");
		GetDlgItem(IDC_BUTTON6)->SetWindowTextW(L"连接点胶机");
	}
}

void ShowImg(void* lpDlg)
{
	CDispenserDlg* dlg = (CDispenserDlg*)lpDlg;
	
	while (dlg->OpenCame)
	{
		if (dlg->Start == false)
		{
			BYTE* imgData = dlg->m_camera.GetFramePtr();
			Sleep(10);
			dlg->img=dlg->m_halconArithmetic.SdKImage2Ha(imgData,dlg->m_camera.GetWidth(), dlg->m_camera.GetHeight(), dlg->m_camera.GetBitCount());
			try
			{
				dlg->m_halconWin.dispObj(dlg->img);
				dlg->re = dlg->m_halconArithmetic.Action(dlg->img);
				dlg->m_halconWin.dispObj(dlg->re.Point);
			}
			catch(...)
			{
				continue;
			}
		}
		Sleep(1);
	}
	Sleep(10);
}
void CDispenserDlg::OnBnClickedButton4()
{
	thread tA(ShowImg, this);
	tA.detach();

}

void Work(CDispenserDlg* dlg)
{
		dlg->Start = true;
		dlg->re.X.TupleLength() - 1;
		HTuple end_val18 = (dlg->re.X.TupleLength())- 1;
		HTuple step_val18 = 1;
		for (HTuple(hv_Index) = 0; hv_Index.Continue(end_val18, step_val18); hv_Index += step_val18)
		{
			float x = dlg->re.X[hv_Index].D();
			float y = dlg->re.Y[hv_Index].D();
			dlg->mc.MoveXYZ(x, y, -70);
			Sleep(1 * 50);
			dlg->mc.MoveXYZ(x, y, -80);
			Sleep(1 * 50);
			dlg->mc.MoveXYZ(x, y, -70);
			Sleep(1 * 100);
		}
		dlg->Start = false;
}
void CDispenserDlg::OnBnClickedButton7()
{
	thread tB(Work, this);
	tB.detach();
}


//相机参数
void CDispenserDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		m_camera.SetCameraAttr();//设置相机属性
	}
	catch (...)//异常处理代码，如果发生异常则跳转到此句
	{
		AfxMessageBox(L"Error", MB_ICONERROR);
	}
}

////保存图像
void CDispenserDlg::OnBnClickedButton5()
{
	time_t t = time(&t);
	CString name;
	name.Format(L"%d", t);
	CFileDialog fileDlg(FALSE, L"", name, OFN_OVERWRITEPROMPT, L"PNG 文件|*.png|BMP 文件|*.bmp|JPG 文件|*.jpg||");
	if (IDOK == fileDlg.DoModal())//点击保存图像按钮返回IDOK
	{
		//获取保存图像的文件名称和绝对路径
		CString strFileName = fileDlg.GetPathName();
		//宽字节字符串转为多字节字符串
		USES_CONVERSION;
		WriteImage(img, "png", (Hlong)0, T2A(strFileName));
	}
}
