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

#include "firmware_check_data_processor.h"

#include "firmware_log.h"

namespace OHOS {
namespace UpdateEngine {
FirmwareCheckDataProcessor::FirmwareCheckDataProcessor()
{
    FIRMWARE_LOGD("FirmwareCheckDataProcessor::FirmwareCheckDataProcessor");
}

FirmwareCheckDataProcessor::~FirmwareCheckDataProcessor()
{
    FIRMWARE_LOGD("FirmwareCheckDataProcessor::~FirmwareCheckDataProcessor");
}

void FirmwareCheckDataProcessor::SetCheckResult(
    CheckStatus status, const Duration &duration, const std::vector<FirmwareComponent> &componentList)
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor::SetCheckResult status %{public}d duration %{public}d",
        static_cast<uint32_t>(status), duration.duration);
    status_ = status;
    duration_ = duration;
    componentList_ = componentList;
}

bool FirmwareCheckDataProcessor::IsCheckFailed()
{
    return true;
}

bool FirmwareCheckDataProcessor::HasNewVersion()
{
    return !componentList_.empty();
}

bool FirmwareCheckDataProcessor::IsSameWithDb()
{
    return true;
}

void FirmwareCheckDataProcessor::HandleCheckResult()
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor HandleCheckResult");
}

bool FirmwareCheckDataProcessor::HasUndoneTask()
{
    FIRMWARE_LOGI("HasUndoneTask");
    return true;
}

bool FirmwareCheckDataProcessor::IsUpdateOnStatus()
{
    return true;
}

void FirmwareCheckDataProcessor::HandleNewVersion()
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor::HandleNewVersion");
}

void FirmwareCheckDataProcessor::BuildComponentSPath()
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor::BuildComponentSPath");
}

bool FirmwareCheckDataProcessor::IsVersionSameWithDb()
{
    return true;
}

void FirmwareCheckDataProcessor::UpdateFirmwareComponent()
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor::UpdateFirmwareComponent");
}

void FirmwareCheckDataProcessor::HandleNoNewVersion()
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor::HandleNoNewVersion");
    isSameWithDb_ = IsVersionSameWithDb();
}

CombinationType FirmwareCheckDataProcessor::GetCombinationType()
{
    uint32_t combinationType = CAST_UINT(CombinationType::INVALLID_TYPE);
    FIRMWARE_LOGI("CombinationType::%{public}u", combinationType);
    return static_cast<CombinationType>(combinationType);
}

void FirmwareCheckDataProcessor::RefreshPollingCycle()
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor::RefreshPollingCycle");
}

void FirmwareCheckDataProcessor::BuildCheckResult(CheckResult &checkResult)
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor::BuildCheckResult");
}

void FirmwareCheckDataProcessor::BuildVersionDigest(
    NewVersionInfo &newVersionInfo, const std::vector<FirmwareComponent> &componentList)
{
    FIRMWARE_LOGI("FirmwareCheckDataProcessor::BuildVersionDigest");
}
} // namespace UpdateEngine
} // namespace OHOS