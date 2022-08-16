#pragma once
#include <optional>
#include "transport_catalogue.h"
#include "domain.h"

namespace transport_catalogue
{
    class RequestHandler {

    public:
        // MapRenderer понадобится в следующей части итогового проекта
        //RequestHandler(const TransportCatalogue& db, const renderer::MapRenderer& renderer);

        // Возвращает информацию о маршруте (запрос Bus)
        std::optional<BusInfo> GetBusStat(const std::string_view& bus_name) const;

        // Возвращает маршруты, проходящие через
        const std::unordered_set<std::string> GetBusesByStop(const std::string_view& stop_name) const;

        // Этот метод будет нужен в следующей части итогового проекта
        //svg::Document RenderMap() const;

    private:
        // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
        const TransportCatalogue& db_;
        //const renderer::MapRenderer& renderer_;
    };
}
