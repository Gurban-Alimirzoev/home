#include "search_server.h"

using namespace std;

SearchServer::SearchServer()
{
}

SearchServer::SearchServer(const string_view stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text))
{
}

SearchServer::SearchServer(const string& stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text))
{
}

void SearchServer::AddDocument(int document_id, const string_view document, DocumentStatus status, const vector<int>& ratings)
{
    if ((document_id < 0) || (documents_.count(document_id) > 0))
    {
        throw invalid_argument("Invalid document_id"s);
    }

    const auto words = SplitIntoWordsNoStop(document);

    const double inv_word_count = 1.0 / words.size();

    map<string, double> freqs;
    vector<string> data_all(words);
    for (const string& word : words)
    {
        word_to_document_freqs_[word][document_id] += inv_word_count;
        freqs[word] += inv_word_count;
    }
    documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status, data_all, freqs });
    document_ids_.insert(document_id);
}

vector<Document> SearchServer::FindTopDocuments(const string_view raw_query, DocumentStatus status) const
{
    return SearchServer::FindTopDocuments(raw_query, [status](int document_id, DocumentStatus document_status, int rating)
        { return document_status == status; });
}

vector<Document> SearchServer::FindTopDocuments(const string_view raw_query) const
{
    return SearchServer::FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const
{
    return documents_.size();
}

std::set<int>::const_iterator SearchServer::begin() const
{
    return document_ids_.begin();
}

std::set<int>::const_iterator SearchServer::end() const
{
    return document_ids_.end();
}

const std::map<std::string, double>& SearchServer::GetWordFrequencies(int document_id) const
{
    static const std::map<std::string, double> EMPTY_MAP;
    if (documents_.count(document_id) > 0)
    {
        return documents_.at(document_id).freqs;
    }
    return EMPTY_MAP;
}

void SearchServer::RemoveDocument(execution::parallel_policy policy, int document_id)
{
    {
        if (documents_.count(document_id) == 0)
        {
            return;
        }

        map<string, double> word_freq = (documents_[document_id]).freqs;
        vector<const string*> varStr(word_freq.size());

        transform(
            execution::par,
            word_freq.begin(), word_freq.end(),
            varStr.begin(),
            [&](const auto& word)
            { return &word.first; });

        for_each(
            execution::par,
            varStr.begin(), varStr.end(),
            [&document_id, this](const string* word)
            {
                word_to_document_freqs_.find(*word)->second.erase(document_id);
            });

        documents_.erase(document_id);
        document_ids_.erase(document_id);
    }
}

void SearchServer::RemoveDocument(int document_id)
{
    RemoveDocument(execution::seq, document_id);
}

void SearchServer::RemoveDocument(execution::sequenced_policy policy, int document_id)
{
    if (documents_.count(document_id) == 0)
    {
        return;
    }

    for_each(
        documents_.at(document_id).freqs.begin(),
        documents_.at(document_id).freqs.end(),
        [this, document_id](const auto& word)
        {
            word_to_document_freqs_[word.first].erase(document_id);
        });

    documents_.erase(document_id);
    document_ids_.erase(document_id);
}

tuple<vector<string_view>, DocumentStatus> SearchServer::MatchDocument(const string_view raw_query, int document_id) const
{
    return MatchDocument(execution::seq, raw_query, document_id);
}

tuple<vector<string_view>, DocumentStatus> SearchServer::MatchDocument(execution::parallel_policy policy, const string_view raw_query, int document_id) const
{
    const auto query = ParseQuery(raw_query);

    vector<string_view> matched_words;

    for (const string& word : (documents_.at(document_id)).data)
    {
        if (query.minus_words.count(word))
        {
            return { matched_words, documents_.at(document_id).status };
        }
    }

    for (const string& word : (documents_.at(document_id)).data)
    {
        if (query.plus_words.count(word))
        {
            matched_words.push_back(word);
        }
    }

    return { matched_words, documents_.at(document_id).status };
}

tuple<vector<string_view>, DocumentStatus> SearchServer::MatchDocument(execution::sequenced_policy policy, const string_view raw_query, int document_id) const
{
    const auto query = ParseQuery(raw_query);

    vector<string_view> matched_words;
    for (const string_view& word : query.plus_words)
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
    for (const string_view& word : query.minus_words)
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
    return { matched_words, documents_.at(document_id).status };
}

bool SearchServer::IsStopWord(const string& word) const
{
    return stop_words_.count(string(word)) > 0;
}

bool SearchServer::IsValidWord(const string& word)
{
    return none_of(word.begin(), word.end(), [](char c)
        { return c >= '\0' && c < ' '; });
}

vector<string> SearchServer::SplitIntoWordsNoStop(const string_view text) const
{
    vector<string> words;
    for (string word : SplitIntoWords(text))
    {
        if (!IsValidWord(word))
        {
            throw invalid_argument("Word "s + string(word) + " is invalid"s);
        }
        if (!IsStopWord(word))
        {
            words.push_back(word);
        }
    }
    return words;
}

int SearchServer::ComputeAverageRating(const vector<int>& ratings)
{
    if (ratings.empty())
    {
        return 0;
    }
    int rating_sum = accumulate(ratings.begin(), ratings.end(), 0);
    return rating_sum / static_cast<int>(ratings.size());
}

SearchServer::QueryWord SearchServer::ParseQueryWord(const string text) const
{
    if (text.empty())
    {
        throw invalid_argument("Query word is empty"s);
    }
    string word = text;
    bool is_minus = false;
    if (word[0] == '-')
    {
        is_minus = true;
        word = word.substr(1);
    }
    if (word.empty() || word[0] == '-' || !IsValidWord(string(word)))
    {
        throw invalid_argument("Query word "s + string(text) + " is invalid");
    }

    return { word, is_minus, IsStopWord(word) };
}

SearchServer::Query SearchServer::ParseQuery(const string_view text) const
{
    SearchServer::Query result;
    for (string& word : SplitIntoWords(text))
    {
        const auto query_word = ParseQueryWord(word);
        if (!query_word.is_stop)
        {
            if (query_word.is_minus)
            {
                result.minus_words.insert(query_word.data);
            }
            else
            {
                result.plus_words.insert(query_word.data);
            }
        }
    }
    return result;
}

double SearchServer::ComputeWordInverseDocumentFreq(const string word) const
{
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}