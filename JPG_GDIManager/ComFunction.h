

class ComFunction
{
public:
    ComFunction();
    ~ComFunction();

    static std::string ConvertWStringToAnsi(const std::wstring& wstr);
    static std::wstring ConvertAnsiToWString(const std::string& str);
    static bool IsNumber(const std::string &str);
    static std::wstring GetFilePath(std::wstring& strFullPathName);
};

