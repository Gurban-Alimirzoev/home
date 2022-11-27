#pragma once

#include "common.h"
#include "formula.h"

template <class... Args>
struct variant_cast_proxy
{
	std::variant<Args...> v;

	template <class... ToArgs>
	operator std::variant<ToArgs...>() const
	{
		return std::visit([](auto&& arg) -> std::variant<ToArgs...> { return arg; },
			v);
	}
};

template <class... Args>
auto variant_cast(const std::variant<Args...>& v) -> variant_cast_proxy<Args...>
{
	return { v };
}

class Impl
{
public:
	virtual std::string GetText() = 0;
	virtual CellInterface::Value GetValue() = 0;
};

class Cell : public CellInterface {
public:
	Cell() = default;
	~Cell();

	void Set(std::string text);
	void Clear();

	Value GetValue() const override;
	std::string GetText() const override;
	
private:
	std::unique_ptr<Impl> impl_;
};