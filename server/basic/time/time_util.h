#pragma once

#include <cstdint>
#include <ctime>
#include <chrono>

namespace quanttrader {
namespace time {

/**
 * @brief Utility functions for time manipulation in the data subsystem
 */
class TimeUtil {
public:
    /**
     * @brief Align a timestamp to the start of a second period
     * 
     * @param time_ns Timestamp in nanoseconds
     * @param seconds The number of seconds in the period
     * @return Aligned timestamp in nanoseconds
     */
    static uint64_t align_to_second(uint64_t time_ns, unsigned int seconds = 1) {
        constexpr uint64_t ns_per_second = 1000000000ULL;
        uint64_t seconds_epoch = time_ns / ns_per_second;
        seconds_epoch = (seconds_epoch / seconds) * seconds;
        return seconds_epoch * ns_per_second;
    }
    
    /**
     * @brief Align a timestamp to the start of a minute period
     * 
     * @param time_ns Timestamp in nanoseconds
     * @param minutes The number of minutes in the period
     * @return Aligned timestamp in nanoseconds
     */
    static uint64_t align_to_minute(uint64_t time_ns, unsigned int minutes = 1) {
        constexpr uint64_t ns_per_minute = 60ULL * 1000000000ULL;
        uint64_t minutes_epoch = time_ns / ns_per_minute;
        minutes_epoch = (minutes_epoch / minutes) * minutes;
        return minutes_epoch * ns_per_minute;
    }
    
    /**
     * @brief Align a timestamp to the start of an hour period
     * 
     * @param time_ns Timestamp in nanoseconds
     * @param hours The number of hours in the period
     * @return Aligned timestamp in nanoseconds
     */
    static uint64_t align_to_hour(uint64_t time_ns, unsigned int hours = 1) {
        constexpr uint64_t ns_per_hour = 3600ULL * 1000000000ULL;
        uint64_t hours_epoch = time_ns / ns_per_hour;
        hours_epoch = (hours_epoch / hours) * hours;
        return hours_epoch * ns_per_hour;
    }
    
    /**
     * @brief Align a timestamp to the start of a day period
     * 
     * @param time_ns Timestamp in nanoseconds
     * @param days The number of days in the period
     * @return Aligned timestamp in nanoseconds
     */
    static uint64_t align_to_day(uint64_t time_ns, unsigned int days = 1) {
        // Convert to time_t (seconds)
        time_t time_secs = static_cast<time_t>(time_ns / 1000000000ULL);
        
        // Convert to calendar time
        std::tm* cal_time = std::gmtime(&time_secs);
        
        // Reset to start of day
        cal_time->tm_hour = 0;
        cal_time->tm_min = 0;
        cal_time->tm_sec = 0;
        
        // Convert back to time_t
        time_t day_start = std::mktime(cal_time);
        
        // Align to multi-day period if necessary
        if (days > 1) {
            // Get the day of the epoch (days since Jan 1, 1970)
            const time_t seconds_per_day = 86400;
            time_t days_since_epoch = day_start / seconds_per_day;
            
            // Align to the period
            days_since_epoch = (days_since_epoch / days) * days;
            
            // Convert back to seconds
            day_start = days_since_epoch * seconds_per_day;
        }
        
        // Convert to nanoseconds
        return static_cast<uint64_t>(day_start) * 1000000000ULL;
    }
    
    /**
     * @brief Align a timestamp to the start of a week
     * 
     * @param time_ns Timestamp in nanoseconds
     * @return Aligned timestamp in nanoseconds
     */
    static uint64_t align_to_week(uint64_t time_ns) {
        // Convert to time_t (seconds)
        time_t time_secs = static_cast<time_t>(time_ns / 1000000000ULL);
        
        // Convert to calendar time
        std::tm* cal_time = std::gmtime(&time_secs);
        
        // Reset to start of day
        cal_time->tm_hour = 0;
        cal_time->tm_min = 0;
        cal_time->tm_sec = 0;
        
        // Calculate days since start of week (assuming Monday is start of week)
        int day_of_week = cal_time->tm_wday;
        if (day_of_week == 0) day_of_week = 7;  // Convert Sunday (0) to 7
        day_of_week -= 1;  // Adjust to make Monday = 0
        
        // Go back to the start of the week
        time_t day_start = std::mktime(cal_time);
        time_t week_start = day_start - (day_of_week * 86400);
        
        // Convert to nanoseconds
        return static_cast<uint64_t>(week_start) * 1000000000ULL;
    }
    
    /**
     * @brief Align a timestamp to the start of a month
     * 
     * @param time_ns Timestamp in nanoseconds
     * @return Aligned timestamp in nanoseconds
     */
    static uint64_t align_to_month(uint64_t time_ns) {
        // Convert to time_t (seconds)
        time_t time_secs = static_cast<time_t>(time_ns / 1000000000ULL);
        
        // Convert to calendar time
        std::tm* cal_time = std::gmtime(&time_secs);
        
        // Reset to start of month
        cal_time->tm_mday = 1;
        cal_time->tm_hour = 0;
        cal_time->tm_min = 0;
        cal_time->tm_sec = 0;
        
        // Convert back to time_t
        time_t month_start = std::mktime(cal_time);
        
        // Convert to nanoseconds
        return static_cast<uint64_t>(month_start) * 1000000000ULL;
    }
};

} // namespace time
} // namespace quanttrader