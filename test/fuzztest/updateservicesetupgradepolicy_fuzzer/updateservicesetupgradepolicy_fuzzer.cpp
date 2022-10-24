/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "updateservicesetupgradepolicy_fuzzer.h"

using namespace OHOS::UpdateEngine;

namespace OHOS {
bool FuzzUpdateServiceSetUpgradePolicy(const uint8_t* data, size_t size)
{
    if (size < FUZZ_DATA_LEN) {
        ENGINE_LOGE("Input data's size too short, size is %d, need min len is %d", size, FUZZ_DATA_LEN);
        return false;
    }

    if (!DelayedSingleton<FuzztestHelper>::GetInstance()->TrySetData(data, size)) {
        ENGINE_LOGE("FuzztestHelper TrySetData failed");
        return false;
    }

    BusinessError businessError;
    return UpdateServiceKits::GetInstance().SetUpgradePolicy(
        DelayedSingleton<FuzztestHelper>::GetInstance()->BuildUpgradeInfo(),
        DelayedSingleton<FuzztestHelper>::GetInstance()->BuildUpgradePolicy(), businessError) == 0;
}
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    OHOS::FuzzUpdateServiceSetUpgradePolicy(data, size);
    return 0;
}
