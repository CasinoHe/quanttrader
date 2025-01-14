#include "time/time_with_zone.h"
#include "test/test_base.h"

#include <iostream>

namespace qtime = quanttrader::time;

// TODO: change to build and run it under python/lua script
// int main(int argc, const char **argv) {
namespace quanttrader {
namespace test {
class TestTimeWithZone : public TestBase {
public:
    TestTimeWithZone(): TestBase("TestTimeWithZone") {
        register_test<TestTimeWithZone>();
    }

    virtual void run() override {
        // std::string time_string2 = "20240510 09:29:30.786545431US/Eastern";
        std::string time_string2 = "19791231 19:00:00";
        // std::string time_string2 = "20240510 09:29:30 ";
        // std::string time_string2 = "20240510 09:29:30-05:00";
        // std::string time_string2 = "20240510 09:29:30 -05:00";

        std::optional<qtime::TimeWithZone> zone_time_opt = qtime::TimeWithZone::from_ibapi_string(time_string2, "America/New_York");
        if (!zone_time_opt.has_value())
        {
            std::cout << "Failed to parse time string: " << time_string2 << std::endl;
            return;
        }
        auto zone_time = zone_time_opt.value();
        std::cout << "Time with zone: " << zone_time.to_string() << std::endl;
        std::cout << "Time with zone offset: " << zone_time.to_string_with_offset() << std::endl;
        std::cout << "Time with zone name: " << zone_time.to_string_with_name() << std::endl;

        std::cout << "Time in zone Hong Kong: " << zone_time.get_zone_time("Asia/Hong_Kong") << std::endl;
        std::cout << "Time in seconds epoch: " << zone_time.get_seconds_epoch() << std::endl;
        std::cout << "Time in nanoseconds epoch: " << zone_time.get_nano_epoch() << std::endl;

        auto serialized_bytes = zone_time.serialize_to_vector();
        auto deserialized_time = qtime::TimeWithZone::unserialize_from_vector(serialized_bytes).value();
        std::cout << "Deserialized time: " << deserialized_time.to_string() << std::endl;
        std::cout << "Deserialized time in zone Hong Kong: " << deserialized_time.get_zone_time("Asia/Hong_Kong") << std::endl;
        std::cout << "Deserialized time in nanoseconds epoch: " << deserialized_time.get_nano_epoch() << std::endl;

        auto time_with_zone = qtime::TimeWithZone(1736755739, "America/New_York");
        std::cout << "Time with zone: " << time_with_zone.to_string() << std::endl;

        auto time_with_zone2 = qtime::TimeWithZone(1736755739000000000, "America/New_York");
        std::cout << "Time with zone: " << time_with_zone2.to_string() << std::endl;
    }
};

// dummy object to register test
static TestTimeWithZone test_time_with_zone;
}
}
