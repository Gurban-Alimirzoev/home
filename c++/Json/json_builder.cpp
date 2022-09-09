#include "json_builder.h"

using namespace std;
namespace json {

	Builder& Builder::Key(std::string key)
	{
		root_.AsDict().at(key);
		return *this;
	}
	Builder& Builder::Value(Node::Value value)
	{
		if (root_.IsArray())
		{
			get<Array>(root_.GetValue()).emplace_back(value);
		}
		else if (root_.IsDict())
		{
			get<Dict>(root_.GetValue()).emplace(value);
		}
		else
		{
			nodes_stack_.push_back(get<Node>(value));
		}
	}
		
	Builder& Builder::StartDict()
	{
		
	}
	Builder& Builder::StartArray()
	{

	}
	Builder& Builder::EndDict()
	{

	}
	Builder& Builder::EndArray()
	{

	}
	json::Node Builder::Build()
	{

	}

}