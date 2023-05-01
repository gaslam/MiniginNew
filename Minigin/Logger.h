#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <Windows.h>
#include <sstream>

class Logger {
public:
	static void LogInfo(const char* message) {
		std::cout << "[INFO] " << message << std::endl;
	}

	static void LogWarning(const char* file, const int line, const char* function, const char* message)
	{
		std::cout << "File: " << file << "\nLine: " << line << "\nFunction: " << function << "\nAssertion: " << message;
	}

	static void LogError(const char* file, const int line, const char* function, const char* message) {
		std::stringstream ss;
		ss << "File: " << file << "\nLine: " << line << "\nFunction: " << function << "\nAssertion: " << message;
		auto messageStr = ss.str();
		MessageBox(0, messageStr.c_str(), "[ERROR]", MB_OK | MB_ICONERROR);
	}
};

#define MG_ASSERT(expr) \
    if (!(expr)) \
Logger::LogError(__FILE__, __LINE__, __FUNCTION__, #expr);

#define MG_ASSERT_WARNING(expr) \
    if (!(expr)) \
Logger::LogError(__FILE__, __LINE__, __FUNCTION__, #expr);