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

#ifndef CHOGORI_GATE_CLIENT_H
#define CHOGORI_GATE_CLIENT_H

#include "yb/pggate/k2doc.h"
#include "yb/common/status.h"

namespace k2 {
namespace gate {

using namespace yb;

class K2Client {
 public:
  K2Client(DocApi doc_api) : doc_api_(doc_api) {
  };

  ~K2Client();

  // Create a new namespace with the given name.
  CHECKED_STATUS CreateNamespace(const std::string& namespace_name,
                                 const std::string& creator_role_name = "",
                                 const std::string& namespace_id = "",
                                 const std::string& source_namespace_id = "",
                                 const std::optional<uint32_t>& next_pg_oid = std::nullopt);
                                 
  // Delete namespace with the given name.
  CHECKED_STATUS DeleteNamespace(const std::string& namespace_name,
                                 const std::string& namespace_id = "");

  // Delete the specified table.
  // Set 'wait' to true if the call must wait for the table to be fully deleted before returning.
  CHECKED_STATUS DeleteTable(const std::string& table_id, bool wait = true);    

private:
  DocApi doc_api_;
};

}  // namespace gate
}  // namespace k2

#endif //CHOGORI_GATE_CLIENT_H