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

#include "firmware_check_analyze_utils.h"

#include <cinttypes>
#include <iostream>
#include <memory>
#include <map>
#include <string>

#include "constant.h"
#include "updateservice_json_utils.h"
#include "file_utils.h"
#include "firmware_combine_version_utils.h"
#include "firmware_constant.h"
#include "firmware_log.h"
#include "firmware_preferences_utils.h"
#include "string_utils.h"

namespace OHOS {
namespace UpdateService {
void FirmwareCheckAnalyzeUtils::DoAnalyze(const std::string &rawJson, std::vector<FirmwareComponent> &components,
    Duration &duration, CheckAndAuthInfo &checkAndAuthInfo)
{
    BlCheckResponse response;
    int32_t ret = CAST_INT(JsonParseError::ERR_OK);
    cJSON *root = UpdateServiceJsonUtils::ParseAndGetJsonObject(rawJson);
    if (!root) {
        FIRMWARE_LOGE("fail to parse out a json object");
        return;
    }

    int32_t status = CAST_INT(CheckResultStatus::STATUS_SYSTEM_ERROR);
    UpdateServiceJsonUtils::GetValueAndSetTo(root, "searchStatus", status);

    checkAndAuthInfo.responseStatus = std::to_string(status);
    if (!IsLegalStatus(status)) {
        FIRMWARE_LOGI("not found new version!");
        cJSON_Delete(root);
        return;
    }
    if (status == CAST_INT(CheckResultStatus::STATUS_NEW_VERSION_AVAILABLE)) {
        ret += AnalyzeBlVersionCheckResults(root, response);
        ret += AnalyzeComponents(root);
    }

    // 解析的都是必须字段，全部解析正常，才能给component赋值
    if (ret == CAST_INT(JsonParseError::ERR_OK)) {
        components = components_;
    }
    cJSON_Delete(root);
}

int32_t FirmwareCheckAnalyzeUtils::AnalyzeBlVersionCheckResults(cJSON *root, BlCheckResponse &response)
{
    cJSON *itemCheckResults = cJSON_GetObjectItemCaseSensitive(root, "checkResults");
    if (!itemCheckResults) {
        FIRMWARE_LOGE("FirmwareCheckAnalyzeUtils::AnalyzeBlVersionCheckResults no key checkResults");
        return CAST_INT(JsonParseError::MISSING_PROP);
    }

    FIRMWARE_LOGI("checkResults size is %{public}" PRIu64 "",
        static_cast<uint64_t>(cJSON_IsArray(itemCheckResults) ? cJSON_GetArraySize(itemCheckResults) : 0));

    int32_t ret = CAST_INT(JsonParseError::ERR_OK);
    int32_t status = CAST_INT(CheckResultStatus::STATUS_SYSTEM_ERROR);
    UpdateServiceJsonUtils::GetValueAndSetTo(root, "searchStatus", status);
    if (status == CAST_INT(CheckResultStatus::STATUS_NEW_VERSION_AVAILABLE)) {
        for (int i = 0; i < cJSON_GetArraySize(itemCheckResults); i++) {
            auto item = cJSON_GetArrayItem(itemCheckResults, i);
            BlVersionCheckResult checkResult;
            ret += UpdateServiceJsonUtils::GetValueAndSetTo(item, "descriptPackageId", checkResult.descriptPackageId);
            checkResult.blVersionType = 1;
            checkResult.status = std::to_string(status);
            UpdatePackage package;
            package.versionId = "1";
            int32_t versionPackageType = CAST_INT(PackageType::DYNAMIC);
            ret += UpdateServiceJsonUtils::GetValueAndSetTo(item, "packageType", versionPackageType);
            package.versionPackageType = static_cast<PackageType>(versionPackageType);
            package.packageIndex = 0;
            checkResult.updatePackages.push_back(package);
            TargetBlComponent component;
            component.versionPackageType = package.versionPackageType;
            ret += UpdateServiceJsonUtils::GetValueAndSetTo(item, "versionName", component.displayVersionNumber);
            ret += UpdateServiceJsonUtils::GetValueAndSetTo(item, "versionName", component.versionNumber);
            checkResult.targetBlComponents.push_back(component);
            UpdateServiceJsonUtils::GetValueAndSetTo(item, "blVersionInfo", checkResult.blVersionInfo);
            response.blVersionCheckResults.push_back(checkResult);
            Version version;
            version.versionId = "1";
            ret += UpdateServiceJsonUtils::GetValueAndSetTo(item, "versionCode", version.versionNumber);
            ret += UpdateServiceJsonUtils::GetValueAndSetTo(item, "url", version.url);
            response.versionList.push_back(version);
        }
    }
    return ret;
}

int32_t FirmwareCheckAnalyzeUtils::AnalyzeComponents(cJSON *root)
{
    // 检查 "checkResults" 是否存在
    cJSON *itemCheckResults = cJSON_GetObjectItemCaseSensitive(root, "checkResults");
    if (!itemCheckResults) {
        FIRMWARE_LOGE("FirmwareCheckAnalyzeUtils::AnalyzeComponents no key checkResults");
        return CAST_INT(JsonParseError::MISSING_PROP);
    }
    FIRMWARE_LOGI("checkResults size is %{public}" PRIu64 "",
        static_cast<uint64_t>(cJSON_IsArray(itemCheckResults) ? cJSON_GetArraySize(itemCheckResults) : 0));

    // 初始化返回值
    int32_t ret = CAST_INT(JsonParseError::ERR_OK);

    // 处理 "checkResults" 部分
    ret += ProcessCheckResults(itemCheckResults);

    // 检查 "descriptInfo" 是否存在
    cJSON *itemDescriptInfo = cJSON_GetObjectItemCaseSensitive(root, "descriptInfo");
    if (!itemDescriptInfo) {
        FIRMWARE_LOGE("FirmwareCheckAnalyzeUtils::AnalyzeComponents no key descriptInfo");
        return CAST_INT(JsonParseError::MISSING_PROP);
    }

    // 处理 "descriptInfo" 部分
    ret += ProcessDescriptInfo(itemDescriptInfo);

    return ret;
}

int32_t FirmwareCheckAnalyzeUtils::ProcessCheckResults(cJSON *checkResults)
{
    int32_t ret = CAST_INT(JsonParseError::ERR_OK);
    std::string componentId;

    if (!checkResults) {
        FIRMWARE_LOGE("AnalyzeComponents no key checkResults");
        return CAST_INT(JsonParseError::MISSING_PROP);
    }

    for (int i = 0; i < cJSON_GetArraySize(checkResults); i++) {
        cJSON *itemResult = cJSON_GetArrayItem(checkResults, i);

        FirmwareComponent component;
        int32_t componetSize = 0;

        // 获取组件相关属性
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemResult, "descriptPackageId", component.descriptPackageId);
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemResult, "url", component.url);
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemResult, "size", componetSize);
        component.size = static_cast<int64_t>(componetSize);
        component.fileName = StringUtils::GetLastSplitString(component.url, "/");
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemResult, "verifyInfo", component.verifyInfo);
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemResult, "versionCode", component.versionNumber);
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemResult, "versionName", component.targetBlVersionNumber);

        int32_t versionPackageType = CAST_INT(PackageType::DYNAMIC);
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemResult, "packageType", versionPackageType);
        component.versionPackageType = static_cast<PackageType>(versionPackageType);

        int32_t otaType = CAST_INT(OtaType::REGULAR);
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemResult, "otaType", otaType);
        component.otaType = static_cast<OtaType>(otaType);

        component.targetBlDisplayVersionNumber = component.targetBlVersionNumber;
        component.blVersionType = 1;
        component.componentId = component.descriptPackageId;
        componentId = component.descriptPackageId;

        components_.push_back(component);
    }
    return ret;
}

int32_t FirmwareCheckAnalyzeUtils::ProcessDescriptInfo(cJSON *descriptInfo)
{
    int32_t ret = CAST_INT(JsonParseError::ERR_OK);
    std::string componentId = components_.empty() ? "" : components_.back().descriptPackageId;

    if (!descriptInfo) {
        FIRMWARE_LOGE("ProcessDescriptInfo no descriptInfo");
        return CAST_INT(JsonParseError::MISSING_PROP);
    }

    for (int i = 0; i < cJSON_GetArraySize(descriptInfo); i++) {
        cJSON *itemInfo = cJSON_GetArrayItem(descriptInfo, i);
        int32_t descriptInfoType;
        std::string descContent;
        std::string subString = "quota";
        std::string replString = "\"";

        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemInfo, "descriptionType", descriptInfoType);
        ret += UpdateServiceJsonUtils::GetValueAndSetTo(itemInfo, "content", descContent);

        StringUtils::ReplaceStringAll(descContent, subString, replString);

        std::string changelogFilePath = Firmware::CHANGELOG_PATH + "/" + componentId + ".xml";
        FIRMWARE_LOGI("changelog file %{public}s", changelogFilePath.c_str());

        std::string data = std::to_string(descriptInfoType) + "|" + descContent;
        if (!FileUtils::SaveDataToFile(changelogFilePath, data)) {
            FIRMWARE_LOGE("write data to description file error, %{public}s", changelogFilePath.c_str());
        }
    }

    return ret;
}

bool FirmwareCheckAnalyzeUtils::IsLegalStatus(int32_t status)
{
    return status == CAST_INT(CheckResultStatus::STATUS_NEW_VERSION_AVAILABLE) ||
           status == CAST_INT(CheckResultStatus::STATUS_NEW_VERSION_NOT_AVAILABLE);
}
} // namespace UpdateService
} // namespace OHOS