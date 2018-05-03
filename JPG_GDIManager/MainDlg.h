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
    /** GDI+�ڲ�����
    */
    ULONG_PTR m_gdiplusToken;
    static Gdiplus::Graphics m_graphics; 
    /** ͼƬ·��
    */
    std::wstring m_wstrImagePath;
    /** ǰһ��ͼƬ·��
    */
    std::wstring m_wstrPrePath;
    /** ��һ��ͼƬ·��
    */
    std::wstring m_wstrNextPath;
    /**ͼƬ������
    */
    std::vector<std::string> m_strFileNameAry;
    /**��ǰͼƬ����
    */
    int m_iCurNumber;
    /** ��������
    */
    float m_fFactor;    
    /** ��ת����
    */
    float m_fRoate;    
    

    /** ��ʾͼƬ
    @param[in] pFilepath Ҫ��ʾ��ͼƬ·��
    */
    void CMainDlg::ShowPicture(const std::wstring& pFilepath);

    /** ����ͼƬ
    */
    void CMainDlg::DestoryPicture();

    /** ����ͼƬ
    @param[in] pFilepath Ҫ���ŵ�ͼƬ·��
    @param[in] fFator    Ҫ���ŵ�ϵ��
    */
    void CMainDlg::ZoomPicture(const std::wstring& pFilepath, const float fFator);

    /** ��תͼƬ
    @param[in] source  Ҫ��ת��ͼƬָ��
    @param[in] fRoate  Ҫ��ת��ϵ��
    */
    Gdiplus::Image* CMainDlg::RotateImage(Gdiplus::Image& source,   float fRoate);

    /** ˮƽ��תͼƬ
    @param[in] source  Ҫ��ת��ͼƬָ��
    */
    Gdiplus::Image* CMainDlg::FilpImageX(Gdiplus::Image& source);

    /** ��ֱ��תͼƬ
    @param[in] source  Ҫ��ת��ͼƬָ��
    */
    Gdiplus::Image* CMainDlg::FilpImageY(Gdiplus::Image& source);

    /** ��¡ͼƬ
    @param[in] source  Ҫ��¡��ͼƬָ��
    */
    Gdiplus::Image* CMainDlg::CloneImage(Gdiplus::Image& source);

    /** ����ͼƬ
    @param[in] source  Ҫ�����ͼƬָ��
    */
    Gdiplus::Image* CMainDlg::CastImage(Gdiplus::Image& source);

    /** ��ȡ��תͼƬ����ת�Ƕ�
    @param[in] width  ͼƬ�Ŀ�
    @param[in] height ͼƬ�ĸ�
    @param[in] angle  ͼƬ����ת�Ƕ�
    */
    Gdiplus::Rect* CMainDlg::GetRotateRectangle(int width, int height, float angle);
   
    /** ��ȡ�ļ���ͬ��ʽ�ļ�
    @param[in] path   �ļ���·��
    @param[out] files �ļ�·������
    @param[in] format �ļ���ʽ
    */
    void CMainDlg::GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format);

    /** DLG��ʼ��
    */
    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

    /** OnAppAbout
    */
    LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** OK��ť�¼�
    */
    LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** CANCEL��ť�¼�
    */
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ���밴ť
    */
    LRESULT OnBnClickedBtninsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬɾ����ť
    */
    LRESULT OnBnClickedBtndelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ�Ŵ�ť
    */
    LRESULT OnBnClickedBtnpus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ��С��ť
    */
    LRESULT OnBnClickedBtndec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ��ʱ�밴ť
    */
    LRESULT OnBnClickedBtnnagtive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ˳ʱ�밴ť
    */
    LRESULT OnBnClickedBtnpostive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬˮƽ��ת��ť
    */
    LRESULT OnBnClickedBtnhoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ��ֱ��ת��ť
    */
    LRESULT OnBnClickedBtnvert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ��¡��ť
    */
    LRESULT OnBnClickedBtnclone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ͼƬ���䰴ť
    */
    LRESULT OnBnClickedBtncast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ����ı䰴ť
    */
    LRESULT OnEnChangeEditFator(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ����ı䰴ť
    */
    LRESULT OnEnChangeEditRotate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ǰһ�Ű�ť
    */
    LRESULT OnBnClickedBtnprepage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    /** ��һ�Ű�ť
    */
    LRESULT OnBnClickedBtnnextpage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


#endif