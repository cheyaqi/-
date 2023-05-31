
// DispenserDlg.h: 头文件
//

#pragma once
#include <MotionControl.h>
#include "HalconWin.h"
#include "HalconArithmetic.h"
#include <CameraDS.h>


// CDispenserDlg 对话框
class CDispenserDlg : public CDialogEx
{
// 构造
public:
	CDispenserDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPENSER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:

	//点胶机对象
	CMotionControl mc;
	bool mcOpen=false;//点胶机状态
	bool Start=false;//开始检测

	//halcon对象
	HalconWin m_halconWin;
	HalconArithmetic m_halconArithmetic;
	HObject img;
	CheckResult re;

	//相机对象
	CCameraDS m_camera;
	int CameraID;//相机ID
	bool OpenCame= false;//相机状态判断

	/**************************************************
	 *函数名称:InitCame
	 *说明:相机初始化化函数
	 *参数:
	 *返回值:
	 *功能:
	**************************************************/
	void InitCame();


	

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CameraList;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	CStatic m_SText;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton6();
};
