#include "json_builder.h"

using namespace std;
using namespace std::string_literals;
namespace json {

	Builder::Builder()
		: root_(nullptr)
		, nodes_stack_{ &root_ }
		, key_(std::nullopt) {}

	Builder& Builder::Key(std::string key) {
		if (nodes_stack_.back()->IsDict() && !key_) {
			key_ = std::move(key);
		}
		else {
			throw std::logic_error("The command \"Key\" was called in wrong place"s);
		}
		return *this;
	}

	Builder& Builder::Value(Node::Value value) {
		AddElement(value);
		return *this;
	}

	Builder& Builder::StartDict() {
		if (IsEmpty() || key_ || PrevIsArray()) {
			nodes_stack_.emplace_back(std::move(AddElement(Dict())));
			return *this;
		}
		else {
			throw std::logic_error("The command \"StartDict\" was called in wrong place"s);
		}
	}

	Builder& Builder::StartArray() {
		if (IsEmpty() || key_ || PrevIsArray()) {
			nodes_stack_.emplace_back(std::move(AddElement(Array())));
			return *this;
		}
		else {
			throw std::logic_error("The command \"StartArray\" was called in wrong place"s);
		}
	}

	Builder& Builder::EndDict() {
		if (PrevIsDict()) {
			nodes_stack_.pop_back();
			return *this;
		}
		else {
			throw std::logic_error("The command \"EndDict\" was called in wrong place"s);
		}
	}

	Builder& Builder::EndArray() {
		if (PrevIsArray()) {
			nodes_stack_.pop_back();
			return *this;
		}
		else {
			throw std::logic_error("The command \"EndArray\" was called in wrong place"s);
		}
	}

	json::Node Builder::Build() {
		if (nodes_stack_.size() == 1 && !nodes_stack_.back()->IsNull()) {
			return root_;
		}
		else {
			throw std::logic_error("The command \"Build\" was called in wrong place"s);
		}
	}

	Node* Builder::AddElement(Node::Value value) {
		if (IsEmpty()) {
			nodes_stack_.back()->GetValue() = std::move(value);
			return nodes_stack_.back();
		}
		else if (PrevIsArray()) {
			Array& link_arr = std::get<Array>(nodes_stack_.back()->GetValue());
			return &link_arr.emplace_back(value);
		}
		else if (PrevIsDict() && key_) {
			Dict& link_dict = std::get<Dict>(nodes_stack_.back()->GetValue());
			auto ptr = link_dict.emplace(make_pair(key_.value(), value));
			key_ = nullopt;
			return &ptr.first->second;
		}
		else {
			throw std::logic_error("The command \"Value\" was called in wrong place"s);
		}
	}

	bool Builder::IsEmpty() {
		return nodes_stack_.back()->IsNull();
	}

	bool Builder::PrevIsArray() {
		return nodes_stack_.back()->IsArray();
	}

	bool Builder::PrevIsDict() {
		return nodes_stack_.back()->IsDict();
	}
}