/*
MIT License

Copyright(c) 2020 Futurewei Cloud

    Permission is hereby granted,
    free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

    The above copyright notice and this permission notice shall be included in all copies
    or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS",
    WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER
    LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef CHOGORI_SQL_DEFAULTS_H
#define CHOGORI_SQL_DEFAULTS_H

#include <string>

namespace k2pg {
namespace sql {
namespace catalog {

struct CatalogConsts {
    static const std::string default_cluster_id;
    static const std::string skv_collection_name_sql_primary;
    static const std::string skv_schema_name_cluster_info;
    static const std::string skv_schema_name_namespace_info;
    static const std::string skv_schema_name_sys_catalog_tablehead;
    static const std::string skv_schema_name_sys_catalog_tablecolumn;
    static const std::string skv_schema_name_sys_catalog_indexcolumn;

    static const std::string TABLE_ID_COLUMN_NAME;
    static const std::string INDEX_ID_COLUMN_NAME;
    static const std::string INDEXED_TABLE_ID_COLUMN_NAME;
};

} // namespace catalog
}  // namespace sql
}  // namespace k2pg

#endif //CHOGORI_SQL_DEFAULTS_H     