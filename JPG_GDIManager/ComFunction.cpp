
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

// ��wstringת����string
std::string ComFunction::ConvertWStringToAnsi(const std::wstring& wstr)
{
    std::string result;
    std::wstring wstrTemp(wstr.c_str());
    int len = WideCharToMultiByte(CP_ACP, 0, wstrTemp.c_str(), wstrTemp.size(), NULL, 0, NULL, NULL);
    if (len <= 0)
    {
        return result;
    }
    char* buffer = new char[len + 1];
    if (buffer == NULL)
    {
        return result;
    }
    WideCharToMultiByte(CP_ACP, 0, wstrTemp.c_str(), wstrTemp.size(), buffer, len, NULL, NULL);
    // �ַ�����β
    buffer[len] = '\0';              
    result.append(buffer);           
    delete[] buffer;                  
    // ����ֵ
    return result;
}

// ��stringת����wstring
std::wstring ComFunction::ConvertAnsiToWString(const std::string& str)
{
    std::wstring result;
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    if (len < 0)
    {
        return result;
    }
    wchar_t* buffer = new wchar_t[len + 1];
    if (buffer == NULL)
    {
        return result;
    } 
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    //�ַ�����β
    buffer[len] = '\0';  
    //��ֵ
    result.append(buffer);  
    //ɾ��������
    delete[] buffer;                       
    //����ֵ
    return result;
}

std::wstring ComFunction::GetFilePath(std::wstring& strFullPathName)
{
    if (strFullPathName.empty())
    {
        return _T("");
    }
    std::string::size_type iPos = strFullPathName.find_last_of(_T('\\'));
    return strFullPathName.substr(0, iPos);
}

// �ж�������Ƿ�Ϊ��������
bool ComFunction::IsFloatNumber(const std::string &str)
{
    bool bflagPoint = true;
    if (str.size() > 8)
    {
        return false;
    }
    if (str.size() > 0)
    {
        if (str.c_str()[0] != '0')
        {
            for (size_t i = 0; i < str.size(); i++)
            {
                if (str.c_str()[i] == '.' && bflagPoint)
                {
                    bflagPoint = false;
                    continue;
                }
                if (str.c_str()[i] < '0' || str.c_str()[i] > '9')
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
            for (size_t i = 1; i < str.size(); i++)
            {
                if (str.c_str()[i] == '.' && bflagPoint)
                {
                    bflagPoint = false;
                    continue;
                }
                if (str.c_str()[i] < '0' || str.c_str()[i] > '9')
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

// �ж�������Ƿ�Ϊ��������
bool ComFunction::IsIntNumber(const std::string &str)
{
    if (str.size() > 8)
    {
        return false;
    }
    if (str.size() > 0)
    {
        for (size_t i = 0; i < str.size(); i++)
        {
            if (str.c_str()[i] < '0' || str.c_str()[i] > '9')
            {
                return false;
            }
        }
    }
    return true;
}