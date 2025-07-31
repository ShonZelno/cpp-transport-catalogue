#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

namespace transport {
namespace output {

void ParseAndPrintStat(const catalogue::TransportCatalogue& tansport_catalogue, std::string_view request,
                       std::ostream& output);

} // namespace output
} // namespace transport