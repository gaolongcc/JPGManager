


#include "stdafx.h"
#include "MainDlg.h"
#include <atldlgs.h>
#include <atltypes.h>
#include <atlstr.h>
#include <math.h>
#include <comutil.h>
//using namespace Gdiplus;
//#pragma comment(lib,"gdiplus.lib")
//缩放因子限值
#define MAX_FATOR   1.0
#define MIN_FATOR   0.1


LRESULT CMainDlg::OnBnClickedBtninsert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{

    DestoryPicture();
    //清空图片路径
    m_wstrImagePath = _T("");
    //弹出对话框
    TCHAR strFileFilters[1024] = _T("JPG Files(*.jpg)\0*.jpg\0\0");
    CFileDialog selImageDialog(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFileFilters, this->m_hWnd);
    TCHAR temp[1024];
    ZeroMemory(temp, sizeof(TCHAR) * 1024);
    selImageDialog.m_ofn.lpstrFile = temp;
    selImageDialog.m_ofn.nMaxFile = 1024;
    m_wstrImagePath = _T("");
    if (IDOK == selImageDialog.DoModal())
    {
        m_wstrImagePath = temp;
    }
    //判断图片路径是否为空
    if (!m_wstrImagePath.empty())
    {
    
        ShowPicture(m_wstrImagePath);
        //初始化缩放因子
        m_fFactor = 1.0;
        //初始化旋转因子
        m_fRoate = 0.0;
    }
    else
    {
        ::MessageBox(NULL, _T("请选择需要插入的图片！"), _T("Tips"), MB_OK);
    }
    return 0;
}


LRESULT CMainDlg::OnBnClickedBtndelete(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add your control notification handler code here
    DestoryPicture();
    //清空图片路径
    m_wstrImagePath = _T("");
    return 0;
}


LRESULT CMainDlg::OnBnClickedBtnpus(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    //判断图片路径是否为空
    if (!m_wstrImagePath.empty())
    {
        //放大
        if (m_fFactor < MAX_FATOR)
        {
            std::wstring wstrTemp = _T("");
            float fTemp = m_fFactor;
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT_FATOR, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
            float nfator = _wtof(wstrTemp.c_str());
            m_fFactor = m_fFactor + nfator;
            if (m_fFactor > MAX_FATOR)
            {
                m_fFactor = fTemp;
                //return 0;
            }
            DestoryPicture();
            ZoomPicture(m_wstrImagePath, m_fFactor);
        }
        else
        {
            ::MessageBox(NULL, _T("以当前比例无法在继续放大！"), _T("Tips"), MB_OK);
            return 0;
        }
    }
    return 0;
}



LRESULT CMainDlg::OnBnClickedBtndec(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    //判断图片路径是否为空
    if (!m_wstrImagePath.empty())
    {
        //缩小
        if (m_fFactor > MIN_FATOR)
        {
            std::wstring wstrTemp = _T("");
            float fTemp = m_fFactor;
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT_FATOR, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
            float nfator = _wtof(wstrTemp.c_str());
            m_fFactor = m_fFactor - nfator;
            if (m_fFactor <= MIN_FATOR)
            {
                m_fFactor = fTemp;
                ::MessageBox(NULL, _T("以当前比例无法在继续缩小！"), _T("Tips"), MB_OK);
                return 0;
            }
            DestoryPicture();
            ZoomPicture(m_wstrImagePath, m_fFactor);
        }
        else
        {
            return 0;
        }
    }
    return 0;
}


LRESULT CMainDlg::OnBnClickedBtnnagtive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //逆时针
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            std::wstring wstrTemp = _T("");
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
            int nRotar = _wtoi(wstrTemp.c_str());
            m_fRoate = m_fRoate - nRotar;
            //
            Gdiplus::Image *roteimage = RotateImage(image, m_fRoate);
            //清空
            DestoryPicture();  
            CRect rect;
            CRect roterect;
            //得到图片高度 
            int height = roteimage->GetHeight();
            int width = roteimage->GetWidth();
            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //获得pictrue控件所在的矩形区域
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //防止失真
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

            //如果图片的尺寸小于图片控件尺寸，则不缩放显示  
            if (width <= rect.Width() && height <= rect.Width())
            {
                roterect = CRect(rect.TopLeft(), CSize(width, height));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
            }
            //如果图片的尺寸大于图片控件的尺寸 
            else
            {
                //求Y方向的缩放因子 
                float xScale = (float)rect.Width() / (float)width;    
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                roterect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}


LRESULT CMainDlg::OnBnClickedBtnpostive(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //逆时针
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            std::wstring wstrTemp = _T("");
            ::GetDlgItemText(this->m_hWnd, IDC_EDIT_ROTATE, (LPWSTR)wstrTemp.c_str(), wstrTemp.max_size());
            int nRotar = _wtoi(wstrTemp.c_str());
            m_fRoate = m_fRoate + nRotar;
            Gdiplus::Image *roteimage = RotateImage(image, m_fRoate);
            //清空
            DestoryPicture();
            CRect rect;
            CRect roterect;
            //得到图片高度 
            int height = roteimage->GetHeight();
            //得到图片宽度 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //获得pictrue控件所在的矩形区域
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //防止失真
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //如果图片的尺寸小于图片控件尺寸，则不缩放显示  
            if (width <= rect.Width() && height <= rect.Width())
            {
                roterect = CRect(rect.TopLeft(), CSize(width, height));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
          
            }
            //如果图片的尺寸大于图片控件的尺寸 
            else
            {
                //求Y方向的缩放因子
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                roterect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, roterect.top, roterect.left, roterect.Width(), roterect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}
LRESULT CMainDlg::OnBnClickedBtnhoriz(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //水平
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            Gdiplus::Image *roteimage = FilpImageX(image);
            //清空
            DestoryPicture();
            CRect rect;
            CRect filprect;
            //得到图片高度 
            int height = roteimage->GetHeight();
            //得到图片宽度 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //获得pictrue控件所在的矩形区域
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //防止失真
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //如果图片的尺寸小于图片控件尺寸，则不缩放显示  
            if (width <= rect.Width() && height <= rect.Width())
            {
                filprect = CRect(rect.TopLeft(), CSize(width, height));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());

            }
            //如果图片的尺寸大于图片控件的尺寸 
            else
            {
                //求Y方向的缩放因子
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                filprect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;

}

LRESULT CMainDlg::OnBnClickedBtnvert(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //垂直
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            Gdiplus::Image *roteimage = FilpImageY(image);
            //清空
            DestoryPicture();
            CRect rect;
            CRect filprect;
            //得到图片高度 
            int height = roteimage->GetHeight();
            //得到图片宽度 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //获得pictrue控件所在的矩形区域
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //防止失真
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //如果图片的尺寸小于图片控件尺寸，则不缩放显示  
            if (width <= rect.Width() && height <= rect.Width())
            {
                filprect = CRect(rect.TopLeft(), CSize(width, height));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());

            }
            //如果图片的尺寸大于图片控件的尺寸 
            else
            {
                //求Y方向的缩放因子
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                filprect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}

LRESULT CMainDlg::OnBnClickedBtnclone(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //垂直
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            Gdiplus::Image *roteimage = CloneImage(image);
            //清空
            DestoryPicture();
            CRect rect;
            CRect filprect;
            //得到图片高度 
            int height = roteimage->GetHeight();
            //得到图片宽度 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //获得pictrue控件所在的矩形区域
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //防止失真
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //如果图片的尺寸小于图片控件尺寸，则不缩放显示  
            if (width <= rect.Width() && height <= rect.Width())
            {
                filprect = CRect(rect.TopLeft(), CSize(width, height));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());

            }
            //如果图片的尺寸大于图片控件的尺寸 
            else
            {
                //求Y方向的缩放因子
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                filprect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
    }
    return 0;
}
LRESULT CMainDlg::OnBnClickedBtncast(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    if (!m_wstrImagePath.empty())
    {
        //折射
        Gdiplus::Image *image = new Gdiplus::Image(m_wstrImagePath.c_str());
        //Gdiplus::Image image1;
        if (NULL != image)
        {
            Gdiplus::Image *roteimage = CastImage(image);
            //清空
            DestoryPicture();
            CRect rect;
            CRect filprect;
            //得到图片高度 
            int height = roteimage->GetHeight();
            //得到图片宽度 
            int width = roteimage->GetWidth();

            CWindow wnd = this->GetDlgItem(IDC_PIC);
            //获得pictrue控件所在的矩形区域
            wnd.GetClientRect(&rect);
            CDC pDc(wnd.GetDC());
            Gdiplus::Graphics graphics(pDc.m_hDC);
            //防止失真
            SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);
            //如果图片的尺寸小于图片控件尺寸，则不缩放显示  
            if (width <= rect.Width() && height <= rect.Width())
            {
                filprect = CRect(rect.TopLeft(), CSize(width, height));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());

            }
            //如果图片的尺寸大于图片控件的尺寸 
            else
            {
                //求Y方向的缩放因子
                float xScale = (float)rect.Width() / (float)width;
                float yScale = (float)rect.Height() / (float)height;
                float ScaleIndex = (xScale <= yScale ? xScale : yScale);
                filprect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
                //将图片画到Picture控件表示的矩形区域  
                graphics.DrawImage(roteimage, filprect.top, filprect.left, filprect.Width(), filprect.Height());
                ReleaseDC(pDc);
            }
        }
        delete image;
        image = nullptr;
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
    //求X方向的缩放因子  
    float xScale = (float)rect.Width() / (float)width;
    float yScale = (float)rect.Height() / (float)height;
    float ScaleIndex = (xScale <= yScale ? xScale : yScale);
    fatorect = CRect(rect.TopLeft(), CSize((int)width*ScaleIndex, (int)height*ScaleIndex));
    //将图片显示到控件上
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
    Gdiplus::Image * image = new Gdiplus::Image(pFilepath.c_str());
    int height = image->GetHeight();
    int width = image->GetWidth();
    CWindow wnd = this->GetDlgItem(IDC_PIC);
    wnd.GetClientRect(&rect);
    CDC pDc(wnd.GetDC());
    Gdiplus::Graphics graphics(pDc.m_hDC);
    //防止失真
    SetStretchBltMode(pDc.m_hDC, STRETCH_HALFTONE);

    //如果图片的尺寸小于图片控件尺寸，则不缩放显示  
    if (width <= rect.Width() && height <= rect.Width())
    {
        fatorrect = CRect(rect.TopLeft(), CSize(width * fFator, height * fFator));
        graphics.DrawImage(image, fatorrect.top, fatorrect.left, fatorrect.Width(), fatorrect.Height());
    }
    //如果图片的尺寸大于图片控件的尺寸 
    else
    {
        //求XY方向的缩放因子  
        float xScale = (float)rect.Width() / (float)width;
        float yScale = (float)rect.Height() / (float)height;
        float ScaleIndex = (xScale <= yScale ? xScale : yScale);
        fatorrect = CRect(rect.TopLeft(), CSize((int)width * ScaleIndex * fFator, (int)height * ScaleIndex * fFator));
        graphics.DrawImage(image, fatorrect.top, fatorrect.left, fatorrect.Width(), fatorrect.Height());
    }
    delete image;
    ReleaseDC(pDc);

}
Gdiplus::Image* CMainDlg::RotateImage(Gdiplus::Image* source,  float fRoate)
{
    int nAngle = (int)fRoate % 360;
    int srcWidth = source->GetWidth();
    int srcHeight = source->GetHeight();
    //图像旋转之后所占区域宽和高
    Gdiplus::Rect *rotateRec = GetRotateRectangle(srcWidth, srcHeight, nAngle);
    int rotateWidth = rotateRec->Width;
    int rotateHeight = rotateRec->Height;
    //定义画布，宽高为图像旋转后的宽高
    Gdiplus::Bitmap  *pImageScale = new Gdiplus::Bitmap(rotateWidth, rotateHeight);
    //graphics根据destImage创建，因此其原点此时在destImage左上角
    Gdiplus::Graphics graphics(pImageScale);
    //要让graphics围绕某矩形中心点旋转N度，分三步
    //第一步，将graphics坐标原点移到矩形中心点,假设其中点坐标（x,y）
    //第二步，graphics旋转相应的角度(沿当前原点)
    //第三步，移回（-x,-y）
    //获取画布中心点
    Gdiplus::Point *centerPoint = new Gdiplus::Point(rotateWidth / 2, rotateHeight / 2);
    //将graphics坐标原点移到中心点
    graphics.TranslateTransform(centerPoint->X, centerPoint->Y);
    //graphics旋转相应的角度(绕当前原点)
    graphics.RotateTransform(nAngle);
    //恢复graphics在水平和垂直方向的平移(沿当前原点)
    graphics.TranslateTransform(-centerPoint->X, -centerPoint->Y);
    //此时已经完成了graphics的旋转 
    //计算:如果要将源图像画到画布上且中心与画布中心重合，需要的偏移量
    Gdiplus::Point *Offset = new Gdiplus::Point((rotateWidth - srcWidth) / 2, (rotateHeight - srcHeight) / 2);
    //将源图片画到rect里（rotateRec的中心）
    //Gdiplus::TextureBrush brush(source);
    graphics.DrawImage(source,Offset->X,Offset->Y,srcWidth,srcHeight);
    //重至绘图的所有变换
    graphics.ResetTransform();
    graphics.Save();          
    return pImageScale;
}

Gdiplus::Rect* CMainDlg::GetRotateRectangle(int width, int height, float angle)
{
    float radian = angle * 3.1415926 / 180;;
    float fcos = cos(radian);
    float fsin = sin(radian);
    //只需要考虑到第四象限和第三象限的情况取大值(中间用绝对值就可以包括第一和第二象限)
    int newWidth = (int)(max(abs(width * fcos - height * fsin), abs(width * fcos + height * fsin)));
    int newHeight = (int)(max(abs(width * fsin - height * fcos), abs(width * fsin + height * fcos)));
    //返回旋转后的图片大小
    return new Gdiplus::Rect(0, 0, newWidth, newHeight);
}
Gdiplus::Image* CMainDlg::FilpImageX(Gdiplus::Image* source)
{
    int width = source->GetWidth();
    int height = source->GetHeight();
    Gdiplus::Matrix matrix;
    Gdiplus::Graphics graphics(source);
    Gdiplus::Image *Filpimge = source->Clone();
    Filpimge->RotateFlip(Gdiplus::RotateNoneFlipX);
    return Filpimge;
}
Gdiplus::Image* CMainDlg::FilpImageY(Gdiplus::Image* source)
{
    int width = source->GetWidth();
    int height = source->GetHeight();
    Gdiplus::Matrix matrix;
    Gdiplus::Graphics graphics(source);
    Gdiplus::Image *Filpimge = source->Clone();
    Filpimge->RotateFlip(Gdiplus::RotateNoneFlipY);
    return Filpimge;
}
Gdiplus::Image* CMainDlg::CloneImage(Gdiplus::Image* source)
{
    //Bitmap bmp(L"wlh.bmp");
    int height = source->GetHeight();
    int width = source->GetWidth();
    //Bitmap类可以实现局部克隆和整体克隆，而Image类只能整体克隆
    Gdiplus::Bitmap *scrBitTemp = new Gdiplus::Bitmap(width, height);
    Gdiplus::Image *scrBit = new Gdiplus::Bitmap(width, height);
    scrBitTemp = (Gdiplus::Bitmap*)source;
    Gdiplus::Rect rect[4];
    rect[0] = Gdiplus::Rect(0, 0, width / 2, height / 2);
    rect[1] = Gdiplus::Rect(width / 2, 0, width / 2, height / 2);
    rect[2] = Gdiplus::Rect(0, height / 2, width / 2, height / 2);
    rect[3] = Gdiplus::Rect(width / 2, height / 2, width / 2, height / 2);
    //把原图像分为分四块克隆  
    Gdiplus::Bitmap *scrBitmap[4];
    scrBitmap[0] = scrBitTemp->Clone(rect[0], PixelFormatDontCare);
    scrBitmap[1] = scrBitTemp->Clone(rect[1], PixelFormatDontCare);
    scrBitmap[2] = scrBitTemp->Clone(rect[2], PixelFormatDontCare);
    scrBitmap[3] = scrBitTemp->Clone(rect[3], PixelFormatDontCare);

    Gdiplus::Graphics graphics(scrBit);
    //绘图  
    graphics.DrawImage(scrBitmap[0], Gdiplus::Rect(0, 0, width / 2, height / 2));
    graphics.DrawImage(scrBitmap[1], Gdiplus::Rect(width / 2 + 10, 0, width / 2, height / 2));
    graphics.DrawImage(scrBitmap[2], Gdiplus::Rect(0, height / 2 + 10, width / 2, height / 2));
    graphics.DrawImage(scrBitmap[3], Gdiplus::Rect(width / 2 + 10, height / 2 + 10, width / 2, height / 2));
    graphics.ResetTransform();
    graphics.Save();

    //delete scrBitTemp;
    //scrBitTemp = nullptr;
    return scrBit;
}
Gdiplus::Image* CMainDlg::CastImage(Gdiplus::Image* source)
{
    int left = 0;
    int top = 700;
    int height = source->GetHeight();
    int width = source->GetWidth();
    Gdiplus::Image *scrImage = new Gdiplus::Bitmap(width*2, height);
    Gdiplus::Graphics graphics(scrImage);
    //
    //graphics.Clear(Gdiplus::Color::Green);
    //填充模式
    graphics.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic); 
    //正面  
    Gdiplus::Point desFace[] = {
        Gdiplus::Point(left, top),
        Gdiplus::Point(left + width, top),
        Gdiplus::Point(left, top + width),
    };
    graphics.DrawImage(source, desFace, 3);
    //上面投射  
    Gdiplus::Point destTop[] = {
        Gdiplus::Point(left + width / 2, top - width / 2),
        Gdiplus::Point(left + width / 2 + width, top - width / 2),
        Gdiplus::Point(left, top),
    };
    graphics.DrawImage(source, destTop, 3);
    //侧面投射  
    Gdiplus::Point desRight[] = {
        Gdiplus::Point(left + width, top),
        Gdiplus::Point(left + width / 2 + width, top - width / 2),
        Gdiplus::Point(left + width, top + width)
    };
    graphics.DrawImage(source, desRight, 3);

    return scrImage;
}








