// Compilers Fall 2017. Project -- part I.
#ifndef LEXER_SYMBOL_TABLE_H
#define LEXER_SYMBOL_TABLE_H

#include <string>
#include <map>
#include <list>

class SymbolTable {
 public:
  // Entry in the symbol table (we will extend it in later parts of the
  // project).
  class Entry {
   public:
    std::string name;
  };

  // Returns the size of the symbol table.
  size_t size() const { return data_.size(); }

  // Clears the symbol table.
  void clear() { data_.clear(); }

  // Returns entry in symbol table for 'name' if exists, otherwise nullptr.
  SymbolTable::Entry* lookup(std::string name) {
    auto it = data_.find(name);
    return (it == data_.end()) ? nullptr : &(it->second);
  }

  // Add a new entry to the symbol table
  // (overriding an existing entry with same name, if any).
  SymbolTable::Entry* add(const SymbolTable::Entry& entry) {
    data_[entry.name] = entry;
    return lookup(entry.name);
  }

  // Returns a list of the entries in the symbol table ordered by name.
  std::list<SymbolTable::Entry> entries() const {
    std::list<SymbolTable::Entry> list_of_entries;
    for (auto it : data_) {
      list_of_entries.push_back(it.second);
    }
    return list_of_entries;
  }

 private:
  std::map<std::string, SymbolTable::Entry> data_;
};

#endif //LEXER_SYMBOL_TABLE_H
