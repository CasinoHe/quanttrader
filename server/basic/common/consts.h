#pragma once

namespace quanttrader
{
    constexpr int kSpdLogQueueSize = 1 << 16;
    constexpr int kSpdLogThreadsCount = 1;
    constexpr char kSpdLogPattern[] = "[%Y-%m-%d %H:%M:%S.%F%z][%P][%t][%n][%^%l%$] %v";
    constexpr int kSpdLogMaxRotatingFiles = 180;

    constexpr char kDefaultLogDir[] = "log";
    constexpr unsigned long long kMinimumNanosecondsEpoch = static_cast<unsigned long long>(1e+17);
}