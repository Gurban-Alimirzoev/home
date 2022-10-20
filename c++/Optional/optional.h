#pragma once
#include <stdexcept>
#include <utility>

class BadOptionalAccess : public std::exception {
public:
	using exception::exception;

	virtual const char* what() const noexcept override {
		return "Bad optional access";
	}
};

template <typename T>
class Optional {
public:

	Optional() = default;

	Optional(const T& value) 
	{
		value_ = new (data_) T(value);
		is_initialized_ = true;
	}

	Optional(T&& value) 
	{
		value_ = new (data_) T(std::move(value));
		is_initialized_ = true;
	}

	constexpr Optional(Optional const& other) : Optional() 
	{
		if (other.is_initialized_) {
			Reset();
			value_ = new (data_) T(*other.value_);
			is_initialized_ = true;
		}
	}

	constexpr Optional(Optional&& other) : Optional() 
	{
		if (other.is_initialized_) {
			Reset();
			value_ = new (data_) T(std::move(*other.value_));
			is_initialized_ = true;
		}
	}

	Optional& operator=(const T& value)
	{
		if (is_initialized_) {
			*value_ = std::move(value);
		}
		else {
			value_ = new ((void*)std::addressof(data_)) T(value);
			is_initialized_ = true;
		}
		return *this;
	}

	Optional& operator=(T&& rhs)
	{
		if (is_initialized_) {
			*value_ = std::move(rhs);
		}
		else {
			value_ = new ((void*)std::addressof(data_)) T(std::move(rhs));
			is_initialized_ = true;
		}
		return *this;
	}

	Optional& operator=(const Optional& rhs) 
	{
		if (rhs.is_initialized_) {
			if (is_initialized_) {
				*value_ = *rhs.value_;
			}
			else {
				value_ = new (data_) T(*rhs.value_);

				is_initialized_ = true;
			}
		}
		else {
			Reset();
		}
		return *this;
	}


	Optional& operator=(Optional&& rhs)
	{
		if (rhs.is_initialized_) {
			if (is_initialized_) {
				*value_ = std::move(*rhs.value_);
			}
			else {
				value_ = new (data_) T(std::move(*rhs.value_));
				is_initialized_ = true;
			}
		}
		else {
			Reset();
		}
		return *this;
	}

	~Optional()
	{
		Reset();
	}

	bool HasValue() const 
	{
		return is_initialized_;
	}

	T& operator*()&
	{
		return *value_;
	}

	const T& operator*() const&
	{
		return *value_;
	}

	T* operator->() 
	{
		return value_;
	}

	const T* operator->() const {
		return value_;
	}

	T& Value()&
	{
		if (!this->is_initialized_)
		{
			throw BadOptionalAccess();
		}
		return *value_;
	}

	const T& Value() const&
	{
		if (!this->is_initialized_) {
			throw BadOptionalAccess();
		}
		return  *this->value_;
	}

	T&& operator*()&&
	{
		return std::move(* value_);
	}
	T&& Value()&&
	{
		if (!this->is_initialized_) {
			throw BadOptionalAccess();
		}
		return  std::move(* value_);
	}

	void Reset()
	{
		if (is_initialized_) {
			value_->~T();
			is_initialized_ = false;
		}
	}

	template<typename... Args>
	void Emplace(Args&&... args)
	{
		if (is_initialized_)
		{
			Reset();
		}

		value_ = new (data_) T(std::forward<Args>(args)...);
		is_initialized_ = true;
	}

private:
	alignas(T) char data_[sizeof(T)];
	bool is_initialized_ = false;
	T* value_{};
};