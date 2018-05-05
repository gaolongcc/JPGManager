
#include "stdafx.h"
#include <windows.h>
#include <string>
#include "ComFunction.h"

ComFunction::ComFunction()
{

}

ComFunction::~ComFunction()
{

}

std::string ComFunction::ConvertWstringToAnsi(const std::wstring& srcWidBytes)
{
    std::string objBytes;
    std::wstring widBytesBuf(srcWidBytes.c_str());
    int nWidBytesLen = WideCharToMultiByte(CP_ACP, 0, widBytesBuf.c_str(), widBytesBuf.size(), NULL, 0, NULL, NULL);
    if (nWidBytesLen <= 0)
    {
        return objBytes;
    }
    char* pBytesBuf = new char[nWidBytesLen + 1];
    if (pBytesBuf == NULL)
    {
        return objBytes;
    }
    WideCharToMultiByte(CP_ACP, 0, widBytesBuf.c_str(), widBytesBuf.size(), pBytesBuf, nWidBytesLen, NULL, NULL);
    pBytesBuf[nWidBytesLen] = '\0';
    objBytes.append(pBytesBuf);

    delete[] pBytesBuf;
    return objBytes;
}

std::wstring ComFunction::ConvertAnsiToWstring(const std::string& srcBytes)
{
    std::wstring objWidBytes;
    int nBytesLen = MultiByteToWideChar(CP_ACP, 0, srcBytes.c_str(), srcBytes.size(), NULL, 0);
    if (nBytesLen < 0)
    {
        return objWidBytes;
    }
    wchar_t* pWidBytesBuf = new wchar_t[nBytesLen + 1];
    if (pWidBytesBuf == NULL)
    {
        return objWidBytes;
    } 
    MultiByteToWideChar(CP_ACP, 0, srcBytes.c_str(), srcBytes.size(), pWidBytesBuf, nBytesLen);
    pWidBytesBuf[nBytesLen] = '\0';
    objWidBytes.append(pWidBytesBuf);

    delete[] pWidBytesBuf;
    return objWidBytes;
}

std::wstring ComFunction::GetFilePath(std::wstring& fullPathName)
{
    if (fullPathName.empty())
    {
        return _T("");
    }
    std::string::size_type iPos = fullPathName.find_last_of(_T('\\'));
    return fullPathName.substr(0, iPos);
}

bool ComFunction::IsFloatNumber(const std::string& strNumber)
{
    bool flagPoint = true;
    if (strNumber.size() > 8)
    {
        return false;
    }
    if (strNumber.size() > 0)
    {
        if (strNumber.c_str()[0] != '0')
        {
            for (size_t i = 0; i < strNumber.size(); i++)
            {
                if (strNumber.c_str()[i] == '.' && flagPoint)
                {
                    flagPoint = false;
                    continue;
                }
                if (strNumber.c_str()[i] < '0' || strNumber.c_str()[i] > '9')
                {
                    return false;
                }
                else
                {
                    continue;
                }
            }
        }
        else
        {
            for (size_t i = 1; i < strNumber.size(); i++)
            {
                if (strNumber.c_str()[i] == '.' && flagPoint)
                {
                    flagPoint = false;
                    continue;
                }
                if (strNumber.c_str()[i] < '0' || strNumber.c_str()[i] > '9')
                {
                    return false;
                }
                else
                {
                    continue;
                }
            }
        }     
    }
    return true;
}

// 判断输入的是否为整型数字
bool ComFunction::IsIntNumber(const std::string& strNumber)
{
    if (strNumber.size() > 8)
    {
        return false;
    }
    if (strNumber.size() > 0)
    {
        for (size_t i = 0; i < strNumber.size(); i++)
        {
            if (strNumber.c_str()[i] < '0' || strNumber.c_str()[i] > '9')
            {
                return false;
            }
        }
    }
    return true;
}