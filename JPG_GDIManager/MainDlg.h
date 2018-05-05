/******************************************************************************
*  ��Ȩ���У�C��2008-2015���Ϻ�����������������޹�˾                         *
*  ��������Ȩ����                                                             *
******************************************************************************
*  ���� : <gaolongcc>
*  �汾 : <v1.0>
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
    
    

    /** ��ʾͼƬ
    @param[in] pFilepath ͼƬ��·��
    */
    void CMainDlg::ShowImage(const std::wstring& srcImagePath);

    /** ����ͼƬ
    */
    void CMainDlg::DestoryImage();

    /** ����ͼƬ
    @param[in] srcImage ԭʼͼƬ
    @param[in] zoomImage ���ź�ͼƬ
    @param[in] fFator ����ϵ��
    */
    void ZoomImage(Gdiplus::Image& srcImage, Gdiplus::Image& zoomImage, double fFator);

    /** ��תͼƬ
    @param[in] srcImage ԭʼͼƬ
    @param[in] roateImage ��ת��ͼƬ
    @param[in] nRoate ��ת�Ƕ�
    */
    void RotateImage(Gdiplus::Image& srcImage, Gdiplus::Image& roateImage, int nRoate);

    /** ��תͼƬ
    @param[in] srcImage ԭʼͼƬ
    @param[in] filpImage ��ת��ͼƬ
    @param[in] nStyle ��ת��ʽ
    */
    void FilpImage(Gdiplus::Image& srcImage, Gdiplus::Image& filpImage, int nStyle);

    /** ��¡ͼƬ
    @param[in] srcImage ԭʼͼƬ
    @param[in] cloneImage ��¡��ͼƬ
    */
    void CloneImage(Gdiplus::Image& srcImage, Gdiplus::Image& cloneImage);

    /** ����ͼƬ
    @param[in] srcImage ԭʼͼƬ
    @param[in] castImage �����ͼƬ
    */
    void CastImage(Gdiplus::Image& srcImage, Gdiplus::Image& castImage);
   
    /** ��ȡ�ļ������ļ�·��
    @param[in] path �ļ���·��
    @param[out] files �ļ�·������
    @param[in] format �ļ���ʽ
    */
    void GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format);

    /** ����ͼƬ��ʾ������
    @param[in] objImage Ŀ��ͼƬ
    @param[in] fFator ����ϵ��
    @param[in] zoomEnable �Ƿ�����:true,�����ţ�false������
    */
    void SetWindowImage(Gdiplus::Image& objImage, double dFator = 1.0, bool zoomEnable = false);

    /** DLG��ʼ��
    */
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    /** OnAppAbout
    */
    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** OK��ť�¼�
    */
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** CANCEL��ť�¼�
    */
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ���밴ť
    */
    LRESULT OnClickedBtnInsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬɾ����ť
    */
    LRESULT OnClickedBtnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ�Ŵ�ť
    */
    LRESULT OnClickedBtnZoomPlus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ��С��ť
    */
    LRESULT OnClickedBtnZoomDec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ��ʱ�밴ť
    */
    LRESULT OnClickedBtnRoateNagt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ˳ʱ�밴ť
    */
    LRESULT OnClickedBtnRoatePost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬˮƽ��ת��ť
    */
    LRESULT OnClickedBtnFlipHoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ��ֱ��ת��ť
    */
    LRESULT OnClickedBtnFlipVert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ��¡��ť
    */
    LRESULT OnClickedBtnClone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ���䰴ť
    */
    LRESULT OnClickedBtnCast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ����ı䰴ť
    */
    LRESULT OnEnChangeEditFator(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ����ı䰴ť
    */
    LRESULT OnEnChangeEditRotate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ǰһ�Ű�ť
    */
    LRESULT OnClickedBtnPrepage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ��һ�Ű�ť
    */
    LRESULT OnClickedBtnNextpage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** GDI+�ڲ�����
    */
    ULONG_PTR m_gdiplusToken;
    static Gdiplus::Graphics m_graphics;

    /** ͼƬ·��
    */
    std::wstring m_imagePath;

    /** ǰһ��ͼƬ·��
    */
    std::vector<std::string> m_fileNameAry;

    /**��ǰͼƬ����
    */
    size_t m_iCurNumber;

    /** ��������
    */
    double m_dFactor;

    /** ��ת����
    */
    int m_nRoate;
};


#endif