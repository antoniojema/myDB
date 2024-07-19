////#pragma once
////
////#include <string>
////#include <optional>
////#include <unordered_map>
////#include <map>
////#include <set>
////#include <variant>
////
////namespace Database {
////
////struct StringHash {
////    using is_transparent = void;
////    [[nodiscard]] size_t operator()(const char* txt) const {
////        return std::hash<std::string_view>{}(txt);
////    }
////    [[nodiscard]] size_t operator()(std::string_view txt) const {
////        return std::hash<std::string_view>{}(txt);
////    }
////    [[nodiscard]] size_t operator()(const std::string& txt) const {
////        return std::hash<std::string>{}(txt);
////    }
////};
////
////
////using TableKey = std::string;
////using TableValue = signed long long;
////using Map = std::unordered_map<TableKey, TableValue, StringHash, std::equal_to<>>;
////
////using TableID = size_t;
////
////
////class TableEntry {
////public:
////    const TableKey& get_key() const {return this->_entry.first;}
////
////    TableValue& get_value() {return this->_entry.second;}
////    const TableValue& get_value() const {return this->_entry.second;}
////
////private:
////    Map::value_type& _entry;
////    
////    TableEntry(Map::value_type& entry) : _entry(entry) {}
////
////    friend class Table;
////};
////
////
////class ConstTableEntry {
////public:
////    const TableKey& get_key() const {return this->_entry.first;}
////    const TableValue& get_value() const {return this->_entry.second;}
////
////private:
////    const Map::value_type& _entry;
////    
////    ConstTableEntry(const Map::value_type& entry) : _entry(entry) {}
////
////    friend class Table;
////};
////
////
////class Table {
////public:
////    Table(TableID id, std::string&& name) : _id(id), _name(std::move(name)) {}
////
////    std::optional<TableEntry> get(const std::string_view& key);
////    std::optional<ConstTableEntry> get(const std::string_view& key) const;
////    
////    bool create(std::string&& key, signed long long val);
////    bool update(const std::string_view& key, signed long long val);
////    
////    void update_or_create(std::string&& key, signed long long val);
////
////    const std::string& get_name() {return this->_name;}
////
////private:
////    TableID _id;
////    std::string _name;
////    Map data {};
////};
////
////
////class Database {
////public:
////    Database() = default;
////
////    void create_table(std::string&& name);
////
////    bool delete_table(TableID id);
////    bool delete_table(const std::string_view& name);
////    
////    size_t delete_tables(const std::string_view& name);
////
////    std::optional<Table*> get_table(TableID id);
////    std::optional<const Table*> get_table(TableID id) const;
////
////    std::set<TableID> get_tables(const std::string_view& name) const;
////
////private:
////    std::map<TableID, Table> tables;
////    std::unordered_map<std::string, std::vector<TableID>, StringHash, std::equal_to<>> names_table;
////
////    TableID get_new_ID();
////};
////
////
////struct QueryGet {
////    std::variant<TableID, std::string> table;
////    std::string key;
////};
////
////struct QueryUpdate {
////    std::variant<TableID, std::string> table;
////    std::string key;
////    TableValue val;
////};
////
////struct QueryCreateEntry {
////    std::variant<TableID, std::string> table;
////    std::string key;
////    TableValue val;
////};
////
////struct QueryCreateTable {
////    std::string table;
////};
////
////struct QueryDeleteEntry{
////    std::variant<TableID, std::string> table;
////    std::string key;
////    TableValue val;
////};
////
////struct QueryDeleteTable{
////    std::variant<TableID, std::string> table;
////    bool all;
////};
////
////struct Query {
////    std::variant<
////        QueryGet,
////        QueryUpdate,
////        QueryCreateEntry,
////        QueryCreateTable,
////        QueryDeleteEntry,
////        QueryDeleteTable
////    > query;
////};
////
////std::optional<Query> parse_query(std::string_view str);
////
////
////} // namespace Database
//
//
//#pragma once
//
//#include <vector>
//
//namespace Database {
//
//
//class Data {
//public:
//    
//private:
//    std::variant<>
//};
//
//
//struct TableRow {
//    std::vector<Data> data;
//    size_t id;
//};
//
//struct TableMedadata {
//
//};
//
//
//} // namespace Database 
