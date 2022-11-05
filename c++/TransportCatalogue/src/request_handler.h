#pragma once
#include <optional>
#include "transport_catalogue.h"
#include "domain.h"
#include "map_renderer.h"

namespace transport_catalogue
{
    class RequestHandler {

    public:
        RequestHandler(const TransportCatalogue& db
            , const renderer::MapRenderer& renderer)
            : db_(db), renderer_(renderer)
        {}        

        std::optional<BusInfo> GetBusStat(const std::string_view& bus_name) const;
        
        int GetStopCount(const std::string_view& bus_name) const;

        bool ChekBus(const std::string_view& bus_name) const;
        bool ChekStop(const std::string_view& stop_name) const;

        const std::unordered_set<Bus*> GetBusesByStop(const std::string_view& stop_name) const;        
        const std::vector<Stop*> GetStopsByBus(const std::string_view& bus_name) const;

    private:
        const TransportCatalogue& db_;
        const renderer::MapRenderer& renderer_;
    };
}
