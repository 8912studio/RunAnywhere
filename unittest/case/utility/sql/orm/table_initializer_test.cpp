#include <gtest/gtest.h>
#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/orm_support.h"
#include "utility/sql/orm/table_initializer.h"

using namespace ra::utility::sql;

namespace {

class TableInitializerTestFixture : zaf::NonCopyableNonMovable {
public:
    TableInitializerTestFixture() {

        auto db_path = "test_data\\table_initializer_test.db";
        std::filesystem::remove(db_path);

        database_ = Database::Open(db_path);
    }

    ~TableInitializerTestFixture() {
        database_.reset();
    }

    Database& DB() {
        return *database_;
    }

private:
    std::optional<Database> database_;
};

}


struct TableNoPK {
    int integer_field{};
    float float_field{};
    std::string string_field;
};

SQL_TABLE_BEGIN(TableNoPK, TableNoPK);
SQL_COLUMN(IntegerField, integer_field);
//SQL_COLUMN(FloatField, float_field);
SQL_COLUMN(StringField, string_field);
SQL_TABLE_END;

TEST(TableInitializerTest, NewTableNoPK) {

    TableInitializerTestFixture fixture;

    TableInitializer::Initialize(Table<TableNoPK>::GetInstance(), fixture.DB());

    auto table_info = fixture.DB().GetTableInfo("TableNoPK");
    ASSERT_TRUE(table_info.has_value());
    ASSERT_EQ(table_info->columns.size(), 2);

    {
        const auto& column0 = table_info->columns[0];
        ASSERT_EQ(column0.name, "IntegerField");
        ASSERT_EQ(column0.data_type, DataType::Integer);
        ASSERT_EQ(column0.is_not_null, false);
        ASSERT_EQ(column0.is_primary_key, false);
    }

    {
        const auto& column1 = table_info->columns[1];
        ASSERT_EQ(column1.name, "StringField");
        ASSERT_EQ(column1.data_type, DataType::Text);
        ASSERT_EQ(column1.is_not_null, false);
        ASSERT_EQ(column1.is_primary_key, false);
    }
}


struct TablePK1 {
    int integer_field{};
    std::string string_field;
};

SQL_TABLE_BEGIN(TablePK1, TablePK1);
SQL_COLUMN(IntegerField, integer_field);
SQL_COLUMN(StringField, string_field);
SQL_PRIMARY_KEY(IntegerField);
SQL_TABLE_END;

TEST(TableInitializerTest, NewTablePK1) {

    TableInitializerTestFixture fixture;

    TableInitializer::Initialize(Table<TablePK1>::GetInstance(), fixture.DB());

    auto table_info = fixture.DB().GetTableInfo("TablePK1");
    ASSERT_TRUE(table_info.has_value());
    ASSERT_EQ(table_info->columns.size(), 2);

    {
        const auto& column0 = table_info->columns[0];
        ASSERT_EQ(column0.name, "IntegerField");
        ASSERT_EQ(column0.data_type, DataType::Integer);
        ASSERT_EQ(column0.is_not_null, false);
        ASSERT_EQ(column0.is_primary_key, true);
    }
}


struct TablePK1AutoInc {
    int integer_field{};
    std::string string_field;
};

SQL_TABLE_BEGIN(TablePK1AutoInc, TablePK1AutoInc);
SQL_COLUMN(IntegerField, integer_field);
SQL_COLUMN(StringField, string_field);
SQL_PRIMARY_KEY_AUTOINCREMENT(IntegerField);
SQL_TABLE_END;

TEST(TableInitializerTest, NewTablePK1AutoInc) {

    TableInitializerTestFixture fixture;

    TableInitializer::Initialize(Table<TablePK1AutoInc>::GetInstance(), fixture.DB());

    auto table_info = fixture.DB().GetTableInfo("TablePK1AutoInc");
    ASSERT_TRUE(table_info.has_value());
    ASSERT_EQ(table_info->columns.size(), 2);

    {
        const auto& column0 = table_info->columns[0];
        ASSERT_EQ(column0.name, "IntegerField");
        ASSERT_EQ(column0.data_type, DataType::Integer);
        ASSERT_EQ(column0.is_not_null, false);
        ASSERT_EQ(column0.is_primary_key, true);
    }

    fixture.DB().ExecuteSQL("insert into TablePK1AutoInc (StringField) values ('1')");
    ASSERT_EQ(fixture.DB().LastInsertRowID(), 1);
    fixture.DB().ExecuteSQL("insert into TablePK1AutoInc (StringField) values ('2')");
    ASSERT_EQ(fixture.DB().LastInsertRowID(), 2);
}


struct TablePK2 {
    int integer_field{};
    std::string string_field;
};

SQL_TABLE_BEGIN(TablePK2, TablePK2);
SQL_COLUMN(IntegerField, integer_field);
SQL_COLUMN(StringField, string_field);
SQL_PRIMARY_KEY(IntegerField, StringField);
SQL_TABLE_END;

TEST(TableInitializerTest, NewTablePK2) {

    TableInitializerTestFixture fixture;

    TableInitializer::Initialize(Table<TablePK2>::GetInstance(), fixture.DB());

    auto table_info = fixture.DB().GetTableInfo("TablePK2");
    ASSERT_TRUE(table_info.has_value());
    ASSERT_EQ(table_info->columns.size(), 2);

    {
        const auto& column0 = table_info->columns[0];
        ASSERT_EQ(column0.name, "IntegerField");
        ASSERT_EQ(column0.data_type, DataType::Integer);
        ASSERT_EQ(column0.is_not_null, false);
        ASSERT_EQ(column0.is_primary_key, true);
    }

    {
        const auto& column1 = table_info->columns[1];
        ASSERT_EQ(column1.name, "StringField");
        ASSERT_EQ(column1.data_type, DataType::Text);
        ASSERT_EQ(column1.is_not_null, false);
        ASSERT_EQ(column1.is_primary_key, true);
    }
}


struct OldTable {
    int integer_field{};
};

SQL_TABLE_BEGIN(AlterTable, OldTable);
SQL_COLUMN(IntField, integer_field);
SQL_TABLE_END;

struct NewTable {
    int integer_field{};
    std::string string_field;
};

SQL_TABLE_BEGIN(AlterTable, NewTable);
SQL_COLUMN(IntField, integer_field);
SQL_COLUMN(StringField, string_field);
SQL_TABLE_END;

TEST(TableInitializerTest, AlterTable) {

    TableInitializerTestFixture fixture;

    TableInitializer::Initialize(Table<OldTable>::GetInstance(), fixture.DB());
    auto old_table_info = fixture.DB().GetTableInfo("AlterTable");
    ASSERT_TRUE(old_table_info.has_value());
    ASSERT_EQ(old_table_info->columns.size(), 1);

    TableInitializer::Initialize(Table<NewTable>::GetInstance(), fixture.DB());
    auto new_table_info = fixture.DB().GetTableInfo("AlterTable");
    ASSERT_TRUE(new_table_info.has_value());
    ASSERT_EQ(new_table_info->columns.size(), 2);

    {
        const auto& column0 = new_table_info->columns[0];
        ASSERT_EQ(column0.name, "IntField");
        ASSERT_EQ(column0.data_type, DataType::Integer);
        ASSERT_EQ(column0.is_not_null, false);
        ASSERT_EQ(column0.is_primary_key, false);
    }

    {
        const auto& column1 = new_table_info->columns[1];
        ASSERT_EQ(column1.name, "StringField");
        ASSERT_EQ(column1.data_type, DataType::Text);
        ASSERT_EQ(column1.is_not_null, false);
        ASSERT_EQ(column1.is_primary_key, false);
    }
}