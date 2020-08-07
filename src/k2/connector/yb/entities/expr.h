// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
// The following only applies to changes made to this file as part of YugaByte development.
//
// Portions Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//
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

#ifndef CHOGORI_SQL_EXPR_H
#define CHOGORI_SQL_EXPR_H

#include "yb/entities/value.h"

namespace k2 {
namespace sql {

    enum Opcode {
        PG_EXPR_CONSTANT,
        PG_EXPR_COLREF,
        PG_EXPR_VARIABLE,

        // The logical expression for defining the conditions when we support WHERE clause.
        PG_EXPR_NOT,
        PG_EXPR_EQ,
        PG_EXPR_NE,
        PG_EXPR_GE,
        PG_EXPR_GT,
        PG_EXPR_LE,
        PG_EXPR_LT,

        // Aggregate functions.
        PG_EXPR_AVG,
        PG_EXPR_SUM,
        PG_EXPR_COUNT,
        PG_EXPR_MAX,
        PG_EXPR_MIN,
    };

    class SqlExpr {
        public:
        explicit SqlExpr(Opcode op, SQLValue value) : value_(std::move(value)) {
            op_ = op;
        }

        ~SqlExpr() {
        }

        Opcode op() {
            return op_;
        }

        SQLValue value() {
            return value_;
        }

        const std::string ToString() const;   

        private:
        Opcode op_;
        SQLValue value_;
    };

}  // namespace sql
}  // namespace k2

#endif //CHOGORI_SQL_EXPR_H