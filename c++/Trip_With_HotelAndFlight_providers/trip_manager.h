#pragma once
#include "flight_provider.h"
#include "hotel_provider.h"

#include <string>
#include <vector>

class Trip {
public:

    Trip(HotelProvider& hp, FlightProvider& fp)
        :hp_(hp), fp_(fp)
    {}

    Trip(const Trip&) = delete;
    Trip& operator=(const Trip&) = delete;

    Trip(Trip&&) = default;
    Trip& operator=(Trip&&) = default;

    void Cancel()
    {
        for (auto flyght : flights)
        {
            fp_.Cancel(flyght);
        }
        for (auto flyght : hotels)
        {
            hp_.Cancel(flyght);
        }
    }

    ~Trip()
    {
        Cancel();
    }

    std::vector<int> flights;
    std::vector<int> hotels;
private:
    HotelProvider& hp_;
    FlightProvider& fp_;
};

class TripManager {
public:
    using BookingId = std::string;
    struct BookingData {
        std::string city_from;
        std::string city_to;
        std::string date_from;
        std::string date_to;
    };

    TripManager() = default;

    Trip Book(const BookingData& data) {
        Trip trip(hotel_provider_, flight_provider_);
        {
            FlightProvider::BookingData flight_booking_data;
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        {
            HotelProvider::BookingData hotel_booking_data;
            trip.hotels.push_back(hotel_provider_.Book(hotel_booking_data));
        }
        {
            FlightProvider::BookingData flight_booking_data;
            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
        }
        return trip;
    }

    void Cancel(Trip& trip) {
        trip.Cancel();
    }

private:
    HotelProvider hotel_provider_;
    FlightProvider flight_provider_;
};