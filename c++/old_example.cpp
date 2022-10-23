#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>

using namespace std;

struct Nucleotide {
    char symbol;
    size_t position;
    int chromosome_num;
    int gene_num;
    bool is_marked;
    char service_info;
};

struct CompactNucleotide {
    uint32_t position;
    uint16_t gene_num_and_is_marked;
    uint16_t chromosome_num_and_symbol;
    uint8_t service_info;
};

CompactNucleotide Compress(const Nucleotide& n) {
    return CompactNucleotide({
        static_cast<uint32_t>(n.position)
        ,static_cast<uint16_t>(static_cast<uint16_t>(n.gene_num) + (n.is_marked ? 25001 - static_cast<uint16_t>(n.gene_num) : 0))
        ,static_cast<uint16_t>(static_cast<uint8_t>(n.chromosome_num) + static_cast<uint8_t>(n.symbol) * 100)
        ,static_cast<uint8_t>(n.service_info) });
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    return Nucleotide({
        static_cast<char>(cn.chromosome_num_and_symbol / 100)
        ,static_cast<size_t>(cn.position)
        ,static_cast<int>(cn.chromosome_num_and_symbol % 100)
        ,static_cast<int>(cn.gene_num_and_is_marked - (cn.gene_num_and_is_marked == 25001 ? 1 : 0))
        ,static_cast<bool>(cn.gene_num_and_is_marked > 25001 ? 1 : 0)
        ,static_cast<char>(cn.service_info) });
}

static_assert(sizeof(CompactNucleotide) <= 8, "Your CompactNucleotide is not compact enough");
static_assert(alignof(CompactNucleotide) == 4, "Don't use '#pragma pack'!");
bool operator==(const Nucleotide& lhs, const Nucleotide& rhs) {
    return (lhs.symbol == rhs.symbol) && (lhs.position == rhs.position) && (lhs.chromosome_num == rhs.chromosome_num)
        && (lhs.gene_num == rhs.gene_num) && (lhs.is_marked == rhs.is_marked) && (lhs.service_info == rhs.service_info);
}
void TestSize() {
    assert(sizeof(CompactNucleotide) <= 8);
}
void TestCompressDecompress() {
    Nucleotide source;
    source.symbol = 'T';
    source.position = 1'000'000'000;
    source.chromosome_num = 48;
    source.gene_num = 1'000;
    source.is_marked = true;
    source.service_info = '!';

    CompactNucleotide compressedSource = Compress(source);
    Nucleotide decompressedSource = Decompress(compressedSource);

    assert(source == decompressedSource);
}

int main() {
    TestSize();
    TestCompressDecompress();
}