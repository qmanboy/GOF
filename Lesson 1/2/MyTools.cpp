
#include <conio.h>
#include <windows.h>
#include <stdint.h>
#include <time.h> 

#include <string>
#include <iostream>
#include <fstream>
#include <chrono>


#include "MyTools.h"

using namespace std;

namespace MyTools {

    ofstream logOut;

    //=============================================================================================

    void ClrScr()
    {
        system("cls");
    }

    void __fastcall GotoXY(double x, double y)
    {
        const COORD cc = { short(x), short(y) };
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cc);
    }

    uint16_t GetMaxX()
    {
        HANDLE hWndConsole;
        if (hWndConsole = GetStdHandle(-12))
        {
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            {
                return consoleInfo.srWindow.Right;
                int height = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top + 1;
            }
        }

        return 0;
    }

    uint16_t GetMaxY()
    {
        HANDLE hWndConsole;
        if (hWndConsole = GetStdHandle(-12))
        {
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            if (GetConsoleScreenBufferInfo(hWndConsole, &consoleInfo))
            {
                return consoleInfo.srWindow.Bottom;
            }
        }

        return 0;
    }

    void MyTools::SetColor(MyTools::ConsoleColor color)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color); // color =  (WORD)((BackgroundColor << 4) | TextColor))
    }

    //=============================================================================================


    FileLoggerSingleton* FileLoggerSingleton::uniqInstance = nullptr;

    FileLoggerSingleton* FileLoggerSingleton::getInstance()
    {
        if (FileLoggerSingleton::uniqInstance == nullptr)
            FileLoggerSingleton::uniqInstance = new FileLoggerSingleton;
        return  uniqInstance;
    }

    FileLoggerSingleton::~FileLoggerSingleton()
    {
        delete uniqInstance;
    }

    void __fastcall FileLoggerSingleton::OpenLogFile(const string& FN)
    {
        logOut.open(FN, ios_base::out);
    }

    void __fastcall FileLoggerSingleton::CloseLogFile()
    {
        if (logOut.is_open())
        {
            logOut.close();
        }
    }

    string GetCurDateTime()
    {
        auto cur = std::chrono::system_clock::now();
        time_t time = std::chrono::system_clock::to_time_t(cur);
        char buf[64] = { 0 };
        ctime_s(buf, 64, &time);
        buf[strlen(buf) - 1] = '\0';
        return string(buf);
    }

    void __fastcall FileLoggerSingleton::WriteToLog(const string& str)
    {
        if (logOut.is_open())
        {
            logOut << GetCurDateTime() << " - " << str << endl;
        }
    }

    void __fastcall FileLoggerSingleton::WriteToLog(const string& str, int n)
    {
        if (logOut.is_open())
        {
            logOut << GetCurDateTime() << " - " << str << n << endl;
        }
    }

    void __fastcall FileLoggerSingleton::WriteToLog(const string& str, double d)
    {
        if (logOut.is_open())
        {
            logOut << GetCurDateTime() << " - " << str << d << endl;
        }
    }

    //=====================================================================

    LoggerSingleton* LoggerSingleton::uniqProxyInstance = nullptr;

    LoggerSingleton* LoggerSingleton::getProxyInstance()
    {        
        if (LoggerSingleton::uniqProxyInstance == nullptr)
            LoggerSingleton::uniqProxyInstance = new LoggerSingleton;
        if (!logOut.is_open())
        {
            uniqProxyInstance->ProxyOpenLogFile("log.txt");
            logOut << uniqProxyInstance->counter++ << "). Proxy open log file" << std::endl;
        }
        return  uniqProxyInstance;
    }

    LoggerSingleton::~LoggerSingleton()
    {
        delete uniqProxyInstance;
    }

    void __fastcall LoggerSingleton::ProxyOpenLogFile(const string& FN)
    {
        realSubject = FileLoggerSingleton::getInstance();
        realSubject->OpenLogFile(FN);
    }

    void __fastcall LoggerSingleton::ProxyCloseLogFile()
    {
        realSubject = FileLoggerSingleton::getInstance();
        if (logOut.is_open())
        {
            logOut << uniqProxyInstance->counter++ << "). Proxy closed log file" << std::endl;
        }
        realSubject->CloseLogFile();
    }

    void __fastcall LoggerSingleton::ProxyWriteToLog(const string& str)
    {
        realSubject = FileLoggerSingleton::getInstance();
        if (logOut.is_open())
        {
            logOut << counter++ << "). ";
        }
        realSubject->WriteToLog(str);
        
    }

    void __fastcall  LoggerSingleton::ProxyWriteToLog(const string& str, int n)
    {
        realSubject = FileLoggerSingleton::getInstance();
        if (logOut.is_open())
        {
            logOut << counter++ << "). ";
        }
        realSubject->WriteToLog(str, n);
    }

    void __fastcall LoggerSingleton::ProxyWriteToLog(const string& str, double d)
    {
        realSubject = FileLoggerSingleton::getInstance();
        if (logOut.is_open())
        {
            logOut << counter++ << "). ";
        }
        realSubject->WriteToLog(str, d);
    }





    //=============================================================================================


} // namespace MyTools