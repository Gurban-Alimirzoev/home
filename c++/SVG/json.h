#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <variant>

namespace json {

class Node;
// Сохраните объявления Dict и Array без изменения
using Dict = std::map<std::string, Node>;
using Array = std::vector<Node>;

// Эта ошибка должна выбрасываться при ошибках парсинга JSON
class ParsingError : public std::runtime_error {
public:
    using runtime_error::runtime_error;
};

struct PrintContext {
    std::ostream& out;
    int indent_step = 4;
    int indent = 0;

    void PrintIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    // Возвращает новый контекст вывода с увеличенным смещением
    PrintContext Indented() const {
        return { out, indent_step, indent_step + indent };
    }
};

using Value = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

struct NodePrinter {

    void operator()(std::nullptr_t) const
    {
        out << "null"sv;
    }
    void operator()(Array value_) const
    {        
        out << value_[0].GetValue();
        for (int i = 1; i < value_.size(); i++)
        {
            out << ", " << value_[i];
        }
    }
    void operator()(Dict value_) const
    {
        //dict = move(value_);
    }
    void operator()(bool value_) const
    {
        out << value_;
    }
    void operator()(int value_) const
    {
        out << value_;
    }
    void operator()(double value_) const
    {
        out << value_;
    }
    void operator()(std::string value_) const {
        out << value_;
    }

    std::ostream& out;
};

class Node {
public:
   /* Реализуйте Node, используя std::variant */

    explicit Node(Array array);
    explicit Node(Dict map);
    explicit Node(int value);
    explicit Node(std::string value);

    bool IsInt() const;
    //Возвращает true, если в Node хранится int либо double.
    bool IsDouble() const;
    //Возвращает true, если в Node хранится double.
    bool IsPureDouble() const;
    bool IsBool() const;
    bool IsString() const;
    bool IsNull() const;
    bool IsArray() const;
    bool IsMap() const;

    int AsInt() const;
    bool AsBool() const;
    //Возвращает значение типа double, если внутри хранится double либо int.В последнем случае возвращается приведённое в double значение.
    double AsDouble() const;
    const std::string& AsString() const;
    const Array& AsArray() const;
    const Dict& AsMap() const;
    const Value GetValue() const;
private:
    Value value_;

    /*Array as_array_;
    Dict as_map_;
    int as_int_ = 0;
    std::string as_string_;*/
};

void PrintNode(const Node& node, std::ostream& out) {
    std::visit(NodePrinter{ out }, node.GetValue());
}

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};

Document Load(std::istream& input);

void Print(const Document& doc, std::ostream& output);

}  // namespace json