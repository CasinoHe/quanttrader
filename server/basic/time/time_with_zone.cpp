#include "time_with_zone.h"
#include "logger/quantlogger.h"
#include <sstream>
#include <cstring>
#include <optional>
#include <map>

namespace quanttrader {
namespace time {

#if defined(__APPLE__)
const date::tzdb& TimeWithZone::tzdb_ = date::get_tzdb();
#endif

// Helper macro for cross-platform parse/format (only inside function bodies)
#if defined(__APPLE__)
#define CHRONO_PARSE date::parse
#define CHRONO_CURRENT_ZONE date::current_zone
#else
#define CHRONO_PARSE std::chrono::parse
#define CHRONO_CURRENT_ZONE std::chrono::current_zone
#endif

TimeWithZone::TimeWithZone(std::string_view time_zone, LocalTime local_time)
#if defined(__APPLE__)
    : zoned_time_(std::string(time_zone), local_time)
#else
    : zoned_time_(std::string(time_zone), local_time)
#endif
{}

TimeWithZone::TimeWithZone(std::string_view time_zone, SysTime sys_time)
    : zoned_time_(std::string(time_zone), sys_time) {}

TimeWithZone::TimeWithZone(const ZonedTime& zoned_time)
    : zoned_time_(zoned_time) {}

TimeWithZone::TimeWithZone(ZonedTime&& zoned_time) noexcept
    : zoned_time_(std::move(zoned_time)) {}

TimeWithZone::TimeWithZone(const TimeWithZone& other)
    : zoned_time_(other.zoned_time_) {}

TimeWithZone::TimeWithZone(TimeWithZone&& other) noexcept
    : zoned_time_(std::move(other.zoned_time_)) {}

TimeWithZone::TimeWithZone(uint64_t nanoseconds_epoch, const std::string& zone_name)
    : zoned_time_(get_zoned_time(nanoseconds_epoch, zone_name)) {}

std::string TimeWithZone::to_string() const {
#if defined(__APPLE__)
    return date::format("%F %T%z", zoned_time_);
#else
    std::ostringstream oss;
    oss << std::format("%F %T%z", zoned_time_);
    return oss.str();
#endif
}

std::string TimeWithZone::to_string_with_offset() const {
    return to_string();
}

std::string TimeWithZone::to_string_with_name() const {
#if defined(__APPLE__)
    return date::format("%F %T %Z", zoned_time_);
#else
    std::ostringstream oss;
    oss << std::format("%F %T %Z", zoned_time_);
    return oss.str();
#endif
}

std::optional<TimeWithZone> TimeWithZone::from_now() {
#if defined(__APPLE__)
    return TimeWithZone(date::zoned_time<std::chrono::nanoseconds>(date::current_zone(), std::chrono::system_clock::now()));
#else
    return TimeWithZone(std::chrono::zoned_time<std::chrono::nanoseconds>(std::chrono::current_zone(), std::chrono::system_clock::now()));
#endif
}

std::optional<TimeWithZone> TimeWithZone::from_offset_string(const std::string& data) {
    std::istringstream iss(data);
    LocalTime tp;
    std::chrono::minutes offset;
    if (iss >> CHRONO_PARSE("%F %T %z", tp, offset)) {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(offset);
        auto zone_name = find_zone_by_offset(seconds);
        if (zone_name.has_value()) {
            return TimeWithZone(zone_name.value(), tp);
        } else {
            log::Error("Failed to find zone for offset: {} {}", data, offset.count());
            return std::nullopt;
        }
    } else {
        log::Error("Failed to parse offset time string: {}, Underlying error: {}", data, iss.fail());
        return std::nullopt;
    }
}

std::optional<TimeWithZone> TimeWithZone::from_zone_string(const std::string& data) {
    std::istringstream iss(data);
    LocalTime tp;
    std::string abbrev;
    if (iss >> CHRONO_PARSE("%F %T %Z", tp, abbrev)) {
        auto zone_name = get_canonical_zone_name(abbrev);
        if (!zone_name.empty()) {
            return TimeWithZone(zone_name, tp);
        } else {
            log::Error("Failed to find zone for abbreviation: {} {}", data, abbrev);
            return std::nullopt;
        }
    } else {
        log::Error("Failed to parse time string with zone: {}, Underlying error: {}", data, iss.fail());
        return std::nullopt;
    }
}

std::optional<TimeWithZone> TimeWithZone::from_ibapi_string(const std::string& data, std::string_view zone_name) {
    std::istringstream iss(data);
    LocalTime tp;
    std::string abbrev;
    if (iss >> CHRONO_PARSE("%Y%m%d %T %Z", tp, abbrev)) {
        auto canonical_zone = get_canonical_zone_name(abbrev);
        if (!canonical_zone.empty()) {
            return TimeWithZone(canonical_zone, tp);
        } else {
            log::Warn("Failed to find zone for abbreviation: {} {}", data, abbrev);
            return TimeWithZone(zone_name, tp);
        }
    } else {
        iss.clear();
        iss.seekg(0, std::ios::beg);
        if (iss >> CHRONO_PARSE("%Y%m%d %T", tp)) {
            return TimeWithZone(zone_name, tp);
        } else {
            iss.clear();
            iss.seekg(0, std::ios::beg);
            if (iss >> CHRONO_PARSE("%Y%m%d", tp)) {
                return TimeWithZone(zone_name, tp);
            } else {
                log::Error("Failed to parse IB API time string: {}, Underlying error: {}", data, iss.fail());
                return std::nullopt;
            }
        }
    }
}

std::optional<TimeWithZone> TimeWithZone::from_datetime_string(const std::string& datetime, const std::string& timezone) {
    std::istringstream iss(datetime);
    LocalTime tp;
    if (iss >> CHRONO_PARSE("%F %T", tp)) {
        auto canonical_zone = get_canonical_zone_name(timezone);
        if (!canonical_zone.empty()) {
            return TimeWithZone(canonical_zone, tp);
        } else {
            log::Error("Invalid timezone specified: {}", timezone);
            return std::nullopt;
        }
    } else {
        iss.clear();
        iss.seekg(0, std::ios::beg);
        if (iss >> CHRONO_PARSE("%F", tp)) {
            auto canonical_zone = get_canonical_zone_name(timezone);
            if (!canonical_zone.empty()) {
                return TimeWithZone(canonical_zone, tp);
            } else {
                log::Error("Invalid timezone specified: {}", timezone);
                return std::nullopt;
            }
        } else {
            log::Error("Failed to parse datetime string: {}, Underlying error: {}", datetime, iss.fail());
            return std::nullopt;
        }
    }
}

std::optional<std::string> TimeWithZone::find_zone_by_offset(std::chrono::seconds &offset) {
#if defined(__APPLE__)
    for (const auto& zone : tzdb_.zones) {
        const auto& current_offset = zone.get_info(std::chrono::system_clock::now()).offset;
        if (current_offset == offset) {
            return std::string(zone.name());
        }
    }
#else
    for (const auto& zone : std::chrono::get_tzdb().zones) {
        const auto& current_offset = zone.get_info(std::chrono::system_clock::now()).offset;
        if (current_offset == offset) {
            return std::string(zone.name());
        }
    }
#endif
    return std::nullopt;
}

bool TimeWithZone::is_valid_time_zone(std::string_view time_zone) {
#if defined(__APPLE__)
    for (const auto& zone : tzdb_.zones) {
        if (zone.name() == time_zone) {
            return true;
        }
    }
#else
    for (const auto& zone : std::chrono::get_tzdb().zones) {
        if (zone.name() == time_zone) {
            return true;
        }
    }
#endif
    return false;
}

std::string_view TimeWithZone::get_canonical_zone_name(std::string_view time_zone) {
    if (is_valid_time_zone(time_zone)) {
        return time_zone;
    }
    const auto& legacy_map = get_legacy_zone_to_canonical();
    auto result = legacy_map.find(std::string(time_zone));
    if (result != legacy_map.end()) {
        return result->second;
    }
    return "";
}

const TimeWithZone::ZonedTime& TimeWithZone::get_zoned_time() const {
    return zoned_time_;
}

TimeWithZone::LocalTime TimeWithZone::get_local_time() const {
    return zoned_time_.get_local_time();
}

TimeWithZone::SysTime TimeWithZone::get_utc_time() const {
    return zoned_time_.get_sys_time();
}

TimeWithZone::SysTime TimeWithZone::get_sys_time() const {
    return zoned_time_.get_sys_time();
}

TimeWithZone::LocalTime TimeWithZone::get_zone_time(const std::string &zone_name) const {
    if (!is_valid_time_zone(zone_name)) {
        log::Warn("Invalid time zone: '{}'", zone_name);
        return zoned_time_.get_local_time();
    }
    auto dest_time = TimeWithZone::ZonedTime(zone_name, zoned_time_.get_sys_time());
    return dest_time.get_local_time();
}

uint64_t TimeWithZone::get_nano_epoch() const {
    return std::chrono::time_point_cast<std::chrono::nanoseconds>(zoned_time_.get_sys_time()).time_since_epoch().count();
}

uint64_t TimeWithZone::get_milli_epoch() const {
    return std::chrono::time_point_cast<std::chrono::milliseconds>(zoned_time_.get_sys_time()).time_since_epoch().count();
}

uint64_t TimeWithZone::get_micro_epoch() const {
    return std::chrono::time_point_cast<std::chrono::microseconds>(zoned_time_.get_sys_time()).time_since_epoch().count();
}

uint64_t TimeWithZone::get_seconds_epoch() const {
    return std::chrono::time_point_cast<std::chrono::seconds>(zoned_time_.get_sys_time()).time_since_epoch().count();
}

std::vector<uint8_t> TimeWithZone::serialize_to_vector() const {
    auto nano_epoch = get_nano_epoch();
    std::vector<uint8_t> bytes(sizeof(uint64_t));
    std::memcpy(bytes.data(), &nano_epoch, sizeof(uint64_t));
    return bytes;
}

void TimeWithZone::serialize_to_buffer(uint8_t* buffer) const {
    auto nano_epoch = get_nano_epoch();
    std::memcpy(buffer, &nano_epoch, sizeof(uint64_t));
}

std::optional<TimeWithZone> TimeWithZone::unserialize_from_vector(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != sizeof(uint64_t)) {
        log::Error("Vector size must be {} to represent uint64_t for nanoseconds", sizeof(uint64_t));
        return std::nullopt;
    }
    uint64_t nano_epoch = 0;
    std::memcpy(&nano_epoch, bytes.data(), sizeof(uint64_t));
    return TimeWithZone(get_zoned_time(nano_epoch, ""));
}

std::optional<TimeWithZone> TimeWithZone::unserialize_from_buffer(const uint8_t* buffer) {
    uint64_t nano_epoch = 0;
    std::memcpy(&nano_epoch, buffer, sizeof(uint64_t));
    return TimeWithZone(get_zoned_time(nano_epoch, ""));
}

// Use ZonedTime as return type, and only use macros inside function body
TimeWithZone::ZonedTime TimeWithZone::get_zoned_time(uint64_t nano_epoch, const std::string_view zone_name) {
    if (nano_epoch < kMinimumNanosecondsEpoch) {
        log::Error("The nanoseconds epoch value is too small: {}, it may be milliseconds or seconds", nano_epoch);
    }
    std::chrono::nanoseconds duration_since_epoch(nano_epoch);
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time_point_ns(duration_since_epoch);
    if (!zone_name.empty()) {
        return ZonedTime(std::string(zone_name), time_point_ns);
    } else {
        auto tz_ptr = CHRONO_CURRENT_ZONE();
        return ZonedTime(tz_ptr, time_point_ns);
    }
}

const std::map<std::string, std::string>& TimeWithZone::get_legacy_zone_to_canonical() {
    static const std::map<std::string, std::string> legacy_zone_to_canonical = {
        {"US/Eastern", "America/New_York"},
        {"US/Central", "America/Chicago"},
        {"US/Mountain", "America/Denver"},
        {"US/Pacific", "America/Los_Angeles"},
    };
    return legacy_zone_to_canonical;
}

#undef CHRONO_PARSE
#undef CHRONO_CURRENT_ZONE

} // namespace time
} // namespace quanttrader
