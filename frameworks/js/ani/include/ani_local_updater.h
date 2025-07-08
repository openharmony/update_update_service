/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANI_LOCAL_UPDATER_H
#define ANI_LOCAL_UPDATER_H

#include "ohos.update.proj.hpp"

#include "ani_base_updater.h"

namespace OHOS::UpdateService{
class AniLocalUpdater final : public AniBaseUpdater {
public:
    AniLocalUpdater() = default;

    void VerifyUpgradePackageSync(const ohos::update::UpgradeFile &upgradeFile, taihe::string_view certsFile);
    void ApplyNewVersionSync(const taihe::array_view<ohos::update::UpgradeFile> &upgradeFiles);
};
}
#endif // ANI_LOCAL_UPDATER_H
