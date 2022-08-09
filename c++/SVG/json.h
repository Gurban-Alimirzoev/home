#pragma once

#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <variant>
#include <initializer_list>


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

using Value = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

// Контекст вывода, хранит ссылку на поток вывода и текущий отсуп
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

class Node {
public:
   /* Реализуйте Node, используя std::variant */

    Node() = default;
    explicit Node(Value value);

    template <typename Node_type>
    Node(Node_type value)
        : value_(value)
    {
    }

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
    int GetIndex() const;

    friend bool operator==(const Node& node_right, const Node& node_left);
    friend bool operator!=(const Node& node_right, const Node& node_left);
private:
    Value value_;
};

bool operator==(const Node& node_right, const Node& node_left);
bool operator!=(const Node& node_right, const Node& node_left);

void PrintValue(const int value, const PrintContext& ctx);
void PrintValue(const double value, const PrintContext& ctx);
void PrintValue(std::nullptr_t, const PrintContext& ctx);
void PrintValue(const bool value, const PrintContext& ctx);
void PrintValue(const std::string value, const PrintContext& ctx);
void PrintValue(const Array array, const PrintContext& ctx);
void PrintValue(const Dict dict, const PrintContext& ctx);

void PrintNode(const Node& node, const PrintContext& cont);

class Document {
public:
    explicit Document(Node root);

    const Node& GetRoot() const;

private:
    Node root_;
};

Document Load(std::istream& input);

bool operator==(const Document& doc_right, const Document& doc_left);
bool operator!=(const Document& doc_right, const Document& doc_left);


void Print(const Document& doc, std::ostream& output);

}  // namespace json