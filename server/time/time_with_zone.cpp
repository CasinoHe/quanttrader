#include "time_with_zone.h"
#include "logger/quantlogger.h"

#include <iostream>
#include <sstream>
#include <map>

const static std::map<std::string, std::string> legacy_zone_to_canonical = {
    {"US/Eastern", "America/New_York"},
    {"US/Central", "America/Chicago"},
    {"US/Mountain", "America/Denver"},
    {"US/Pacific", "America/Los_Angeles"},
};


namespace quanttrader {
namespace time {

    TimeWithZone::TimeWithZone(std::string time_zone, std::chrono::local_time<std::chrono::nanoseconds> localTime) : zoned_time_(time_zone, localTime) {
    }

    std::string TimeWithZone::to_string() const {
        return std::format("{:%F %T%z}", zoned_time_);
    }

    std::string TimeWithZone::to_string_with_offset() const {
        return std::format("{:%F %T%z}", zoned_time_);
    }

    std::string TimeWithZone::to_string_with_name() const {
        return std::format("{:%F %T%Z}", zoned_time_);
    }

    std::shared_ptr<TimeWithZone> TimeWithZone::from_offset_string(const std::string& data) {
        std::istringstream iss(data);
        std::chrono::local_time<std::chrono::nanoseconds> tp;
        std::chrono::minutes offset;

        if (iss >> std::chrono::parse("%F %T %z", tp, offset)) {
            auto seconds = std::chrono::duration_cast<std::chrono::seconds>(offset);
            auto zone_name = TimeWithZone::find_zone_by_offset(seconds);
            if (zone_name.has_value()) {
                return std::make_shared<TimeWithZone>(zone_name.value(), tp);
            }
            else {
                quanttrader::log::Error(std::format("Failed to find zone for offset: {} {}", data, offset));
                return nullptr;
            }
        }
        else
        {
            quanttrader::log::Error(std::format("Failed to parse offset time string: {}", data));
            return nullptr;
        }
    }

    std::shared_ptr<TimeWithZone> TimeWithZone::from_zone_string(const std::string& data) {
        std::istringstream iss(data);
        std::chrono::local_time<std::chrono::nanoseconds> tp;
        std::string abbrev;

        if (iss >> std::chrono::parse("%F %T %Z", tp, abbrev)) {
            auto zone_name = TimeWithZone::get_canonical_zone_name(abbrev);
            if (!zone_name.empty()) {
                return std::make_shared<TimeWithZone>(zone_name, tp);
            }
            else {
                quanttrader::log::Error(std::format("Failed to find zone for abbrev: {} {}", data, abbrev));
                return nullptr;
            }
        }
        else
        {
            quanttrader::log::Error(std::format("Failed to parse offset time string: {}", data));
            return nullptr;
        }
    }

    std::shared_ptr<TimeWithZone> TimeWithZone::from_ibapi_string(const std::string& data, const std::string &zone_name) {
        std::istringstream iss(data);
        std::chrono::local_time<std::chrono::nanoseconds> tp;
        std::string abbrev;

        if (iss >> std::chrono::parse("%Y%m%d %T %Z", tp, abbrev)) {
            abbrev = TimeWithZone::get_canonical_zone_name(abbrev);
            if (!abbrev.empty()) {
                return std::make_shared<TimeWithZone>(abbrev, tp);
            }
            else {
                log::Warn(std::format("Failed to find zone for abbrev: {} {}", data, abbrev));
                return std::make_shared<TimeWithZone>(zone_name, tp);
            }
        }
        else if (iss >> std::chrono::parse("%Y%m%d %T", tp)) {
            return std::make_shared<TimeWithZone>(zone_name, tp);
        }
        else
        {
            quanttrader::log::Error(std::format("Failed to parse offset time string: {}", data));
            return nullptr;
        }
    }

    std::optional<std::string> TimeWithZone::find_zone_by_offset(std::chrono::seconds &offset) {
        const auto& tzdb = std::chrono::get_tzdb();

        for (const auto& zone : tzdb.zones) {
            // Get the current offset for the zone
            const auto& current_offset = zone.get_info(std::chrono::system_clock::now()).offset;

            if (current_offset == offset) {
                return std::string(zone.name()); // Return the matching zone name
            }
        }

        return std::nullopt; // No matching zone found
    }

    bool TimeWithZone::is_valid_time_zone(const std::string& time_zone) {
        const auto& tzdb =  std::chrono::get_tzdb();

        for (const auto& zone : tzdb.zones) {
            if (zone.name() == time_zone) {
                return true;
            }
        }
        return false;
    }

    std::string TimeWithZone::get_canonical_zone_name(const std::string& time_zone) {
        if (TimeWithZone::is_valid_time_zone(time_zone)) {
            return time_zone;
        }

        auto result = legacy_zone_to_canonical.find(time_zone);
        if (result != legacy_zone_to_canonical.end()) {
            return result->second;
        }
        else {
            return "";
        }
    }

    std::chrono::local_time<std::chrono::nanoseconds> TimeWithZone::get_zone_time(const std::string &zone_name) const {
        auto dest_time = std::chrono::zoned_time{zone_name, zoned_time_.get_sys_time()};
        return dest_time.get_local_time();
    }

    std::vector<uint8_t> TimeWithZone::serialize_to_vector() const {
        auto nano_epoch = get_nano_epoch();
        std::vector<uint8_t> bytes(8); // uint64_t is 8 bytes
        for (int i = 0; i < 8; ++i) {
            bytes[i] = static_cast<uint8_t>((nano_epoch >> (i * 8)) & 0xFF); // Extract each byte
        }
        return bytes;
    }

    void TimeWithZone::serialize_to_buffer(uint8_t* buffer) const {
        auto nano_epoch = get_nano_epoch();
        std::memcpy(buffer, &nano_epoch, sizeof(uint64_t));
    }

    std::shared_ptr<TimeWithZone> TimeWithZone::unserialize_from_vector(const std::vector<uint8_t>& bytes) {
        if (bytes.size() != 8) {
            log::Error("Vector size must be 8 to represent uint64_t for nanoseconds");
            return nullptr;
        }

        uint64_t nano_epoch = 0;
        for (int i = 0; i < 8; ++i) {
            nano_epoch |= static_cast<uint64_t>(bytes[i]) << (i * 8);

        }

        std::chrono::nanoseconds duration_since_epoch(nano_epoch);
        // Convert the duration to a time_point with nanosecond precision
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time_point_ns(duration_since_epoch);

        // Create a zoned_time for a specific time zone (e.g., UTC)
        auto timezone = std::chrono::current_zone(); // Gets the current system time zone
        std::chrono::zoned_time<std::chrono::nanoseconds> ztime(timezone, time_point_ns);

        return std::make_shared<TimeWithZone>(std::move(ztime));
    }

    std::shared_ptr<TimeWithZone> TimeWithZone::unserialize_from_buffer(const uint8_t* buffer) {
        uint64_t nano_epoch = 0;
        for (int i = 0; i < 8; ++i) {
            nano_epoch |= static_cast<uint64_t>(static_cast<uint8_t>(buffer[i])) << (i * 8);
        }

        std::chrono::nanoseconds duration_since_epoch(nano_epoch);
        // Convert the duration to a time_point with nanosecond precision
        std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time_point_ns(duration_since_epoch);

        // Create a zoned_time for a specific time zone (e.g., UTC)
        auto timezone = std::chrono::current_zone(); // Gets the current system time zone
        std::chrono::zoned_time<std::chrono::nanoseconds> ztime(timezone, time_point_ns);

        return std::make_shared<TimeWithZone>(std::move(ztime));
    }

} // namespace time
} // namespace quanttrader
