#include "search_server.h"

using namespace std;

SearchServer::SearchServer()
{
}

SearchServer::SearchServer(const string& stop_words_text)
    : SearchServer(SplitIntoWords(stop_words_text))
{
}

SearchServer::SearchServer(const string_view stop_words_text)
    : stop_words_constr(stop_words_text)
{
    SearchServer(SplitIntoWords(stop_words_constr));
}

void SearchServer::AddDocument(int document_id, const string_view document, DocumentStatus status, const vector<int>& ratings)
{
    if ((document_id < 0) || (documents_.count(document_id) > 0))
    {
        throw invalid_argument("Invalid document_id"s);
    }

    documents_.emplace(document_id, DocumentData{ ComputeAverageRating(ratings), status, document.data()});

    auto words = SplitIntoWordsNoStop(documents_[document_id].data_);

    all_doc_word[document_id].reserve(words.size());

    for (auto& i : words) {
        all_doc_word[document_id].push_back(i);
    }

    const double inv_word_count = 1.0 / words.size();

    map<string, double> freqs;
    for (const string_view& word : all_doc_word[document_id])
    {
        word_to_document_freqs_[word][document_id] += inv_word_count;
        doc_to_word_freqs[document_id][word] += inv_word_count;
    }
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

const std::map<std::string_view, double>& SearchServer::GetWordFrequencies(int document_id) const
{
    static const std::map<std::string_view, double> EMPTY_MAP;
    if (documents_.count(document_id) > 0)
    {
        return doc_to_word_freqs.at(document_id);
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

        map<string_view, double> word_freq = doc_to_word_freqs[document_id];
        vector<string_view> varStr(word_freq.size());

        transform(
            execution::par,
            word_freq.begin(), word_freq.end(),
            varStr.begin(),
            [&](auto word)
            { return word.first; });

        for_each(
            execution::par,
            varStr.begin(), varStr.end(),
            [&document_id, this](string_view word)
            {
                word_to_document_freqs_.find(word)->second.erase(document_id);
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
        doc_to_word_freqs.at(document_id).begin(),
        doc_to_word_freqs.at(document_id).end(),
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
    const string var(raw_query);
    const auto query = ParseQuery(var);

    vector<string_view> matched_words;

    for (const auto [word, second] : doc_to_word_freqs.at(document_id))
    {
        if (query.minus_words.count(string(word)))
        {
            return { matched_words, documents_.at(document_id).status };
        }
    }

    for (const auto [word, second] : doc_to_word_freqs.at(document_id))
    {
        if (query.plus_words.count(string(word)))
        {
            matched_words.push_back(move(word));
        }
    }

    return { matched_words, documents_.at(document_id).status };
}

tuple<vector<string_view>, DocumentStatus> SearchServer::MatchDocument(execution::sequenced_policy policy, const string_view raw_query, int document_id) const
{
    const string var(raw_query);
    const auto query = ParseQuery(var);

    vector<string_view> matched_words;
    for (const string& word : query.plus_words)
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
    for (const string& word : query.minus_words)
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
    return stop_words_.count(word) > 0;
}

bool SearchServer::IsValidWord(const string& word)
{
    return none_of(word.begin(), word.end(), [](char c)
        { return c >= '\0' && c < ' '; });
}

vector<string> SearchServer::SplitIntoWordsNoStop(const string& text) const
{
    vector<string> words;
    for (const string& word : SplitIntoWords(text))
    {
        if (!IsValidWord(word))
        {
            throw invalid_argument("Word "s + word + " is invalid"s);
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

SearchServer::QueryWord SearchServer::ParseQueryWord(const string& text) const
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
    if (word.empty() || word[0] == '-' || !IsValidWord(word))
    {
        throw invalid_argument("Query word "s + text + " is invalid");
    }

    return { word, is_minus, IsStopWord(word) };
}

SearchServer::Query SearchServer::ParseQuery(const string& text) const
{
    SearchServer::Query result;
    for (const string& word : SplitIntoWords(text))
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

double SearchServer::ComputeWordInverseDocumentFreq(const string& word) const
{
    return log(GetDocumentCount() * 1.0 / word_to_document_freqs_.at(word).size());
}