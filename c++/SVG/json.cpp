#include "json.h"

using namespace std;

namespace json {

namespace {

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
    Array result;

    for (char c; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    return Node(move(result));
}

Node LoadInt(istream& input) {
    int result = 0;
    while (isdigit(input.peek())) {
        result *= 10;
        result += input.get() - '0';
    }
    return Node(result);
}

Node LoadString(istream& input) {
    string line;
    getline(input, line, '"');
    return Node(move(line));
}

Node LoadDict(istream& input) {
    Dict result;

    for (char c; input >> c && c != '}';) {
        if (c == ',') {
            input >> c;
        }

        string key = LoadString(input).AsString();
        input >> c;
        result.insert({move(key), LoadNode(input)});
    }

    return Node(move(result));
}

Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[') {
        return LoadArray(input);
    } else if (c == '{') {
        return LoadDict(input);
    } else if (c == '"') {
        return LoadString(input);
    } else {
        input.putback(c);
        return LoadInt(input);
    }
}

}  // namespace

Node::Node(Array array)
    : value_(move(array)) {
}

Node::Node(Dict map)
    : value_(move(map)) {
}

Node::Node(int value)
    : value_(value) {
}

Node::Node(string value)
    : value_(move(value)) {
}

bool Node::IsInt() const
{
    return value_.index() == 4 ? 1 : 0;;
}

bool Node::IsDouble() const
{
    bool result = (value_.index() == 4 || value_.index() == 5) ? 1 : 0;
    return result;
}

bool Node::IsPureDouble() const
{
    bool result = (value_.index() == 5) ? 1 : 0;
    return result;
}

bool Node::IsBool() const
{
    bool result = (value_.index() == 3) ? 1 : 0;
    return result;
}

bool Node::IsString() const
{
    bool result = (value_.index() == 6) ? 1 : 0;
    return result;
}

bool Node::IsNull() const
{
    bool result = (value_.index() == 0) ? 1 : 0;
    return result;
}

bool Node::IsArray() const
{
    bool result = (value_.index() == 1) ? 1 : 0;
    return result;
}

bool Node::IsMap() const
{
    bool result = (value_.index() == 2) ? 1 : 0;
    return result;
}

int Node::AsInt() const 
{
    if (IsInt())
        return std::get<int>(value_);
    else
        throw std::logic_error{"logic_err"};
}

bool Node::AsBool() const
{
    if (IsBool())
        return std::get<bool>(value_);
    else
        throw std::logic_error{ "logic_err" };
}

double Node::AsDouble() const
{
    if (IsDouble())
        return std::get<double>(value_);
    else
        throw std::logic_error{ "logic_err" };
}

const Array& Node::AsArray() const {
    if (IsArray())
        return std::get<Array>(value_);
    else
        throw std::logic_error{ "logic_err" };
}

const Dict& Node::AsMap() const {
    if (IsMap())
        return std::get<Dict>(value_);
    else
        throw std::logic_error{ "logic_err" };
}

const string& Node::AsString() const {
    if (IsString())
        return std::get<string>(value_);
    else
        throw std::logic_error{ "logic_err" };
}

const Value Node::GetValue() const
{
    return value_;
}

////////////////////////////////////////

Document::Document(Node root)
    : root_(move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Document Load(istream& input) {
    return Document{LoadNode(input)};
}

void Print(const Document& doc, std::ostream& output) {
    (void) &doc;
    (void) &output;

    // Реализуйте функцию самостоятельно
}

}  // namespace json