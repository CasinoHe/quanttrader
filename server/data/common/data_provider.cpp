#include "data_provider.h"
#include "resampler/data_resampler.h"
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>

namespace quanttrader {
namespace data {
namespace provider {

std::shared_ptr<DataProvider> DataProvider::resample(BarType target_type, unsigned int target_size) {
    // Return early if we're already at the target timeframe
    if (bar_type_ == target_type && bar_size_ == target_size) {
        return nullptr;
    }
    
    // Check if the current timeframe can be resampled to the target timeframe
    if (!resampler::can_resample(bar_type_, target_type)) {
        logger_->error("Cannot resample from {} to {}", 
            get_bar_type_string(bar_type_, bar_size_), 
            get_bar_type_string(target_type, target_size));
        return nullptr;
    }
    
    // Create a resampled data provider
    auto resampled_provider = std::make_shared<resampler::DataResampler>(
        this->shared_from_this(), target_type, target_size);
    
    if (!resampled_provider->prepare_data()) {
        logger_->error("Failed to prepare resampled data provider");
        return nullptr;
    }
    
    return resampled_provider;
}

bool DataProvider::rewind() {
    // Only allow rewind if we have a bar line
    if (!bar_line_) {
        logger_->error("Cannot rewind data provider: bar line is not initialized");
        return false;
    }
    
    // Reset the bar line to the beginning
    bar_line_->reset();
    return true;
}

std::pair<BarType, unsigned int> DataProvider::get_bar_type_from_string(const std::string &bar_type_str) {
    static const std::regex bar_regex(R"((\d+)\s+(\w+))");
    std::smatch match;
    
    if (!std::regex_match(bar_type_str, match, bar_regex) || match.size() != 3) {
        return {BarType::NONE, 0};
    }
    
    int size = std::stoi(match[1]);
    std::string unit = match[2];
    
    // Convert unit to lowercase for case-insensitive comparison
    std::transform(unit.begin(), unit.end(), unit.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    
    // Handle singular/plural variations
    if (unit == "second" || unit == "seconds" || unit == "sec" || unit == "secs" || unit == "s") {
        return {BarType::Second, static_cast<unsigned int>(size)};
    } else if (unit == "minute" || unit == "minutes" || unit == "min" || unit == "mins" || unit == "m") {
        return {BarType::Minute, static_cast<unsigned int>(size)};
    } else if (unit == "hour" || unit == "hours" || unit == "hr" || unit == "hrs" || unit == "h") {
        return {BarType::Hour, static_cast<unsigned int>(size)};
    } else if (unit == "day" || unit == "days" || unit == "d") {
        return {BarType::Day, static_cast<unsigned int>(size)};
    } else if (unit == "week" || unit == "weeks" || unit == "w") {
        return {BarType::Week, static_cast<unsigned int>(size)};
    } else if (unit == "month" || unit == "months" || unit == "mo") {
        return {BarType::Month, static_cast<unsigned int>(size)};
    }
    
    return {BarType::NONE, 0};
}

std::string DataProvider::get_bar_type_string(BarType type, unsigned int size) {
    std::stringstream ss;
    ss << size << " ";
    
    switch (type) {
        case BarType::Second:
            ss << (size == 1 ? "second" : "seconds");
            break;
        case BarType::Minute:
            ss << (size == 1 ? "minute" : "minutes");
            break;
        case BarType::Hour:
            ss << (size == 1 ? "hour" : "hours");
            break;
        case BarType::Day:
            ss << (size == 1 ? "day" : "days");
            break;
        case BarType::Week:
            ss << (size == 1 ? "week" : "weeks");
            break;
        case BarType::Month:
            ss << (size == 1 ? "month" : "months");
            break;
        default:
            ss << "unknown";
            break;
    }
    
    return ss.str();
}

} // namespace provider
} // namespace data
} // namespace quanttrader