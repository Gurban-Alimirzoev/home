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

class Node {
public:
   /* Реализуйте Node, используя std::variant */

    using Value = std::variant<std::nullptr_t, Array, Dict, bool, int, double, std::string>;

    /*struct ValueReturner {

        void operator()(std::nullptr_t) const 
        {
        }
        void operator()(Array value_) const
        {
            arr = move(value_);
        }
        void operator()(Dict value_) const
        {
            dict = move(value_);
        }
        void operator()(bool value_) const
        {
            flag = move(value_);
        }
        void operator()(int value_) const
        {
            num = move(value_);
        }
        void operator()(double value_) const
        {
            double_num = move(value_);
        }
        void operator()(std::string value_) const {
            str = move(value_);
        }


        Array& arr;
        Dict& dict;
        bool& flag;
        int& num;
        double& double_num;
        std::string& str;
    };*/

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

private:
    Value value_;

    /*Array as_array_;
    Dict as_map_;
    int as_int_ = 0;
    std::string as_string_;*/
};

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