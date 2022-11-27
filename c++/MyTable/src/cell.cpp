#include "cell.h"

#include <cassert>
#include <iostream>
#include <string>
#include <optional>

class EmptyImpl : public Impl
{
public:
	EmptyImpl() = default;

	std::string GetText() override
	{
		return {};
	}

	CellInterface::Value GetValue() override
	{
		return {};
	}
};

class TextImpl : public Impl
{
public:
	TextImpl(std::string expression)
		:expression_(expression)
	{}
	std::string GetText() override
	{
		return expression_;
	}

	CellInterface::Value GetValue() override
	{
		if (expression_[0] == ESCAPE_SIGN)
		{
			return expression_.substr(1);
		}
		return expression_;
	}

private:
	std::string expression_;
};

class FormulaImpl : public Impl
{
public:
	FormulaImpl(std::string expression)
		:ptr_(ParseFormula(expression))
	{}

	std::string GetText() override
	{
		return "=" + ptr_->GetExpression();
	}

	CellInterface::Value GetValue() override
	{
		return variant_cast(ptr_->Evaluate());
	}
private:
	std::unique_ptr<FormulaInterface> ptr_;
};

// Реализуйте следующие методы

Cell::~Cell() {
	impl_.release();
}

void Cell::Set(std::string text) {
	if (text[0] == FORMULA_SIGN && text.size() > 1)
	{
		impl_ = std::make_unique<FormulaImpl>(std::move(text.substr(1)));
	}
	else
	{
		impl_ = std::make_unique<TextImpl>(std::move(text));
	}
}

void Cell::Clear() {
	impl_ = std::make_unique<EmptyImpl>();
}

Cell::Value Cell::GetValue() const {
	return impl_->GetValue();
}
std::string Cell::GetText() const {
	return impl_->GetText();
}