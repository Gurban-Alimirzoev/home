#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
using namespace std;

/* Подставьте вашу реализацию класса SearchServer сюда */
const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine()
{
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber()
{
    int result;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string &text)
{
    vector<string> words;
    string word;
    for (const char c : text)
    {
        if (c == ' ')
        {
            if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
        else
        {
            word += c;
        }
    }
    if (!word.empty())
    {
        words.push_back(word);
    }

    return words;
}

struct Document
{
    int id;
    double relevance;
    int rating;
};

enum class DocumentStatus
{
    ACTUAL,
    IRRELEVANT,
    BANNED,
    REMOVED,
};

class SearchServer
{
public:
    void SetStopWords(const string &text)
    {
        for (const string &word : SplitIntoWords(text))
        {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string &document, DocumentStatus status, const vector<int> &ratings)
    {
        const vector<string> words = SplitIntoWordsNoStop(document);
        const double inv_word_count = 1.0 / words.size();
        for (const string &word : words)
        {
            word_to_document_freqs_[word][document_id] += inv_word_count;
        }
        documents_.emplace(document_id,
                           DocumentData{
                               ComputeAverageRating(ratings),
                               status});
    }

    template <typename DocumentPredicate>
    vector<Document> FindTopDocuments(const string &raw_query, DocumentPredicate document_predicate) const
    {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query, document_predicate);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document &lhs, const Document &rhs)
             {
                 if (abs(lhs.relevance - rhs.relevance) < 1e-6)
                 {
                     return lhs.rating > rhs.rating;
                 }
                 else
                 {
                     return lhs.relevance > rhs.relevance;
                 }
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT)
        {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

    vector<Document> FindTopDocuments(const string &raw_query, DocumentStatus status) const
    {
        return FindTopDocuments(raw_query, [status]([[maybe_unused]] int document_id, DocumentStatus document_status, [[maybe_unused]] int rating)
                                { return document_status == status; });
    }

    vector<Document> FindTopDocuments(const string &raw_query) const
    {
        return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
    }

    int GetDocumentCount() const
    {
        return documents_.size();
    }

    tuple<vector<string>, DocumentStatus> MatchDocument(const string &raw_query, int document_id) const
    {
        const Query query = ParseQuery(raw_query);
        vector<string> matched_words;
        for (const string &word : query.plus_words)
        {
            if (word_to_document_freqs_.count(word) == 0)
            {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id))
            {
                matched_words.push_back(word);
            }
        }
        for (const string &word : query.minus_words)
        {
            if (word_to_document_freqs_.count(word) == 0)
            {
                continue;
            }
            if (word_to_document_freqs_.at(word).count(document_id))
            {
                matched_words.clear();
                break;
            }
        }
        return {matched_words, documents_.at(document_id).status};
    }

private:
    struct DocumentData
    {
        int rating;
        DocumentStatus status;
    };

    set<string> stop_words_;
    map<string, map<int, double>> word_to_document_freqs_;
    map<int, DocumentData> documents_;

    bool IsStopWord(const string &word) const
    {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string &text) const
    {
        vector<string> words;
        for (const string &word : SplitIntoWords(text))
        {
            if (!IsStopWord(word))
            {
                words.push_back(word);
            }
        }
        return words;
    }

    int ComputeAverageRating(const vector<int> &ratings)
    {
        if (ratings.empty())
        {
            return 0;
        }
        int rating_sum = 0;
        for (const int rating : ratings)
        {
            rating_sum += rating;
        }
        return rating_sum / static_cast<int>(ratings.size());
    }

    struct QueryWord
    {
        string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(string text) const
    {
        bool is_minus = false;
        if (text[0] == '-')
        {
            is_minus = true;
            text = text.substr(1);
        }
        return {
            text,
            is_minus,
            IsStopWord(text)};
    }

    struct Query
    {
        set<string> plus_words;
        set<string> minus_words;
    };

    Query ParseQuery(const string &text) const
    {
        Query query;
        for (const string &word : SplitIntoWords(text))
        {
            const QueryWord query_word = ParseQueryWord(word);
            if (!query_word.is_stop)
            {
                if (query_word.is_minus)
                {
                    query.minus_words.insert(query_word.data);
                }
                else
                {
                    query.plus_words.insert(query_word.data);
                }
            }
        }
        return query;
    }

    double ComputeWordInverseDocumentFreq(const string &word) const
    {
        return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
    }

    template <typename DocumentPredicate>
    vector<Document> FindAllDocuments(const Query &query, DocumentPredicate document_predicate) const
    {
        map<int, double> document_to_relevance;
        for (const string &word : query.plus_words)
        {
            if (word_to_document_freqs_.count(word) == 0)
            {
                continue;
            }
            const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
            for (const auto [document_id, term_freq] : word_to_document_freqs_.at(word))
            {
                const auto &document_data = documents_.at(document_id);
                if (document_predicate(document_id, document_data.status, document_data.rating))
                {
                    document_to_relevance[document_id] += term_freq * inverse_document_freq;
                }
            }
        }

        for (const string &word : query.minus_words)
        {
            if (word_to_document_freqs_.count(word) == 0)
            {
                continue;
            }
            for (const auto [document_id, _] : word_to_document_freqs_.at(word))
            {
                document_to_relevance.erase(document_id);
            }
        }

        vector<Document> matched_documents;
        for (const auto [document_id, relevance] : document_to_relevance)
        {
            matched_documents.push_back({document_id,
                                         relevance,
                                         documents_.at(document_id).rating});
        }
        return matched_documents;
    }
};


template <typename Test, typename TestName>
void RunTestImpl(const Test& test, const TestName& func) {
    test();
    cerr << func << " OK" << endl;
}

#define RUN_TEST(func)  RunTestImpl(func, #func)

template <typename T, typename U>
void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file,
                     const string& func, unsigned line, const string& hint) {
    if (t != u) {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
                const string& hint) {
    if (!value) {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty()) {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(), "Stop words must be excluded from documents"s);
    }
}

void TestMinusWord()
{
    const int doc_id = 42;
    const string contentMinus = "cat"s;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    
    server.AddDocument(doc_id, contentMinus, DocumentStatus::ACTUAL, ratings);
    ASSERT_EQUAL(server.FindTopDocuments("cat"s).empty(), false);
    server.SetStopWords("cat"s);
    ASSERT_HINT(server.FindTopDocuments("cat"s).empty(), "Documents with minus words must be excluded in search results");
}

void TestMatchedDoc()
{
    const int doc_id = 42;
    const string contentMinus = "cat"s;
    const string contentPlus = "dog"s;
    const string contentPlusWithMinus = "cat dog"s;
    
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    server.SetStopWords("cat"s);
    server.AddDocument(doc_id, contentMinus, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id, contentPlusWithMinus, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id, contentPlus, DocumentStatus::ACTUAL, ratings);
    string message = "Documents do not match the search query";
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat"s).size(), 0u, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog"s).size(), 1u, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("dog"s).size(), 1u, message);
}

void TestRaitingCompute()
{

    const string contentMinus = "cat dog bad"s;
    const vector<int> ratings = {1, 2, 3};
    const vector<int> zero = {0};
    const vector<int> one = {1, 1, 1};

    SearchServer server;
    server.AddDocument(1, contentMinus, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(5, contentMinus, DocumentStatus::ACTUAL, zero);
    server.AddDocument(9, contentMinus, DocumentStatus::ACTUAL, one);
    string message = "Rating is incorrect";
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog bad")[0].rating, 2, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog")[1].rating, 1, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat")[2].rating, 0, message);
}

void TestSortRelev()
{
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const int doc_id_for = 4;
    const string contentOne = "cat dog bad good"s;
    const string contentTwo = "cat"s;
    const string contentThree = "dog bad cat"s;
    const string contentFore = "bad good"s;
    
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    server.AddDocument(doc_id_one, contentOne, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, contentTwo, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_tr, contentThree, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_for, contentFore, DocumentStatus::ACTUAL, ratings);

    string message = "Documents are sorted incorrectly";
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog")[0].id, 3, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog")[1].id, 2, message);
    ASSERT_EQUAL_HINT(server.FindTopDocuments("cat dog")[2].id, 1, message);
}

// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer()
{

    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestMinusWord);
    RUN_TEST(TestMatchedDoc);
    RUN_TEST(TestRaitingCompute);
    RUN_TEST(TestSortRelev);
}

// --------- Окончание модульных тестов поисковой системы -----------

int main()
{
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}