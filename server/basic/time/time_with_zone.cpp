#include "time_with_zone.h"
#include "logger/quantlogger.h"

#include <iostream>
#include <sstream>
#include <map>


namespace quanttrader {
namespace time {

namespace qlog = quanttrader::log;

const date::tzdb& TimeWithZone::tzdb_ = date::get_tzdb();

std::string TimeWithZone::to_string() const {
    if (!cached_to_string_) {
        cached_to_string_ = date::format("%F %T%z", zoned_time_);
    }
    return *cached_to_string_;
}

std::string TimeWithZone::to_string_with_offset() const {
    return to_string();
}

std::string TimeWithZone::to_string_with_name() const {
    if (!cached_to_string_with_name_) {
        cached_to_string_with_name_ = date::format("%F %T %Z", zoned_time_);
    }
    return *cached_to_string_with_name_;
}

std::optional<TimeWithZone> TimeWithZone::from_offset_string(const std::string& data) {
    std::istringstream iss(data);
    date::local_time<std::chrono::nanoseconds> tp;
    std::chrono::minutes offset;
    if (iss >> date::parse("%F %T %z", tp, offset)) {
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
    date::local_time<std::chrono::nanoseconds> tp;
    std::string abbrev;
    if (iss >> date::parse("%F %T %Z", tp, abbrev)) {
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
    date::local_time<std::chrono::nanoseconds> tp;
    std::string abbrev;
    if (iss >> date::parse("%Y%m%d %T %Z", tp, abbrev)) {
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
        if (iss >> date::parse("%Y%m%d %T", tp)) {
            return TimeWithZone(zone_name, tp);
        } else {
            iss.clear();
            iss.seekg(0, std::ios::beg);
            if (iss >> date::parse("%Y%m%d", tp)) {
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
    date::local_time<std::chrono::nanoseconds> tp;
    if (iss >> date::parse("%F %T", tp)) {
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
        if (iss >> date::parse("%F", tp)) {
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
    for (const auto& zone : tzdb_.zones) {
        const auto& current_offset = zone.get_info(std::chrono::system_clock::now()).offset;
        if (current_offset == offset) {
            return std::string(zone.name());
        }
    }
    return std::nullopt;
}

bool TimeWithZone::is_valid_time_zone(std::string_view time_zone) {
    for (const auto& zone : tzdb_.zones) {
        if (zone.name() == time_zone) {
            return true;
        }
    }
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

date::local_time<std::chrono::nanoseconds> TimeWithZone::get_local_time() const {
    return zoned_time_.get_local_time();
}

std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> TimeWithZone::get_utc_time() const {
    return zoned_time_.get_sys_time();
}

std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> TimeWithZone::get_sys_time() const {
    return zoned_time_.get_sys_time();
}

date::local_time<std::chrono::nanoseconds> TimeWithZone::get_zone_time(const std::string &zone_name) const {
    if (!is_valid_time_zone(zone_name)) {
        log::Warn("Invalid time zone: '{}'", zone_name);
        return zoned_time_.get_local_time();
    }
    auto dest_time = date::zoned_time<std::chrono::nanoseconds>(zone_name, zoned_time_.get_sys_time());
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

date::zoned_time<std::chrono::nanoseconds> TimeWithZone::get_zoned_time(uint64_t nano_epoch, const std::string_view zone_name) {
    if (nano_epoch < kMinimumNanosecondsEpoch) {
        log::Error("The nanoseconds epoch value is too small: {}, it may be milliseconds or seconds", nano_epoch);
    }
    std::chrono::nanoseconds duration_since_epoch(nano_epoch);
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time_point_ns(duration_since_epoch);
    if (!zone_name.empty()) {
        return date::zoned_time<std::chrono::nanoseconds>(std::string(zone_name), time_point_ns);
    } else {
        auto timezone = date::current_zone();
        return date::zoned_time<std::chrono::nanoseconds>(timezone, time_point_ns);
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

} // namespace time
} // namespace quanttrader
