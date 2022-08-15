#include "json.h"

using namespace std;

namespace json {

namespace {

Node LoadNode(istream& input);

Node LoadArray(istream& input) {
    Array result;
    
    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    if (it == end) {
        throw ParsingError("Array parsing error");
    }

    for (char c; input >> c && c != ']';) {
        if (c != ',') {
            input.putback(c);
        }
        result.push_back(LoadNode(input));
    }

    return Node(move(result));
}

using Number = std::variant<int, double>;

Node LoadNumber(std::istream& input) {
    using namespace std::literals;

    std::string parsed_num;

    // Считывает в parsed_num очередной символ из input
    auto read_char = [&parsed_num, &input] {
        parsed_num += static_cast<char>(input.get());
        if (!input) {
            throw ParsingError("Failed to read number from stream"s);
        }
    };

    // Считывает одну или более цифр в parsed_num из input
    auto read_digits = [&input, read_char] {
        if (!std::isdigit(input.peek())) {

            throw ParsingError("A digit is expected"s);
        }
        while (std::isdigit(input.peek())) {
            read_char();
        }
    };

    if (input.peek() == '-') {
        read_char();
    }
    // Парсим целую часть числа
    if (input.peek() == '0') {
        read_char();
        // После 0 в JSON не могут идти другие цифры
    }
    else {
        read_digits();
    }

    bool is_int = true;
    // Парсим дробную часть числа
    if (input.peek() == '.') {
        read_char();
        read_digits();
        is_int = false;
    }

    // Парсим экспоненциальную часть числа
    if (int ch = input.peek(); ch == 'e' || ch == 'E') {
        read_char();
        if (ch = input.peek(); ch == '+' || ch == '-') {
            read_char();
        }
        read_digits();
        is_int = false;
    }

    try {
        if (is_int) {
            // Сначала пробуем преобразовать строку в int
            try {
                return std::stoi(parsed_num);
            }
            catch (...) {
                // В случае неудачи, например, при переполнении,
                // код ниже попробует преобразовать строку в double
            }
        }
        return std::stod(parsed_num);
    }
    catch (...) {
        throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
    }
}

// Считывает содержимое строкового литерала JSON-документа
// Функцию следует использовать после считывания открывающего символа ":
Node LoadString(std::istream& input) {
    using namespace std::literals;

    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    std::string s;
    while (true) {
        if (it == end) {
            // Поток закончился до того, как встретили закрывающую кавычку?
            throw ParsingError("String parsing error");
        }
        const char ch = *it;
        if (ch == '"') {
            // Встретили закрывающую кавычку
            ++it;
            break;
        }
        else if (ch == '\\') {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end) {
                // Поток завершился сразу после символа обратной косой черты
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char) {
            case 'n':
                s.push_back('\n');
                break;
            case 't':
                s.push_back('\t');
                break;
            case 'r':
                s.push_back('\r');
                break;
            case '"':
                s.push_back('"');
                break;
            case '\\':
                s.push_back('\\');
                break;
            default:
                // Встретили неизвестную escape-последовательность
                throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        }
        else if (ch == '\n' || ch == '\r') {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
            throw ParsingError("Unexpected end of line"s);
        }
        else {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            s.push_back(ch);
        }
        ++it;
    }
    return s;
}

Node LoadDict(istream& input) {
    Dict result;

    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    if (it == end) {
        throw ParsingError("Dict parsing error");
    }
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

void StringParse(istream& input, string& word)
{
    using namespace std::literals;

    auto it = std::istreambuf_iterator<char>(input);
    auto end = std::istreambuf_iterator<char>();
    int flag = 0;
    while (true)
    {
        if (it == end || flag == 2)
        {
            //++it;
            if (*word.begin() == 'n')
            {
                if (*next(word.end(), -1) == 'l')
                    break;
                else
                    throw ParsingError("Null parsing error");
            }
            else
            {
                if (*next(word.end(), -1) == 'e')
                    break;
                else
                    throw ParsingError("Bool parsing error");
            }
        }

        char ch = *it;
        if (ch == '\\') {
            // Встретили начало escape-последовательности
            ++it;
            if (it == end) {
                // Поток завершился сразу после символа обратной косой черты
                throw ParsingError("String parsing error");
            }
            const char escaped_char = *(it);
            // Обрабатываем одну из последовательностей: \\, \n, \t, \r, \"
            switch (escaped_char) {
            case 'n':
                word.push_back('\n');
                break;
            case 't':
                word.push_back('\t');
                break;
            case 'r':
                word.push_back('\r');
                break;
            case '"':
                word.push_back('"');
                break;
            case '\\':
                word.push_back('\\');
                break;
            default:
                // Встретили неизвестную escape-последовательность
                throw ParsingError("Unrecognized escape sequence \\"s + escaped_char);
            }
        }
        else if (ch == '\n' || ch == '\r') {
            // Строковый литерал внутри- JSON не может прерываться символами \r или \n
            throw ParsingError("Unexpected end of line"s);
        }
        else {
            // Просто считываем очередной символ и помещаем его в результирующую строку
            if (ch == 'n' || 
                ch == 'u' || 
                ch == 'l' || 
                ch == 't' || 
                ch == 'r' ||
                ch == 'e' || 
                ch == 'f' || 
                ch == 'a' || 
                ch == 's')
                word.push_back(ch);
            else
                break;
        }
        if (!word.empty())
        {
            if (*word.begin() == 'n')
            {
                const char find_second_l = *(it);
                flag += (find_second_l == 'l') ? 1 : 0;
            }
            const char find_second_l = *(it);
            flag += (find_second_l == 'e') ? 2 : 0;
        }
        ++it;
    }
}

Node LoadOther(istream& input)
{
    std::string s;
    StringParse(input, s);
    if (s == "null")
        return nullptr;
    else if (s == "true")
        return true;
    else if (s == "false")
        return false;
    else
        return LoadNumber(input);
}

Node LoadNode(istream& input) {
    char c;
    input >> c;

    if (c == '[')
        return LoadArray(input);
    else if (c == '{')
        return LoadDict(input);
    else if (c == '"')
        return LoadString(input);
    else
    {
        input.putback(c);
        return LoadOther(input);
    }
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////

Node::Node(Value value)
    : value_(move(value))
{
}

bool Node::IsInt() const
{
    return value_.index() == 4;
}

bool Node::IsDouble() const
{
    return value_.index() == 4 || value_.index() == 5;
}

bool Node::IsPureDouble() const
{
    return value_.index() == 5;
}

bool Node::IsBool() const
{
    return value_.index() == 3;
}

bool Node::IsString() const
{
    return value_.index() == 6;
}

bool Node::IsNull() const
{
    return value_.index() == 0;
}

bool Node::IsArray() const
{
    return value_.index() == 1;
}

bool Node::IsMap() const
{
    return value_.index() == 2;
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
    {
        if (IsInt())
            return get<int>(value_);
        else
            return get<double>(value_);
    }
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

int Node::GetIndex() const
{
    return static_cast<int>(value_.index());
}

////////////////////////////////////////////////////////////////////////////////

bool operator==(const Node& node_right, const Node& node_left)
{
    return node_right.GetValue() == node_left.GetValue();
}

bool operator!=(const Node& node_right, const Node& node_left)
{
    return node_right.GetValue() != node_left.GetValue();
}

////////////////////////////////////////////////////////////////////////////////

void PrintNode(const Node& node, const PrintContext& cont);

void PrintValue(std::nullptr_t, const PrintContext& ctx) {
    ctx.PrintIndent();
    ctx.out << "null";
}

void PrintValue(const bool value, const PrintContext& ctx)
{
    ctx.PrintIndent();
    if (value)
        ctx.out << "true";
    else
        ctx.out << "false";
}

void PrintValue(const std::string value, const PrintContext& ctx)
{
    ctx.PrintIndent();
    ctx.out << "\"";
    for (char c : value)
    {
        switch (c)
        {
            case '\n':
                ctx.out << "\\n";
                break;
            case '\t':
                ctx.out << "\t";
                break;
            case '\r':
                ctx.out << "\\r";
                break;
            case '\"':
                ctx.out << "\\\"";
                break;
            case '\\':
                ctx.out << "\\\\";
                break;
            default:
                ctx.out << c;
                break;
        }
    } 
    ctx.out << "\"";
}

void PrintValue(const Array array, const PrintContext& ctx)
{
    ctx.PrintIndent();
    ctx.out << "[";
    for (int i = 0; i < static_cast<int>(array.size()) - 1; i++)
    {
        PrintNode(array[i], ctx);
        ctx.out << ", ";
    }
    if (!array.empty())
        PrintNode(array[array.size() - 1], ctx);
    ctx.out << "]";
}

void PrintValue(const Dict dict, const PrintContext& ctx)
{
    ctx.PrintIndent();
    ctx.out << "{";
    for (Dict::const_iterator it = dict.begin(); it != std::next(dict.end(), -1); it++)
    {
        PrintValue(it->first, ctx);
        ctx.out << " : ";
        PrintNode(it->second, ctx);
        ctx.out << ", ";
    }

    if (!dict.empty())
    {
        PrintValue(next(dict.end(), -1)->first, ctx);
        ctx.out << " : ";
        PrintNode(next(dict.end(), -1)->second, ctx);
    }

    ctx.out << "}";
}

void PrintNode(const Node& node, const PrintContext& cont) 
{
    std::visit(
        [&cont](const auto& value) {
            PrintValue(value, cont);
        },
        node.GetValue());
}

////////////////////////////////////////////////////////////////////////////////

Document::Document(Node root)
    : root_(move(root)) {
}

const Node& Document::GetRoot() const {
    return root_;
}

Document Load(istream& input) {
    return Document{LoadNode(input)};
}

bool operator==(Document& doc_right, Document& doc_left)
{
    return doc_right.GetRoot() == doc_left.GetRoot();
}

bool operator!=(Document& doc_right, Document& doc_left)
{
    return doc_right != doc_left;
}

void Print(const Document& doc, std::ostream& output) {

    PrintContext ctx{ output };
    PrintNode(doc.GetRoot(), ctx);
    // Реализуйте функцию самостоятельно
}

}  // namespace json