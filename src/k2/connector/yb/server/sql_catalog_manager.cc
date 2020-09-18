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

#include "sql_catalog_manager.h"
#include <algorithm>
#include <list>
#include <thread>
#include <vector>

#include <glog/logging.h>
#include <gflags/gflags.h>

#include "yb/common/status.h"
#include "yb/common/flag_tags.h"
#include "yb/common/env.h"

using namespace yb;

DEFINE_int32(sql_executor_svc_num_threads, -1,
"Number of threads for the SQLCatalogManager service. If -1, it is auto configured.");
TAG_FLAG(sql_executor_svc_num_threads, advanced);

namespace k2 {

namespace sql {
    static yb::Env* default_env;

    SQLCatalogManager::~SQLCatalogManager() {
        Shutdown();
    }

    Status SQLCatalogManager::Init() {
        CHECK(!initted_.load(std::memory_order_acquire));
        log_prefix_ = Format("P $0: ", cluster_uuid());

        // TODO: initialization steps

        initted_.store(true, std::memory_order_release);
        return Status::OK();
    }

    Status SQLCatalogManager::WaitInited() {
        //TODO: WaitForAllBootstrapsToFinish();
        return Status::OK();
    }

    void SQLCatalogManager::AutoInitServiceFlags() {
        const int32 num_cores = base::NumCPUs();

        if (FLAGS_sql_executor_svc_num_threads == -1) {
            // Auto select number of threads for the TS service based on number of cores.
            // But bound it between 64 & 512.
            const int32 num_threads = std::min(512, num_cores * 32);
            FLAGS_sql_executor_svc_num_threads = std::max(64, num_threads);
            LOG(INFO) << "Auto setting FLAGS_sql_executor_svc_num_threads to "
                      << FLAGS_sql_executor_svc_num_threads;
        }
    }

    Status SQLCatalogManager::RegisterServices() {
        // TODO: wire service components

        return Status::OK();
    }

    Status SQLCatalogManager::Start() {
        CHECK(initted_.load(std::memory_order_acquire));

        AutoInitServiceFlags();

        RETURN_NOT_OK(RegisterServices());

        // TODO: start up steps

        google::FlushLogFiles(google::INFO); // Flush the startup messages.

        return Status::OK();
    }

    void SQLCatalogManager::Shutdown() {
        LOG(INFO) << "SQLCatalogManager shutting down...";

        bool expected = true;
        if (initted_.compare_exchange_strong(expected, false, std::memory_order_acq_rel)) {
            // TODO: shut down steps

        }

        LOG(INFO) << "SQLCatalogManager shut down complete. Bye!";
    }

    void SQLCatalogManager::set_cluster_uuid(const std::string& cluster_uuid) {
        std::lock_guard<simple_spinlock> l(lock_);
        cluster_uuid_ = cluster_uuid;
    }

    std::string SQLCatalogManager::cluster_uuid() const {
        std::lock_guard<simple_spinlock> l(lock_);
        return cluster_uuid_;
    }

    Env* SQLCatalogManager::GetEnv() {
        return default_env;
    }

    void SQLCatalogManager::SetCatalogVersion(uint64_t new_version) {
        std::lock_guard<simple_spinlock> l(lock_);
        uint64_t ysql_catalog_version_ = catalog_version_.load(std::memory_order_acquire);
        if (new_version > ysql_catalog_version_) {
            catalog_version_.store(new_version, std::memory_order_release);
        } else if (new_version < ysql_catalog_version_) {
            LOG(DFATAL) << "Ignoring ysql catalog version update: new version too old. "
                        << "New: " << new_version << ", Old: " << ysql_catalog_version_;
        }
    }
}
}



