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

#ifndef FIRMWARE_STATUS_CACHE_H
#define FIRMWARE_STATUS_CACHE_H

#include "singleton.h"

#include "status_cache.h"

namespace OHOS {
namespace UpdateService {
class FirmwareStatusCache : public DelayedSingleton<FirmwareStatusCache> {
    DECLARE_DELAYED_SINGLETON(FirmwareStatusCache);

public:
    bool IsChecking();
    void SetIsChecking(bool isChecking);
    bool IsDownloadTriggered();
    bool IsUpgradeTriggered();
private:
    std::shared_ptr<StatusCache> statusCache_ = nullptr;
};
} // namespace UpdateService
} // namespace OHOS
#endif // FIRMWARE_STATUS_CACHE_H