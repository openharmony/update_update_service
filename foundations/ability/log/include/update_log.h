/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef UPDATE_LOG_H
#define UPDATE_LOG_H

#include <cstdint>
#include <string>

#include "hilog/log.h"

namespace OHOS {
namespace UpdateEngine {
#ifdef UPDATE_SERVICE
static constexpr OHOS::HiviewDFX::HiLogLabel UPDATE_LABEL = {LOG_CORE, 0xD002E00, "UPDATE_SA"};
#else
static constexpr OHOS::HiviewDFX::HiLogLabel UPDATE_LABEL = {LOG_CORE, 0xD002E00, "UPDATE_KITS"};
#endif

const std::string DEFAULT_LABEL = "%";
const std::string DEFAULT_FMT_LABEL = "%s";
const std::string PRIVATE_FMT_LABEL = "%{private}s";
const std::string PUBLIC_FMT_LABEL = "%{public}s";

static constexpr uint32_t  UPDATER_SA_DOMAIN_ID = 0xD002E00;
enum UpdaterSaSubModule {
    UPDATE_ENGINE_TAG = 0,
    UPDATE_KITS_TAG,
    UPDATE_FIRMWARE_TAG,
    UPDATE_MODULEMGR_TAG,
    UPDATE_MODULE_MAX
};

static constexpr OHOS::HiviewDFX::HiLogLabel UPDATE_MODULE_LABEL[UPDATE_MODULE_MAX] = {
    { LOG_CORE, UPDATER_SA_DOMAIN_ID, "UPDATE_SA" },
    { LOG_CORE, UPDATER_SA_DOMAIN_ID, "UPDATE_KITS" },
    { LOG_CORE, UPDATER_SA_DOMAIN_ID, "UPDATE_FIRMWARE" },
    { LOG_CORE, UPDATER_SA_DOMAIN_ID, "UPDATER_MODULE_MGR" }
};

enum class UpdateLogLevel {
    UPDATE_DEBUG = 0,
    UPDATE_INFO,
    UPDATE_WARN,
    UPDATE_ERROR,
    UPDATE_FATAL
};

struct UpdateLogContent {
    HiviewDFX::HiLogLabel label;
    UpdateLogLevel level;
    std::string log;
    std::string args;
    std::string fileName;
    int32_t line;

    UpdateLogContent BuildWithArgs(const std::string &argsInput) const
    {
        return {label, level, log, argsInput, fileName, line};
    };

    UpdateLogContent BuildWithFmtAndArgs(const std::string &logInput, const std::string &argsInput) const
    {
        return {label, level, logInput, argsInput, fileName, line};
    };
};

class UpdateLog {
public:
    static bool JudgeLevel(const UpdateLogLevel &level);
    static void SetLogLevel(const UpdateLogLevel &level);
    static const UpdateLogLevel &GetLogLevel();
    static std::string GetBriefFileName(const std::string &file);
    static void PrintLongLog(const uint32_t subModuleTag, const UpdateLogContent &logContent);

private:
    static void PrintLog(const uint32_t module, const UpdateLogContent &logContent);
    static void PrintSingleLine(const uint32_t module, const UpdateLogContent &logContent);
    static std::pair<std::string, std::string> SplitLogByFmtLabel(const std::string &log, const std::string &fmtLabel);
    static std::string GetFmtLabel(const std::string &log);
    static int32_t FindSubStrCount(const std::string &str, const std::string &subStr);

private:
    static UpdateLogLevel level_;
};

#define R_FILENAME    (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#define R_FORMATED(fmt, ...) "[%{public}s] %{public}s#]" fmt, R_FILENAME, __FUNCTION__, ##__VA_ARGS__

#define EXEC_PRINT_HILOG(module, fmtlabel, level, fmt)    \
    if ((level) == UpdateLogLevel::UPDATE_ERROR) {    \
        EXEC_PRINT_HILOGE(module, fmtlabel, fmt);    \
    } else if ((level) == UpdateLogLevel::UPDATE_DEBUG) {    \
       EXEC_PRINT_HILOGD(module, fmtlabel, fmt);    \
    } else {    \
       EXEC_PRINT_HILOGI(module, fmtlabel, fmt);    \
    }

#define EXEC_PRINT_HILOGE(module, fmtlabel, fmt, ...)    \
    if (fmtlabel == PUBLIC_FMT_LABEL) {    \
        PRINT_HILOGE(module, "%{public}s", fmt);    \
    } else if (fmtlabel == PRIVATE_FMT_LABEL) {    \
       PRINT_HILOGE(module, "%{private}s", fmt);    \
    } else {    \
       PRINT_HILOGE(module, "%s", fmt);    \
    }

#define EXEC_PRINT_HILOGD(module, fmtlabel, fmt, ...)    \
    if (fmtlabel == PUBLIC_FMT_LABEL) {    \
        PRINT_HILOGD(module, "%{public}s", fmt);    \
    } else if (fmtlabel == PRIVATE_FMT_LABEL) {    \
       PRINT_HILOGD(module, "%{private}s", fmt);    \
    } else {    \
       PRINT_HILOGD(module, "%s", fmt);    \
    }

#define EXEC_PRINT_HILOGI(module, fmtlabel, fmt, ...)    \
    if (fmtlabel == PUBLIC_FMT_LABEL) {    \
        PRINT_HILOGI(module, "%{public}s", fmt);    \
    } else if (fmtlabel == PRIVATE_FMT_LABEL) {    \
       PRINT_HILOGI(module, "%{private}s", fmt);    \
    } else {    \
       PRINT_HILOGI(module, "%s", fmt);    \
    }

#define PRINT_HILOGE(module, fmt, ...) (void)HILOG_IMPL(LOG_CORE, LOG_ERROR,    \
    OHOS::UpdateEngine::UPDATE_MODULE_LABEL[module].domain, OHOS::UpdateEngine::UPDATE_MODULE_LABEL[module].tag,    \
    "[%{public}s] %{public}s#]" fmt, R_FILENAME, __FUNCTION__, ##__VA_ARGS__)
#define PRINT_HILOGI(module, fmt, ...) (void)HILOG_IMPL(LOG_CORE, LOG_INFO,    \
    OHOS::UpdateEngine::UPDATE_MODULE_LABEL[module].domain, OHOS::UpdateEngine::UPDATE_MODULE_LABEL[module].tag,    \
    "[%{public}s] %{public}s#]" fmt, R_FILENAME, __FUNCTION__, ##__VA_ARGS__)
#define PRINT_HILOGD(module, fmt, ...) (void)HILOG_IMPL(LOG_CORE, LOG_DEBUG,    \
    OHOS::UpdateEngine::UPDATE_MODULE_LABEL[module].domain, OHOS::UpdateEngine::UPDATE_MODULE_LABEL[module].tag,    \
    "[%{public}s] %{public}s#]" fmt, R_FILENAME, __FUNCTION__, ##__VA_ARGS__)

#define ENGINE_LOGE(fmt, ...) PRINT_HILOGE(UPDATE_ENGINE_TAG, fmt, ##__VA_ARGS__)
#define ENGINE_LOGI(fmt, ...) PRINT_HILOGI(UPDATE_ENGINE_TAG, fmt, ##__VA_ARGS__)
#define ENGINE_LOGD(fmt, ...) PRINT_HILOGD(UPDATE_ENGINE_TAG, fmt, ##__VA_ARGS__)

#define PRINT_LONG_LOGD(module, label, fmt, args) UpdateLog::PrintLongLog(module, {label,    \
    UpdateLogLevel::UPDATE_DEBUG, std::string(fmt), std::string(args), std::string(__FILE__), __LINE__})
#define PRINT_LONG_LOGI(module, label, fmt, args) UpdateLog::PrintLongLog(module, {label,    \
    UpdateLogLevel::UPDATE_INFO, std::string(fmt), std::string(args), std::string(__FILE__), __LINE__})
#define PRINT_LONG_LOGE(module, label, fmt, args) UpdateLog::PrintLongLog(module, {label,    \
    UpdateLogLevel::UPDATE_ERROR, std::string(fmt), std::string(args), std::string(__FILE__), __LINE__})

#define ENGINE_LONG_LOGD(fmt, args) PRINT_LONG_LOGD(UPDATE_ENGINE_TAG, UPDATE_LABEL, fmt, args)
#define ENGINE_LONG_LOGI(fmt, args) PRINT_LONG_LOGI(UPDATE_ENGINE_TAG, UPDATE_LABEL, fmt, args)
#define ENGINE_LONG_LOGE(fmt, args) PRINT_LONG_LOGE(UPDATE_ENGINE_TAG, UPDATE_LABEL, fmt, args)
} // namespace UpdateEngine
} // namespace OHOS
#endif // UPDATE_LOG_H