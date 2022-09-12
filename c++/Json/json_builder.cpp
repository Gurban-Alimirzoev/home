#include "json_builder.h"

using namespace std;
namespace json {

	Builder& Builder::Key(std::string key)
	{
		key_and_value.first = key;
		return *this;
	}

	Builder& Builder::Value(Node::Value value)
	{
		Node* node = (nodes_stack_.size() == 0) ? &root_ : nodes_stack_.back();
		if (node->IsArray()) 
		{
			std::get<Array>(node->GetValue()).emplace_back(move(value));
		}
		else if (node->IsDict())
		{
			key_and_value.second.GetValue() = value;
			get<Dict>(node->GetValue()).emplace(move(key_and_value));
		}
		else
		{
			node->GetValue() =  move(value);
		}
	}
		
	Builder& Builder::StartDict()
	{
		Dict dict;
		root_.GetValue() = move(dict);
	}
	Builder& Builder::StartArray()
	{
		Array array;
		root_.GetValue() = move(array);
	}
	Builder& Builder::EndDict()
	{
		nodes_stack_.emplace_back(move(root_));
	}
	Builder& Builder::EndArray()
	{
		nodes_stack_.emplace_back(move(root_));
	}
	json::Node Builder::Build()
	{
		return root_;
	}

}