#pragma once
#include <memory>
#include <utility>

namespace raii {
    template <typename Provider>

    class Booking{
    private:
        using BookingId = typename Provider::BookingId;

        Provider* provider_;
        BookingId counter_ = 0;
    public:
        Booking(Provider* p, int counter)
            : provider_(p), counter_(counter)
        {}

        Booking(const Booking&) = delete;
        Booking& operator=(const Booking&) = delete;

        Booking(Booking&& other) noexcept
        {
            provider_ = std::move(other.provider_);
        }

        Booking& operator=(Booking&& other)
        {
            provider_ = std::move(other.provider_);
            return *this;
        }

        ~Booking()
        {
            if (provider_ != nullptr)
            {
                counter_ = provider_->Provider::counter;
                provider_->Provider::CancelOrComplete(*this);
                provider_ = nullptr;
            }
        }

        // Эта функция не требуется в тестах, но в реальной программе она может быть нужна
        BookingId GetId() const
        {
            return Provider::counter;
        }
    };
}