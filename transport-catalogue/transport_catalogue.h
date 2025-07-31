#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <set>

#include "geo.h"


namespace transport {
namespace catalogue {

struct Stop {
    std::string name;        // Название остановки
    geo::Coordinates coordinates; // Широта и долгота
};

struct BusRoute {
    std::string name;         // Название маршрута
    std::vector<std::string> stops; // Список названий остановок
};

struct BusRoutePtrComparator {
    bool operator()(const BusRoute* lhs, const BusRoute* rhs) const {
        return lhs->name < rhs->name;
    }
};

class TransportCatalogue {
public:
    // Добавление остановки в базу
    void AddStop(const std::string& name, geo::Coordinates coords);
    
    // Добавление маршрута в базу
    void AddBus(const std::string& name, const std::vector<std::string>& stops);
    
    // Структура для хранения статистики маршрута
    struct BusStats {
        int num_stops;      // Количество остановок на маршруте
        int unique_stops;   // Количество уникальных остановок
        double route_length; // Длина маршрута в метрах
    };
    
    // Получение информации о маршруте
    std::optional<BusStats> GetBusStats(std::string_view name) const;

    // Получение информации об остановке
    std::optional<const std::set<const BusRoute*, BusRoutePtrComparator>*> GetBusesByStop(std::string_view stop_name) const;


private:
    std::unordered_map<std::string, Stop> stops_;     // Хранилище остановок
    std::unordered_map<std::string, BusRoute> buses_; // Хранилище маршрутов
    std::unordered_map<std::string, std::set<const BusRoute*, BusRoutePtrComparator>> stop_to_buses_; // Хранилище автобусов для остановок 
};

} // namespace catalogue
} // namespace transport