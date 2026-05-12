#pragma once

#include "cell.h"
#include "common.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <ostream>

struct PositionHash {
    size_t operator()(const Position& pos) const {
        size_t h1 = std::hash<int>{}(pos.row);
        size_t h2 = std::hash<int>{}(pos.col);
        h1 ^= h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2);
        return h1;
    }
};

class Sheet : public SheetInterface {
public:
    ~Sheet() override = default;

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

    const Cell* GetConcreteCell(Position pos) const;
    Cell* GetConcreteCell(Position pos);

private:
    std::unordered_map<Position, std::unique_ptr<Cell>, PositionHash> cells_;
    
    int min_row_ = 0;
    int max_row_ = -1;
    int min_col_ = 0;
    int max_col_ = -1;

    void UpdatePrintableArea(const Position& pos, bool is_cleared);
    bool HasCycle(const Position& start_pos, const Position& target_pos,std::unordered_set<Position>& visited) const;
    
    void InvalidateCacheRecursive(const Position& pos, std::unordered_set<Position>& visited);
    void RemoveOldDependencies(const Position& pos, const std::vector<Position>& old_deps);
    void AddNewDependencies(const Position& pos, const std::vector<Position>& new_deps);
};

std::unique_ptr<SheetInterface> CreateSheet();