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

#include "firmware_result_process.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>

#include "string_utils.h"
#include "firmware_callback_utils.h"
#include "firmware_common.h"
#include "firmware_preferences_utils.h"
#include "firmware_task.h"
#include "firmware_task_operator.h"
#include "firmware_update_adapter.h"
#include "firmware_update_helper.h"

namespace OHOS {
namespace UpdateEngine {
static const std::string UPDATER_RESULT_FILE = "/data/updater/updater_result";
constexpr int32_t SYMBOL_LENGTH = 1;
constexpr uint32_t UPDATE_SUCCESSED = 1;
constexpr uint32_t UPDATE_FAILED = 2;
constexpr size_t  PKG_PATH_INDEX = 0;
constexpr size_t  RESULT_INDEX = 1;
constexpr size_t  REASON_INDEX = 2;

void FirmwareResultProcess::ParseResult(const std::vector<std::string> &results, std::string &value, size_t index)
{
    if (index >= results.size()) {
        return;
    }
    value = results[index];
    StringUtils::Trim(value);
}

UpdateResultCode FirmwareResultProcess::GetUpdaterResult(const std::vector<FirmwareComponent> &components,
    std::map<std::string, UpdateResult> &resultMap)
{
    FIRMWARE_LOGE("GetUpdaterResult");
    if (components.empty()) {
        FIRMWARE_LOGE("components is empty");
        return UpdateResultCode::FAILURE;
    }
    resultMap.clear();
    std::ifstream infile;
    infile.open(UPDATER_RESULT_FILE, std::ios_base::in);
    if (!infile.is_open()) {
        FIRMWARE_LOGE("open update status file fail!");
        HandleFileError(resultMap, components);
    } else {
        std::string buffer;
        while (!infile.eof()) {
            getline(infile, buffer);
            ParseUpdaterResultRecord(buffer, resultMap);
        }
        infile.close();
    }
    return HandleFileResults(resultMap, components);
}

UpdateResult FirmwareResultProcess::CompareVersion(const FirmwareComponent &component)
{
    bool isResultSuccess = false;
    isResultSuccess = component.versionNumber == FirmwareUpdateAdapter::GetDisplayVersion();
    FIRMWARE_LOGI("component.versionNumber=%{pubilc}s, GetDisplayVersion=%{pubilc}s",
        component.versionNumber.c_str(), FirmwareUpdateAdapter::GetDisplayVersion().c_str());
    UpdateResult updateResult;
    updateResult.spath = component.spath;
    if (isResultSuccess) {
        updateResult.result = UPDATER_RESULT_SUCCESS;
        updateResult.reason = UPDATER_RESULT_SUCCESS_REASON;
    } else {
        updateResult.result = UPDATER_RESULT_FAILURE;
        updateResult.reason = UPDATER_RESULT_FAILURE_REASON;
    }
    return updateResult;
}

/*
    /data/update/ota_package/firmware/versions/0856210b1bf14427a0706aff1bdd4aed/updater.zip|pass
    /data/update/ota_package/firmware/versions/1faa6ba19df044449ab8a10cb05bf1a6/updater.zip|
    fail:;02:145768,41554,454656487,1463ac:-1
    /data/update/ota_package/firmware/versions/971c50415d604c80a170f911993c2e2a/updater.zip
    spath为 /data/update/ota_package/firmware/versions/52e700cdd0974ee79c721dad4a54f119/updater.zip
    result为 pass或者fail 没有则为空
    reason为 ;02:145768,41554,454656487,1463ac:-1 没有则为空
*/
void FirmwareResultProcess::ParseUpdaterResultRecord(const std::string &resultLine,
    std::map<std::string, UpdateResult> &resultMap)
{
    FIRMWARE_LOGE("ParseUpdaterResultRecord");
    if (resultLine.empty()) {
        FIRMWARE_LOGE("resultLine is null");
        return;
    }
    UpdateResult updateResult;
    std::vector<std::string> results;
    std::stringstream stringStream(resultLine);
    std::string token;
    while (std::getline(stringStream, token, '|')) {
        results.push_back(token);
    }

    ParseResult(results, updateResult.spath, PKG_PATH_INDEX);
    ParseResult(results, updateResult.result, RESULT_INDEX);
    ParseResult(results, updateResult.reason, REASON_INDEX);

    auto colonPlace = updateResult.result.find_first_of(":");
    if (colonPlace != std::string::npos) {
        updateResult.result = updateResult.result.substr(0, colonPlace);
        updateResult.reason = results[RESULT_INDEX].substr(colonPlace + SYMBOL_LENGTH);
    }
    StringUtils::Trim(updateResult.spath);
    StringUtils::Trim(updateResult.result);
    StringUtils::Trim(updateResult.reason);
    resultMap.emplace(std::make_pair(updateResult.spath, updateResult));
}

void FirmwareResultProcess::HandleFileError(std::map<std::string, UpdateResult> &resultMap,
    const std::vector<FirmwareComponent> &components)
{
    resultMap.clear();
    for (const auto &component : components) {
        UpdateResult updateResult = CompareVersion(component);
        resultMap.emplace(std::make_pair(updateResult.spath, updateResult));
    }
}

UpdateResultCode FirmwareResultProcess::HandleFileResults(std::map<std::string, UpdateResult> &resultMap,
    const std::vector<FirmwareComponent> &components)
{
    FirmwareTask task;
    FirmwareTaskOperator().QueryTask(task);
    if (!task.isExistTask) {
        FIRMWARE_LOGI("HandleFileResults has no task");
        return UpdateResultCode::FAILURE;
    }
    uint32_t hotaUpdateResult = 0;
    for (const auto &component : components) {
        std::string updateResultStatus;
        auto result = resultMap.find(component.spath);
        if (result == resultMap.end()) {
            UpdateResult updateResult = CompareVersion(component);
            resultMap.emplace(std::make_pair(updateResult.spath, updateResult));
            FIRMWARE_LOGE("spath %{public}s, result %{public}s", component.spath.c_str(), updateResult.result.c_str());
            updateResultStatus = updateResult.result;
        } else {
            updateResultStatus = result->second.result;
        }
        hotaUpdateResult |= updateResultStatus == UPDATER_RESULT_SUCCESS ? UPDATE_SUCCESSED : UPDATE_FAILED;
    }

    if (task.combinationType == CombinationType::HOTA) {
        return hotaUpdateResult == UPDATE_SUCCESSED ? UpdateResultCode::SUCCESS : UpdateResultCode::FAILURE;
    }

    if (hotaUpdateResult != UPDATE_SUCCESSED) {
        return UpdateResultCode::FAILURE;
    }
    return UpdateResultCode::SUCCESS;
}
} // namespace UpdateEngine
} // namespace OHOS
