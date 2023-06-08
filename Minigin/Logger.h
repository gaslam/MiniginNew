#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <Windows.h>
#include <sstream>

class Logger {
public:
	static void LogInfo(const char* file, const int line, const char* function, const char* message) {
		std::cout << "[INFO]:\n\nMessage: " << message <<"\nFile: " << file << "\nLine: " << line << "\nFunction: " << function << "\n\n";
	}

	static void LogWarning(const char* file, const int line, const char* function, const char* assertion, const char* message)
	{
		std::cout << "[WARNING]:\n\nMessage: " << message << "\nFile: " << file << "\nLine: " << line << "\nAssertion: " << assertion << "\nFunction: " << function << "\n\n";
	}

	static void LogError(const char* file, const int line, const char* function, const char* assertion, const char* message) {
		std::stringstream ss;
		ss << "Message: " << message << "\nFile: " << file << "\nLine: " << line << "\nAssertion: " << assertion << "\nFunction: " << function << "\n\n";
		auto messageStr = ss.str();
		//Only supports windows for now
		MessageBoxA(0, messageStr.c_str(), "[ERROR]", MB_OK | MB_ICONERROR);
	}
};

#define MG_ASSERT(expr,message) \
    if (!(expr)) \
Logger::LogError(__FILE__, __LINE__, __FUNCTION__, #expr, message);

#define MG_ASSERT_WARNING(expr,message) \
    if (!(expr)) \
Logger::LogWarning(__FILE__, __LINE__, __FUNCTION__, #expr, message);

#define MG_ASSERT_INFO(expr) \
Logger::LogInfo(__FILE__, __LINE__, __FUNCTION__, #expr);