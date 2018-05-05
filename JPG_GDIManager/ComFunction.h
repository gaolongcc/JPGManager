

class ComFunction
{
public:
    ComFunction();
    ~ComFunction();

    /** 宽字符转字符
    @param[in] srcWidBytes 宽字符串
    */
    static std::string ConvertWstringToAnsi(const std::wstring& srcWidBytes);

    /** 字符转宽字符
    @param[in] srcBytes 字符串
    */
    static std::wstring ConvertAnsiToWstring(const std::string& srcBytes);

    /** 检查浮点数字
    @param[in] strNumber 字符串
    */
    static bool IsFloatNumber(const std::string& strNumber);

    /** 检查整型数字
    @param[in] strNumber 字符串
    */
    static bool IsIntNumber(const std::string& strNumber);

    /** 获得文件夹路径
    @param[in] strFullPathName 文件夹中文件路径
    */
    static std::wstring GetFilePath(std::wstring& fullPathName);
};

