
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
#define MAX_FATOR 1.0
#define MIN_FATOR 0.1

/** 旋转因子限值
*/
#define MAX_ROTAT 360
#define MIN_ROTAT 1

/** 翻转方式
*/
enum enumType
{
    STYLE_X = 0,
    STYLE_Y,
    STYLE_XY
};

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    CenterWindow();
    HICON hIcon = ::AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = ::AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
    SetIcon(hIconSmall, FALSE);
    // GDI+的初始化配置
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
    // 初始化默认比例及角度
    GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T("0.1"));
    GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T("30"));
    GetDlgItem(IDC_STA_F).SetWindowTextW(_T(" Scaling:"));
    GetDlgItem(IDC_STA_R).SetWindowTextW(_T(" Angle Rotation:"));
    return TRUE;
}

LRESULT CMainDlg::OnClickedBtnInsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DestoryImage();
    m_imagePath = _T("");
    m_fileNameAry.clear();

    TCHAR strFileFilters[1024] = _T("JPG Files(*.jpg)\0*.jpg\0\0");
    CFileDialog selImageDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFileFilters, this->m_hWnd);
    TCHAR cTemp[1024] = {0};
    ZeroMemory(cTemp, sizeof(TCHAR) * 1024);
    selImageDialog.m_ofn.lpstrFile = cTemp;
    selImageDialog.m_ofn.nMaxFile = 1024;
    if (IDOK == selImageDialog.DoModal())
    {
        m_imagePath = cTemp;
    }
    if (!m_imagePath.empty())
    {
        // 初始化缩放因子
        m_dFactor = 1.0;
        m_nRoate = 0;
        m_iCurNumber = 0;
        ShowImage(m_imagePath);
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请选择需要插入的图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnClickedBtnDelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    DestoryImage();
    m_imagePath = _T("");
    return 0;
}

LRESULT CMainDlg::OnClickedBtnZoomPlus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_imagePath.empty())
    {
        std::wstring editText;
        double dFatorBuf = m_dFactor;
        ::GetDlgItemText(m_hWnd, IDC_EDIT_FATOR, (LPWSTR)editText.c_str(), editText.max_size());
        std::wstring editTextBuf(editText.c_str());
        if (editTextBuf.empty())
        {
            if (::MessageBox(m_hWnd, _T("放大比例不能为空！"), _T("tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
                return 0;
            }
        }
        double dFator = _wtof(editText.c_str());
        // 限值判断
        if (dFator > MAX_FATOR || dFator < MIN_FATOR)
        {
            TCHAR cTemp[1024] = {0};
            ::swprintf_s(cTemp, _T("输入的系数必须在%3.1f~%3.1f之间!"), MIN_FATOR, MAX_FATOR);
            std::wstring editTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)editTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
                return 0;
            }
        }
        if (m_dFactor < MAX_FATOR)
        {
            m_dFactor = m_dFactor + dFator;
            if (m_dFactor > MAX_FATOR)
            {
                m_dFactor = dFatorBuf;
            }
            // 图片预处理
            Gdiplus::Image srcImage(m_imagePath.c_str());
            int srcWidth = srcImage.GetWidth();
            int srcHeight = srcImage.GetHeight();
            int zoomWidth = (int)(srcWidth * m_dFactor);
            int zoomHeight = (int)(srcHeight * m_dFactor);
            Gdiplus::Image* pZoomImage = new Gdiplus::Bitmap(zoomWidth, zoomHeight);
            if (NULL != pZoomImage)
            {
                // 图片放大操作
                ZoomImage(srcImage, *pZoomImage, m_dFactor);
                DestoryImage();
                SetWindowImage(*pZoomImage, m_dFactor, true);
                delete pZoomImage;
                pZoomImage = NULL;
            }
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

LRESULT CMainDlg::OnClickedBtnZoomDec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_imagePath.empty())
    {
        std::wstring editText;
        double dFatorBuf = m_dFactor;
        ::GetDlgItemText(m_hWnd, IDC_EDIT_FATOR, (LPWSTR)editText.c_str(), editText.max_size());
        std::wstring editTextBuf(editText.c_str());
        if (editTextBuf.empty())
        {
            if (::MessageBox(m_hWnd, _T("缩小比例不能为空！"), _T("tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
                return 0;
            }
        }
        double dFator = _wtof(editTextBuf.c_str());
        // 限值判断
        if (dFator > MAX_FATOR || dFator < MIN_FATOR)
        {
            TCHAR cTemp[1024] = {0};
            ::swprintf_s(cTemp, _T("输入的系数必须在%3.1f~%3.1f之间!"), MIN_FATOR, MAX_FATOR);
            std::wstring editTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)editTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_FATOR).SetFocus();
                return 0;
            }
        }
        if (m_dFactor > MIN_FATOR)
        {
            m_dFactor = m_dFactor - dFator;
            if (m_dFactor <= MIN_FATOR)
            {
                m_dFactor = dFatorBuf;
                ::MessageBox(m_hWnd, _T("以当前比例无法在继续缩小"), _T("tips"), MB_OK);
                return 0;
            }
            // 图片预处理
            Gdiplus::Image srcImage(m_imagePath.c_str());
            int srcWidth = srcImage.GetWidth();
            int srcHeight = srcImage.GetHeight();
            int zoomWidth = (int)(srcWidth * dFatorBuf);
            int zoomHeight = (int)(srcHeight * dFatorBuf);
            Gdiplus::Image* pZoomImage = new Gdiplus::Bitmap(zoomWidth, zoomHeight);
            if (NULL != pZoomImage)
            {
                // 图片缩小操作
                ZoomImage(srcImage, *pZoomImage, m_dFactor);
                DestoryImage();
                SetWindowImage(*pZoomImage, m_dFactor, true);
                delete pZoomImage;
                pZoomImage = NULL;
            }
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnClickedBtnRoateNagt(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_imagePath.empty())
    {
        std::wstring editText;
        ::GetDlgItemText(m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)editText.c_str(), editText.max_size());
        std::wstring editTextBuf(editText.c_str());
        if (editTextBuf.empty())
        {
            if (::MessageBox(m_hWnd, _T("旋转角度不能为空！"), _T("tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
                return 0;
            }
        }
        int nRoate = _wtoi(editText.c_str());
        if (nRoate > MAX_ROTAT || nRoate < MIN_ROTAT)
        {
            TCHAR cTemp[1024] = { 0 };
            ::swprintf_s(cTemp, _T("输入的角度必须在%d~%d之间!"), MIN_ROTAT, MAX_ROTAT);
            std::wstring editTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)editTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
                return 0;
            }
        }
        m_nRoate = m_nRoate - nRoate;

        // 图片预处理
        Gdiplus::Image srcImage(m_imagePath.c_str());
        int srcWidth = srcImage.GetWidth();
        int srcHeight = srcImage.GetHeight();
        double fRadian = m_nRoate * 3.1415926 / 180;
        double fCos = cos(fRadian);
        double fSin = sin(fRadian);
        int roateWidth = (int)(max(abs(srcWidth * fCos - srcHeight * fSin), abs(srcWidth * fCos + srcHeight * fSin)));
        int roateHeight = (int)(max(abs(srcWidth * fSin - srcHeight * fCos), abs(srcWidth * fSin + srcHeight * fCos)));
        Gdiplus::Image* pRoateImage = new Gdiplus::Bitmap(roateWidth, roateHeight);
        if (NULL != pRoateImage)
        {
            //图片逆方向旋转
            RotateImage(srcImage, *pRoateImage, m_nRoate);
            DestoryImage();
            SetWindowImage(*pRoateImage);
            delete pRoateImage;
            pRoateImage = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnClickedBtnRoatePost(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_imagePath.empty())
    {
        std::wstring editText;
        ::GetDlgItemText(m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)editText.c_str(), editText.max_size());
        std::wstring editTextBuf(editText.c_str());
        if (editTextBuf.empty())
        {
            if (::MessageBox(m_hWnd, _T("旋转角度不能为空！"), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
                return 0;
            }
        }
        int nRotar = _wtoi(editText.c_str());
        if (nRotar > MAX_ROTAT || nRotar < MIN_ROTAT)
        {
            TCHAR cTemp[1024] = {0};
            swprintf_s(cTemp, _T("输入的角度必须在%d~%d之间!"), MIN_ROTAT, MAX_ROTAT);
            std::wstring editTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)editTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
                return 0;
            }
        }
        m_nRoate = m_nRoate + nRotar;
        //图片预处理
        Gdiplus::Image srcImage(m_imagePath.c_str());
        int srcWidth = srcImage.GetWidth();
        int srcHeight = srcImage.GetHeight();
        double fRadian = m_nRoate * 3.1415926 / 180;
        double fCos = cos(fRadian);
        double fSin = sin(fRadian);
        int roateWidth = (int)(max(abs(srcWidth * fCos - srcHeight * fSin), abs(srcWidth * fCos + srcHeight * fSin)));
        int roateHeight = (int)(max(abs(srcWidth * fSin - srcHeight * fCos), abs(srcWidth * fSin + srcHeight * fCos)));
        Gdiplus::Image* pRoateImage = new Gdiplus::Bitmap(roateWidth, roateHeight);
        if (NULL != pRoateImage)
        {
            //图片顺方向旋转
            RotateImage(srcImage, *pRoateImage, m_nRoate);
            DestoryImage();
            SetWindowImage(*pRoateImage);
            delete pRoateImage;
            pRoateImage = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnClickedBtnFlipHoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_imagePath.empty())
    {
        Gdiplus::Image srcImage(m_imagePath.c_str());
        int srcWidth = srcImage.GetWidth();
        int srcHeight = srcImage.GetHeight();
        Gdiplus::Image* pHorizImage = new Gdiplus::Bitmap(srcWidth, srcHeight);
        if (NULL != pHorizImage)
        {
            FilpImage(srcImage, *pHorizImage, STYLE_X);
            DestoryImage();
            SetWindowImage(*pHorizImage);
            delete pHorizImage;
            pHorizImage = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;

}

LRESULT CMainDlg::OnClickedBtnFlipVert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_imagePath.empty())
    {
        Gdiplus::Image srcImage(m_imagePath.c_str());
        int srcWidth = srcImage.GetWidth();
        int srcHeight = srcImage.GetHeight();
        Gdiplus::Image* pVertImage = new Gdiplus::Bitmap(srcWidth, srcHeight);
        if (NULL != pVertImage)
        {
            FilpImage(srcImage, *pVertImage, STYLE_Y);
            DestoryImage();
            SetWindowImage(*pVertImage);
            delete pVertImage;
            pVertImage = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnClickedBtnClone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_imagePath.empty())
    {
        Gdiplus::Image srcImage(m_imagePath.c_str());
        int srcHeight = srcImage.GetHeight();
        int srcWidth = srcImage.GetWidth();
        Gdiplus::Image* pCloneImage = new Gdiplus::Bitmap(srcWidth, srcHeight);
        if (NULL != pCloneImage)
        {
            CloneImage(srcImage, *pCloneImage);
            DestoryImage();
            SetWindowImage(*pCloneImage);
            delete pCloneImage;
            pCloneImage = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnClickedBtnCast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_imagePath.empty())
    {
        Gdiplus::Image srcImage(m_imagePath.c_str());
        int srcHeight = srcImage.GetHeight();
        int srcWidth = srcImage.GetWidth();
        Gdiplus::Image* pCastImage = new Gdiplus::Bitmap(srcWidth * 2, srcHeight); 
        if (NULL != pCastImage)
        {
            CastImage(srcImage, *pCastImage);
            DestoryImage();
            SetWindowImage(*pCastImage);
            delete pCastImage;
            pCastImage = NULL;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

void CMainDlg::ShowImage(const std::wstring& srcImagePath)
{
    CWindow wndPic = GetDlgItem(IDC_PIC);
    CDC pDc(wndPic.GetDC());
    Gdiplus::Graphics grapWindow(pDc.m_hDC);
    Gdiplus::Image srcImage(srcImagePath.c_str());

    CRect windowRect;
    CRect imageRect;
    wndPic.GetClientRect(&windowRect);
    int srcHeight = srcImage.GetHeight();
    int srcWidth = srcImage.GetWidth();
    double xScale = (double)windowRect.Width() / (double)srcWidth;
    double yScale = (double)windowRect.Height() / (double)srcHeight;
    double scaleIndex = (xScale <= yScale ? xScale : yScale);
    imageRect = CRect(windowRect.TopLeft(), CSize((int)(srcWidth * scaleIndex), (int)(srcHeight * scaleIndex)));
    grapWindow.DrawImage(&srcImage, 0, 0, imageRect.Width(), imageRect.Height());
    ReleaseDC(pDc);
}

void CMainDlg::DestoryImage()
{
    CWindow wndPic = GetDlgItem(IDC_PIC);
    CRect windowRect;
    wndPic.GetClientRect(&windowRect);
    CDC pDc(wndPic.GetDC());
    pDc.FillSolidRect(windowRect.left, windowRect.top, windowRect.Width(), windowRect.Height(), RGB(240, 240, 240));
    ReleaseDC(pDc);
}

void CMainDlg::ZoomImage(Gdiplus::Image& srcImage, Gdiplus::Image& zoomImage, double fFator)
{
    int srcWidth = srcImage.GetWidth();
    int srcHeight = srcImage.GetHeight();
    int zoomWidth = (int)(srcWidth * fFator);
    int zoomHeight = (int)(srcHeight * fFator);
    Gdiplus::Graphics grapImage(&zoomImage);
    Gdiplus::Image* pTempImage = srcImage.Clone();
    if (NULL != pTempImage)
    {
        Gdiplus::Matrix matrix;
        grapImage.GetTransform(&matrix);
        grapImage.DrawImage(pTempImage, Gdiplus::Rect(0, 0, (int)zoomWidth, (int)zoomHeight));
        delete pTempImage;
        pTempImage = NULL;
    }
}

void CMainDlg::RotateImage(Gdiplus::Image& srcImage, Gdiplus::Image& roateImage, int nRoate)
{
    int srcWidth = srcImage.GetWidth();
    int srcHeight = srcImage.GetHeight();
    double fRadian = nRoate * 3.1415926 / 180;
    double fCos = cos(fRadian);
    double fSin = sin(fRadian);
    int roateWidth = (int)(max(abs(srcWidth * fCos - srcHeight * fSin), abs(srcWidth * fCos + srcHeight * fSin)));
    int roateHeight = (int)(max(abs(srcWidth * fSin - srcHeight * fCos), abs(srcWidth * fSin + srcHeight * fCos)));

    Gdiplus::Graphics grapImage(&roateImage);
    Gdiplus::Point* centerPoint = new Gdiplus::Point(roateWidth / 2, roateHeight / 2);
    if (NULL != centerPoint)
    {
        grapImage.TranslateTransform((Gdiplus::REAL)centerPoint->X, (Gdiplus::REAL)centerPoint->Y);
        grapImage.RotateTransform((Gdiplus::REAL)nRoate);
        grapImage.TranslateTransform(-(Gdiplus::REAL)centerPoint->X, -(Gdiplus::REAL)centerPoint->Y);
        Gdiplus::Point* roatePoint = new Gdiplus::Point((roateWidth - srcWidth) / 2, (roateHeight - srcHeight) / 2);
        if (NULL != roatePoint)
        {
            grapImage.DrawImage(&srcImage, roatePoint->X, roatePoint->Y, srcWidth, srcHeight);
            // 重至绘图的所有变换
            grapImage.ResetTransform();
            grapImage.Save();
            delete roatePoint;
            roatePoint = NULL;
        }
        delete centerPoint;
        centerPoint = NULL;
    }
}

void CMainDlg::FilpImage(Gdiplus::Image& srcImage, Gdiplus::Image& filpImage, int nStyle)
{
    int srcWidth = srcImage.GetWidth();
    int srcHeight = srcImage.GetHeight();
    Gdiplus::Graphics grapImage(&filpImage);
    Gdiplus::Image* pTempImage = srcImage.Clone();
    if (NULL != pTempImage)
    {
        switch (nStyle)
        {
            case STYLE_X:
                pTempImage->RotateFlip(Gdiplus::RotateNoneFlipX);
                break;
            case STYLE_Y:
                pTempImage->RotateFlip(Gdiplus::RotateNoneFlipY);
                break;
            case STYLE_XY:
                pTempImage->RotateFlip(Gdiplus::RotateNoneFlipXY);
                break;
            default:
                break;
        }
        Gdiplus::Matrix matrix;
        grapImage.GetTransform(&matrix);
        grapImage.DrawImage(pTempImage, Gdiplus::Rect(0, 0, srcWidth, srcHeight));
        delete pTempImage;
        pTempImage = NULL;
    }
}

void CMainDlg::CloneImage(Gdiplus::Image& srcImage, Gdiplus::Image& cloneImage)
{
    int srcHeight = srcImage.GetHeight();
    int srcWidth = srcImage.GetWidth();
    // 实现局部克隆
    Gdiplus::Bitmap* srcTempImage = new Gdiplus::Bitmap(srcWidth, srcHeight);
    if (NULL != srcTempImage)
    {
        srcTempImage = (Gdiplus::Bitmap*)&srcImage;
        Gdiplus::Rect rect[4];
        rect[0] = Gdiplus::Rect(0, 0, srcWidth / 2, srcHeight / 2);
        rect[1] = Gdiplus::Rect(srcWidth / 2, 0, srcWidth / 2, srcHeight / 2);
        rect[2] = Gdiplus::Rect(0, srcHeight / 2, srcWidth / 2, srcHeight / 2);
        rect[3] = Gdiplus::Rect(srcWidth / 2, srcHeight / 2, srcWidth / 2, srcHeight / 2);

        // 把原图像分为分四块克隆
        Gdiplus::Bitmap* scrBitmap[4];
        scrBitmap[0] = srcTempImage->Clone(rect[0], PixelFormatDontCare);
        scrBitmap[1] = srcTempImage->Clone(rect[1], PixelFormatDontCare);
        scrBitmap[2] = srcTempImage->Clone(rect[2], PixelFormatDontCare);
        scrBitmap[3] = srcTempImage->Clone(rect[3], PixelFormatDontCare);

        // 绘图
        Gdiplus::Graphics grapImage(&cloneImage);
        grapImage.DrawImage(scrBitmap[0], Gdiplus::Rect(0, 0, srcWidth / 2, srcHeight / 2));
        grapImage.DrawImage(scrBitmap[1], Gdiplus::Rect(srcWidth / 2 + 10, 0, srcWidth / 2, srcHeight / 2));
        grapImage.DrawImage(scrBitmap[2], Gdiplus::Rect(0, srcHeight / 2 + 10, srcWidth / 2, srcHeight / 2));
        grapImage.DrawImage(scrBitmap[3], Gdiplus::Rect(srcWidth / 2 + 10, srcHeight / 2 + 10, srcWidth / 2, srcHeight / 2));
        grapImage.ResetTransform();
        grapImage.Save();
    }
}

void CMainDlg::CastImage(Gdiplus::Image& srcImage, Gdiplus::Image& castImage)
{
    int castLeft = 0;
    int CastTop = 700;
    int srcHeight = srcImage.GetHeight();
    int srcWidth = srcImage.GetWidth();
    Gdiplus::Graphics grapIamge(&castImage);
    grapIamge.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);

    // 正面投射
    Gdiplus::Point desFace[] = {
        Gdiplus::Point(castLeft, CastTop),
        Gdiplus::Point(castLeft + srcWidth, CastTop),
        Gdiplus::Point(castLeft, CastTop + srcWidth),
    };
    grapIamge.DrawImage(&srcImage, desFace, 3);

    // 上面投射
    Gdiplus::Point destTop[] = {
        Gdiplus::Point(castLeft + srcWidth / 2, CastTop - srcWidth / 2),
        Gdiplus::Point(castLeft + srcWidth / 2 + srcWidth, CastTop - srcWidth / 2),
        Gdiplus::Point(castLeft, CastTop),
    };
    grapIamge.DrawImage(&srcImage, destTop, 3);

    // 侧面投射
    Gdiplus::Point desRight[] = {
        Gdiplus::Point(castLeft + srcWidth, CastTop),
        Gdiplus::Point(castLeft + srcWidth / 2 + srcWidth, CastTop - srcWidth / 2),
        Gdiplus::Point(castLeft + srcWidth, CastTop + srcWidth)
    };
    grapIamge.DrawImage(&srcImage, desRight, 3);
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
    std::wstring editText;
    ::GetDlgItemText(m_hWnd, IDC_EDIT_FATOR, (LPWSTR)editText.c_str(), editText.max_size());
    std::string editTextBuf = ComFunction::ConvertWstringToAnsi(editText);
    bool isNumber = ComFunction::IsFloatNumber(editTextBuf);
    if (!isNumber)
    {
        TCHAR cTemp[1024] = {0};
        ::swprintf_s(cTemp, _T("请输入一个有效的浮点型数字！"), MIN_ROTAT, MAX_ROTAT);
        std::wstring wstrTitle(cTemp);
        if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
        {
            GetDlgItem(IDC_EDIT_FATOR).SetWindowTextW(_T(""));
            GetDlgItem(IDC_EDIT_FATOR).SetFocus();
        }
    }
    double dfator = _wtof(editText.c_str());
    if (dfator > 0)
    {
        if (dfator > MAX_FATOR || dfator < MIN_FATOR)
        {
            TCHAR cTemp[1024] = {0};
            ::swprintf_s(cTemp, _T("输入的系数必须在%3.1f~%3.1f之间!"), MIN_FATOR, MAX_FATOR);
            std::wstring editTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)editTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
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
    std::wstring editText;
    ::GetDlgItemText(m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)editText.c_str(), editText.max_size());
    std::string editTextBuf = ComFunction::ConvertWstringToAnsi(editText);
    bool isNumber = ComFunction::IsIntNumber(editTextBuf);
    if (!isNumber)
    {
        TCHAR cTemp[1024] = {0};
        swprintf_s(cTemp, _T("请输入一个有效的整型数字！"), MIN_ROTAT, MAX_ROTAT);
        std::wstring wstrTitle(cTemp);
        if (::MessageBox(m_hWnd, (LPWSTR)wstrTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
        {
            GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T(""));
            GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
        }
    }
    int nRotar = _wtoi(editText.c_str());
    if (nRotar > 0)
    {
        if (nRotar > MAX_ROTAT || nRotar < MIN_ROTAT)
        {
            TCHAR cTemp[1024] = {0};
            ::swprintf_s(cTemp, _T("输入的角度必须在%d~%d之间!"), MIN_ROTAT, MAX_ROTAT);
            std::wstring editTitle(cTemp);
            if (::MessageBox(m_hWnd, (LPWSTR)editTitle.c_str(), _T("Tips"), MB_OK) == IDOK)
            {
                GetDlgItem(IDC_EDIT_ROTATE).SetWindowTextW(_T(""));
                GetDlgItem(IDC_EDIT_ROTATE).SetFocus();
            }
        }
    }
    return 0;
}

LRESULT CMainDlg::OnClickedBtnPrepage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_fileNameAry.clear();
    if (!m_imagePath.empty())
    {
        std::wstring filePath = ComFunction::GetFilePath(m_imagePath);
        std::string filePathBuf = ComFunction::ConvertWstringToAnsi(filePath);
        GetAllFormatFiles(filePathBuf, m_fileNameAry, ".jpg");
        if (m_fileNameAry.size() > 0)
        {
            for (size_t i = 0; i < m_fileNameAry.size(); i++)
            {
                std::string imagePath = ComFunction::ConvertWstringToAnsi(m_imagePath);
                if (imagePath == m_fileNameAry[i])
                {
                    m_iCurNumber = i;
                }
            }
            std::wstring curImagePath;
            std::string curImagePathBuf;
            if ((m_iCurNumber - 1) < 0)
            {
                curImagePathBuf = m_fileNameAry[m_fileNameAry.size() - 1];
                curImagePath = ComFunction::ConvertAnsiToWstring(curImagePathBuf);
            }
            else
            {
                curImagePathBuf = m_fileNameAry[m_iCurNumber - 1];
                curImagePath = ComFunction::ConvertAnsiToWstring(curImagePathBuf);
            }
            // 更新全局图片路径
            m_imagePath = curImagePath;
            DestoryImage();
            ShowImage(m_imagePath);
            // 初始化缩放因子
            m_dFactor = 1.0;
            m_nRoate = 0;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

LRESULT CMainDlg::OnClickedBtnNextpage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    m_fileNameAry.clear();
    if (!m_imagePath.empty())
    {
        std::wstring filePath = ComFunction::GetFilePath(m_imagePath);
        std::string filePathBuf = ComFunction::ConvertWstringToAnsi(filePath);
        GetAllFormatFiles(filePathBuf, m_fileNameAry, ".jpg");
        if (m_fileNameAry.size() > 0)
        {
            for (size_t i = 0; i < m_fileNameAry.size(); i++)
            {
                std::string imagePath = ComFunction::ConvertWstringToAnsi(m_imagePath);
                if (imagePath == m_fileNameAry[i])
                {
                    m_iCurNumber = i;
                }
            }
            std::wstring curImagePath;
            std::string curImagePathBuf;
            if ((m_iCurNumber + 1) >= m_fileNameAry.size())
            {
                curImagePathBuf = m_fileNameAry[0];
                curImagePath = ComFunction::ConvertAnsiToWstring(curImagePathBuf);
            }
            else
            {
                curImagePathBuf = m_fileNameAry[m_iCurNumber + 1];
                curImagePath = ComFunction::ConvertAnsiToWstring(curImagePathBuf);
            }
            // 更新全局图片路径
            m_imagePath = curImagePath;
            DestoryImage();
            ShowImage(m_imagePath);
            // 初始化缩放因子
            m_dFactor = 1.0;
            m_nRoate = 0;
        }
    }
    else
    {
        ::MessageBox(m_hWnd, _T("请先插入图片"), _T("Tips"), MB_OK);
    }
    return 0;
}

void CMainDlg::GetAllFormatFiles(const std::string& filePath, std::vector<std::string>& imagePath, const std::string& format)
{
    // 文件句柄    
    long hFile = 0;
    // 文件信息    
    ::_finddata_t fileInfo;
    std::string tempFliePath;
    if ((hFile = ::_findfirst(tempFliePath.assign(filePath).append("\\*" + format).c_str(), &fileInfo)) != -1)
    {
        do
        {
            if ((fileInfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileInfo.name, ".") != 0 && strcmp(fileInfo.name, "..") != 0)
                {
                    GetAllFormatFiles(tempFliePath.assign(filePath).append("\\").append(fileInfo.name), imagePath, format);
                }
            }
            else
            {
                imagePath.push_back(tempFliePath.assign(filePath).append("\\").append(fileInfo.name));
            }
        } 
        while (::_findnext(hFile, &fileInfo) == 0);
        ::_findclose(hFile);
    }
}

void CMainDlg::SetWindowImage(Gdiplus::Image& objImage, double dFator, bool zoomEnable)
{
    double dFatorBuf = 1.0;
    if (zoomEnable)
    {
        dFatorBuf = dFator;
    }
    CRect windowRect;
    CWindow wndPic = GetDlgItem(IDC_PIC);
    wndPic.GetClientRect(&windowRect);
    CDC pDc(wndPic.GetDC());
    Gdiplus::Graphics grapWindow(pDc.m_hDC);
    //防止图片失真
    SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

    CRect imageRect;
    int imageHeight = objImage.GetHeight();
    int imageWidth = objImage.GetWidth();
    // 图片的尺寸小于图片控件尺寸
    if (imageWidth <= windowRect.Width() && imageHeight <= windowRect.Height())
    {
        imageRect = CRect(windowRect.TopLeft(), CSize((int)(imageWidth * dFatorBuf), (int)(imageWidth * dFatorBuf)));
        grapWindow.DrawImage(&objImage, imageRect.top, imageRect.left, imageRect.Width(), imageRect.Height());
    }
    // 图片的尺寸大于图片控件的尺寸 
    else
    {
        double xScale = (double)windowRect.Width() / (double)imageWidth;
        double yScale = (double)windowRect.Height() / (double)imageHeight;
        double scaleIndex = (xScale <= yScale ? xScale : yScale);
        imageRect = CRect(windowRect.TopLeft(), CSize((int)(imageWidth * scaleIndex * dFatorBuf), (int)(imageHeight * scaleIndex * dFatorBuf)));
        grapWindow.DrawImage(&objImage, imageRect.top, imageRect.left, imageRect.Width(), imageRect.Height());
        ReleaseDC(pDc);
    }
}
