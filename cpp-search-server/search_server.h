#pragma once
#include <set>
#include <vector>
#include <string>
#include <string_view>
#include <list>
#include <map>
#include <deque>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <functional>
#include <stdexcept>
#include <execution>
#include <unordered_set>
#include "string_processing.h"
#include "document.h"
#include "log_duration.h"

using namespace std::string_literals;
const double precision = 1e-6;
const int MAX_RESULT_DOCUMENT_COUNT = 5;

class SearchServer
{
public:
    SearchServer();

    template <typename StringContainer>
    SearchServer(const StringContainer &stop_words)
        : stop_words_(MakeUniqueNonEmptyStrings(stop_words))
    {
        if (!all_of(stop_words_.begin(), stop_words_.end(), IsValidWord))
        {
            throw std::invalid_argument("Some of stop words are invalid"s);
        }
    }

    explicit SearchServer(const std::string &stop_words_text);
    explicit SearchServer(const std::string_view stop_words_text);

    void AddDocument(int document_id, const std::string_view document, DocumentStatus status, const std::vector<int> &ratings);

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string_view raw_query, DocumentPredicate document_predicate) const;
    std::vector<Document> FindTopDocuments(const std::string_view raw_query, DocumentStatus status) const;
    std::vector<Document> FindTopDocuments(const std::string_view raw_query) const;

    int GetDocumentCount() const;
    const std::map<std::string_view, double> &GetWordFrequencies(int document_id) const;

    void RemoveDocument(int document_id);
    void RemoveDocument(std::execution::parallel_policy policy, int document_id);
    void RemoveDocument(std::execution::sequenced_policy policy, int document_id);

    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(const std::string_view raw_query, int document_id) const;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::execution::sequenced_policy policy, const std::string_view raw_query, int document_id) const;
    std::tuple<std::vector<std::string_view>, DocumentStatus> MatchDocument(std::execution::parallel_policy policy, const std::string_view raw_query, int document_id) const;

    std::set<int>::const_iterator begin() const;
    std::set<int>::const_iterator end() const;

private:
    struct DocumentData
    {
        int rating;
        DocumentStatus status;
        std::unordered_set<std::string_view> words;
    };
    struct QueryWord
    {
        std::string data;
        bool is_minus;
        bool is_stop;
    };
    struct Query
    {
        std::deque<std::string_view> plus_words;
        std::deque<std::string_view> minus_words;
    };

    const std::set<std::string, std::less<>> stop_words_;
    const std::string stop_words_constr;
    std::map<int, std::vector<std::string>> all_doc_word;

    std::unordered_set<std::string> unique_words;
    std::string_view AddUniqueWord(const std::string &word);

    std::map<std::string_view, std::map<int, double>> word_to_document_freqs_;
    std::map<int, std::map<std::string_view, double>> document_to_word_freqs_;

    std::map<int, DocumentData> documents_;
    std::set<int> document_ids_;

    bool IsStopWord(const std::string &word) const;

    static bool IsValidWord(const std::string &word);

    std::deque<std::string_view> SplitIntoWordsNoStop(const std::string_view text) const;
    Query ParseQuery(const std::string_view text) const;
    QueryWord ParseQueryWord(const std::string_view text) const;

    static int ComputeAverageRating(const std::vector<int> &ratings);
    double ComputeWordInverseDocumentFreq(const std::string_view word) const;

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(const Query &query, DocumentPredicate document_predicate) const;
};

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(const std::string_view raw_query, DocumentPredicate document_predicate) const
{
    const std::string var(raw_query);
    const auto query = ParseQuery(var);

    auto matched_documents = FindAllDocuments(query, document_predicate);

    sort(matched_documents.begin(), matched_documents.end(), [](const Document &lhs, const Document &rhs)
         {
            if (std::abs(lhs.relevance - rhs.relevance) < precision) {
                return lhs.rating > rhs.rating;
            }
            else {
                return lhs.relevance > rhs.relevance;
            } });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT)
    {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }

    return matched_documents;
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query &query, DocumentPredicate document_predicate) const
{
    std::map<int, double> document_to_relevance;
    for (const std::string_view word : query.plus_words)
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

    for (const std::string_view word : query.minus_words)
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

    std::vector<Document> matched_documents;
    for (const auto [document_id, relevance] : document_to_relevance)
    {
        matched_documents.push_back({document_id, relevance, documents_.at(document_id).rating});
    }
    return matched_documents;
}