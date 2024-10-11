#pragma once

#include <string>
#include <optional>
#include <vector>
#include "values.h"
#include "tokens.h"
#include "common/utils.h"


namespace SQL {

struct Wildcard {};

using InsertColumns = std::vector<std::string>;
using InsertValues = std::vector<Value::Value>;

struct BinaryOperation;
using ColumnOperation = std::variant<std::string, Value::Integer, Value::Float, std::unique_ptr<BinaryOperation>>;
struct BinaryOperation {
    BinaryOperator op;
    ColumnOperation lhs, rhs;
};
using SelectColumns = std::variant<Wildcard, std::vector<ColumnOperation>>;

using Conditions = ColumnOperation;

struct ColumnUpdate {
    std::string column;
    ColumnOperation result;
};
using UpdateValues = std::vector<ColumnUpdate>;

struct ColumnInfo {
    std::string column;
    Value::Type datatype;
};
using CreateColumns = std::vector<ColumnInfo>;


struct Query {
    virtual ~Query() = 0;
};

struct InsertQuery : Query {
    std::string table;
    std::optional<InsertColumns> columns;
    InsertValues values;
};

struct SelectQuery : Query {
    std::string table;
    SelectColumns columns;
    std::optional<Conditions> conditions;
};

struct UpdateQuery : Query {
    std::string table;
    UpdateValues update_values;
    std::optional<Conditions> conditions;
};

struct DeleteQuery : Query {
    std::string table;
    std::optional<Conditions> conditions;
};

struct CreateTableQuery : Query {
    std::string table;
    CreateColumns columns;
};

struct DropTableQuery : Query {
    std::string table;
};

} // namespace SQL
