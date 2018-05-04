
#include "stdafx.h"
#include "MainDlg.h"
#include <atldlgs.h>
#include <atltypes.h>
#include <atlstr.h>
#include <math.h>
#include <comutil.h>
#include <io.h>
#include <algorithm> 
#include "ComFunction.h"

/** 缩放因子限值
*/
#define MAX_FATOR   1.0
#define MIN_FATOR   0.1
/** 旋转因子限值
*/
#define MAX_ROTAT   360
#define MIN_ROTAT   1


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow();
    HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);
    // GDI+的初始化配置
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    // 初始化默认比例及角度
    GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T("0.1"));
    GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T("30"));
    GetDlgItem(IDC_STA_F).SetWindowTextW(_T(" scaling:"));
    GetDlgItem(IDC_STA_R).SetWindowTextW(_T(" angle rotation:"));
    return TRUE;
}

LRESULT CMainDlg::OnBnClickedBtninsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DestoryPicture();
    m_wstrImagePath = _T("");
    m_strFileNameAry.clear();

    TCHAR strFileFilters[1024] = _T("JPG Files(*.jpg)\0*.jpg\0\0");
    CFileDialog selImageDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFileFilters, this->m_hWnd);
    TCHAR temp[1024];
    ZeroMemory(temp, sizeof(TCHAR) * 1024);
    selImageDialog.m_ofn.lpstrFile = temp;
    selImageDialog.m_ofn.nMaxFile = 1024;
    if (IDOK == selImageDialog.DoModal())
    {
        m_wstrImagePath = temp;
    }
    if (!m_wstrImagePath.empty())
    {
        // 初始化缩放因子
        m_fFactor = 1.0;
        m_fRoate = 0.0;
        ShowPicture(m_wstrImagePath);

        std::wstring strTempPath = ComFunction::GetFilePath(m_wstrImagePath);
        std::string strFilePath = ComFunction::ConvertWStringToAnsi(strTempPath);
        GetAllFormatFiles(strFilePath, m_strFileNameAry, ".jpg");
        std::sort(m_strFileNameAry.begin(), m_strFileNameAry.end());
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请选择需要插入的图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtndelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DestoryPicture();
    // 清空图片路径
    m_wstrImagePath = _T("");
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnpus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        std::wstring wstrTemp;
        float fTemp = m_fFactor;
        ::GetDlgItemText(m_hWnd, IDC_EDIT_FATOR, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
        std::wstring wstrTempBuf(wstrTemp.c_str());
        if (wstrTempBuf.empty())
        {
            if (::MessageBox(m_hWnd, _T("放大比例不能为空！"), _T("tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
                return 0;
            }
        }
        float nfator = (float)_wtof(wstrTemp.c_str());
        if (nfator > MAX_FATOR || nfator < MIN_FATOR)
        {
            TCHAR cTemp[1024];
            swprintf_s(cTemp, _T("输入的系数必须在%3.1f~%3.1f之间!"), MIN_FATOR, MAX_FATOR);
            std::wstring wstrTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
                return 0;
            }
        }
        if (m_fFactor < MAX_FATOR)
        {
            m_fFactor = m_fFactor + nfator;
            if (m_fFactor > MAX_FATOR)
            {
                m_fFactor = fTemp;
            }
            DestoryPicture();
            ZoomPicture(m_wstrImagePath, m_fFactor);
        }
        else
        {
            ::MessageBox(m_hWnd, _T("以当前比例无法在继续放大"), _T("Tips"), MB_OK);
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtndec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        std::wstring wstrTemp;
        float fTemp = m_fFactor;
        ::GetDlgItemText(m_hWnd, IDC_EDIT_FATOR, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
        std::wstring wstrTempBuf(wstrTemp.c_str());
        if (wstrTempBuf.empty())
        {
            if (::MessageBox(m_hWnd, _T("缩小比例不能为空！"), _T("tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
                return 0;
            }
        }
        float nfator = (float)_wtof(wstrTemp.c_str());
        if (nfator > MAX_FATOR || nfator < MIN_FATOR)
        {
            TCHAR cTemp[1024];
            swprintf_s(cTemp, _T("输入的系数必须在%3.1f~%3.1f之间!"), MIN_FATOR, MAX_FATOR);
            std::wstring wstrTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
                return 0;
            }
        }
        if (m_fFactor > MIN_FATOR)
        {
            m_fFactor = m_fFactor - nfator;
            if (m_fFactor <= MIN_FATOR)
            {
                m_fFactor = fTemp;
                ::MessageBox(m_hWnd, _T("以当前比例无法在继续缩小"), _T("tips"), MB_OK);
            }
            DestoryPicture();
            ZoomPicture(m_wstrImagePath, m_fFactor);
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnnagtive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        std::wstring wstrTemp;
        ::GetDlgItemText(m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
        std::wstring wstrTempBuf(wstrTemp.c_str());
        if (wstrTempBuf.empty())
        {
            if (::MessageBox(m_hWnd, _T("旋转角度不能为空！"), _T("tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
                return 0;
            }
        }
        int nRotar = _wtoi(wstrTemp.c_str());
        if (nRotar > MAX_ROTAT || nRotar < MIN_ROTAT)
        {
            TCHAR cTemp[1024];
            swprintf_s(cTemp, _T("输入的角度必须在%d~%d之间!"), MIN_ROTAT, MAX_ROTAT);
            std::wstring wstrTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
                return 0;
            }
        }
        m_fRoate = m_fRoate - nRotar;
        Gdiplus::Image* image = new Gdiplus::Image(m_wstrImagePath.c_str());
        if (NULL != image)
        {
            Gdiplus::Image* roteimage = RotateImage(*image, m_fRoate);
            // 清空
            DestoryPicture();
            if (NULL != roteimage)
            {
                CRect rect;
                CRect roterect;
                int height = roteimage->GetHeight();
                int width = roteimage->GetWidth();
                CWindow wnd = GetDlgItem(IDC_PIC);
                wnd.GetClientRect(&rect);
                CDC pDc(wnd.GetDC());
                Gdiplus::Graphics graphics(pDc.m_hDC);
                // 防止失真
                SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

                // 如果图片的尺寸小于图片控件尺寸 
                if (width <= rect.Width() && height <= rect.Width())
                {
                    roterect = CRect(rect.TopLeft(), CSize(width, height));
                    graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
                }
                // 如果图片的尺寸大于图片控件的尺寸 
                else
                {
                    float xScale = (float)rect.Width() / (float)width;
                    float yScale = (float)rect.Height() / (float)height;
                    float scaleIndex = (xScale <= yScale ? xScale : yScale);
                    roterect = CRect(rect.TopLeft(), CSize((int)(width*scaleIndex), (int)(height*scaleIndex)));
                    graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
                    ReleaseDC(pDc);
                }
            }
            delete image;
            image = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnpostive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        std::wstring wstrTemp;
        ::GetDlgItemText(m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
        std::wstring wstrTempBuf(wstrTemp.c_str());
        if (wstrTempBuf.empty())
        {
            if (::MessageBox(m_hWnd, _T("旋转角度不能为空！"), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
                return 0;
            }
        }
        int nRotar = _wtoi(wstrTemp.c_str());
        if (nRotar > MAX_ROTAT || nRotar < MIN_ROTAT)
        {
            TCHAR cTemp[1024];
            swprintf_s(cTemp, _T("输入的角度必须在%d~%d之间!"), MIN_ROTAT, MAX_ROTAT);
            std::wstring wstrTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
                return 0;
            }
        }
        m_fRoate = m_fRoate + nRotar;
        Gdiplus::Image* image = new Gdiplus::Image(m_wstrImagePath.c_str());
        if (NULL != image)
        {
            Gdiplus::Image* roteimage = RotateImage(*image, m_fRoate);
            // 清空
            DestoryPicture();
            if (NULL != roteimage)
            {
                CRect rect;
                CRect roterect;
                int height = roteimage->GetHeight();
                int width = roteimage->GetWidth();

                CWindow wnd = GetDlgItem(IDC_PIC);
                wnd.GetClientRect(&rect);
                CDC pDc(wnd.GetDC());
                Gdiplus::Graphics graphics(pDc.m_hDC);
                // 防止失真
                SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

                // 如果图片的尺寸小于图片控件尺寸
                if (width <= rect.Width() && height <= rect.Width())
                {
                    roterect = CRect(rect.TopLeft(), CSize(width, height));
                    graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
                }
                // 如果图片的尺寸大于图片控件的尺寸 
                else
                {
                    float xScale = (float)rect.Width() / (float)width;
                    float yScale = (float)rect.Height() / (float)height;
                    float scaleIndex = (xScale <= yScale ? xScale : yScale);
                    roterect = CRect(rect.TopLeft(), CSize((int)(width*scaleIndex), (int)(height*scaleIndex)));
                    graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
                    ReleaseDC(pDc);
                }
                delete roteimage;
                roteimage = NULL;
            }
            delete image;
            image = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnhoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        Gdiplus::Image* image = new Gdiplus::Image(m_wstrImagePath.c_str());
        if (NULL != image)
        {
            Gdiplus::Image* roteimage = FilpImageX(*image);
            // 清空
            DestoryPicture();
            if (NULL != roteimage)
            {
                CRect rect;
                CRect filprect;
                int height = roteimage->GetHeight();
                int width = roteimage->GetWidth();

                CWindow wnd = GetDlgItem(IDC_PIC);
                wnd.GetClientRect(&rect);
                CDC pDc(wnd.GetDC());
                Gdiplus::Graphics graphics(pDc.m_hDC);
                // 防止失真
                SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

                // 如果图片的尺寸小于图片控件尺寸
                if (width <= rect.Width() && height <= rect.Width())
                {
                    filprect = CRect(rect.TopLeft(), CSize(width, height));
                    graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                }
                //如果图片的尺寸大于图片控件的尺寸 
                else
                {
                    float xScale = (float)rect.Width() / (float)width;
                    float yScale = (float)rect.Height() / (float)height;
                    float scaleIndex = (xScale <= yScale ? xScale : yScale);
                    filprect = CRect(rect.TopLeft(), CSize((int)(width*scaleIndex), (int)(height*scaleIndex)));
                    graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                    ReleaseDC(pDc);
                }
                delete roteimage;
                roteimage = NULL;
            }
            delete image;
            image = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;

}

LRESULT CMainDlg::OnBnClickedBtnvert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        Gdiplus::Image* image = new Gdiplus::Image(m_wstrImagePath.c_str());
        if (NULL != image)
        {
            Gdiplus::Image* roteimage = FilpImageY(*image);
            //清空
            DestoryPicture();
            if (NULL != roteimage)
            {
                CRect rect;
                CRect filprect;
                int height = roteimage->GetHeight();
                int width = roteimage->GetWidth();

                CWindow wnd = GetDlgItem(IDC_PIC);
                wnd.GetClientRect(&rect);
                CDC pDc(wnd.GetDC());
                Gdiplus::Graphics graphics(pDc.m_hDC);
                // 防止失真
                SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

                // 如果图片的尺寸小于图片控件尺寸 
                if (width <= rect.Width() && height <= rect.Width())
                {
                    filprect = CRect(rect.TopLeft(), CSize(width, height));
                    graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                }
                // 如果图片的尺寸大于图片控件的尺寸 
                else
                {
                    float xScale = (float)rect.Width() / (float)width;
                    float yScale = (float)rect.Height() / (float)height;
                    float scaleIndex = (xScale <= yScale ? xScale : yScale);
                    filprect = CRect(rect.TopLeft(), CSize((int)(width*scaleIndex), (int)(height*scaleIndex)));
                    graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                    ReleaseDC(pDc);
                }
                delete roteimage;
                roteimage = NULL;
            }
            delete image;
            image = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnclone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {

        Gdiplus::Image* image = new Gdiplus::Image(m_wstrImagePath.c_str());
        if (NULL != image)
        {
            Gdiplus::Image* roteimage = CloneImage(*image);
            // 清空
            DestoryPicture();
            if (NULL != roteimage)
            {
                CRect rect;
                CRect filprect;
                int height = roteimage->GetHeight();
                int width = roteimage->GetWidth();

                CWindow wnd = GetDlgItem(IDC_PIC);
                wnd.GetClientRect(&rect);
                CDC pDc(wnd.GetDC());
                Gdiplus::Graphics graphics(pDc.m_hDC);
                // 防止失真
                SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

                // 如果图片的尺寸小于图片控件尺寸
                if (width <= rect.Width() && height <= rect.Width())
                {
                    filprect = CRect(rect.TopLeft(), CSize(width, height));
                    graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                }
                // 如果图片的尺寸大于图片控件的尺寸 
                else
                {
                    float xScale = (float)rect.Width() / (float)width;
                    float yScale = (float)rect.Height() / (float)height;
                    float scaleIndex = (xScale <= yScale ? xScale : yScale);
                    filprect = CRect(rect.TopLeft(), CSize((int)(width*scaleIndex), (int)(height*scaleIndex)));
                    graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                    ReleaseDC(pDc);
                }
                delete roteimage;
                roteimage = NULL;
            }
            delete image;
            image = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtncast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        Gdiplus::Image* image = new Gdiplus::Image(m_wstrImagePath.c_str());
        if (NULL != image)
        {
            Gdiplus::Image* roteimage = CastImage(*image);
            // 清空
            DestoryPicture();
            if (NULL != roteimage)
            {
                CRect rect;
                CRect filprect;
                int height = roteimage->GetHeight();
                int width = roteimage->GetWidth();

                CWindow wnd = GetDlgItem(IDC_PIC);
                wnd.GetClientRect(&rect);
                CDC pDc(wnd.GetDC());
                Gdiplus::Graphics graphics(pDc.m_hDC);
                //防止失真
                SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

                // 如果图片的尺寸小于图片控件尺寸
                if (width <= rect.Width() && height <= rect.Width())
                {
                    filprect = CRect(rect.TopLeft(), CSize(width, height));
                    graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                }
                // 如果图片的尺寸大于图片控件的尺寸 
                else
                {
                    float xScale = (float)rect.Width() / (float)width;
                    float yScale = (float)rect.Height() / (float)height;
                    float scaleIndex = (xScale <= yScale ? xScale : yScale);
                    filprect = CRect(rect.TopLeft(), CSize((int)(width*scaleIndex), (int)(height*scaleIndex)));
                    graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                    ReleaseDC(pDc);
                }
                delete roteimage;
                roteimage = NULL;
            }
            delete image;
            image = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

void CMainDlg::ShowPicture(const std::wstring& pFilepath)
{
    CWindow wnd = GetDlgItem(IDC_PIC);
    CDC pDc(wnd.GetDC());

    Gdiplus::Graphics graphics(pDc.m_hDC);
    Gdiplus::Image image(pFilepath.c_str()); 
    int height = image.GetHeight();
    int width = image.GetWidth();

    CRect rect;
    CRect fatorect;
    wnd.GetClientRect(&rect);
    float xScale = (float)rect.Width() / (float)width;
    float yScale = (float)rect.Height() / (float)height;
    float scaleIndex = (xScale <= yScale ? xScale : yScale);
    fatorect = CRect(rect.TopLeft(), CSize((int)(width*scaleIndex), (int)(height*scaleIndex)));
    graphics.DrawImage(&image, 0, 0, fatorect.Width(), fatorect.Height());
    ReleaseDC(pDc);
}

void CMainDlg::DestoryPicture()
{
    CWindow wnd = GetDlgItem(IDC_PIC);
    CRect rect;
    wnd.GetClientRect(&rect);
    CDC pDc(wnd.GetDC());
    pDc.FillSolidRect(rect.left, rect.top, rect.Width(), rect.Height(), RGB(240, 240, 240));
    ReleaseDC(pDc);
}

void CMainDlg::ZoomPicture(const std::wstring& pFilepath, const float fFator)
{
    CRect rect;
    CRect fatorrect;
    Gdiplus::Image* image = new Gdiplus::Image(pFilepath.c_str());
    if (NULL != image)
    {
        int height = image->GetHeight();
        int width = image->GetWidth();
        CWindow wnd = GetDlgItem(IDC_PIC);
        wnd.GetClientRect(&rect);
        CDC pDc(wnd.GetDC());
        Gdiplus::Graphics graphics(pDc.m_hDC);
        // 防止失真
        SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

        // 图片的尺寸小于图片控件尺寸
        if (width <= rect.Width() && height <= rect.Width())
        {
            fatorrect = CRect(rect.TopLeft(), CSize((int)(width * fFator), (int)(height * fFator)));
            graphics.DrawImage(image, fatorrect.top, fatorrect.left, fatorrect.Width(), fatorrect.Height());
        }
        // 图片的尺寸大于图片控件的尺寸 
        else
        {
            float xScale = (float)rect.Width() / (float)width;
            float yScale = (float)rect.Height() / (float)height;
            float scaleIndex = (xScale <= yScale ? xScale : yScale);
            fatorrect = CRect(rect.TopLeft(), CSize((int)(width * scaleIndex * fFator), (int)(height * scaleIndex * fFator)));
            graphics.DrawImage(image, fatorrect.top, fatorrect.left, fatorrect.Width(), fatorrect.Height());
            ReleaseDC(pDc);
        }
    }
    delete image;
    image = NULL;
}

Gdiplus::Image* CMainDlg::RotateImage(Gdiplus::Image& source,  float fRoate)
{
    float fAngle = (float)((int)fRoate % 360);
    int srcWidth = source.GetWidth();
    int srcHeight = source.GetHeight();
    // 图像旋转之后所占区域宽和高
    Gdiplus::Rect* rotateRec = GetRotateRectangle(srcWidth, srcHeight, fAngle);
    int rotateWidth = 0;
    int rotateHeight = 0;
    if (NULL != rotateRec)
    {
        rotateWidth = rotateRec->Width;
        rotateHeight = rotateRec->Height;
    }
    // 定义画布大小
    Gdiplus::Bitmap* pImageScale = new Gdiplus::Bitmap(rotateWidth, rotateHeight);
    if (NULL != pImageScale)
    {
        Gdiplus::Graphics graphics(pImageScale);
        Gdiplus::Point* centerPoint = new Gdiplus::Point(rotateWidth / 2, rotateHeight / 2);
        if (NULL != centerPoint)
        {
            graphics.TranslateTransform((Gdiplus::REAL)centerPoint->X, (Gdiplus::REAL)centerPoint->Y);
            graphics.RotateTransform(fAngle);
            graphics.TranslateTransform(-(Gdiplus::REAL)centerPoint->X, -(Gdiplus::REAL)centerPoint->Y);
            Gdiplus::Point* offset = new Gdiplus::Point((rotateWidth - srcWidth) / 2, (rotateHeight - srcHeight) / 2);
            if (NULL != offset)
            {
                graphics.DrawImage(&source, offset->X, offset->Y, srcWidth, srcHeight);
                // 重至绘图的所有变换
                graphics.ResetTransform();
                graphics.Save();
                delete offset;
                offset = NULL;
            }
            delete centerPoint;
            centerPoint = NULL;
        }
    }
    return pImageScale;
}

Gdiplus::Rect* CMainDlg::GetRotateRectangle(int width, int height, float angle)
{
    float radian = (float)(angle * 3.1415926 / 180.0);
    float fcos = cos(radian);
    float fsin = sin(radian);
    int newWidth = (int)(max(abs(width * fcos - height * fsin), abs(width * fcos + height * fsin)));
    int newHeight = (int)(max(abs(width * fsin - height * fcos), abs(width * fsin + height * fcos)));
    // 旋转后的图片大小
    return new Gdiplus::Rect(0, 0, newWidth, newHeight);
}

Gdiplus::Image* CMainDlg::FilpImageX(Gdiplus::Image& source)
{
    int width = source.GetWidth();
    int height = source.GetHeight();
    // 水平翻转
    Gdiplus::Matrix matrix;
    Gdiplus::Graphics graphics(&source);
    Gdiplus::Image* Filpimge = source.Clone();
    if (NULL != Filpimge)
    {
        Filpimge->RotateFlip(Gdiplus::RotateNoneFlipX);
    }
    return Filpimge;
}

Gdiplus::Image* CMainDlg::FilpImageY(Gdiplus::Image& source)
{
    int width = source.GetWidth();
    int height = source.GetHeight();
    // 垂直翻转
    Gdiplus::Matrix matrix;
    Gdiplus::Graphics graphics(&source);
    Gdiplus::Image* Filpimge = source.Clone();
    if (NULL != Filpimge)
    {
        Filpimge->RotateFlip(Gdiplus::RotateNoneFlipY);
    }
    return Filpimge;
}

Gdiplus::Image* CMainDlg::CloneImage(Gdiplus::Image& source)
{
    int height = source.GetHeight();
    int width = source.GetWidth();
    // 实现局部克隆
    Gdiplus::Image* scrBit = new Gdiplus::Bitmap(width, height);
    if (NULL != scrBit)
    {
        Gdiplus::Bitmap* scrBitTemp = new Gdiplus::Bitmap(width, height);
        if (NULL != scrBitTemp)
        {
             scrBitTemp = (Gdiplus::Bitmap*)&source;
             Gdiplus::Rect rect[4];
             rect[0] = Gdiplus::Rect(0, 0, width / 2, height / 2);
             rect[1] = Gdiplus::Rect(width / 2, 0, width / 2, height / 2);
             rect[2] = Gdiplus::Rect(0, height / 2, width / 2, height / 2);
             rect[3] = Gdiplus::Rect(width / 2, height / 2, width / 2, height / 2);

             // 把原图像分为分四块克隆  
             Gdiplus::Bitmap *scrBitmap[4];
             scrBitmap[0] = scrBitTemp->Clone(rect[0], PixelFormatDontCare);
             scrBitmap[1] = scrBitTemp->Clone(rect[1], PixelFormatDontCare);
             scrBitmap[2] = scrBitTemp->Clone(rect[2], PixelFormatDontCare);
             scrBitmap[3] = scrBitTemp->Clone(rect[3], PixelFormatDontCare);

             // 绘图  
             Gdiplus::Graphics graphics(scrBit);
             graphics.DrawImage(scrBitmap[0], Gdiplus::Rect(0, 0, width / 2, height / 2));
             graphics.DrawImage(scrBitmap[1], Gdiplus::Rect(width / 2 + 10, 0, width / 2, height / 2));
             graphics.DrawImage(scrBitmap[2], Gdiplus::Rect(0, height / 2 + 10, width / 2, height / 2));
             graphics.DrawImage(scrBitmap[3], Gdiplus::Rect(width / 2 + 10, height / 2 + 10, width / 2, height / 2));
             graphics.ResetTransform();
             graphics.Save();
        }
    }
    return scrBit;
}

Gdiplus::Image* CMainDlg::CastImage(Gdiplus::Image& source)
{
    int left = 0;
    int top = 700;
    int height = source.GetHeight();
    int width = source.GetWidth();

    // 定义画布
    Gdiplus::Image* scrImage = new Gdiplus::Bitmap(width*2, height);
    if (NULL != scrImage)
    {
        Gdiplus::Graphics graphics(scrImage);
        graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

        // 正面投射
        Gdiplus::Point desFace[] = {
            Gdiplus::Point(left, top),
            Gdiplus::Point(left + width, top),
            Gdiplus::Point(left, top + width),
        };
        graphics.DrawImage(&source, desFace, 3);

        // 上面投射  
        Gdiplus::Point destTop[] = {
            Gdiplus::Point(left + width / 2, top - width / 2),
            Gdiplus::Point(left + width / 2 + width, top - width / 2),
            Gdiplus::Point(left, top),
        };
        graphics.DrawImage(&source, destTop, 3);

        // 侧面投射  
        Gdiplus::Point desRight[] = {
            Gdiplus::Point(left + width, top),
            Gdiplus::Point(left + width / 2 + width, top - width / 2),
            Gdiplus::Point(left + width, top + width)
        };
        graphics.DrawImage(&source, desRight, 3);
    }
    return scrImage;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnEnChangeEditFator(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    std::wstring wstrTemp;
    float fTemp = m_fFactor;
    ::GetDlgItemText(this->m_hWnd, IDC_EDIT_FATOR, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
    std::string strLen = ComFunction::ConvertWStringToAnsi(wstrTemp);
    bool isTrue = ComFunction::IsFloatNumber(strLen);
    if (!isTrue)
    {
        TCHAR cTemp[1024];
        swprintf_s(cTemp, _T("请输入一个有效的浮点型数字！"), MIN_ROTAT, MAX_ROTAT);
        std::wstring wstrTitle(cTemp);
        if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
        {
            GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T(""));
            GetDlgItem(IDC_EDIT_FATOR).SetFocus();
        }
    }
    float nfator = (float)_wtof(wstrTemp.c_str());
    if (nfator > 0)
    {
        if (nfator > MAX_FATOR || nfator < MIN_FATOR)
        {
            TCHAR cTemp[1024];
            swprintf_s(cTemp, _T("输入的系数必须在%3.1f~%3.1f之间!"), MIN_FATOR, MAX_FATOR);
            std::wstring wstrTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
            }
        }
    }
    return 0;
}

LRESULT CMainDlg::OnEnChangeEditRotate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    std::wstring wstrTemp;
    ::GetDlgItemText(m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
    std::string strLen = ComFunction::ConvertWStringToAnsi(wstrTemp);
    bool isTrue = ComFunction::IsIntNumber(strLen);
    if (!isTrue)
    {
        TCHAR cTemp[1024];
        swprintf_s(cTemp, _T("请输入一个有效的整型数字！"), MIN_ROTAT, MAX_ROTAT);
        std::wstring wstrTitle(cTemp);
        if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
        {
            GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T(""));
            GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
        }
    }
    int nRotar = _wtoi(wstrTemp.c_str());
    if (nRotar > 0)
    {
        if (nRotar > MAX_ROTAT || nRotar < MIN_ROTAT)
        {
            TCHAR cTemp[1024];
            swprintf_s(cTemp, _T("输入的角度必须在%d~%d之间!"), MIN_ROTAT, MAX_ROTAT);
            std::wstring wstrTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
            }
        }
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnprepage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_strFileNameAry.clear();
    if (!m_wstrImagePath.empty())
    {
        std::wstring strTempPath = ComFunction::GetFilePath(m_wstrImagePath);
        std::string strFilePath = ComFunction::ConvertWStringToAnsi(strTempPath);
        GetAllFormatFiles(strFilePath, m_strFileNameAry, ".jpg");
        if (m_strFileNameAry.size() > 0)
        {
            for (size_t i = 0; i < m_strFileNameAry.size(); i++)
            {
                std::string strImagepath = ComFunction::ConvertWStringToAnsi(m_wstrImagePath);
                if (strImagepath == m_strFileNameAry[i])
                {
                    m_iCurNumber = (int)i;
                }
            }
            std::wstring wstrCurPath;
            std::string strFilename;
            if ((m_iCurNumber - 1) <= 0)
            {
                strFilename = m_strFileNameAry[m_strFileNameAry.size() - 1];
                wstrCurPath = ComFunction::ConvertAnsiToWString(strFilename);
            }
            else
            {
                strFilename = m_strFileNameAry[m_iCurNumber - 1];
                wstrCurPath = ComFunction::ConvertAnsiToWString(strFilename);
            }
            // 更新全局图片路径
            m_wstrImagePath = wstrCurPath;
            DestoryPicture();
            ShowPicture(m_wstrImagePath);
            // 初始化缩放因子
            m_fFactor = 1.0;
            m_fRoate = 0.0;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnnextpage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_strFileNameAry.clear();
    if (!m_wstrImagePath.empty())
    {
        std::wstring strTempPath = ComFunction::GetFilePath(m_wstrImagePath);
        std::string strFilePath = ComFunction::ConvertWStringToAnsi(strTempPath);
        GetAllFormatFiles(strFilePath, m_strFileNameAry, ".jpg");
        if (m_strFileNameAry.size() > 0)
        {
            for (size_t i = 0; i < m_strFileNameAry.size(); i++)
            {
                std::string strImagepath = ComFunction::ConvertWStringToAnsi(m_wstrImagePath);
                if (strImagepath == m_strFileNameAry[i])
                {
                    m_iCurNumber = (int)i;
                }
            }
            std::wstring wstrCurPath;
            std::string strFilename;
            if ((m_iCurNumber + 1) >= (int)m_strFileNameAry.size())
            {
                strFilename = m_strFileNameAry[0];
                wstrCurPath = ComFunction::ConvertAnsiToWString(strFilename);
            }
            else
            {
                strFilename = m_strFileNameAry[m_iCurNumber + 1];
                wstrCurPath = ComFunction::ConvertAnsiToWString(strFilename);
            }
            // 更新全局图片路径
            m_wstrImagePath = wstrCurPath;
            DestoryPicture();
            ShowPicture(m_wstrImagePath);
            // 初始化缩放因子
            m_fFactor = 1.0;
            m_fRoate = 0.0;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

void CMainDlg::GetAllFormatFiles(const std::string& path, std::vector<std::string>& files, const std::string& format)
{
    // 文件句柄    
    long hFile = 0;
    // 文件信息    
    ::_finddata_t fileinfo;
    std::string p;
    if ((hFile = ::_findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
    {
        do
        {
            if ((fileinfo.attrib &  _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                {
                    GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
                }
            }
            else
            {
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
            }
        } while (::_findnext(hFile, &fileinfo) == 0);
        ::_findclose(hFile);
    }
}

