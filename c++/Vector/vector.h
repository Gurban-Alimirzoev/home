#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>
#include <memory>
#include <tuple>
#include <algorithm>

template <typename T>
class RawMemory {
public:
	RawMemory() = default;

	explicit RawMemory(size_t capacity)
		: buffer_(Allocate(capacity))
		, capacity_(capacity) {
	}

	~RawMemory() {
		Deallocate(buffer_);
	}

	RawMemory(const RawMemory&) = delete;
	RawMemory& operator=(const RawMemory& rhs) = delete;

	RawMemory(RawMemory&& other) noexcept
		: buffer_(other.buffer_)
		, capacity_(other.capacity_)
	{
		other.buffer_ = nullptr;
		other.capacity_ = 0;
	}
	RawMemory& operator=(RawMemory&& rhs) noexcept
	{
		buffer_ = rhs.buffer_;
		capacity_ = rhs.capacity_;
		rhs.buffer_ = nullptr;
		rhs.capacity_ = 0u;
		return *this;
	}

	T* operator+(size_t offset) noexcept {
		assert(offset <= capacity_);
		return buffer_ + offset;
	}

	const T* operator+(size_t offset) const noexcept {
		return const_cast<RawMemory&>(*this) + offset;
	}

	const T& operator[](size_t index) const noexcept {
		return const_cast<RawMemory&>(*this)[index];
	}

	T& operator[](size_t index) noexcept {
		assert(index < capacity_);
		return buffer_[index];
	}

	void Swap(RawMemory& other) noexcept {
		std::swap(buffer_, other.buffer_);
		std::swap(capacity_, other.capacity_);
	}

	const T* GetAddress() const noexcept {
		return buffer_;
	}

	T* GetAddress() noexcept {
		return buffer_;
	}

	size_t Capacity() const {
		return capacity_;
	}

private:
	static T* Allocate(size_t n) {
		return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
	}

	static void Deallocate(T* buf) noexcept {
		operator delete(buf);
	}

	T* buffer_ = nullptr;
	size_t capacity_ = 0;
};

template <typename T>
class Vector {
public:
	using iterator = T*;
	using const_iterator = const T*;

	Vector() = default;

	explicit Vector(size_t size)
		: data_(size)
		, size_(size)
	{
		std::uninitialized_value_construct_n(data_.GetAddress(), size);
	}

	Vector(const Vector& other)
		: data_(other.size_)
		, size_(other.size_)  //
	{
		std::uninitialized_copy_n(other.data_.GetAddress(), size_, data_.GetAddress());
	}

	Vector(Vector&& other) noexcept
		: data_(std::move(other.data_))
		, size_(other.size_)
	{
		other.size_ = 0;
	}

	iterator begin() noexcept
	{
		return iterator(data_.GetAddress());
	}
	iterator end() noexcept
	{
		return iterator(data_.GetAddress() + size_);
	}
	const_iterator begin() const noexcept
	{
		return const_iterator(data_.GetAddress());
	}
	const_iterator end() const noexcept
	{
		return const_iterator(data_.GetAddress() + size_);
	}
	const_iterator cbegin() const noexcept
	{
		return const_iterator(data_.GetAddress());
	}
	const_iterator cend() const noexcept
	{
		return const_iterator(data_.GetAddress() + size_);
	}

	Vector& operator=(const Vector& rhs) {
		if (this != &rhs) {
			if (rhs.size_ > data_.Capacity()) {
				Vector rhs_copy(rhs);
				Swap(rhs_copy);
			}
			else {
				//std::copy_n(rhs.begin(), rhs.size_, begin());
				if (size_ > rhs.size_)
				{
					std::copy_n(rhs.begin(), rhs.size_, begin());
					for (size_t i = rhs.size_; i < size_; i++)
					{
						std::destroy_at(data_ + i);
					}
				}
				else
				{
					/*size_t i = 0;
					for (; i < size_; i++)
					{
						data_[i] = (rhs[i]);
					}*/
					std::copy_n(rhs.begin(), size_, begin());
					std::uninitialized_copy_n(rhs.data_.GetAddress() + size_
						, rhs.size_ - size_
						, data_.GetAddress() + size_);
				}
				size_ = rhs.size_;
			}
		}
		return *this;
	}

	Vector& operator=(Vector&& rhs) noexcept
	{
		if (this != &rhs) {
			data_ = std::move(rhs.data_);
			size_ = rhs.size_;
			rhs.size_ = 0u;
		}
		return *this;
	}

	void Swap(Vector& other) noexcept
	{
		std::swap(data_, other.data_);
		std::swap(size_, other.size_);
	}

	size_t Size() const noexcept {
		return size_;
	}

	size_t Capacity() const noexcept {
		return data_.Capacity();
	}

	const T& operator[](size_t index) const noexcept {
		return const_cast<Vector&>(*this)[index];
	}

	T& operator[](size_t index) noexcept {
		assert(index < size_);
		return data_[index];
	}

	void Reserve(size_t new_capacity) {
		if (new_capacity <= data_.Capacity()) {
			return;
		}
		RawMemory<T> new_data(new_capacity);
		if (MustMove()) {
			std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
		}
		else {
			std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
		}
		std::destroy_n(data_.GetAddress(), size_);
		data_.Swap(new_data);
	}

	void Resize(size_t new_size)
	{
		if (new_size > size_)
		{
			Reserve(new_size);
			std::uninitialized_value_construct_n(data_.GetAddress(), new_size - size_);
		}
		else
		{
			for (size_t i = new_size; i < size_; i++)
			{
				std::destroy_at(data_ + i);
			}
		}
		size_ = new_size;
	}
	void PushBack(const T& value)
	{
		EmplaceBack(value);
	}

	void PushBack(T&& value)
	{
		EmplaceBack(std::move(value));
	}

	void PopBack() noexcept
	{
		std::destroy_at(data_ + (--size_));
	}

	template <typename... Args>
	T& EmplaceBack(Args&&... args)
	{
		if (size_ != Capacity())
		{
			new (data_ + size_) T(std::forward<Args>(args)...);
		}
		else
		{
			RawMemory<T> new_data(size_ > 0 ? size_ * 2 : 1);
			new (new_data + size_) T(std::forward<Args>(args)...);
			if (MustMove())
			{
				std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
			}
			else
			{
				std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
			}
			std::destroy_n(data_.GetAddress(), size_);
			data_.Swap(new_data);
		}
		return data_[size_++];
	}

	iterator Insert(const_iterator pos, const T& value)
	{
		return Emplace(pos, value);
	}
	iterator Insert(const_iterator pos, T&& value)
	{
		return Emplace(pos, std::move(value));
	}

	template <typename... Args>
	iterator Emplace(const_iterator pos, Args&&... args)
	{
		iterator iter = const_cast<iterator>(pos);
		int dist = std::distance(begin(), iter);

		if (size_ != Capacity())
		{
			if (iter == nullptr || iter == end())
			{
				if (std::is_nothrow_move_constructible_v<T> || std::is_copy_constructible_v<T>)
				{
					try
					{
						new (data_ + dist) T(std::forward<Args>(args)...);
					}
					catch (...)
					{
						std::destroy_at(data_ + dist);
						throw;
					}
				}
				else
				{
					new (data_ + dist) T(std::forward<Args>(args)...);
				}
			}
			else
			{
				new(end()) T(std::forward<T>(data_[size_]));
				std::move_backward(data_ + dist, end(), std::next(end()));
				std::destroy_at(data_ + size_);
				new (data_ + dist) T(std::forward<Args>(args)...);
			}
		}
		else
		{
			RawMemory<T> new_data(size_ > 0 ? size_ * 2 : 1);
			new (new_data + dist) T(std::forward<Args>(args)...);
			if (MustMove())
			{
				std::uninitialized_move_n(data_.GetAddress(), dist, new_data.GetAddress());
				std::uninitialized_move_n(data_.GetAddress() + dist, size_ - dist, new_data.GetAddress() + dist + 1);
			}
			else
			{
				std::uninitialized_copy_n(data_.GetAddress(), dist, new_data.GetAddress());
				std::uninitialized_copy_n(data_.GetAddress() + dist, size_ - dist, new_data.GetAddress() + dist + 1);
			}
			std::destroy_n(data_.GetAddress(), size_);
			data_.Swap(new_data);
		}
		size_++;
		return iterator(begin() + dist);
	}

	iterator Erase(const_iterator pos) noexcept(std::is_nothrow_move_assignable_v<T>)
	{
		iterator iter = const_cast<iterator>(pos);
		auto dist = std::distance(begin(), iter);
		if (MustMove())
		{
			std::move(std::next(iter), end(), iter);
		}
		else
		{
			std::copy(std::next(iter), end(), iter);
		}
		PopBack();
		return iterator(begin() + dist);
	}

	~Vector() {
		std::destroy_n(data_.GetAddress(), size_);
	}
private:
	RawMemory<T> data_;
	size_t size_ = 0;

	static void CopyConstruct(T* buf, const T& elem) 
	{
		new (buf) T(elem);
	}

	static void Destroy(T* buf) noexcept 
	{
		buf->~T();
	}

	bool constexpr MustMove()
	{
		return std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>;
	}

};