#pragma once
#include <memory>
#include <utility>

namespace raii {
    template <typename Provider>

    class Booking{
    private:
        using BookingId = typename Provider::BookingId;

        std::unique_ptr<Provider> provider_;
        BookingId counter_ = 0;
    public:
        Booking(Provider* p, int counter)
            : provider_(p), counter_(counter)
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
            counter_ = provider_->Provider::counter;
            provider_->Provider::CancelOrComplete(*this);
            //provider_.reset();
            delete (provider_.get());
        }

        // Эта функция не требуется в тестах, но в реальной программе она может быть нужна
        BookingId GetId() const
        {
            return Provider::counter;
        }
    };
}