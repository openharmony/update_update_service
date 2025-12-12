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

#include "update_service_restorer.h"

#include "access_token.h"
#include "accesstoken_kit.h"
#ifndef UPDATER_FUZZ
#include "fs_manager/mount.h"
#endif
#include "ipc_skeleton.h"
#include "updaterkits/updaterkits.h"

#include "update_define.h"
#include "update_log.h"
#include "update_system_event.h"

namespace OHOS {
namespace UpdateService {
const std::string MISC_PATH = "/misc";
const std::string MISC_FILE = "/dev/block/by-name/misc";
const std::string CMD_WIPE_DATA = "--user_wipe_data";
const std::string HW_PREFIX = "com.huawei.hmos";
const std::string HAP_SETTINGS = HW_PREFIX + ".settings";
const std::string HAP_PROJECT_MENU = HW_PREFIX + ".projectmenu";
const std::string HAP_FIND_SERVICE = HW_PREFIX + ".findservice";

// hap名称： 设置菜单 + 工程菜单 + 手机找回
// key: hapName, value: errCode
std::unordered_map<std::string, int32_t> pointCodeMap = {
    {HAP_SETTINGS, CAST_INT(ResetPointCode::SETTINGS)},
    {HAP_PROJECT_MENU, CAST_INT(ResetPointCode::PROJECT_MENU)},
    {HAP_FIND_SERVICE, CAST_INT(ResetPointCode::FIND_SERVICE)}
}

std::string UpdateServiceRestorer::GetApplicationHapName()
{
    std::string callingUid = std::to_string(IPCSkeleton::GetCallingUid());
    OHOS::Security::AccessToken::AccessTokenID callerToken = IPCSkeleton::GetCallingTokenID();
    Security::AccessToken::HapTokenInfo  hapTokenInfo;
    if (Security::AccessToken::AccessTokenKit::GetHapTokenInfo(callerToken, hapTokenInfo) != 0) {
        ENGINE_LOGE("Get hap token info error");
        return "";
    }
    ENGINE_LOGI("service restorer bundleName: %{public}s", hapTokenInfo.bundleName.c_str());
    return hapTokenInfo.bundleName;
}

std::string UpdateServiceRestorer::GetResetPointCode()
{
    std::string pointCode;
    std::string hapName = GetApplicationHapName();
    auto it = pointCodeMap.find(hapName);
    if (it != pointCodeMap.end()) {
        pointCode = std::to_string(it->second);
    } else {
        pointCode = "";
    }
    return pointCode;
}

sptr<StorageManager::IStorageManager> UpdateServiceRestorer::GetStorageMgrProxy()
{
    auto samgr = OHOS::SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (samgr == nullptr) {
        ENGINE_LOGE("samgr empty error");
        return nullptr;
    }

    auto remote = samgr->GetSystemAbility(OHOS::STORAGE_MANAGER_MANAGER_ID);
    if (remote == nullptr) {
        ENGINE_LOGE("storage manager client samgr ability empty error");
        return nullptr;
    }

    auto storageMgrProxy = iface_cast<StorageManager::IStorageManager>(remote);
    if (storageMgrProxy == nullptr) {
        ENGINE_LOGE("storageMgrProxy empty error");
        return nullptr;
    }
    return storageMgrProxy;
}

int32_t UpdateServiceRestorer::FileManagerEraseKeys()
{
    sptr<StorageManager::IStorageManager> client = GetStorageMgrProxy();
    if (client == nullptr) {
        ENGINE_LOGE("get storage manager service failed");
        return INT_CALL_FAIL;
    }
    return client->EraseAllUserEncryptedKeys();
}

int32_t UpdateServiceRestorer::FactoryReset(BusinessError &businessError)
{
#ifndef UPDATER_UT
    businessError.errorNum = CallResult::SUCCESS;
    auto miscBlockDev = Updater::GetBlockDeviceByMountPoint(MISC_PATH);
    ENGINE_LOGI("FactoryReset::misc path : %{public}s", miscBlockDev.c_str());
    ENGINE_CHECK(!miscBlockDev.empty(), miscBlockDev = MISC_FILE, "cannot get block device of partition");
    std::string paramData = CMD_WIPE_DATA + "\n--reset_enter:" + GetResetPointCode();
    int32_t ret = RebootAndCleanUserData(miscBlockDev, paramData) ? INT_CALL_SUCCESS : INT_CALL_FAIL;
    ENGINE_LOGI("FactoryReset result : %{public}d", ret);
    SYS_EVENT_SYSTEM_RESET(
        0, ret == INT_CALL_SUCCESS ? UpdateSystemEvent::EVENT_SUCCESS_RESULT : UpdateSystemEvent::EVENT_FAILED_RESULT);
    return ret;
#else
    return INT_CALL_SUCCESS;
#endif
}

int32_t UpdateServiceRestorer::ForceFactoryReset(BusinessError &businessError)
{
    // 删除文件类秘钥
    int32_t ret = FileManagerEraseKeys();
    if (ret != 0) {
        ENGINE_LOGE("file manager erase keys error");
        return INT_CALL_FAIL;
    }
    return FactoryReset(businessError);
}
} // namespace UpdateService
} // namespace OHOS
