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

#include "config_parse.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "constant.h"
#include "updateservice_json_utils.h"
#include "firmware_constant.h"

namespace OHOS {
namespace UpdateService {
ConfigParse::ConfigParse()
{
    ENGINE_LOGI("ConfigParse::ConfigParse");
}

ConfigParse::~ConfigParse()
{
    ENGINE_LOGI("ConfigParse::~ConfigParse");
}

uint32_t ConfigParse::GetAbInstallerTimeout()
{
    return configInfo_.abInstallTimeout;
}

uint32_t ConfigParse::GetStreamInstallerTimeout()
{
    return configInfo_.streamInstallTimeout;
}

std::string ConfigParse::GetModuleLibPath()
{
    return configInfo_.moduleLibPath;
}

void ConfigParse::LoadConfigInfo()
{
    std::ifstream readFile;
    readFile.open(Constant::DUPDATE_ENGINE_CONFIG_PATH);
    if (readFile.fail()) {
        ENGINE_LOGE("open config fail");
        return;
    }
    std::stringstream streambuffer;
    streambuffer << readFile.rdbuf();
    std::string rawJson(streambuffer.str());
    readFile.close();

    auto root = UpdateServiceJsonUtils::ParseJson(rawJson.c_str());
    if (root == nullptr) {
        ENGINE_LOGE("json Create error!");
        return;
    }

    UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "abInstallTimeout", configInfo_.abInstallTimeout);
    UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "streamInstallTimeout", configInfo_.streamInstallTimeout);
    UpdateServiceJsonUtils::GetValueAndSetTo(root.get(), "moduleLibPath", configInfo_.moduleLibPath);
}
} // namespace UpdateService
} // namespace OHOS
