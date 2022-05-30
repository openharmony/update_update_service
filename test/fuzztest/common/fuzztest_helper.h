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

#ifndef UPDATE_SERVICE_COMMON_FUZZER_H
#define UPDATE_SERVICE_COMMON_FUZZER_H

#include "update_service_kits_impl.h"

#include <cstddef>
#include <cstdint>
#include <securec.h>

namespace OHOS {
namespace update_engine {
class FuzztestHelper {
public:
    FuzztestHelper(const uint8_t* data, size_t size);
    virtual ~FuzztestHelper() = default;
    DISALLOW_COPY_AND_MOVE(FuzztestHelper);

    UpdateCallbackInfo BuildUpdateCallbackInfo();

    UpdateContext BuildUpdateContext();

    UpdatePolicy BuildUpdatePolicy();

    UpgradeInfo BuildUpgradeInfo();

    VersionInfo BuildVersionInfo();

    int32_t GetInt();

private:
    void GetCharArray(char *charArray, uint32_t arraySize);

    uint32_t GetUInt();

    const uint32_t FUZZ_DATA_LEN = 500;
    uint32_t index_ = 0;
    uint8_t data_[FUZZ_DATA_LEN] {};
};
} // namespace update_engine
} // namespace OHOS

#endif // UPDATE_SERVICE_COMMON_FUZZER_H
