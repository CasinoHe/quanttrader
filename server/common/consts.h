#pragma once

namespace quanttrader
{
    constexpr int kSpdLogQueueSize = 1 << 16;
    constexpr int kSpdLogThreadsCount = 3;
    constexpr char kSpdLogPattern[] = "[%Y-%m-%d %H:%M:%S.%e][%z][%n][%^%l%$] %v";
    constexpr int kSpdLogMaxRotatingFiles = 180;

    constexpr char kDefaultLogDir[] = "log";
}