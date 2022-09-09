#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include "json.h"

namespace json {
	class Builder
	{
	public:
		Builder() = default;

		Builder& Key(std::string);
		Builder& Value(Node::Value);
		Builder& StartDict();
		Builder& StartArray();
		Builder& EndDict();
		Builder& EndArray();
		json::Node Build();

	private:
		Node root_;
		std::vector<Node*> nodes_stack_;
	};

}