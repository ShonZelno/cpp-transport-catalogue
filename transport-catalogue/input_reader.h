#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <iostream>

#include "geo.h"
#include "transport_catalogue.h"

namespace transport {
namespace input {

struct CommandDescription {
    // Определяет, задана ли команда (поле command непустое)
    explicit operator bool() const {
        return !command.empty();
    }

    bool operator!() const {
        return !operator bool();
    }

    std::string command;      // Название команды
    std::string id;           // id маршрута или остановки
    std::string description;  // Параметры команды
};

class InputReader {
public:
    /**
     * Парсит строку в структуру CommandDescription и сохраняет результат в commands_
     */
    void ParseLine(std::string_view line);

    /**
     * Наполняет данными транспортный справочник, используя команды из commands_
     */
    void ApplyCommands(catalogue::TransportCatalogue& catalogue) const;

private:
    std::vector<CommandDescription> commands_;
};

std::string_view Trim(std::string_view str);
std::vector<std::string_view> Split(std::string_view string, char delim);
std::vector<std::string_view> ParseRoute(std::string_view route);
geo::Coordinates ParseCoordinates(std::string_view str);

void ProcessBaseRequests(catalogue::TransportCatalogue& catalogue, std::istream& input);

} // namespace input
} // namespace transport