

class ComFunction
{
public:
    ComFunction();
    ~ComFunction();

    /** ���ַ�ת�ַ�
    @param[in] srcWidBytes ���ַ���
    */
    static std::string ConvertWstringToAnsi(const std::wstring& srcWidBytes);

    /** �ַ�ת���ַ�
    @param[in] srcBytes �ַ���
    */
    static std::wstring ConvertAnsiToWstring(const std::string& srcBytes);

    /** ��鸡������
    @param[in] strNumber �ַ���
    */
    static bool IsFloatNumber(const std::string& strNumber);

    /** �����������
    @param[in] strNumber �ַ���
    */
    static bool IsIntNumber(const std::string& strNumber);

    /** ����ļ���·��
    @param[in] strFullPathName �ļ������ļ�·��
    */
    static std::wstring GetFilePath(std::wstring& fullPathName);
};

