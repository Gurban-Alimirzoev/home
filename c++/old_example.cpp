#include <array>
#include <iostream>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <random>

using namespace std;

const static int N = 4;
const std::array<char, N> Letters = {
        'A', 'T', 'G', 'C' };

struct Nucleotide {
    char symbol; // 4
    size_t position; //3,3*10*9
    int chromosome_num; // 46
    int gene_num; // 25000
    bool is_marked; //1
    char service_info; //255
};

struct CompactNucleotide {
    uint32_t position;
    uint32_t gene_num : 15;
    uint32_t is_marked : 1;
    uint32_t chromosome_num : 6;
    uint32_t symbol : 2;
    uint32_t service_info : 8;
};

CompactNucleotide Compress(const Nucleotide& n) {
    CompactNucleotide result;
    result.position = n.position;
    result.gene_num = n.gene_num;
    result.chromosome_num = n.chromosome_num;
    result.service_info = n.service_info;
    result.is_marked = n.is_marked;
    result.symbol = distance(Letters.begin(), std::find(Letters.begin(), Letters.end(), n.symbol));
    return result;
}

Nucleotide Decompress(const CompactNucleotide& cn) {
    Nucleotide result;
    result.position = cn.position;
    result.gene_num = cn.gene_num;
    result.chromosome_num = cn.chromosome_num;
    result.service_info = cn.service_info;
    result.is_marked = cn.is_marked;
    result.symbol = Letters[cn.symbol];
    return result;
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