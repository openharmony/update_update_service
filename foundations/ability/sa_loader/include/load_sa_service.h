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

#ifndef LOAD_SA_SERVICE_H
#define LOAD_SA_SERVICE_H

#include <atomic>
#include <mutex>
#include <unistd.h>

#include "iservice_registry.h"
#include "system_ability_definition.h"
#include "system_ability_load_callback_stub.h"

namespace OHOS::UpdateService {
class LoadSaService : public SystemAbilityLoadCallbackStub {
public:
    static sptr<LoadSaService> GetInstance();
    bool TryLoadSa(int systemAbilityId);

private:
    LoadSaService();
    ~LoadSaService() override;

    static std::mutex instanceLock_;
    static sptr<LoadSaService> instance_;
};
} // namespace OHOS::UpdateService
#endif // LOAD_SA_SERVICE_H