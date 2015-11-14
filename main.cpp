#include <iostream>
#include <string>

#include "DirtyWordChecker.h"

using namespace std;

string ReplaceTestHelper(DirtyWordChecker& pChecker, const char* pInput)
{
    string tRet(pInput);
    pChecker.ReplaceDirtyWord((char*)tRet.data(), '*');
    return tRet;
}

void ChineseModeTest()
{
    DirtyWordChecker tChecker;
    tChecker.SetWholeWordMode(false);  // 中文模式下不启用全字匹配

    // 脏词表
    tChecker.AddWord("艹");
    tChecker.AddWord("操");
    tChecker.AddWord("傻逼");
    tChecker.AddWord("操你妈");
    tChecker.AddWord("你妈逼");
    tChecker.AddWord("日你妈");

    // 测试
    cout << ">>>>> CHINESE MODE" << endl;
    cout << tChecker.CheckDirtyWord(NULL) << endl;  // 测试样例：空串1
    cout << tChecker.CheckDirtyWord("") << endl;  // 测试样例：空串2
    cout << tChecker.CheckDirtyWord(" ") << endl;  // 测试样例：空串3
    cout << tChecker.CheckDirtyWord("对面的傻逼我日你妈") << endl;  // 测试样例：被匹配词在句尾
    cout << tChecker.CheckDirtyWord("操你妈，垃圾") << endl;  // 测试样例：被匹配词在句首
    cout << tChecker.CheckDirtyWord("什么傻逼操作") << endl;  // 测试样例：被匹配词在句中

    // 替换测试
    cout << ReplaceTestHelper(tChecker, "") << endl;
    cout << ReplaceTestHelper(tChecker, " ") << endl;
    cout << ReplaceTestHelper(tChecker, "对面的傻逼我日你妈") << endl;
    cout << ReplaceTestHelper(tChecker, "操你妈，垃圾") << endl;
    cout << ReplaceTestHelper(tChecker, "什么傻逼操作") << endl;
    cout << ReplaceTestHelper(tChecker, "艹，日你妈逼") << endl;
    cout << ReplaceTestHelper(tChecker, "艹日你妈逼") << endl;
}

void EnglishModeTest()
{
    DirtyWordChecker tChecker;
    tChecker.SetWholeWordMode(true);  // 英文模式下启用全字匹配

    // 脏词表
    tChecker.AddWord("shit");
    tChecker.AddWord("fuck");
    tChecker.AddWord("bullshit");
    tChecker.AddWord("bitch");
    tChecker.AddWord("fuck u");
    tChecker.AddWord("son of bitch");

    // 测试
    cout << ">>>>> ENGLISH MODE" << endl;
    cout << tChecker.CheckDirtyWord(NULL) << endl;  // 测试样例：空串1
    cout << tChecker.CheckDirtyWord("") << endl;  // 测试样例：空串2
    cout << tChecker.CheckDirtyWord(" ") << endl;  // 测试样例：空串3
    cout << tChecker.CheckDirtyWord("watch ya fire, bitch") << endl;  // 测试样例：被匹配词在句尾
    cout << tChecker.CheckDirtyWord("fuck you") << endl;  // 测试样例：被匹配词在句首
    cout << tChecker.CheckDirtyWord("u son of bitch!!!") << endl;  // 测试样例：被匹配词在句中
    cout << tChecker.CheckDirtyWord("motherfuck") << endl;  // 测试样例：非全字匹配1
    cout << tChecker.CheckDirtyWord("shithead") << endl;  // 测试样例：非全字匹配2

    // 替换测试
    cout << ReplaceTestHelper(tChecker, "") << endl;
    cout << ReplaceTestHelper(tChecker, " ") << endl;
    cout << ReplaceTestHelper(tChecker, "watch ya fire, bitch") << endl;
    cout << ReplaceTestHelper(tChecker, "fuck you") << endl;
    cout << ReplaceTestHelper(tChecker, "u son of bitch!!!") << endl;
    cout << ReplaceTestHelper(tChecker, "motherfuck") << endl;
    cout << ReplaceTestHelper(tChecker, "shithead") << endl;
    cout << ReplaceTestHelper(tChecker, "fuck u, u son of bitch") << endl;
}

int main()
{
    ChineseModeTest();
    EnglishModeTest();

    return 0;
}
