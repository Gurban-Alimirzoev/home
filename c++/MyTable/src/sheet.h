#pragma once

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

class PosHasher {
public:
    size_t operator()(const Position pos) const
    {
        return pos_hasher(pos.row) + pos_hasher(pos.col) * 11;
    }
    std::hash<int> pos_hasher;
};

/*class PtrHasher {
public:
    size_t operator()(const std::shared_ptr<CellInterface> cell) const
    {
        return shared_ptr_hasher(cell);
    }
    std::hash<std::shared_ptr<CellInterface>> shared_ptr_hasher;
};*/

class Sheet : public SheetInterface {
public:
    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

    // Можете дополнить ваш класс нужными полями и методами

    void ReplaceCell(Position pos, std::string text);

private:
    std::unordered_map<Position, std::unique_ptr<CellInterface>, PosHasher> pos_and_cells;
    std::set<Position> all_pos;
    //std::vector<std::vector< std::unique_ptr<CellInterface>>> pos_and_cells;
    //std::map<Position, std::unique_ptr<CellInterface>> cells_in_row;
    // Можете дополнить ваш класс нужными полями и методами
};