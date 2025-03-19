#include "time_with_zone.h"
#include "logger/quantlogger.h"

#include <iostream>
#include <sstream>
#include <map>


namespace quanttrader {
namespace time {

namespace qlog = quanttrader::log;

const std::chrono::tzdb& TimeWithZone::tzdb_ = std::chrono::get_tzdb();

TimeWithZone::TimeWithZone(std::string_view time_zone, std::chrono::local_time<std::chrono::nanoseconds> localTime) : zoned_time_(time_zone, localTime) {
}

TimeWithZone::TimeWithZone(std::string_view time_zone, std::chrono::sys_time<std::chrono::nanoseconds> sys_time): zoned_time_(time_zone, sys_time) {
}

TimeWithZone::TimeWithZone(uint64_t nanoseconds_epoch, const std::string& zone_name) {
    zoned_time_ = get_zoned_time(nanoseconds_epoch, zone_name);
}

std::string TimeWithZone::to_string() const {
    if (!cached_to_string_)
    {
        cached_to_string_ = std::format("{:%F %T%z}", zoned_time_);
    }
    return *cached_to_string_;
}

std::string TimeWithZone::to_string_with_offset() const {
    return to_string();
}

std::string TimeWithZone::to_string_with_name() const {
    if (!cached_to_string_with_name_)
    {
        cached_to_string_with_name_ = std::format("{:%F %T%Z}", zoned_time_);
    }
    return *cached_to_string_with_name_;
}

std::optional<TimeWithZone> TimeWithZone::from_offset_string(const std::string& data) {
    std::istringstream iss(data);
    std::chrono::local_time<std::chrono::nanoseconds> tp;
    std::chrono::minutes offset;

    if (iss >> std::chrono::parse("%F %T %z", tp, offset)) {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(offset);
        auto zone_name = TimeWithZone::find_zone_by_offset(seconds);
        if (zone_name.has_value()) {
            return TimeWithZone(zone_name.value(), tp);
        } else {
            qlog::Error("Failed to find zone for offset: {} {}", data, offset);
            return std::nullopt;
        }
    } else {
        qlog::Error("Failed to parse offset time string: {}, Underlying error: {}", data, iss.fail());
        return std::nullopt;
    }
}

std::optional<TimeWithZone> TimeWithZone::from_zone_string(const std::string& data) {
    std::istringstream iss(data);
    std::chrono::local_time<std::chrono::nanoseconds> tp;
    std::string abbrev;

    if (iss >> std::chrono::parse("%F %T %Z", tp, abbrev)) {
        auto zone_name = TimeWithZone::get_canonical_zone_name(abbrev);
        if (!zone_name.empty()) {
            return TimeWithZone(zone_name, tp);
        }
        else {
            qlog::Error("Failed to find zone for abbrev: {} {}", data, abbrev);
            return std::nullopt;
        }
    } else {
        qlog::Error("Failed to parse offset time string: {}, Underlying error: {}", data, iss.fail());
        return std::nullopt;
    }
}

std::optional<TimeWithZone> TimeWithZone::from_ibapi_string(const std::string& data, const std::string_view zone_name) {
    std::istringstream iss(data);
    std::chrono::local_time<std::chrono::nanoseconds> tp;
    std::string abbrev;

    if (iss >> std::chrono::parse("%Y%m%d %T %Z", tp, abbrev)) {
        abbrev = TimeWithZone::get_canonical_zone_name(abbrev);
        if (!abbrev.empty()) {
            return TimeWithZone(abbrev, tp);
        }
        else {
            qlog::Warn("Failed to find zone for abbrev: {} {}", data, abbrev);
            return TimeWithZone(zone_name, tp);
        }
    }
    else {
        iss.clear();
        iss.seekg(0, std::ios::beg);
        if (iss >> std::chrono::parse("%Y%m%d %T", tp)) {
            return TimeWithZone(zone_name, tp);
        } else {
            iss.clear();
            iss.seekg(0, std::ios::beg);
            if (iss >> std::chrono::parse("%Y%m%d", tp)) {
                return TimeWithZone(zone_name, tp);
            } else {
                qlog::Error("Failed to parse offset time string: {}, Underlying error: {}", data, iss.fail());
                return std::nullopt;
            }
        }
    }
}

std::optional<std::string> TimeWithZone::find_zone_by_offset(std::chrono::seconds &offset) {
    for (const auto& zone : tzdb_.zones) {
        // Get the current offset for the zone
        const auto& current_offset = zone.get_info(std::chrono::system_clock::now()).offset;

        if (current_offset == offset) {
            return std::string(zone.name()); // Return the matching zone name
        }
    }

    return std::nullopt; // No matching zone found
}

bool TimeWithZone::is_valid_time_zone(const std::string_view time_zone) {
    for (const auto& zone : tzdb_.zones) {
        if (zone.name() == time_zone) {
            return true;
        }
    }
    return false;
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

std::string_view TimeWithZone::get_canonical_zone_name(std::string_view time_zone) {
    if (TimeWithZone::is_valid_time_zone(std::string(time_zone))) {
        return time_zone;
    }

    const auto& legacy_map = get_legacy_zone_to_canonical();
    auto result = legacy_map.find(std::string(time_zone));
    if (result != legacy_map.end()) {
        return result->second;
    }
    else {
        return "";
    }
}

std::chrono::local_time<std::chrono::nanoseconds> TimeWithZone::get_zone_time(const std::string &zone_name) const {
    if (!is_valid_time_zone(zone_name)) {
        qlog::Warn(std::format("Invalid time zone: '{}'", zone_name));
        return zoned_time_.get_local_time(); // Fallback to current local time
    }
    auto dest_time = std::chrono::zoned_time{zone_name, zoned_time_.get_sys_time()};
    return dest_time.get_local_time();
}

std::vector<uint8_t> TimeWithZone::serialize_to_vector() const {
    auto nano_epoch = get_nano_epoch();
    std::vector<uint8_t> bytes(8); // uint64_t is 8 bytes
    std::memcpy(bytes.data(), &nano_epoch, sizeof(uint64_t));
    return bytes;
}

void TimeWithZone::serialize_to_buffer(uint8_t* buffer) const {
    auto nano_epoch = get_nano_epoch();
    std::memcpy(buffer, &nano_epoch, sizeof(uint64_t));
}

std::optional<TimeWithZone> TimeWithZone::unserialize_from_vector(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != 8) {
        qlog::Error("Vector size must be 8 to represent uint64_t for nanoseconds");
        return std::nullopt;
    }

    uint64_t nano_epoch = 0;
    for (int i = 0; i < 8; ++i) {
        nano_epoch |= static_cast<uint64_t>(bytes[i]) << (i * 8);

    }

    return TimeWithZone(get_zoned_time(nano_epoch, ""));
}

std::optional<TimeWithZone> TimeWithZone::unserialize_from_buffer(const uint8_t* buffer) {
    uint64_t nano_epoch = 0;
    for (int i = 0; i < 8; ++i) {
        nano_epoch |= static_cast<uint64_t>(static_cast<uint8_t>(buffer[i])) << (i * 8);
    }

    return TimeWithZone(get_zoned_time(nano_epoch, ""));
}

std::chrono::zoned_time<std::chrono::nanoseconds> TimeWithZone::get_zoned_time(uint64_t nano_epoch, const std::string_view zone_name) {

    // to avoid send a millisecond epoch or seconds epoch by mistake, we check the nano_epoch value
    // if the value is smaller than 10^18, we assume it is not a nanoseconds epoch
    if (nano_epoch < kMinimumNanosecondsEpoch) {
        qlog::Error(std::format("The nanoseconds epoch value is too small: {}, it maybe milliseconds or seconds", nano_epoch));
    }

    std::chrono::nanoseconds duration_since_epoch(nano_epoch);
    // Convert the duration to a time_point with nanosecond precision
    std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time_point_ns(duration_since_epoch);

    // Create a zoned_time for a specific time zone (e.g., UTC)
    if (!zone_name.empty()) {
        return std::chrono::zoned_time<std::chrono::nanoseconds>(zone_name, time_point_ns);
    } else {
        auto timezone = std::chrono::current_zone(); // Gets the current system time zone
        return std::chrono::zoned_time<std::chrono::nanoseconds>(timezone, time_point_ns);
    }
}

} // namespace time
} // namespace quanttrader
