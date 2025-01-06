#pragma once

namespace quanttrader
{
    const int kSpdLogQueueSize = 1 << 16;
    const int kSpdLogThreadsCount = 3;
    const char* kSpdLogPattern = "[%Y-%m-%d %H:%M:%S.%e][%z][%@][%^%l%$] %v";
    const uint16_t kSpdLogMaxRotatingFiles = 60;

    const char* kDefaultLogDir = "log";
}