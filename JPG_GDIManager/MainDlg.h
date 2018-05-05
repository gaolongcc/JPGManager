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
        COMMAND_HANDLER(IDC_BTNINSERT, BN_CLICKED, OnClickedBtnInsert)
        COMMAND_HANDLER(IDC_BTNDELETE, BN_CLICKED, OnClickedBtnDelete)
        COMMAND_HANDLER(IDC_BTNPUS, BN_CLICKED, OnClickedBtnZoomPlus)
        COMMAND_HANDLER(IDC_BTNDEC, BN_CLICKED, OnClickedBtnZoomDec)
        COMMAND_HANDLER(IDC_BTNNAGTIVE, BN_CLICKED, OnClickedBtnRoateNagt)
        COMMAND_HANDLER(IDC_BTNPOSTIVE, BN_CLICKED, OnClickedBtnRoatePost)
        COMMAND_HANDLER(IDC_BtnHoriz, BN_CLICKED, OnClickedBtnFlipHoriz)
        COMMAND_HANDLER(IDC_BtnVert, BN_CLICKED, OnClickedBtnFlipVert)
        COMMAND_HANDLER(IDC_BtnClone, BN_CLICKED, OnClickedBtnClone)
        COMMAND_HANDLER(IDC_BtnCast, BN_CLICKED, OnClickedBtnCast)
        COMMAND_HANDLER(IDC_EDIT_FATOR, EN_CHANGE, OnEnChangeEditFator)
        COMMAND_HANDLER(IDC_EDIT_ROTATE, EN_CHANGE, OnEnChangeEditRotate)
        COMMAND_HANDLER(IDC_BtnPrePage, BN_CLICKED, OnClickedBtnPrepage)
        COMMAND_HANDLER(IDC_BtnNextPage, BN_CLICKED, OnClickedBtnNextpage)
    END_MSG_MAP()


	

private:
    
    

    /** 显示图片
    @param[in] pFilepath 图片的路径
    */
    void CMainDlg::ShowImage(const std::wstring& srcImagePath);

    /** 销毁图片
    */
    void CMainDlg::DestoryImage();

    /** 缩放图片
    @param[in] srcImage 原始图片
    @param[in] zoomImage 缩放后图片
    @param[in] fFator 缩放系数
    */
    void ZoomImage(Gdiplus::Image& srcImage, Gdiplus::Image& zoomImage, double fFator);

    /** 旋转图片
    @param[in] srcImage 原始图片
    @param[in] roateImage 旋转后图片
    @param[in] nRoate 旋转角度
    */
    void RotateImage(Gdiplus::Image& srcImage, Gdiplus::Image& roateImage, int nRoate);

    /** 翻转图片
    @param[in] srcImage 原始图片
    @param[in] filpImage 翻转后图片
    @param[in] nStyle 翻转方式
    */
    void FilpImage(Gdiplus::Image& srcImage, Gdiplus::Image& filpImage, int nStyle);

    /** 克隆图片
    @param[in] srcImage 原始图片
    @param[in] cloneImage 克隆后图片
    */
    void CloneImage(Gdiplus::Image& srcImage, Gdiplus::Image& cloneImage);

    /** 折射图片
    @param[in] srcImage 原始图片
    @param[in] castImage 折射后图片
    */
    void CastImage(Gdiplus::Image& srcImage, Gdiplus::Image& castImage);
   
    /** 获取文件夹中文件路径
    @param[in] path 文件夹路径
    @param[out] files 文件路径集合
    @param[in] format 文件格式
    */
    void GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format);

    /** 设置图片显示到窗体
    @param[in] objImage 目标图片
    @param[in] fFator 缩放系数
    @param[in] zoomEnable 是否缩放:true,不缩放；false，缩放
    */
    void SetWindowImage(Gdiplus::Image& objImage, double dFator = 1.0, bool zoomEnable = false);

    /** DLG初始化
    */
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    /** OnAppAbout
    */
    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** OK按钮事件
    */
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** CANCEL按钮事件
    */
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片插入按钮
    */
    LRESULT OnClickedBtnInsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片删除按钮
    */
    LRESULT OnClickedBtnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片放大按钮
    */
    LRESULT OnClickedBtnZoomPlus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片缩小按钮
    */
    LRESULT OnClickedBtnZoomDec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片逆时针按钮
    */
    LRESULT OnClickedBtnRoateNagt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片顺时针按钮
    */
    LRESULT OnClickedBtnRoatePost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片水平翻转按钮
    */
    LRESULT OnClickedBtnFlipHoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片垂直翻转按钮
    */
    LRESULT OnClickedBtnFlipVert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片克隆按钮
    */
    LRESULT OnClickedBtnClone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 图片折射按钮
    */
    LRESULT OnClickedBtnCast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 输入改变按钮
    */
    LRESULT OnEnChangeEditFator(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 输入改变按钮
    */
    LRESULT OnEnChangeEditRotate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 前一张按钮
    */
    LRESULT OnClickedBtnPrepage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** 后一张按钮
    */
    LRESULT OnClickedBtnNextpage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** GDI+内部参数
    */
    ULONG_PTR m_gdiplusToken;
    static Gdiplus::Graphics m_graphics;

    /** 图片路径
    */
    std::wstring m_imagePath;

    /** 前一张图片路径
    */
    std::vector<std::string> m_fileNameAry;

    /**当前图片索引
    */
    size_t m_iCurNumber;

    /** 缩放因子
    */
    double m_dFactor;

    /** 旋转因子
    */
    int m_nRoate;
};


#endif