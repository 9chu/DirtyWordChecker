/*
 * \file DirtyWordChecker.h
 * \detail 脏词检测逻辑
 * \date 2015/11/13
 */
#pragma once
#include <cstdlib>

struct TrieNode;

/// \brief 脏词检测器
class DirtyWordChecker
{
private:
    bool m_bWholeWordMode;
    TrieNode* m_pRootTrieNode;
public:
    DirtyWordChecker(bool bWholeWordMode=false);
    ~DirtyWordChecker();
private:  // non-copyable
    DirtyWordChecker(const DirtyWordChecker&);
    DirtyWordChecker& operator=(const DirtyWordChecker&);
public:
    /// \brief 是否为全字匹配模式
    bool IsWholeWordMode()const { return m_bWholeWordMode; }

    /// \brief 设置全字匹配模式
    void SetWholeWordMode(bool bWholeWordMode) { m_bWholeWordMode = bWholeWordMode; }

    /// \brief 追加脏词
    /// \param[in] pszWord 脏词
    /// \return 若内存分配失败或字符串为空则返回false，否则返回true
    bool AddWord(const char* pszWord);

    /// \brief 检查是否存在脏词
    /// \note 函数仅检查首个脏词词汇
    /// \param[in] pszContent 需要检查的字符串
    /// \return 若有脏词，返回true，否则返回false
    bool CheckDirtyWord(const char* pszContent)const;

    /// \brief 替换脏词
    /// \note 函数会替换所有可能的脏词
    /// \param[in] pszContent 需要检查的字符串
    /// \param[in] cReplaceChar 用于替换的字符
    /// \return 若发生替换则返回true，否则返回false
    bool ReplaceDirtyWord(char* pszContent, char cReplaceChar='*')const;
};
