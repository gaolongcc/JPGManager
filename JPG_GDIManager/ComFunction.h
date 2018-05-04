

class ComFunction
{
public:
    ComFunction();
    ~ComFunction();

    /** ���ַ�ת�ַ�
    @param[in] wstr ���ַ��ַ���
    */
    static std::string ConvertWStringToAnsi(const std::wstring& wstr);

    /** �ַ�ת���ַ�
    @param[in] str �ַ��ַ���
    */
    static std::wstring ConvertAnsiToWString(const std::string& str);

    /** ��鸡������
    @param[in] str �ַ���
    */
    static bool IsFloatNumber(const std::string &str);

    /** �����������
    @param[in] str �ַ���
    */
    static bool IsIntNumber(const std::string &str);

    /** ����ļ���·��
    @param[in] strFullPathName �ļ������ļ�·��
    */
    static std::wstring GetFilePath(std::wstring& strFullPathName);
};

