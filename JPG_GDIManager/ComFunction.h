

class ComFunction
{
public:
    ComFunction();
    ~ComFunction();

    /** 宽字符转字符
    @param[in] wstr 宽字符字符串
    */
    static std::string ConvertWStringToAnsi(const std::wstring& wstr);

    /** 字符转宽字符
    @param[in] str 字符字符串
    */
    static std::wstring ConvertAnsiToWString(const std::string& str);

    /** 检查浮点数字
    @param[in] str 字符串
    */
    static bool IsFloatNumber(const std::string &str);

    /** 检查整型数字
    @param[in] str 字符串
    */
    static bool IsIntNumber(const std::string &str);

    /** 获得文件夹路径
    @param[in] strFullPathName 文件夹中文件路径
    */
    static std::wstring GetFilePath(std::wstring& strFullPathName);
};

