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

#include <mutex>
#include <unistd.h>

#include "iservice_registry.h"
#include "system_ability_definition.h"
#include "system_ability_load_callback_stub.h"

namespace OHOS::UpdateEngine {
enum class LoadSaStatus {
    WAIT_RESULT = 0,
    SUCCESS,
    FAIL,
};

class LoadSaService : public SystemAbilityLoadCallbackStub {
public:
    static sptr<LoadSaService> GetInstance();
    bool TryLoadSa(int systemAbilityId);
    void OnLoadSystemAbilitySuccess(int32_t systemAbilityId, const sptr<IRemoteObject> &remoteObject) override;
    void OnLoadSystemAbilityFail(int32_t systemAbilityId) override;

private:
    LoadSaService();
    ~LoadSaService() override;

    void InitStatus();
    bool CheckSaLoaded();
    bool LoadSa(int systemAbilityId);

    LoadSaStatus loadSaStatus_ = LoadSaStatus::WAIT_RESULT;
    static std::mutex instanceLock_;
    static sptr<LoadSaService> instance_;
};
} // namespace OHOS::UpdateEngine
#endif // LOAD_SA_SERVICE_H