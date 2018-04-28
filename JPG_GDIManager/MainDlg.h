/******************************************************************************
*  版权所有（C）2008-2015，上海二三四五网络科有限公司                         *
*  保留所有权利。                                                             *
******************************************************************************
*  作者 : <gaolongcc>
*  版本 : <v1.0>
*****************************************************************************/
#ifndef __MAINDLG_h_
#define __MAINDLG_h_ 

#include "resource.h"
#include <string>
#include <gdiplus.h>//gdi+头文件

class CMainDlg : public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_HANDLER(IDC_BTNINSERT, BN_CLICKED, OnBnClickedBtninsert)
        COMMAND_HANDLER(IDC_BTNDELETE, BN_CLICKED, OnBnClickedBtndelete)
        COMMAND_HANDLER(IDC_BTNPUS, BN_CLICKED, OnBnClickedBtnpus)
        COMMAND_HANDLER(IDC_BTNDEC, BN_CLICKED, OnBnClickedBtndec)
        COMMAND_HANDLER(IDC_BTNNAGTIVE, BN_CLICKED, OnBnClickedBtnnagtive)
        COMMAND_HANDLER(IDC_BTNPOSTIVE, BN_CLICKED, OnBnClickedBtnpostive)
        COMMAND_HANDLER(IDC_BtnHoriz, BN_CLICKED, OnBnClickedBtnhoriz)
        COMMAND_HANDLER(IDC_BtnVert, BN_CLICKED, OnBnClickedBtnvert)
        COMMAND_HANDLER(IDC_BtnClone, BN_CLICKED, OnBnClickedBtnclone)
        COMMAND_HANDLER(IDC_BtnCast, BN_CLICKED, OnBnClickedBtncast)
    END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// center the dialog on the screen
		CenterWindow();

		// set icons
		HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
		SetIcon(hIcon, TRUE);
		HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
		SetIcon(hIconSmall, FALSE);

        //GDIplus init
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

        //初始化默认比例及角度
        GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T("0.1"));
        GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T("30.0"));
        GetDlgItem(IDC_STA_F).SetWindowTextW(_T(" scaling:"));
        GetDlgItem(IDC_STA_R).SetWindowTextW(_T(" angle rotation:"));

        //init numbers
        m_bFlagbyZoom = true;

		return TRUE;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// TODO: Add validation code 
		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

private:
    ULONG_PTR m_gdiplusToken;
    static Gdiplus::Graphics m_graphics; 

    std::wstring m_wstrImagePath;  //图片文件路径
    float m_fFactor;             //缩小放大因子
    float m_fRoate;              //旋转因子

    bool m_bFlagbyZoom;   //缩放标志位

    void CMainDlg::ShowPicture(const std::wstring& pFilepath);
    void CMainDlg::DestoryPicture();
    void CMainDlg::ZoomPicture(const std::wstring& pFilepath, const float fFator);
    Gdiplus::Image* CMainDlg::ScaleImage(Gdiplus::Image* source, int width, int height);
    Gdiplus::Image* CMainDlg::RotateImage(Gdiplus::Image* source,   float fRoate);
    Gdiplus::Image* CMainDlg::FilpImageX(Gdiplus::Image* source);
    Gdiplus::Image* CMainDlg::FilpImageY(Gdiplus::Image* source);
    Gdiplus::Image* CMainDlg::CloneImage(Gdiplus::Image* source);
    Gdiplus::Image* CMainDlg::CastImage(Gdiplus::Image* source);
    Gdiplus::Rect* CMainDlg::GetRotateRectangle(int width, int height, float angle);

    LRESULT OnBnClickedBtninsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedBtndelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedBtnpus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedBtndec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedBtnnagtive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedBtnpostive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
public:
    LRESULT OnBnClickedBtnhoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedBtnvert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedBtnclone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnBnClickedBtncast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


#endif