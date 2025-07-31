#include "transport_catalogue.h"
#include <unordered_set>
#include <stdexcept>

namespace transport {
namespace catalogue {

void TransportCatalogue::AddStop(const std::string& name, geo::Coordinates coords) {
    stops_[name] = {name, coords};
}

void TransportCatalogue::AddBus(const std::string& name, const std::vector<std::string>& stops) {
    buses_[name] = {name, stops};
    auto* bus_ptr = &buses_[name];
    for (const std::string& stop : stops) {
        stop_to_buses_[stop].insert(bus_ptr);
    }
}

std::optional<TransportCatalogue::BusStats> TransportCatalogue::GetBusStats(std::string_view name) const {
    auto it = buses_.find(std::string(name));
    if (it == buses_.end()) {
        return std::nullopt; // Маршрут не найден
    }
    
    const auto& route = it->second;
    const auto& stops = route.stops;
    
    if (stops.empty()) {
        return BusStats{0, 0, 0.0};
    }
    
    // Количество остановок
    int num_stops = stops.size();
    
    // Количество уникальных остановок
    std::set<std::string> unique_stops(stops.begin(), stops.end());
    int unique_count = unique_stops.size();
    
    // Длина маршрута
    double route_length = 0.0;
    for (size_t i = 0; i < stops.size() - 1; ++i) {
        auto stop1_it = stops_.find(stops[i]);
        auto stop2_it = stops_.find(stops[i + 1]);
        if (stop1_it == stops_.end() || stop2_it == stops_.end()) {
            throw std::runtime_error("Остановка не найдена");
        }
        route_length += ComputeDistance(stop1_it->second.coordinates, stop2_it->second.coordinates);
    }
    
    return BusStats{num_stops, unique_count, route_length};
}

std::optional<const std::set<const BusRoute*, BusRoutePtrComparator>*> TransportCatalogue::GetBusesByStop(std::string_view stop_name) const {
    if (stops_.count(std::string(stop_name)) == 0) {
        return std::nullopt; // Остановка не найдена
    }
    auto it = stop_to_buses_.find(std::string(stop_name));
    if (it == stop_to_buses_.end() || it->second.empty()) {
        static const std::set<const BusRoute*, BusRoutePtrComparator> empty_set;
        return &empty_set;
    }
    return &it->second;
}

} // namespace catalogue
} // namespace transport