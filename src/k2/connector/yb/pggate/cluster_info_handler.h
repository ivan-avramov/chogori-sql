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

#ifndef CHOGORI_SQL_CLUSTER_INFO_HANDLER_H
#define CHOGORI_SQL_CLUSTER_INFO_HANDLER_H

#include <string>

#include "yb/common/status.h"
#include "yb/pggate/sql_catalog_persistence.h"
#include "yb/pggate/k2_adapter.h"

namespace k2pg {
namespace sql {

using yb::Status;
using k2pg::gate::K2Adapter;

class ClusterInfoHandler : public std::enable_shared_from_this<ClusterInfoHandler> {
    public:
    typedef std::shared_ptr<ClusterInfoHandler> SharedPtr;

    ClusterInfoHandler(scoped_refptr<K2Adapter> k2_adapter);
    ~ClusterInfoHandler();

    CHECKED_STATUS CreateClusterInfo(ClusterInfo& cluster_info);

    CHECKED_STATUS UpdateClusterInfo(ClusterInfo& cluster_info);

    CHECKED_STATUS ReadClusterInfo(ClusterInfo& cluster_info);

    private:  
    scoped_refptr<K2Adapter> k2_adapter_;    
};

} // namespace sql
} // namespace k2pg

#endif //CHOGORI_SQL_CLUSTER_INFO_HANDLER_H