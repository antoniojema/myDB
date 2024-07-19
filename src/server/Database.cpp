//#include "Database.h"
//
//#include <iostream>
//#include <sstream>
//
//namespace Database {
//
//
//std::optional<TableEntry> Table::get(const std::string_view& key) {
//    auto it = this->data.find(key);
//    if (it == this->data.end()) {
//        return std::nullopt;
//    }
//    return TableEntry{*it};
//}
//std::optional<ConstTableEntry> Table::get(const std::string_view& key) const {
//    auto it = this->data.find(key);
//    if (it == this->data.end()) {
//        return std::nullopt;
//    }
//    return ConstTableEntry{*it};
//}
//
//bool Table::create(std::string&& key, signed long long val) {
//    auto [_it, inserted] = this->data.try_emplace(std::move(key), val);
//    return inserted;
//}
//
//bool Table::update(const std::string_view& key, signed long long val) {
//    auto it = this->data.find(key);
//    if (it == this->data.end()) {
//        return false;
//    }
//    it->second = val;
//}
//
//void Table::update_or_create(std::string&& key, signed long long val) {
//    this->data.insert_or_assign(std::move(key), val);
//}
//
//void Database::create_table(std::string&& name) {
//    auto& names_entry = this->names_table[std::string{name}];
//
//    TableID id = this->get_new_ID();
//    this->tables.try_emplace(id, id, std::move(name));
//
//    names_entry.emplace_back(id);
//}
//
//bool Database::delete_table(TableID id) {
//    auto it = this->tables.find(id);
//    if (it == this->tables.end()) {
//        return false;
//    }
//    auto& name = it->second.get_name();
//    this->tables.erase(id);
//
//    auto& ids = this->names_table.at(name);
//    ids.erase(std::find(ids.begin(), ids.end(), id));
//
//    return true;
//}
//
//bool Database::delete_table(const std::string_view& name) {
//    auto it = this->names_table.find(name);
//    if (it == this->names_table.end() || it->second.size() != 1) {
//        return false;
//    }
//    TableID id = it->second[0];
//    it->second.resize(0);
//    this->tables.erase(id);
//    return true;
//}
//
//size_t Database::delete_tables(const std::string_view& name) {
//    auto it = this->names_table.find(name);
//    if (it == this->names_table.end()) {
//        return false;
//    }
//    for (TableID id : it->second) {
//        this->tables.erase(id);
//    }
//    it->second.resize(0);
//    return true;
//}
//
//std::optional<Table*> Database::get_table(TableID id) {
//    auto it = this->tables.find(id);
//    if (it == this->tables.end()) {
//        return std::nullopt;
//    }
//    return &it->second;
//}
//std::optional<const Table*> Database::get_table(TableID id) const {
//    auto it = this->tables.find(id);
//    if (it == this->tables.end()) {
//        return std::nullopt;
//    }
//    return &it->second;
//}
//
//std::set<TableID> Database::get_tables(const std::string_view& name) const {
//    auto it = this->names_table.find(name);
//    return std::set<TableID>(it->second.begin(), it->second.end());
//}
//
//TableID Database::get_new_ID() {
//    TableID id = 0;
//    
//    bool found = true;
//    while(found) {
//        bool found = false;
//        for (auto& it : this->tables) {
//            if (it.first == id) {
//                ++id;
//                found = true;
//            }
//        }
//    };
//
//    return id;
//}
//
//static std::optional<std::pair<std::string_view,size_t>> get_word(const std::string_view& str) {
//    size_t begin_pos = str.find_first_not_of(" \t\r\n");
//    if (begin_pos == std::string_view::npos) {
//        return std::nullopt;
//    }
//    
//    if (str[begin_pos] != '\"') {
//        auto substr = str.substr(begin_pos);
//        size_t len = substr.find_first_of(" \t\r\n\"");
//        return std::make_pair(substr.substr(0,len), (len == std::string::npos) ? (str.length() - begin_pos) : (begin_pos + len));
//    }
//    else {
//        auto substr = str.substr(begin_pos+1);
//        size_t len = substr.find('\"');
//        return std::make_pair(substr.substr(0,len), (len == std::string::npos) ? (str.length() - begin_pos) : (begin_pos + len + 2));
//    }
//}
//
//std::vector<std::string_view> get_words(std::string_view str) {
//    std::vector<std::string_view> words {};
//    while (true) {
//        auto word_opt = get_word(str);
//        if (!word_opt.has_value()) break;
//
//        auto& [word, advance] = *word_opt;
//
//        words.emplace_back(std::move(word));
//        str = str.substr(advance);
//    }
//    return words;
//}
//
//template <typename T>
//static T to(std::string&& str) {
//    std::istringstream iss(std::move(str));
//    T val {};
//    iss >> val;
//    return val;
//}
//
//std::optional<Query> parse_get(const std::vector<std::string_view> words) {
//    if (words.size() < 2) return std::nullopt;
//
//    if (words[1] == "ENTRY") {
//        if (words.size() != 6) return std::nullopt;
//
//        auto& key = words[2];
//        if (words[3] != "IN") return std::nullopt;
//        if (words[4] != "TABLE") return std::nullopt;
//        auto& table_identifier_str = words[5];
//
//        std::variant<TableID, std::string> table_identifier;
//        if (table_identifier_str[0] >= '0' && table_identifier_str[0] <= '9') {
//            table_identifier = to<TableID>(std::string(table_identifier_str));
//        }
//        else {
//            table_identifier = std::string(table_identifier_str);
//        }
//
//        QueryGet query {
//            .table = table_identifier,
//            .key = std::string(key),
//        };
//        return Query{ query };
//    }
//    else {
//        return std::nullopt;
//    }
//}
//
//std::optional<Query> parse_update(const std::vector<std::string_view> words) {
//    if (words.size() < 2) return std::nullopt;
//
//    if (words[1] == "ENTRY") {
//        if (words.size() != 8) return std::nullopt;
//
//        auto& key = words[2];
//        if (words[3] != "TO") return std::nullopt;
//        auto val = to<TableValue>(std::string(words[4]));
//        if (words[5] != "IN") return std::nullopt;
//        if (words[6] != "TABLE") return std::nullopt;
//        auto& table_identifier_str = words[7];
//
//        std::variant<TableID, std::string> table_identifier;
//        if (table_identifier_str[0] >= '0' && table_identifier_str[0] <= '9') {
//            table_identifier = to<TableID>(std::string(table_identifier_str));
//        }
//        else {
//            table_identifier = std::string(table_identifier_str);
//        }
//        
//        QueryUpdate query {
//            .table = table_identifier,
//            .key = std::string(key),
//            .val = val,
//        };
//        return Query{ query };
//    }
//    else {
//        return std::nullopt;
//    }
//}
//
//std::optional<Query> parse_create(const std::vector<std::string_view> words) {
//    if (words.size() < 2) return std::nullopt;
//
//    if (words[1] == "TABLE") {
//        if (words.size() != 3) return std::nullopt;
//
//        auto& table_name = words[2];
//
//        QueryCreateTable query {
//            .table = std::string(table_name)
//        };
//        return Query{ query };
//    }
//    else if (words[1] == "ENTRY") {
//        if (words.size() != 8) return std::nullopt;
//
//        auto& key = words[2];
//        if (words[3] != "TO") return std::nullopt;
//        auto val = to<TableValue>(std::string(words[4]));
//        if (words[5] != "IN") return std::nullopt;
//        if (words[6] != "TABLE") return std::nullopt;
//        auto& table_identifier_str = words[7];
//
//        std::variant<TableID, std::string> table_identifier;
//        if (table_identifier_str[0] >= '0' && table_identifier_str[0] <= '9') {
//            table_identifier = to<TableID>(std::string(table_identifier_str));
//        }
//        else {
//            table_identifier = std::string(table_identifier_str);
//        }
//
//        QueryCreateEntry query {
//            .table = table_identifier,
//            .key = std::string(key),
//            .val = val,
//        };
//        return Query{ query };
//    }
//    else {
//        return std::nullopt;
//    }
//}
//
//std::optional<Query> parse_delete(const std::vector<std::string_view> words) {
//    if (words.size() < 2) return std::nullopt;
//
//    if (words[1] == "TABLE") {
//        if (words.size() != 3) return std::nullopt;
//
//        auto& table_identifier_str = words[2];
//
//        std::variant<TableID, std::string> table_identifier;
//        if (table_identifier_str[0] >= '0' && table_identifier_str[0] <= '9') {
//            table_identifier = to<TableID>(std::string(table_identifier_str));
//        }
//        else {
//            table_identifier = std::string(table_identifier_str);
//        }
//
//        QueryDeleteTable query{
//            .table = table_identifier,
//        };
//        return Query{ query };
//    }
//    else if (words[1] == "ENTRY") {
//        if (words.size() != 6) return std::nullopt;
//
//        auto& key = words[2];
//        if (words[3] != "IN") return std::nullopt;
//        if (words[4] != "TABLE") return std::nullopt;
//        auto& table_identifier_str = words[5];
//
//        std::variant<TableID, std::string> table_identifier;
//        if (table_identifier_str[0] >= '0' && table_identifier_str[0] <= '9') {
//            table_identifier = to<TableID>(std::string(table_identifier_str));
//        }
//        else {
//            table_identifier = std::string(table_identifier_str);
//        }
//
//        QueryDeleteEntry query {
//            .table = table_identifier,
//            .key = std::string(key),
//        };
//        return Query{ query };
//    }
//    else {
//        return std::nullopt;
//    }
//}
//
//std::optional<Query> parse_query(std::string_view str) {
//    auto words = get_words(str);
//
//    if (words.size() == 0) return std::nullopt;
//
//    if (words[0] == "GET") {
//        return parse_get(words);
//    }
//    if (words[0] == "CREATE") {
//        return parse_create(words);
//    }
//    if (words[0] == "UPDATE") {
//        return parse_update(words);
//    }
//    if (words[0] == "DELETE") {
//        return parse_delete(words);
//    }
//    else {
//        return std::nullopt;
//    }
//}
//
//} //namespace Database