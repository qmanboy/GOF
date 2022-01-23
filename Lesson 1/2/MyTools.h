#pragma once

#include <stdint.h>
#include <string>

namespace MyTools {

    // Палитра цветов от 0 до 15
    enum ConsoleColor
    {
        CC_Black = 0,
        CC_Blue,
        CC_Green,
        CC_Cyan,
        CC_Red,
        CC_Magenta,
        CC_Brown,
        CC_LightGray,
        CC_DarkGray,
        CC_LightBlue,
        CC_LightGreen,
        CC_LightCyan,
        CC_LightRed,
        CC_LightMagenta,
        CC_Yellow,
        CC_White
    };

	//=============================================================================================

	void ClrScr();

	void __fastcall GotoXY(double x, double y);

	uint16_t GetMaxX();

	uint16_t GetMaxY();

    void SetColor(ConsoleColor color);

	//=============================================================================================
    
    class FileLoggerSingleton
    {
    private:
        FileLoggerSingleton() = default;
        FileLoggerSingleton(const FileLoggerSingleton& root) = delete;
        FileLoggerSingleton& operator=(const FileLoggerSingleton&) = delete;
        static FileLoggerSingleton* uniqInstance;        
    public:

        static FileLoggerSingleton* getInstance();

        void __fastcall OpenLogFile(const std::string& FN);

        void CloseLogFile();

        void __fastcall WriteToLog(const std::string& str);

        void __fastcall WriteToLog(const std::string& str, int n);

        void __fastcall WriteToLog(const std::string& str, double d);

        ~FileLoggerSingleton();
    };

    //=============================================================

    class LoggerSingleton
    {
    private:
        LoggerSingleton() :realSubject(nullptr), counter(1){};
        LoggerSingleton(const LoggerSingleton& root) = delete;
        LoggerSingleton& operator=(const LoggerSingleton&) = delete;
        static LoggerSingleton* uniqProxyInstance;
    public:
        FileLoggerSingleton* realSubject;
        size_t counter;
        static LoggerSingleton* getProxyInstance();
        void __fastcall ProxyOpenLogFile(const std::string& FN);
        void ProxyCloseLogFile();
        void __fastcall ProxyWriteToLog(const std::string& str);
        void __fastcall ProxyWriteToLog(const std::string& str, int n);
        void __fastcall ProxyWriteToLog(const std::string& str, double d);
        ~LoggerSingleton();
    };

	//=============================================================================================

};