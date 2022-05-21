/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "RegisterUpdateCallback_fuzzer.h"

#include <stddef.h>
#include <stdint.h>

using namespace OHOS::update_engine;

const int FUZZ_HEAD_DATA = 0;
const int FUZZ_CHAR_LEN_DATA = 1;
const int FUZZ_INT_LEN_DATA = 4;
const int FUZZ_CHAR_ARRAY_UPD_APP_NAME_LEN_DATA = 64;
const int FUZZ_CHAR_ARRAY_DEV_ID_LEN_DATA = 68;

const int FUZZ_CHAR1_DATA = FUZZ_HEAD_DATA;
const int FUZZ_CHAR2_DATA = FUZZ_CHAR1_DATA + FUZZ_CHAR_LEN_DATA;
const int FUZZ_CHAR3_DATA = FUZZ_CHAR2_DATA + FUZZ_CHAR_LEN_DATA;
const int FUZZ_INT1_DATA = FUZZ_CHAR3_DATA + FUZZ_CHAR_LEN_DATA;
const int FUZZ_INT2_DATA = FUZZ_INT1_DATA + FUZZ_INT_LEN_DATA;
const int FUZZ_CHAR_ARRAY1_DATA = FUZZ_INT2_DATA + FUZZ_INT_LEN_DATA;
const int FUZZ_CHAR_ARRAY2_DATA = FUZZ_CHAR_ARRAY1_DATA + FUZZ_CHAR_ARRAY_UPD_APP_NAME_LEN_DATA;
const int FUZZ_CHAR_ARRAY3_DATA = FUZZ_CHAR_ARRAY2_DATA + FUZZ_CHAR_ARRAY_DEV_ID_LEN_DATA;
const int FUZZ_INT3_DATA = FUZZ_CHAR_ARRAY3_DATA + FUZZ_CHAR_ARRAY_DEV_ID_LEN_DATA;

const int FUZZ_DATA_LEN = FUZZ_INT3_DATA + FUZZ_INT_LEN_DATA;

namespace OHOS {
    static void FtCheckNewVersionDone(const VersionInfo &info)
    {
    }

    static void FtDownloadProgress(const Progress &progress)
    {
    }

    static void FtUpgradeProgress(const Progress &progress)
    {
    }

    int32_t FuzzUpdateServiceRegisterUpdateCallbackImpl(UpdateContext &ctx, UpdateCallbackInfo &cb,
        UpdatePolicy &policy, VersionInfo &versionInfo, int32_t service, UpgradeInfo &info)
    {
        int32_t ret = UpdateServiceKits::GetInstance().RegisterUpdateCallback(ctx, cb);
        (void)UpdateServiceKits::GetInstance().SetUpdatePolicy(policy);
        (void)UpdateServiceKits::GetInstance().GetUpdatePolicy(policy);
        (void)UpdateServiceKits::GetInstance().CheckNewVersion();
        (void)UpdateServiceKits::GetInstance().GetNewVersion(versionInfo);
        (void)UpdateServiceKits::GetInstance().DownloadVersion();
        (void)UpdateServiceKits::GetInstance().Cancel(service);
        (void)UpdateServiceKits::GetInstance().GetUpgradeStatus(info);
        (void)UpdateServiceKits::GetInstance().UnregisterUpdateCallback();
        return ret;
    }

    bool FuzzUpdateServiceRegisterUpdateCallback(const uint8_t* data, size_t size)
    {
        if (size < FUZZ_DATA_LEN) {
            return false;
        }
        UpdateContext ctx;
        UpdateCallbackInfo cb = {
            .checkNewVersionDone = FtCheckNewVersionDone,
            .downloadProgress = FtDownloadProgress,
            .upgradeProgress = FtUpgradeProgress,
        };
        UpdatePolicy policy = {static_cast<bool>(data[FUZZ_CHAR1_DATA] % 2),
            static_cast<bool>(data[FUZZ_CHAR2_DATA] % 2),
            static_cast<InstallMode>(data[FUZZ_CHAR3_DATA] % 3), static_cast<AutoUpgradeCondition>(0)};
        policy.autoUpgradeInterval[0] = (static_cast<uint32_t>(data[FUZZ_INT1_DATA]) << 24) +
            (static_cast<uint32_t>(data[FUZZ_INT1_DATA + 1]) << 16) +
            (static_cast<uint32_t>(data[FUZZ_INT1_DATA + 2]) << 8) +
            static_cast<uint32_t>(data[FUZZ_INT1_DATA + 3]);
        policy.autoUpgradeInterval[1] = (static_cast<uint32_t>(data[FUZZ_INT2_DATA]) << 24) +
            (static_cast<uint32_t>(data[FUZZ_INT2_DATA + 1]) << 16) +
            (static_cast<uint32_t>(data[FUZZ_INT2_DATA + 2]) << 8) +
            static_cast<uint32_t>(data[FUZZ_INT2_DATA + 3]);
        int32_t i;
        for (i = 0; i < FUZZ_CHAR_ARRAY_UPD_APP_NAME_LEN_DATA; i++)
        {
            ctx.upgradeApp.push_back(static_cast<char>(data[i + FUZZ_CHAR_ARRAY1_DATA]));
        }
        for (i = 0; i < FUZZ_CHAR_ARRAY_DEV_ID_LEN_DATA; i++)
        {
            ctx.upgradeDevId.push_back(static_cast<char>(data[i + FUZZ_CHAR_ARRAY2_DATA]));
        }
        for (i = 0; i < FUZZ_CHAR_ARRAY_DEV_ID_LEN_DATA; i++)
        {
            ctx.controlDevId.push_back(static_cast<char>(data[i + FUZZ_CHAR_ARRAY3_DATA]));
        }
        VersionInfo versionInfo;
        memset_s(&versionInfo, sizeof(versionInfo), 0, sizeof(versionInfo));
        int32_t service = static_cast<int32_t>((static_cast<uint32_t>(data[FUZZ_INT3_DATA]) << 24) +
            (static_cast<uint32_t>(data[FUZZ_INT3_DATA + 1]) << 16) +
            (static_cast<uint32_t>(data[FUZZ_INT3_DATA + 2]) << 8) +
            static_cast<uint32_t>(data[FUZZ_INT3_DATA + 3]));
        UpgradeInfo info;
        info.status = UPDATE_STATE_INIT;
        return !FuzzUpdateServiceRegisterUpdateCallbackImpl(ctx, cb, policy, versionInfo, service, info);
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::FuzzUpdateServiceRegisterUpdateCallback(data, size);
    return 0;
}

