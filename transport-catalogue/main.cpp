#include "input_reader.h"
#include "stat_reader.h"

#include <string>
#include <iostream>

using namespace transport;

int main() {
    catalogue::TransportCatalogue catalogue;
    input::ProcessBaseRequests(catalogue, std::cin);
    output::ProcessStatRequests(catalogue, std::cin, std::cout);
}