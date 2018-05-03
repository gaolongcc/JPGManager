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
#include <vector>
#include <gdiplus.h>


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
        COMMAND_HANDLER(IDC_EDIT_FATOR, EN_CHANGE, OnEnChangeEditFator)
        COMMAND_HANDLER(IDC_EDIT_ROTATE, EN_CHANGE, OnEnChangeEditRotate)
        COMMAND_HANDLER(IDC_BtnPrePage, BN_CLICKED, OnBnClickedBtnprepage)
        COMMAND_HANDLER(IDC_BtnNextPage, BN_CLICKED, OnBnClickedBtnnextpage)
    END_MSG_MAP()


	

private:
    /** GDI+内部参数
    */
    ULONG_PTR m_gdiplusToken;
    static Gdiplus::Graphics m_graphics; 
    /** 图片路径
    */
    std::wstring m_wstrImagePath;
    /** 前一张图片路径
    */
    std::wstring m_wstrPrePath;
    /** 后一张图片路径
    */
    std::wstring m_wstrNextPath;
    /**图片名集合
    */
    std::vector<std::string> m_strFileNameAry;
    /**当前图片索引
    */
    int m_iCurNumber;
    /** 缩放因子
    */
    float m_fFactor;    
    /** 旋转因子
    */
    float m_fRoate;    
    

    /** 显示图片
    @param[in] pFilepath 要显示的图片路径
    */
    void CMainDlg::ShowPicture(const std::wstring& pFilepath);

    /** 销毁图片
    */
    void CMainDlg::DestoryPicture();

    /** 缩放图片
    @param[in] pFilepath 要缩放的图片路径
    @param[in] fFator    要缩放的系数
    */
    void CMainDlg::ZoomPicture(const std::wstring& pFilepath, const float fFator);

    /** 旋转图片
    @param[in] source  要旋转的图片指针
    @param[in] fRoate  要旋转的系数
    */
    Gdiplus::Image* CMainDlg::RotateImage(Gdiplus::Image& source,   float fRoate);

    /** 水平翻转图片
    @param[in] source  要翻转的图片指针
    */
    Gdiplus::Image* CMainDlg::FilpImageX(Gdiplus::Image& source);

    /** 垂直翻转图片
    @param[in] source  要翻转的图片指针
    */
    Gdiplus::Image* CMainDlg::FilpImageY(Gdiplus::Image& source);

    /** 克隆图片
    @param[in] source  要克隆的图片指针
    */
    Gdiplus::Image* CMainDlg::CloneImage(Gdiplus::Image& source);

    /** 折射图片
    @param[in] source  要折射的图片指针
    */
    Gdiplus::Image* CMainDlg::CastImage(Gdiplus::Image& source);

    /** 获取旋转图片的旋转角度
    @param[in] width  图片的宽
    @param[in] height 图片的高
    @param[in] angle  图片的旋转角度
    */
    Gdiplus::Rect* CMainDlg::GetRotateRectangle(int width, int height, float angle);
   
    /** 获取文件夹同格式文件
    @param[in] path   文件夹路径
    @param[out] files 文件路径集合
    @param[in] format 文件格式
    */
    void CMainDlg::GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format);

    /** DLG初始化
    */
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    /** OnAppAbout
    */
    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** OK按钮事件
    */
    LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** CANCEL按钮事件
    */
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片插入按钮
    */
    LRESULT OnBnClickedBtninsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片删除按钮
    */
    LRESULT OnBnClickedBtndelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片放大按钮
    */
    LRESULT OnBnClickedBtnpus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片缩小按钮
    */
    LRESULT OnBnClickedBtndec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片逆时针按钮
    */
    LRESULT OnBnClickedBtnnagtive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片顺时针按钮
    */
    LRESULT OnBnClickedBtnpostive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片水平翻转按钮
    */
    LRESULT OnBnClickedBtnhoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片垂直翻转按钮
    */
    LRESULT OnBnClickedBtnvert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片克隆按钮
    */
    LRESULT OnBnClickedBtnclone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片折射按钮
    */
    LRESULT OnBnClickedBtncast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 输入改变按钮
    */
    LRESULT OnEnChangeEditFator(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 输入改变按钮
    */
    LRESULT OnEnChangeEditRotate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 前一张按钮
    */
    LRESULT OnBnClickedBtnprepage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 后一张按钮
    */
    LRESULT OnBnClickedBtnnextpage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


#endif