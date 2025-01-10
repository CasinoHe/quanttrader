#pragma once

#include <chrono>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <optional>
#include <cstdint>


namespace quanttrader {
namespace time {

class TimeWithZone {
private:
    TimeWithZone() = delete;

public:
    using ZonedTime = std::chrono::zoned_time<std::chrono::nanoseconds>;

    TimeWithZone(std::string time_zone, std::chrono::local_time<std::chrono::nanoseconds> local_time);
    TimeWithZone(const ZonedTime& zoned_time) : zoned_time_(zoned_time) {}
    TimeWithZone(const ZonedTime&& zoned_time) : zoned_time_(std::move(zoned_time)) {}
    TimeWithZone(const TimeWithZone& other) : zoned_time_(other.zoned_time_) {}
    TimeWithZone(TimeWithZone&& other) noexcept : zoned_time_(std::move(other.zoned_time_)) {}

    // get a readable time string
    std::string to_string() const;
    std::string to_string_with_offset() const;
    std::string to_string_with_name() const;

    // parse from a time string contains time zone offset like "2025-01-10 11:30:19.5094586+0800"
    static std::shared_ptr<TimeWithZone> from_offset_string(const std::string& data);
    // parse from a time string contains time zone name like "20240510 09:29:30 US/Eastern"
    static std::shared_ptr<TimeWithZone> from_zone_string(const std::string& data);
    // parse from a time string from IB API historical data like "20240510 09:29:30 US/Eastern"
    static std::shared_ptr<TimeWithZone> from_ibapi_string(const std::string& data, const std::string& zone_name);
    static std::optional<std::string> find_zone_by_offset(std::chrono::seconds &offset);
    static bool is_valid_time_zone(const std::string& time_zone);
    static std::string get_canonical_zone_name(const std::string& time_zone);

    // get zone time object
    const ZonedTime &get_zoned_time() const { return zoned_time_; }

    // get local time
    std::chrono::local_time<std::chrono::nanoseconds> get_local_time() const { return zoned_time_.get_local_time(); }
    // get UTC time
    std::chrono::sys_time<std::chrono::nanoseconds> get_utc_time() const { return zoned_time_.get_sys_time(); }
    std::chrono::sys_time<std::chrono::nanoseconds> get_sys_time() const { return zoned_time_.get_sys_time(); }
    // get local time in other time zone
    std::chrono::local_time<std::chrono::nanoseconds> get_zone_time(const std::string &zone_name) const;
    // get a numeric value of the time
    uint64_t get_nano_epoch() const { return std::chrono::time_point_cast<std::chrono::nanoseconds>(zoned_time_.get_sys_time()).time_since_epoch().count(); }
    uint64_t get_milli_epoch() const { return std::chrono::time_point_cast<std::chrono::milliseconds>(zoned_time_.get_sys_time()).time_since_epoch().count(); }
    uint64_t get_micro_epoch() const { return std::chrono::time_point_cast<std::chrono::microseconds>(zoned_time_.get_sys_time()).time_since_epoch().count(); }
    uint64_t get_seconds_epoch() const { return std::chrono::time_point_cast<std::chrono::seconds>(zoned_time_.get_sys_time()).time_since_epoch().count(); }

    // Serialize to bytes
    std::vector<uint8_t> serialize_to_vector() const;
    void serialize_to_buffer(uint8_t* buffer) const;

    // Unserialize from bytes
    static std::shared_ptr<TimeWithZone> unserialize_from_vector(const std::vector<uint8_t>& bytes);
    static std::shared_ptr<TimeWithZone> unserialize_from_buffer(const uint8_t* buffer);

    // Comparison operators based on the underlying UTC time_point
    bool operator<(const TimeWithZone& other) const {
        return zoned_time_.get_sys_time() < other.zoned_time_.get_sys_time();
    }

    bool operator>(const TimeWithZone& other) const {
        return zoned_time_.get_sys_time() > other.zoned_time_.get_sys_time();
    }

    bool operator<=(const TimeWithZone& other) const {
        return zoned_time_.get_sys_time() <= other.zoned_time_.get_sys_time();
    }

    bool operator>=(const TimeWithZone& other) const {
        return zoned_time_.get_sys_time() >= other.zoned_time_.get_sys_time();
    }

    bool operator==(const TimeWithZone& other) const {
        return zoned_time_.get_sys_time() == other.zoned_time_.get_sys_time();
    }

    bool operator!=(const TimeWithZone& other) const {
        return zoned_time_.get_sys_time() != other.zoned_time_.get_sys_time();
    }

    // Comparison operators with time_point
    bool operator<(const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& time_point) const {
        return zoned_time_.get_sys_time() < time_point;
    }

    bool operator>(const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& time_point) const {
        return zoned_time_.get_sys_time() > time_point;
    }

    bool operator<=(const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& time_point) const {
        return zoned_time_.get_sys_time() <= time_point;
    }

    bool operator>=(const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& time_point) const {
        return zoned_time_.get_sys_time() >= time_point;
    }

    bool operator==(const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& time_point) const {
        return zoned_time_.get_sys_time() == time_point;
    }

    bool operator!=(const std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds>& time_point) const {
        return zoned_time_.get_sys_time() != time_point;
    }

    // Comparison operators with uint64_t nanoseconds epoch 
    // Caution: It only compares with the nanoseconds, not milliseconds or seconds
    bool operator<(const uint64_t nanoseconds) const {
        return get_nano_epoch() < nanoseconds;
    }

    bool operator>(const uint64_t nanoseconds) const {
        return get_nano_epoch() > nanoseconds;
    }

    bool operator<=(const uint64_t nanoseconds) const {
        return get_nano_epoch() <= nanoseconds;
    }

    bool operator>=(const uint64_t nanoseconds) const {
        return get_nano_epoch() >= nanoseconds;
    }

    bool operator==(const uint64_t nanoseconds) const {
        return get_nano_epoch() == nanoseconds;
    }

    bool operator!=(const uint64_t nanoseconds) const {
        return get_nano_epoch() != nanoseconds;
    }

    // Print the time (for demonstration purposes)
    friend std::ostream& operator<<(std::ostream& os, const TimeWithZone& t) {
        os << std::format("{:%F %T%z}", t.zoned_time_);
        return os;
    }

private:
    ZonedTime zoned_time_ {};
};

} // namespace time
} // namespace quanttrader