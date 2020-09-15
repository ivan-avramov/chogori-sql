// Copyright(c) 2020 Futurewei Cloud
//
// Permission is hereby granted,
//        free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all copies
// or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS",
// WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
//        DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef CHOGORI_GATE_DOC_API_H
#define CHOGORI_GATE_DOC_API_H

#include <string>
#include <list>
#include <vector>
#include <optional>

#include "yb/common/type/slice.h"
#include "yb/entities/entity_ids.h"
#include "yb/entities/schema.h"
#include "yb/entities/type.h"
#include "yb/entities/value.h"
//#include "yb/entities/expr.h"
#include "yb/entities/table.h"

#include "yb/pggate/pg_tuple.h"

namespace k2 {
namespace gate {
    using std::shared_ptr;
    using std::unique_ptr;

    using namespace yb;
    using namespace k2::sql;

    class DocCondition; 

    // An expression in a WHERE condition.
    // - Bind values would be given by client and grouped into a repeated field that can be accessed
    //   by their indexes.
    // - Alias values would be computed by server and grouped into repeated field that can be accessed
    //   by their indexes.
    // - Code generator write indexes as ref. Executor deref indexes to get actual values.
    class DocExpr {
        public:
        enum class ExprType {
            VALUE,
            LIST_VALUES,
            COLUMN_ID,
            BIND_ID,
            ALIAS_ID,
            CONDITION,
        };

        DocExpr(ExprType type, SqlValue* value) : type_(type), value_(value) {
        }

        DocExpr(ExprType type, int32_t id) : type_(type), id_(id) {
        }

        DocExpr(DocCondition *condition) : type_(ExprType::CONDITION), condition_(condition) {
        }

        DocExpr() {       
        }

        ~DocExpr() {
        }

        void setValue(SqlValue *value) {
            type_ = ExprType::VALUE;
            value_ = value;
        }

        void setColumnId(int32_t id) {
            type_ = ExprType::COLUMN_ID;
            id_ = id;
        }

        void setBindId(int32_t id) {
            type_ = ExprType::BIND_ID;
            id_ = id;
        }

        void setAliasId(int32_t id) {
            type_ = ExprType::ALIAS_ID;
            id_ = id;
        }

        void setCondition(DocCondition* condition) {
            type_ = ExprType::CONDITION;
            condition_ = condition;
        }

        void addListValue(SqlValue* value) {
            type_ = ExprType::LIST_VALUES;
            values_.push_back(value);
        }

        ExprType getType() {
            return type_;
        }

        SqlValue* getValue() {
            return value_;
        }

        int32_t getId() {
            return id_;
        }

        DocCondition* getCondition() {
            return condition_;
        }

        std::vector<SqlValue*> getListValues() {
            return values_;
        }

        private:
        ExprType type_;
        SqlValue* value_;
        std::vector<SqlValue*> values_;
        DocCondition* condition_;
        int32_t id_;
    };

    class DocCondition {
        public:  
        DocCondition() = default;
        ~DocCondition() = default;

        void setOp(PgExpr::Opcode op) {
            op_ = op;
        }

        PgExpr::Opcode getOp() {
            return op_;
        }

        void addOperand(DocExpr* expr) {
            operands_.push_back(expr);
        }

        std::vector<DocExpr*>& getOperands() {
            return operands_;
        }

        private: 
        PgExpr::Opcode op_;
        std::vector<DocExpr*> operands_;
    };

    struct DocColumnRefs {
        std::vector<int32_t> ids;
    };

    // pass the information so that the under hood SKV client could generate the actual doc key from it
    struct DocKey { 
        NamespaceId namespace_id;
        TableId table_id;
        std::vector<SqlValue> key_cols;
        DocKey(NamespaceId& nid, TableId& tid, std::vector<SqlValue>& keys) : 
            namespace_id(nid), table_id(tid), key_cols(std::move(keys)) {
        }
    };

    struct ColumnValue {
        int column_id;
        DocExpr* expr;
    };

    struct RSColDesc {
        string name;
        SQLType type;
    };

    struct RSRowDesc {
        RSRowDesc() = default;

        std::vector<RSColDesc> rscol_descs;
    };

    struct DocPagingState {
        TableId table_id;
        string next_token;
        uint64_t total_num_rows_read;
    };

    struct DocReadRequest {
        DocReadRequest() = default;

        string client_id;
        int64_t stmt_id;
        NamespaceId namespace_id;
        TableId table_id;
        uint64_t schema_version;
        uint32_t hash_code;
        std::vector<DocExpr*> partition_column_values;
        std::vector<DocExpr*> range_column_values;
        DocExpr* ybctid_column_value;
        // For select using local secondary index: this request selects the ybbasectids to fetch the rows
        // in place of the primary key above.
        DocReadRequest* index_request;

        RSRowDesc rsrow_desc;
        std::vector<DocExpr*> targets;
        DocExpr* where_expr;
        DocCondition* condition_expr;
        DocColumnRefs* column_refs;
        bool is_forward_scan = true;
        bool distinct = false;
        bool is_aggregate = false;
        uint64_t limit;
        DocPagingState* paging_state;
        bool return_paging_state = false;
        uint32_t max_hash_code;
        uint64_t catalog_version;
        RowMarkType row_mark_type;
        string* max_partition_key;

        std::unique_ptr<DocReadRequest> clone();
    };

    struct DocWriteRequest {
       DocWriteRequest() = default;

       enum StmtType {
            PGSQL_INSERT = 1,
            PGSQL_UPDATE = 2,
            PGSQL_DELETE = 3,
            PGSQL_UPSERT = 4,
            PGSQL_TRUNCATE_COLOCATED = 5
        };
        
        string client_id;
        int64_t stmt_id;
        StmtType stmt_type;
        NamespaceId namespace_id;
        TableId table_id;
        uint64_t schema_version;
        uint32_t hash_code;
        std::vector<DocExpr*> partition_column_values;
        std::vector<DocExpr*> range_column_values;
        DocExpr* ybctid_column_value;
        // Not used with UPDATEs. Use column_new_values to UPDATE a value.
        std::vector<ColumnValue> column_values;
        // Column New Values.
        // - Columns to be overwritten (UPDATE SET clause). This field can contain primary-key columns.
        std::vector<ColumnValue> column_new_values;
        RSRowDesc rsrow_desc;
        std::vector<DocExpr*> targets;
        DocExpr* where_expr;
        DocCondition* condition_expr;
        DocColumnRefs* column_refs;
        uint64_t catalog_version;
        // True only if this changes a system catalog table (or index).
        bool is_ysql_catalog_change;

        std::unique_ptr<DocWriteRequest> clone();
    };

    // Response from K2 storage for both read and write.
    struct DocResponse {
          enum class RequestStatus {
            PGSQL_STATUS_OK = 0,
            PGSQL_STATUS_SCHEMA_VERSION_MISMATCH = 1,
            PGSQL_STATUS_RUNTIME_ERROR = 2,
            PGSQL_STATUS_USAGE_ERROR = 3,
            PGSQL_STATUS_RESTART_REQUIRED_ERROR = 4,
            PGSQL_STATUS_DUPLICATE_KEY_ERROR = 5
        };
        RequestStatus status = RequestStatus::PGSQL_STATUS_OK;
        bool skipped;
        string error_message;
        int32_t rows_data_sidecar;
        DocPagingState* paging_state;
        int32_t rows_affected_count;
   
        //
        // External statuses.
        //
        // If you add more of those, make sure they are correctly picked up, e.g.
        // by PgDocReadCall::ReceiveResponse and PgDocCall::HandleResponseStatus
        //

        // PostgreSQL error code encoded as in errcodes.h or yb_pg_errcodes.h.
        // See https://www.postgresql.org/docs/11/errcodes-appendix.html
        uint64_t pg_error_code;

        // Transaction error code, obtained by static_cast of TransactionErrorTag::Decode
        // of Status::ErrorData(TransactionErrorTag::kCategory)
        int32_t txn_error_code;      
    };

    class DocCall {
        public: 
        enum Type {
            WRITE = 8,
            READ = 9,
        };

        explicit DocCall(const std::shared_ptr<TableInfo>& table);

        ~DocCall();

        virtual std::string ToString() const = 0;
        virtual Type type() const = 0;
        virtual bool read_only() const = 0;
        virtual bool returns_sidecar() = 0;

        const DocResponse& response() const { return *response_; }

        std::string&& rows_data() { return std::move(rows_data_); }

        bool IsTransactional() const {
            return table_->schema().table_properties().is_transactional();
        }

        bool succeeded() const {
            return response().status == DocResponse::RequestStatus::PGSQL_STATUS_OK;
        }

        bool applied() {
            return succeeded() && !response_->skipped;
        }

        bool is_active() const {
            return is_active_;
        }

        void set_active(bool val) {
            is_active_ = val;
        }
        
        protected:
        std::shared_ptr<TableInfo> table_;
        std::unique_ptr<DocResponse> response_;
        std::string rows_data_;
        bool is_active_ = true;
    };

    class DocWriteCall : public DocCall {
        public:
        explicit DocWriteCall(const std::shared_ptr<TableInfo>& table);

        ~DocWriteCall();

        virtual Type type() const { 
            return WRITE; 
        }

        const DocWriteRequest& request() const { return *write_request_; }

        std::string ToString() const;

        bool read_only() const override { return false; };

         // TODO check for e.g. returning clause.
        bool returns_sidecar() override { return true; }

        bool IsTransactional() const;

        void set_is_single_row_txn(bool is_single_row_txn) {
            is_single_row_txn_ = is_single_row_txn;
        }

        // Create a deep copy of this call, copying all fields and request PB content.
        // Does NOT, however, copy response and rows data.
        std::unique_ptr<DocWriteCall> DeepCopy();

        private: 
        std::unique_ptr<DocWriteRequest> write_request_;
        // Whether this operation should be run as a single row txn.
        // Else could be distributed transaction (or non-transactional) depending on target table type.
        bool is_single_row_txn_ = false;
    };

    class DocReadCall : public DocCall {
        public:
        explicit DocReadCall(const std::shared_ptr<TableInfo>& table);

        ~DocReadCall() {};

        DocReadRequest& request() const { return *read_request_; }
  
        std::string ToString() const;

        bool read_only() const override { return true; };

        bool returns_sidecar() override { return true; }

        virtual Type type() const { return READ; }

        // Create a deep copy of this call, copying all fields and request PB content.
        // Does NOT, however, copy response and rows data.
        std::unique_ptr<DocReadCall> DeepCopy();

        void set_return_paging_state(bool return_paging_state) {
            read_request_->return_paging_state = return_paging_state;
        }

        private:
        std::unique_ptr<DocReadRequest> read_request_;
    };


    struct SaveOrUpdateSchemaResponse {
        std::optional<std::string> error_code;
    };

    struct RowRecord {
        std::vector<SqlValue> cols;
    };

    struct SaveOrUpdateRecordResponse {
        std::optional<std::string> error_code;
    };

    struct RowRecords {
        std::vector<RowRecord> rows;
        std::string next_token;
    };

    struct DeleteRecordResponse {
        std::optional<std::string> error_code;
    };

    struct DeleteRecordsResponse {
        std::optional<std::string> error_code;
    };

    // this is the bridge between the SQL layer and the under hood K2 SKV APIs
    class DocApi {
        public:
        DocApi() = default;

        ~DocApi() {
        }

        std::string getDocKey(DocReadRequest& request);

        std::string getDocKey(DocWriteRequest& request);

/*
        DocResponse read(DocReadRequest request);

        DocResponse write(DocWriteRequest request);

        SaveOrUpdateSchemaResponse saveOrUpdateSchema(DocKey& key, Schema& schema);

        SaveOrUpdateRecordResponse saveOrUpdateRecord(DocKey& key, RowRecord& record);

        RowRecord getRecord(DocKey& key);

        RowRecords batchGetRecords(DocKey& key, std::vector<PgExpr>& filters, std::string& token);

        DeleteRecordResponse deleteRecord(DocKey& key);

        DeleteRecordsResponse batchDeleteRecords(DocKey& key, std::vector<PgExpr>& filters);

        DeleteRecordsResponse deleteAllRecordsAndSchema(DocKey& key);
        */
    };

}  // namespace gate
}  // namespace k2

#endif //CHOGORI_GATE_DOC_API_H