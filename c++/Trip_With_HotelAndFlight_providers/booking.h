#pragma once
#include <memory>
#include <utility>

namespace raii {
    template <typename Provider>

    class Booking{
    private:
        using BookingId = typename Provider::BookingId;

        std::unique_ptr<Provider> provider_;
        int counter;

    public:
        Booking(Provider* p, int counter)
            : provider_(p), counter(counter)
        {}

        Booking(const Booking&) = delete;
        Booking& operator=(const Booking&) = delete;

        Booking(Booking&& other)
        {
            provider_ = std::move(other.provider_);
        }

        Booking& operator=(Booking&& other)
        {
            provider_ = std::move(other.provider_);
        }

        ~Booking()
        {
            provider_->Provider::CancelOrComplete(*this);
            provider_.reset();
        }

        // Эта функция не требуется в тестах, но в реальной программе она может быть нужна
        BookingId GetId() const
        {
            return counter;
        }
    };
}