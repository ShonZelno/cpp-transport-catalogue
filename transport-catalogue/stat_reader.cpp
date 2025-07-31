#include "stat_reader.h"
#include "input_reader.h"
#include <ios>   
#include <iomanip> 
#include <string>
#include <iostream>
#include <algorithm> 

namespace transport {
namespace output {

void ParseAndPrintStat(const catalogue::TransportCatalogue& transport_catalogue, std::string_view request, std::ostream& output) {
    auto trimmed = input::Trim(request);
    if (trimmed.substr(0, 4) == "Bus ") {
        std::string bus_name = std::string(trimmed.substr(4));
        auto stats = transport_catalogue.GetBusStats(bus_name);
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
        auto result = transport_catalogue.GetBusesByStop(stop_name);
        if (!result) {
            output << "Stop " << stop_name << ": not found\n";
        } else if ((*result)->empty()) {
            output << "Stop " << stop_name << ": no buses\n";
        } else {
            output << "Stop " << stop_name << ": buses";
            for (const auto& bus : **result) {
                output << " " << bus->name;
            }
            output << "\n";
        }
    }
}

void ProcessStatRequests(const catalogue::TransportCatalogue& catalogue, std::istream& input, std::ostream& output) {
    int stat_request_count;
    input >> stat_request_count >> std::ws;
    for (int i = 0; i < stat_request_count; ++i) {
        std::string line;
        std::getline(input, line);
        ParseAndPrintStat(catalogue, line, output);
    }
}

} // namespace output
} // namespace transport