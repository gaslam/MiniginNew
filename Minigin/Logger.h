#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <WinUser.h>


class LogException final {
public:
    LogException(const char* name) :
        m_Name{ name } {};

    const char* GetName() const { return m_Name; }
private:
    const char* m_Name{};
};

class Logger {
public:
    static void LogInfo(const char* message) {
        std::cout << "[INFO] " << message << std::endl;
    }

    static void LogError(const char* message) {
        try {
            throw LogException(message);
        }
        catch(LogException& e){
            MessageBox(0, e.GetName(), "[ERROR]", MB_OK | MB_ICONERROR);
            exit(0);
        }
    }
};