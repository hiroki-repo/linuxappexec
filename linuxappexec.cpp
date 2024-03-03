// linuxappexec.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include "windows.h"

#ifdef _M_ARM64
/*
mov x8,#221
svc #0
ret
*/
char syscallhandler[] = { 0xA8,0x1B,0x80,0xD2,0x01,0x00,0x00,0xD4,0xC0,0x03,0x5F,0xD6 };
typedef UINT64 typeofsyscallhandler(char*, char**, char**);
#else
#ifdef _M_ARM
/*
mov r7,#11
svc #0
bx lr
*/
char syscallhandler[] = { 0x0B,0x70,0xA0,0xE3,0x00,0x00,0x00,0xEF,0x1E,0xFF,0x2F,0xE1 };
typedef DWORD typeofsyscallhandler(char*, char**, char**);
#else
#ifdef _M_AMD64
/*
push rdi
push rsi
mov rdi,rcx
mov rsi,rdx
mov rdx,r8
mov rax,59
syscall
pop rsi
pop rdi
ret
*/
char syscallhandler[] = { 0x57,0x56,0x48,0x89,0xCF,0x48,0x89,0xD6,0x4C,0x89,0xC2,0x48,0xC7,0xC0,0x3B,0x00,0x00,0x00,0x0F,0x05,0x5E,0x5F,0xC3 };
typedef UINT64 typeofsyscallhandler(char*, char**, char**);
#else
#ifdef _M_IX86
/*
push ebx
push ebp
mov ebp,esp
add ebp,12
mov ebx,dword ptr [ebp + 0]
mov ecx,dword ptr [ebp + 4]
mov edx,dword ptr [ebp + 8]
mov eax,11
int 0x80
pop ebp
pop ebx
ret
*/
char syscallhandler[] = { 0x53,0x55,0x89,0xE5,0x83,0xC5,0x0C,0x8B,0x5D,0x00,0x8B,0x4D,0x04,0x8B,0x55,0x08,0xB8,0x0B,0x00,0x00,0x00,0xCD,0x80,0x5D,0x5B,0xC3 };
typedef DWORD typeofsyscallhandler(char*,char**,char**);
#else
#endif
#endif
#endif
#endif

int main(int argc, char *argv[], char *envp[])
{
    char **argvx = argv;
    argvx++;
    DWORD flOldProtect;
    VirtualProtect(&syscallhandler,sizeof(syscallhandler),0x40,&flOldProtect);
    return (((typeofsyscallhandler *) &syscallhandler)(argvx[0],argvx,envp));
    //std::cout << "Hello World!\n";
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
