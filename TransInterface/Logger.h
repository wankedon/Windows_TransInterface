/**
 * @file Logger.h
 * @brief ��־����
 * @author װ����ҵ������� ���� 
 * @version 0.1
 * @date 2020-12-24
 * 
 * @copyright Copyright (c) 2020  �й����ӿƼ����Ź�˾����ʮһ�о���
 * 
 */
#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger
{
public:
    Logger(const std::string& path)
    {
        consoleLogger = spdlog::stdout_color_mt(CONSOLE_LOGGER_NAME);
        fileLogger = spdlog::basic_logger_mt(FILE_LOGGER_NAME, path);
        consoleLogger->set_level(spdlog::level::debug);
        fileLogger->set_level(spdlog::level::info);
    }
    ~Logger()
    {
		fileLogger->flush();
        consoleLogger.reset();
        fileLogger.reset();
        spdlog::drop_all();
    }

    static std::string PATH;

    static Logger& instance()
    {
        static Logger lg(PATH);
        return lg;
    }

public:
    template<typename T>
    void consoleLog(const T& msg)
    {
        consoleLogger->debug(msg);
    }

    template<typename T>
    void fileLog(const T& msg)
    {
        fileLogger->info(msg);
        fileLogger->flush();
    }

    template<typename FormatString, typename... Args>
    void consoleLog(const FormatString& fmt, const Args &... args)
    {
        consoleLogger->debug(fmt, args...);
    }

    template<typename FormatString, typename... Args>
    void fileLog(const FormatString& fmt, const Args &... args)
    {
        fileLogger->info(fmt, args...);
        fileLogger->flush();
    }

	void flush()
	{
		consoleLogger->flush();
		fileLogger->flush();
	}
private:
	static const std::string CONSOLE_LOGGER_NAME;
	static const std::string FILE_LOGGER_NAME;
private:
    std::shared_ptr<spdlog::logger> consoleLogger;
	std::shared_ptr<spdlog::logger> fileLogger;
};

/**
 * @brief ��־�Ŀ���̨���
 * 
 * @tparam T ��־��Ϣģ������
 * @param msg Ҫ�������־��Ϣ
 */
template<typename T>
void CLOG(const T& msg)
{
    Logger::instance().consoleLog(msg);
}

template<typename FormatString, typename... Args>
void CLOG(const FormatString& fmt, const Args &... args)
{
    Logger::instance().consoleLog(fmt, args...);
}

/**
 * @brief ��־���ļ����
 * 
 * @tparam T 
 * @param msg 
 */
template<typename T>
void FLOG(const T& msg)
{
    Logger::instance().fileLog(msg);
}

template<typename FormatString, typename... Args>
void FLOG(const FormatString& fmt, const Args &...args)
{
    Logger::instance().fileLog(fmt, args...);
}

/**
 * @brief ��־ͬʱ���������̨���ļ�
 * 
 * @tparam T 
 * @param msg 
 */
template<typename T>
void LOG(const T& msg)
{
    CLOG(msg);
    FLOG(msg);
}

/**
 * @brief ��־ͬʱ���
 * 
 * @tparam FormatString 
 * @tparam Args 
 * @param fmt 
 * @param args 
 */
template<typename FormatString, typename... Args>
void LOG(const FormatString& fmt, const Args &... args)
{
    CLOG(fmt, args ...);
    FLOG(fmt, args ...);
}

/**
 * @brief ����������־���壬ͨ���ڳ����˳�ǰ����
 * 
 */
void LOGFlush();

