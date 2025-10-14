/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <gtest/gtest.h>
#include "firmware_stream_installer_install.h"

using namespace testing::ext;
using namespace testing;

namespace OHOS::UpdateService {

UpgradeStatus g_status;
bool g_result;
void OnFirmwareProgressImpl(const FirmwareComponent &component)
{
}

void OnFirmwareEventImpl(bool result, const ErrorMessage &errMsg, UpgradeStatus status)
{
    g_result = result;
    g_status = status;
}

void OnFirmwareStatusImpl(UpgradeStatus status)
{
    g_status = status;
}

class StreamInstallerInstallTest : public ::testing::Test {
protected:
    void SetUp() const override
    {
        installer = std::make_unique<StreamInstallerInstall>();
    }

    std::unique_ptr<StreamInstallerInstall> installer;
};

// 测试 StartInstall 函数
HWTEST_F(StreamInstallerInstallTest, StartInstallSuccess, TestSize.Level1)
{
    FirmwareComponent component;
    component.url = "http://file-examples.com/wp-content/uploads/2017/02/file-example_txt-download.txt";
    component.size = 2250;
    component.status = UpgradeStatus::INIT;
    component.progress = 0;
    component.recordPoint = 0;

    FirmwareInstallCallback callback;
    callback.onFirmwareProgress = OnFirmwareProgressImpl;
    callback.onFirmwareEvent = OnFirmwareEventImpl;
    callback.onFirmwareStatus = OnFirmwareStatusImpl;

    // 创建组件列表
    std::vector<FirmwareComponent> componentList = {component};

    // 调用 StartInstall
    installer->StartInstall(componentList, callback);
    ASSERT_EQ(g_result, true);
}

HWTEST_F(StreamInstallerInstallTest, StartInstallFailure, TestSize.Level1)
{
    // 创建空组件列表
    std::vector<FirmwareComponent> emptyList;

    FirmwareInstallCallback callback;
    callback.onFirmwareProgress = OnFirmwareProgressImpl;
    callback.onFirmwareEvent = OnFirmwareEventImpl;
    callback.onFirmwareStatus = OnFirmwareStatusImpl;

    // 调用 StartInstall
    installer->StartInstall(emptyList, callback);

    ASSERT_EQ(g_result, false);
}

} // namespace OHOS::UpdateService