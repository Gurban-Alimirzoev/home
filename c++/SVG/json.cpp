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
    : as_array_(move(array)) {
}

Node::Node(Dict map)
    : as_map_(move(map)) {
}

Node::Node(int value)
    : as_int_(value) {
}

Node::Node(string value)
    : as_string_(move(value)) {
}

bool Node::IsInt() const
{
    bool result = (value_.index() == 4) ? 1 : 0;
    return result;
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

bool Node::IsPureDouble() const
{
    bool result = (value_.index() == 5) ? 1 : 0;
    return result;
}

bool Node::IsPureDouble() const
{
    bool result = (value_.index() == 5) ? 1 : 0;
    return result;
}
const Array& Node::AsArray() const {
    return as_array_;
}

const Dict& Node::AsMap() const {
    return as_map_;
}

int Node::AsInt() const {
    return as_int_;
}

const string& Node::AsString() const {
    return as_string_;
}

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