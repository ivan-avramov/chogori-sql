//
// THE SOFTWARE IS PROVIDED "AS IS",
// WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
//        DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "yb/pggate/k2client.h"

namespace k2 {
namespace gate {

Status K2Client::CreateNamespace(const std::string& namespace_name,
                                 const std::string& creator_role_name,
                                 const std::string& namespace_id,
                                 const std::string& source_namespace_id,
                                 const std::optional<uint32_t>& next_pg_oid) {
  // TODO: add implementation                                   
  return Status::OK();
}

Status K2Client::DeleteNamespace(const std::string& namespace_name,
                                 const std::string& namespace_id) {
  // TODO: add implementation                                   
  return Status::OK();
}

Status K2Client::DeleteTable(const string& table_id, bool wait) {
  // TODO: add implementation                                   
  return Status::OK();
}

}  // namespace gate
}  // namespace k2