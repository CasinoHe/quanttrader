#pragma once

#if defined(__APPLE__)
#include "date/date.h"
#include "date/tz.h"
#else
#include <chrono>
#endif

#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include <map>

#include "logger/quantlogger.h"

namespace quanttrader {
namespace time {

// TimeWithZone is thread-safety, no global state, and no static members
class TimeWithZone {

public:
#if defined(__APPLE__)
    using LocalTime = date::local_time<std::chrono::nanoseconds>;
    using ZonedTime = date::zoned_time<std::chrono::nanoseconds>;
    using SysTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;
#else
    using LocalTime = std::chrono::local_time<std::chrono::nanoseconds>;
    using ZonedTime = std::chrono::zoned_time<std::chrono::nanoseconds>;
    using SysTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>;
#endif
    static constexpr uint64_t kMinimumNanosecondsEpoch = 1000000000000000000ULL;

    TimeWithZone(std::string_view time_zone, LocalTime local_time);
    TimeWithZone(std::string_view time_zone, SysTime sys_time);
    TimeWithZone(const ZonedTime& zoned_time);
    TimeWithZone(ZonedTime&& zoned_time) noexcept;
    TimeWithZone(const TimeWithZone& other);
    TimeWithZone(TimeWithZone&& other) noexcept;
    TimeWithZone(uint64_t nanoseconds_epoch, const std::string& zone_name);

    std::string to_string() const;
    std::string to_string_with_offset() const;
    std::string to_string_with_name() const;

    static std::optional<TimeWithZone> from_now();
    // parse from a time string contains time zone offset like "2025-01-10 11:30:19.5094586+0800"
    static std::optional<TimeWithZone> from_offset_string(const std::string& data);
    // parse from a time string contains time zone name like "20240510 09:29:30 US/Eastern"
    static std::optional<TimeWithZone> from_zone_string(const std::string& data);
    // parse from a time string from IB API historical data like "20240510 09:29:30 US/Eastern"
    static std::optional<TimeWithZone> from_ibapi_string(const std::string& data, std::string_view zone_name);
    // parse from a datetime string with a specified timezone
    static std::optional<TimeWithZone> from_datetime_string(const std::string& datetime, const std::string& timezone);
    static std::optional<std::string> find_zone_by_offset(std::chrono::seconds &offset);
    static bool is_valid_time_zone(std::string_view time_zone);
    static std::string_view get_canonical_zone_name(std::string_view time_zone);

    const ZonedTime &get_zoned_time() const;
    LocalTime get_local_time() const;
    SysTime get_utc_time() const;
    SysTime get_sys_time() const;
    LocalTime get_zone_time(const std::string &zone_name) const;
    uint64_t get_nano_epoch() const;
    uint64_t get_milli_epoch() const;
    uint64_t get_micro_epoch() const;
    uint64_t get_seconds_epoch() const;

    std::vector<uint8_t> serialize_to_vector() const;
    void serialize_to_buffer(uint8_t* buffer) const;
    static std::optional<TimeWithZone> unserialize_from_vector(const std::vector<uint8_t>& bytes);
    static std::optional<TimeWithZone> unserialize_from_buffer(const uint8_t* buffer);

    // Comparison operators based on the underlying UTC time_point
    bool operator<(const TimeWithZone& other) const {
        return compare_time(other.zoned_time_.get_sys_time(), std::less<>());
    }

    bool operator>(const TimeWithZone& other) const {
        return compare_time(other.zoned_time_.get_sys_time(), std::greater<>());
    }

    bool operator<=(const TimeWithZone& other) const {
        return compare_time(other.zoned_time_.get_sys_time(), std::less_equal<>());
    }

    bool operator>=(const TimeWithZone& other) const {
        return compare_time(other.zoned_time_.get_sys_time(), std::greater_equal<>());
    }

    bool operator==(const TimeWithZone& other) const {
        return compare_time(other.zoned_time_.get_sys_time(), std::equal_to<>());
    }

    bool operator!=(const TimeWithZone& other) const {
        return compare_time(other.zoned_time_.get_sys_time(), std::not_equal_to<>());
    }

    // Comparison operators with time_point
    bool operator<(const SysTime& time_point) const {
        return compare_time(time_point, std::less<>());
    }

    bool operator>(const SysTime& time_point) const {
        return compare_time(time_point, std::greater<>());
    }

    bool operator<=(const SysTime& time_point) const {
        return compare_time(time_point, std::less_equal<>());
    }

    bool operator>=(const SysTime& time_point) const {
        return compare_time(time_point, std::greater_equal<>());
    }

    bool operator==(const SysTime& time_point) const {
        return compare_time(time_point, std::equal_to<>());
    }

    bool operator!=(const SysTime& time_point) const {
        return compare_time(time_point, std::not_equal_to<>());
    }

    // Comparison operators with uint64_t nanoseconds epoch 
    // Caution: It only compares with the nanoseconds, not milliseconds or seconds
    bool operator<(const uint64_t nanoseconds) const {
        if (nanoseconds < kMinimumNanosecondsEpoch) {
            log::Error("Compare TimeWithZone with a non-nanoseconds epoch value: {}, it may be milliseconds or seconds", nanoseconds);
        }
        return get_nano_epoch() < nanoseconds;
    }

    bool operator>(const uint64_t nanoseconds) const {
        if (nanoseconds < kMinimumNanosecondsEpoch) {
            log::Error("Compare TimeWithZone with a non-nanoseconds epoch value: {}, it may be milliseconds or seconds", nanoseconds);
        }
        return get_nano_epoch() > nanoseconds;
    }

    bool operator<=(const uint64_t nanoseconds) const {
        if (nanoseconds < kMinimumNanosecondsEpoch) {
            log::Error("Compare TimeWithZone with a non-nanoseconds epoch value: {}, it may be milliseconds or seconds", nanoseconds);
        }
        return get_nano_epoch() <= nanoseconds;
    }

    bool operator>=(const uint64_t nanoseconds) const {
        if (nanoseconds < kMinimumNanosecondsEpoch) {
            log::Error("Compare TimeWithZone with a non-nanoseconds epoch value: {}, it may be milliseconds or seconds", nanoseconds);
        }
        return get_nano_epoch() >= nanoseconds;
    }

    bool operator==(const uint64_t nanoseconds) const {
        if (nanoseconds < kMinimumNanosecondsEpoch) {
            log::Error("Compare TimeWithZone with a non-nanoseconds epoch value: {}, it may be milliseconds or seconds", nanoseconds);
        }
        return get_nano_epoch() == nanoseconds;
    }

    bool operator!=(const uint64_t nanoseconds) const {
        if (nanoseconds < kMinimumNanosecondsEpoch) {
            log::Error("Compare TimeWithZone with a non-nanoseconds epoch value: {}, it may be milliseconds or seconds", nanoseconds);
        }
        return get_nano_epoch() != nanoseconds;
    }

    // Print the time (for demonstration purposes)
    friend std::ostream& operator<<(std::ostream& os, const TimeWithZone& t) {
#if defined(__APPLE__)
        os << date::format("{:%F %T%z}", t.zoned_time_);
#else
        os << std::format("{:%F %T%z}", t.zoned_time_);
#endif
        return os;
    }

private:
    ZonedTime zoned_time_ {};
    mutable std::optional<std::string> cached_to_string_;
    mutable std::optional<std::string> cached_to_string_with_name_;
#if defined(__APPLE__)
    static const date::tzdb& tzdb_;
#else
    static const std::chrono::tzdb& tzdb_;
#endif

private:
    TimeWithZone() = delete;
    // Static member for legacy zone mapping
    static const std::map<std::string, std::string>& get_legacy_zone_to_canonical();

    template <typename Compare>
    bool compare_time(const SysTime& other_time, Compare comp) const {
        return comp(zoned_time_.get_sys_time(), other_time);
    }

    static ZonedTime get_zoned_time(uint64_t nano_epoch, const std::string_view zone_name);
};

} // namespace time
} // namespace quanttrader