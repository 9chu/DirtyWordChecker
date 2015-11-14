#include "DirtyWordChecker.h"

#include <cstring>

using namespace std;

struct TrieNode
{
    TrieNode* arrNext[255];  // 节省'\0'的空间
    bool bTerminate;  // 当前节点是否为一个单词的词尾
};

static TrieNode* CreateTrieNode()
{
    TrieNode* pRet = static_cast<TrieNode*>(::malloc(sizeof(TrieNode)));
    if (pRet)
    {
        ::memset(&pRet->arrNext, 0, sizeof(pRet->arrNext));
        pRet->bTerminate = false;
    }
    return pRet;
}

static void DestroyTrieNode(TrieNode* pNode)
{
    if (pNode)
    {
        for (size_t i = 0; i < 255; ++i)
            DestroyTrieNode(pNode->arrNext[i]);
        ::free(pNode);
    }
}

/// \brief 匹配一个脏词前缀
/// \param[in] pNode 脏词节点
/// \param[in] pszStart 待匹配字符串
/// \param[in] bLongestMatch 最长匹配模式
/// \param[out] iLengthOut 输出长度
/// \return 若存在匹配返回true，否则返回false
static bool MatchDirtyWord(TrieNode* pNode, const char* pszStart, bool bLongestMatch, size_t& iLengthOut)
{
    iLengthOut = 0;
    if (!pNode || !pszStart)
        return false;

    size_t i = 0;
    unsigned char c;
    bool bMatch = false;
    while ((c = static_cast<unsigned char>(pszStart[i++])) != '\0')
    {
        // 忽略大小写，统一转换为小写字符
        if (c >= 'A' && c <= 'Z')
            c = c - 'A' + 'a';

        pNode = pNode->arrNext[c - 1];
        if (!pNode)
            return bMatch;
        else if (pNode->bTerminate)
        {
            bMatch = true;
            iLengthOut = i;
            if (!bLongestMatch)
                return true;
        }
    }
    return bMatch;
}

/// \brief 检查一个字符是否为符号或者空白符
static bool CheckIfSymbol(char c)
{
    // UTF-8编码与ASCII相兼容
    if (c > '\0' && c < '0')
        return true;
    else if (c > '9' && c < 'A')
        return true;
    else if (c > 'Z' && c < 'a')
        return true;
    else if (c > 'z' && c <= 127)
        return true;
    return false;
}

DirtyWordChecker::DirtyWordChecker(bool bWholeWordMode)
    : m_bWholeWordMode(bWholeWordMode), m_pRootTrieNode(NULL)
{
    m_pRootTrieNode = CreateTrieNode();
}

DirtyWordChecker::~DirtyWordChecker()
{
    DestroyTrieNode(m_pRootTrieNode);
}

bool DirtyWordChecker::AddWord(const char* pszWord)
{
    if (!pszWord || *pszWord == '\0' || !m_pRootTrieNode)
        return false;

    unsigned char c;
    TrieNode* pNode = m_pRootTrieNode;
    while ((c  = static_cast<unsigned char>(*pszWord++)) != '\0')
    {
        // 忽略大小写，统一转换为小写字符
        if (c >= 'A' && c <= 'Z')
            c = c - 'A' + 'a';

        // 构造节点
        TrieNode*& pNextNode = pNode->arrNext[c - 1];
        if (!pNextNode)
        {
            if (!(pNextNode = CreateTrieNode()))
                return false;  // memory allocate failed
        }

        pNode = pNextNode;
    }

    pNode->bTerminate = true;
    return true;
}

bool DirtyWordChecker::CheckDirtyWord(const char* pszContent)const  // 预计复杂度O(nm)
{
    if (!pszContent || !m_pRootTrieNode)
        return false;

    while (true)
    {
        if (m_bWholeWordMode)  // 全字匹配模式下跳过符号字符
        {
            while (CheckIfSymbol(*pszContent))
                ++pszContent;
        }

        if (*pszContent == '\0')
            return false;

        size_t iLengthOut;
        if (MatchDirtyWord(m_pRootTrieNode, pszContent, m_bWholeWordMode, iLengthOut))  // 全字匹配模式下需要匹配最长串
        {
            if (!m_bWholeWordMode)
                return true;
            else
            {
                // 全字匹配模式下需要检查后继字符是否为符号
                if (pszContent[iLengthOut] == '\0' || CheckIfSymbol(pszContent[iLengthOut]))
                    return true;
            }
        }

        if (!m_bWholeWordMode)
            ++pszContent;
        else
        {
            // 全字匹配模式下若不能找到一个匹配，则跳过所有字符直到符号
            while (*pszContent != '\0' && !CheckIfSymbol(*pszContent))
                ++pszContent;
        }
    }
}

bool DirtyWordChecker::ReplaceDirtyWord(char* pszContent, char cReplaceChar)const
{
    if (!pszContent || !m_pRootTrieNode)
        return false;

    bool bReplaced = false;
    while (true)
    {
        if (m_bWholeWordMode)  // 全字匹配模式下跳过符号字符
        {
            while (CheckIfSymbol(*pszContent))
                ++pszContent;
        }

        if (*pszContent == '\0')
            return bReplaced;

        size_t iLengthOut;
        bool bShouldReplace = false;
        if (MatchDirtyWord(m_pRootTrieNode, pszContent, true, iLengthOut))  // 替换模式下需要匹配最长串
        {
            if (!m_bWholeWordMode)
                bShouldReplace = true;
            else
            {
                // 全字匹配模式下需要检查后继字符是否为符号
                if (pszContent[iLengthOut] == '\0' || CheckIfSymbol(pszContent[iLengthOut]))
                    bShouldReplace = true;
            }

            if (bShouldReplace)
            {
                for (size_t i = 0; i < iLengthOut; ++i)
                    pszContent[i] = cReplaceChar;

                bReplaced = true;
                pszContent += iLengthOut;
            }
        }

        if (!bShouldReplace)
        {
            if (!m_bWholeWordMode)
                ++pszContent;
            else
            {
                // 全字匹配模式下若不能找到一个匹配，则跳过所有字符直到符号
                while (*pszContent != '\0' && !CheckIfSymbol(*pszContent))
                    ++pszContent;
            }
        }
    }
}
