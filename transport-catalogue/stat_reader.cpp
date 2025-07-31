#include "stat_reader.h"
#include "input_reader.h"
#include <ios>   
#include <iomanip> 
#include <string>
#include <iostream>
#include <algorithm> 

namespace transport {
namespace output {

void ParseAndPrintStat(const catalogue::TransportCatalogue& tansport_catalogue, std::string_view request, std::ostream& output) {
    auto trimmed = input::Trim(request);
    if (trimmed.substr(0, 4) == "Bus ") {
        std::string bus_name = std::string(trimmed.substr(4));
        auto stats = tansport_catalogue.GetBusStats(bus_name); // Исправлено с catalogue на tansport_catalogue
        if (stats) {
            output << "Bus " << bus_name << ": " 
                   << stats->num_stops << " stops on route, "
                   << stats->unique_stops << " unique stops, "
                   << std::fixed << std::setprecision(6) << stats->route_length << " route length\n";
        } else {
            output << "Bus " << bus_name << ": not found\n";
        }
    } else if (trimmed.substr(0, 5) == "Stop ") {
        std::string stop_name = std::string(trimmed.substr(5));
        auto result = tansport_catalogue.GetBusesByStop(stop_name);
        if (!result) {
            output << "Stop " << stop_name << ": not found\n";
        } else if ((*result)->empty()) {
            output << "Stop " << stop_name << ": no buses\n";
        } else {
            std::vector<std::string> buses((*result)->begin(), (*result)->end());
            std::sort(buses.begin(), buses.end());
            output << "Stop " << stop_name << ": buses";
            for (const auto& bus : buses) {
                output << " " << bus;
            }
            output << "\n";
        }
    }
}

} // namespace output
} // namespace transport