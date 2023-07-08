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

template <typename MapS, typename MapF>
void Print(ostream &out, const map<MapF, MapS> &container)
{
    bool i = 0;
    for (const auto &[key, value] : container)
    {
        if (i == 1)
        {
            out << ", ";
        }
        out << key << ": " << value;
        i = 1;
    }
}

template <typename Data>
void Print(ostream &out, const Data &container)
{
    bool i = 0;
    for (const auto &element : container)
    {
        if (i == 1)
        {
            out << ", ";
        }
        out << element;
        i = 1;
    }
}

template <typename Element>
ostream &operator<<(ostream &out, const vector<Element> &container)
{
    out << "[";
    Print(out, container);
    out << "]";
    return out;
}

template <typename SetElement>
ostream &operator<<(ostream &out, const set<SetElement> &container)
{
    out << "{";
    Print(out, container);
    out << "}";
    return out;
}

//template <typename MapF>
template <typename MapS, typename MapF>
ostream &operator<<(ostream &out, const map<MapF, MapS> &container)
{
    out << "{";
    Print(out, container);
    out << "}";
    return out;
}

template <typename Test, typename TestName>
void RunTestImpl(const Test &test, const TestName &func)
{
    test();
    cerr << func << " OK" << endl;
}

#define RUN_TEST(func) RunTestImpl(func, #func)

template <typename T, typename U>
void AssertEqualImpl(const T &t, const U &u, const string &t_str, const string &u_str, const string &file,
                     const string &func, unsigned line, const string &hint)
{
    if (t != u)
    {
        cerr << boolalpha;
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
        cerr << t << " != "s << u << "."s;
        if (!hint.empty())
        {
            cerr << " Hint: "s << hint;
        }
        cerr << endl;
        abort();
    }
}

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(bool value, const string &expr_str, const string &file, const string &func, unsigned line,
                const string &hint)
{
    if (!value)
    {
        cerr << file << "("s << line << "): "s << func << ": "s;
        cerr << "ASSERT("s << expr_str << ") failed."s;
        if (!hint.empty())
        {
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
void TestExcludeStopWordsFromAddedDocumentContent()
{
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = {1, 2, 3};
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document &doc0 = found_docs[0];
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
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const string doc_1 = "cat"s;
    const string doc_2 = "cat dog"s;
    const string doc_3 = "dog"s;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    server.AddDocument(doc_id_one, doc_1, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, doc_2, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_tr, doc_3, DocumentStatus::ACTUAL, ratings);

    string message = "Documents with minus words must be excluded in search results";
    ASSERT_EQUAL(server.FindTopDocuments("cat"s).empty(), false);
    vector<Document> test = server.FindTopDocuments("dog -cat"s);
    ASSERT_EQUAL_HINT(test.size(), 1u, message);
    ASSERT_EQUAL_HINT(test[0].id, doc_id_tr, message);
}

void TestMatchedDoc()
{
    const int doc_id_one = 1;
    const string contentPlusWithMinus = "cat dog good bad"s;
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    //server.SetStopWords("cat dog"s);
    server.AddDocument(doc_id_one, contentPlusWithMinus, DocumentStatus::ACTUAL, ratings);
    string message = "Documents do not match the search query";

    vector<string> vectorPlusWordInMatchDoc, vectorMinusWordInMatchDoc;
    DocumentStatus TestDocumentStatus;
    tie(vectorPlusWordInMatchDoc, TestDocumentStatus) = server.MatchDocument("cat dog good", doc_id_one);
    tie(vectorMinusWordInMatchDoc, TestDocumentStatus) = server.MatchDocument("-cat dog good bad", doc_id_one);
    vector<string> testVector = {"cat", "dog", "good"};
    ASSERT_EQUAL_HINT(vectorPlusWordInMatchDoc, testVector, message);
    ASSERT_EQUAL_HINT(vectorMinusWordInMatchDoc.size(), 0u, message);
}

void TestRaitingCompute()
{
    const string contentMinus = "cat dog bad"s;
    const vector<int> ratings = {1, 2, 3};
    const vector<int> one = {1, 1, 1};
    const vector<int> zero = {0};
    const int doc_id_one = 1;
    const int doc_id_two = 5;
    const int doc_id_tr = 9;

    SearchServer server;
    server.AddDocument(doc_id_one, contentMinus, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, contentMinus, DocumentStatus::ACTUAL, one);
    server.AddDocument(doc_id_tr, contentMinus, DocumentStatus::ACTUAL, zero);
    string message = "Rating is incorrect";
    vector<Document> test = server.FindTopDocuments(contentMinus);
    ASSERT_EQUAL(test[0].id, doc_id_one);
    ASSERT_EQUAL_HINT(test[0].rating, 2, message);

    ASSERT_EQUAL(test[1].id, doc_id_two);
    ASSERT_EQUAL_HINT(test[1].rating, 1, message);

    ASSERT_EQUAL(test[2].id, doc_id_tr);
    ASSERT_EQUAL_HINT(test[2].rating, 0, message);
}

void TestRelevAndTestSort()
{
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const int doc_id_for = 4;
    const string contentOne = "cat dog bad good"s;
    const string contentTwo = "cat"s;
    const string contentThree = "dog bad cat"s;
    const vector<int> ratings = {1, 2, 3};
    const vector<int> ratingsForSort = {1};

    SearchServer server;
    server.AddDocument(doc_id_one, contentOne, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, contentTwo, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_tr, contentThree, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_for, contentOne, DocumentStatus::ACTUAL, ratingsForSort);

    string message = "Documents are sorted incorrectly";
    vector<Document> test = server.FindTopDocuments("cat dog");
    ASSERT_EQUAL(test[0].id, 3);
    ASSERT_EQUAL_HINT(test[0].relevance, 0.095894024150593615, message);

    ASSERT_EQUAL(test[1].id, 1);
    ASSERT_EQUAL_HINT(test[1].relevance, 0.071920518112945211, message);

    ASSERT_EQUAL(test[2].id, 4);
    ASSERT_EQUAL_HINT(test[2].relevance, 0.071920518112945211, message);
    ASSERT_EQUAL_HINT(test[2].rating, 1, message);

    ASSERT_EQUAL(test[3].id, 2);
    ASSERT_EQUAL_HINT(test[3].relevance, 0, message);
    ASSERT_EQUAL_HINT(test[3].rating, 2, message);
}

void TestSearchWithDocStatus()
{
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const int doc_id_for = 4;
    const string contentOne = "cat dog bad good"s;
    const string contentTwo = "cat god"s;
    const string contentThree = "dog bad cat"s;
    const string contentFore = "dog bad cat god"s;
    const string testContent = "cat";
    const vector<int> ratings = {1, 2, 3};

    SearchServer server;
    server.AddDocument(doc_id_one, contentOne, DocumentStatus::ACTUAL, ratings);
    server.AddDocument(doc_id_two, contentTwo, DocumentStatus::BANNED, ratings);
    server.AddDocument(doc_id_tr, contentThree, DocumentStatus::IRRELEVANT, ratings);
    server.AddDocument(doc_id_for, contentFore, DocumentStatus::REMOVED, ratings);

    string message = "Documents search incorrectly";
    vector<Document> testActual = server.FindTopDocuments(testContent, DocumentStatus::ACTUAL);
    ASSERT_EQUAL(testActual.size(), 1u);
    ASSERT_EQUAL_HINT(testActual[0].id, doc_id_one, message);

    vector<Document> testBanned = server.FindTopDocuments(testContent, DocumentStatus::BANNED);
    ASSERT_EQUAL(testBanned.size(), 1u);
    ASSERT_EQUAL_HINT(testBanned[0].id, doc_id_two, message);

    vector<Document> testIrrelevant = server.FindTopDocuments(testContent, DocumentStatus::IRRELEVANT);
    ASSERT_EQUAL(testIrrelevant.size(), 1u);
    ASSERT_EQUAL_HINT(testIrrelevant[0].id, doc_id_tr, message);

    vector<Document> testRemoved = server.FindTopDocuments(testContent, DocumentStatus::REMOVED);
    ASSERT_EQUAL(testRemoved.size(), 1u);
    ASSERT_EQUAL_HINT(testRemoved[0].id, doc_id_for, message);
}

void TestFilterWithPred()
{
    const int doc_id_one = 1;
    const int doc_id_two = 2;
    const int doc_id_tr = 3;
    const int doc_id_for = 4;
    const string contentOne = "cat dog bad good"s;
    const string contentTwo = "cat"s;
    const string contentThree = "dog bad cat"s;
    const string contentFore = "dog cat"s;
    const int ratingOne = 1;
    const int ratingTwo = 2;
    const vector<int> ratingsOne = {ratingOne};
    const vector<int> ratingsTwo = {ratingTwo};

    SearchServer server;
    server.AddDocument(doc_id_one, contentOne, DocumentStatus::ACTUAL, ratingsOne);
    server.AddDocument(doc_id_two, contentTwo, DocumentStatus::BANNED, ratingsTwo);
    server.AddDocument(doc_id_tr, contentThree, DocumentStatus::IRRELEVANT, ratingsOne);
    server.AddDocument(doc_id_for, contentFore, DocumentStatus::REMOVED, ratingsTwo);
    string message = "Predicat filter worked incorrectly";

    vector<Document> testStatus = server.FindTopDocuments(contentTwo, [](int document_id, DocumentStatus status, int rating)
                                                          { return status == DocumentStatus::ACTUAL; });
    ASSERT_EQUAL(testStatus.size(), 1u);
    ASSERT_EQUAL_HINT(testStatus[0].id, doc_id_one, message);

    vector<Document> testRating = server.FindTopDocuments(contentTwo, [](int document_id, DocumentStatus status, int rating)
                                                          { return rating == 2; });
    ASSERT_EQUAL(testRating.size(), 2u);
    ASSERT_EQUAL_HINT(testRating[0].id, doc_id_two, message);
    ASSERT_EQUAL_HINT(testRating[1].id, doc_id_for, message);

    vector<Document> testId = server.FindTopDocuments(contentTwo, [](int document_id, DocumentStatus status, int rating)
                                                      { return document_id == 1; });
    ASSERT_EQUAL(testId.size(), 1u);
    ASSERT_EQUAL_HINT(testId[0].id, doc_id_one, message);
}

void TestSearchServer()
{
    RUN_TEST(TestExcludeStopWordsFromAddedDocumentContent);
    RUN_TEST(TestMinusWord);
    RUN_TEST(TestMatchedDoc);
    RUN_TEST(TestRaitingCompute);
    RUN_TEST(TestRelevAndTestSort);
    RUN_TEST(TestSearchWithDocStatus);
    RUN_TEST(TestFilterWithPred);
}

// --------- Окончание модульных тестов поисковой системы -----------

int main()
{
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}