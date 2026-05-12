#pragma once

#include "common.h"
#include "formula.h"

#include <memory>
#include <vector>
#include <unordered_set>
#include <optional>
#include <string>

class SheetInterface;

class Cell : public CellInterface {
public:
    Cell();
    ~Cell();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;
    void Set(std::string text);
    void Clear();
    void Recalculate(const SheetInterface& sheet);

    const std::vector<Position>& GetDependencies() const;
    void AddDependent(Position pos);
    void RemoveDependent(Position pos);
    const std::unordered_set<Position>& GetDependents() const;


    void InvalidateCache();
    bool IsCacheValid() const;

private:

    class Impl {
    public:
        virtual ~Impl() = default;
        virtual std::string GetText() const = 0;
        virtual std::vector<Position> GetReferencedCells() const = 0;
        virtual bool IsFormula() const = 0;
    };

    class EmptyImpl : public Impl {
    public:
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;
        bool IsFormula() const override;
    };

    class TextImpl : public Impl {
    public:
        explicit TextImpl(std::string text);
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;
        bool IsFormula() const override;
        
    private:
        std::string text_; // Хранит "сырой" текст, включая апостроф
    };


    class FormulaImpl : public Impl {
    public:
        explicit FormulaImpl(std::string formula_text);
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;
        bool IsFormula() const override;
        const FormulaInterface& GetFormula() const { return *formula_; }
        
    private:
        std::unique_ptr<FormulaInterface> formula_;
        std::string formula_text_;
    };

    std::unique_ptr<Impl> impl_;
    
    mutable std::optional<Value> cached_value_;
    mutable bool is_cache_valid_ = false;

    std::vector<Position> dependencies_;
    std::unordered_set<Position> dependents_;
    std::string text_;
};