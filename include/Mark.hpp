#pragma once

enum class Symbol {EMPTY, X, O};

class Mark {
    private:
        Symbol symbol;


    public:
        // Default mark is EMPTY
        Mark() : symbol(Symbol::EMPTY) {};
        Mark(Symbol s) : symbol(s) {};

        Symbol getSymbol() const { return symbol; }
        void setSymbol(Symbol s) { symbol = s; }

        // Overloaded ==
        bool operator == (const Mark& rhs) const {
            return (this->symbol) == (rhs.symbol);
        }

        // Overloaded !=
        bool operator != (const Mark& rhs) const {
            return (this->symbol) != (rhs.symbol);
        }
};