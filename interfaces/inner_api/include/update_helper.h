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

#ifndef UPDATE_HELPER_H
#define UPDATE_HELPER_H

#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <string_ex.h>
#include <utility>

#include "nlohmann/json.hpp"

#include "common_error_define.h"
#include "json_builder.h"
#include "net_manager_model.h"
#include "update_define.h"

namespace OHOS::UpdateEngine {
const std::string OUC_PACKAGE_NAME = "com.ohos.updateapp";
const std::string OUC_SERVICE_EXT_ABILITY_NAME = "ServiceExtAbility";

// 搜索状态
enum class SearchStatus {
    NET_ERROR = -2,
    SYSTEM_ERROR,
    HAS_NEW_VERSION,
    NO_NEW_VERSION,
    SERVER_BUSY,
    CHECK_EXECUTE_ERR
};

enum class UpgradeStatus {
    INIT = 0,
    CHECKING_VERSION = 10,
    CHECK_VERSION_FAIL,
    CHECK_VERSION_SUCCESS,
    DOWNLOADING = 20,
    DOWNLOAD_PAUSE,
    DOWNLOAD_CANCEL,
    DOWNLOAD_FAIL,
    DOWNLOAD_SUCCESS,
    VERIFYING = 30,
    VERIFY_FAIL,
    VERIFY_SUCCESS,
    AUTHING,
    AUTH_FAIL,
    AUTH_SUCCESS,
    PACKAGE_TRANSING = 70,
    PACKAGE_TRANS_FAIL,
    PACKAGE_TRANS_SUCCESS,
    INSTALLING = 80,
    INSTALL_FAIL,
    INSTALL_SUCCESS,
    UPDATING = 90,
    UPDATE_FAIL,
    UPDATE_SUCCESS
};

enum class PackageType {
    DYNAMIC = 0,
    NORMAL = 1,
    BASE = 2,
    CUST = 3,
    PRELOAD = 4,
    COTA = 5,
    VERSION = 6,
    PATCH = 8,
    SA = 9
};

enum class ComponentType {
    INVALID = 0,
    OTA = 1,
    PATCH = 2,
    COTA = 4,
    PARAM = 8,
    SA = 16
};

enum class EffectiveMode {
    COLD = 1,
    LIVE = 2,
    LIVE_AND_COLD = 3
};

enum class Order {
    DOWNLOAD = 1,
    INSTALL = 2,
    DOWNLOAD_AND_INSTALL = DOWNLOAD | INSTALL,
    APPLY = 4,
    INSTALL_AND_APPLY = INSTALL | APPLY
};

enum class OptionsAfterUpgrade {
    NORMAL = 0,
    REBOOT = 1,
    SHUTDOWN = 2
};

enum class BusinessSubType {
    START_UP = 0,
    FIRMWARE = 1,
    PARAM = 2,
    ROLLBACK = 3,
    ACCESSORY = 4
};

enum class DescriptionType {
    CONTENT = 0,
    URI = 1,
    ID = 2
};

enum class DescriptionFormat {
    STANDARD = 0,
    SIMPLIFIED = 1
};

enum ForceUpgradeSwitch {
    OPEN    = 0x01000000,
    CLOSE   = 0x00000000,
};

enum ForceUpgradeDataNetworkOption {
    CELLULAR_NOT_SUPPORT    = 0x00000000,
    CELLULAR                = 0x00000001,
    ROAMING                 = 0x00000002,
    CELLULAR_AND_ROAMING    = CELLULAR | ROAMING,
};

enum class ForceUpgradeType {
    NOT_SUPPORT            = ForceUpgradeSwitch::CLOSE,
    CELLULAR_NOT_SUPPORT   = ForceUpgradeSwitch::OPEN | ForceUpgradeDataNetworkOption::CELLULAR_NOT_SUPPORT,
    CELLULAR               = ForceUpgradeSwitch::OPEN | ForceUpgradeDataNetworkOption::CELLULAR,
    CELLULAR_AND_ROAMING   = ForceUpgradeSwitch::OPEN | ForceUpgradeDataNetworkOption::CELLULAR_AND_ROAMING,
};

enum class EventClassify {
    TASK = 0x01000000,
    SYSTEM = 0x02000000,
};

const std::list g_eventClassifyList = { EventClassify::TASK, EventClassify::SYSTEM };

enum class EventId {
    EVENT_TASK_BASE = CAST_UINT(EventClassify::TASK),
    EVENT_TASK_RECEIVE,
    EVENT_TASK_CANCEL,
    EVENT_DOWNLOAD_WAIT,
    EVENT_DOWNLOAD_START,
    EVENT_DOWNLOAD_UPDATE,
    EVENT_DOWNLOAD_PAUSE,
    EVENT_DOWNLOAD_RESUME,
    EVENT_DOWNLOAD_SUCCESS,
    EVENT_DOWNLOAD_FAIL,
    EVENT_UPGRADE_WAIT,
    EVENT_UPGRADE_START,
    EVENT_UPGRADE_UPDATE,
    EVENT_APPLY_WAIT,
    EVENT_APPLY_START,
    EVENT_UPGRADE_SUCCESS,
    EVENT_UPGRADE_FAIL,
    EVENT_AUTH_START,
    EVENT_AUTH_SUCCESS,
    EVENT_DOWNLOAD_CANCEL,
    EVENT_INITIALIZE,
    EVENT_TASK_CHANGE,
    EVENT_VERSION_INFO_CHANGE,
    SYSTEM_BASE = CAST_UINT(EventClassify::SYSTEM),
    SYSTEM_BOOT_COMPLETE,
};

enum TaskBodyMemeberMask {
    VERSION_DIGEST_INFO = 0x00000001,
    UPGRADE_STATUS      = 0x00000010,
    SUB_STATUS          = 0x00000100,
    PROGRESS            = 0x00001000,
    INSTALL_MODE        = 0x00010000,
    ERROR_MESSAGE       = 0x00100000,
    VERSION_COMPONENT   = 0x01000000
};

const std::map<EventId, uint32_t> g_taskBodyTemplateMap = {
    { EventId::EVENT_TASK_RECEIVE,     VERSION_DIGEST_INFO },
    { EventId::EVENT_TASK_CHANGE,      VERSION_DIGEST_INFO | UPGRADE_STATUS | PROGRESS },
    { EventId::EVENT_TASK_CANCEL,      VERSION_DIGEST_INFO },
    { EventId::EVENT_VERSION_INFO_CHANGE,      VERSION_DIGEST_INFO | UPGRADE_STATUS },
    { EventId::EVENT_DOWNLOAD_WAIT,    VERSION_DIGEST_INFO | UPGRADE_STATUS | INSTALL_MODE },
    { EventId::EVENT_DOWNLOAD_START,   VERSION_DIGEST_INFO | INSTALL_MODE },
    { EventId::EVENT_DOWNLOAD_UPDATE,  VERSION_DIGEST_INFO | UPGRADE_STATUS | PROGRESS | INSTALL_MODE },
    { EventId::EVENT_DOWNLOAD_PAUSE,   VERSION_DIGEST_INFO | UPGRADE_STATUS | PROGRESS | INSTALL_MODE | ERROR_MESSAGE },
    { EventId::EVENT_DOWNLOAD_RESUME,  VERSION_DIGEST_INFO | UPGRADE_STATUS | PROGRESS | INSTALL_MODE },
    { EventId::EVENT_DOWNLOAD_SUCCESS, VERSION_DIGEST_INFO | INSTALL_MODE },
    { EventId::EVENT_DOWNLOAD_CANCEL,  VERSION_DIGEST_INFO | UPGRADE_STATUS },
    { EventId::EVENT_DOWNLOAD_FAIL,    VERSION_DIGEST_INFO | INSTALL_MODE | ERROR_MESSAGE },
    { EventId::EVENT_UPGRADE_WAIT,     VERSION_DIGEST_INFO | UPGRADE_STATUS | INSTALL_MODE | ERROR_MESSAGE },
    { EventId::EVENT_UPGRADE_START,    VERSION_DIGEST_INFO | UPGRADE_STATUS | INSTALL_MODE },
    { EventId::EVENT_UPGRADE_UPDATE,   VERSION_DIGEST_INFO | UPGRADE_STATUS | PROGRESS | INSTALL_MODE },
    { EventId::EVENT_APPLY_WAIT,       VERSION_DIGEST_INFO | UPGRADE_STATUS | ERROR_MESSAGE },
    { EventId::EVENT_APPLY_START,      VERSION_DIGEST_INFO },
    { EventId::EVENT_UPGRADE_SUCCESS,  VERSION_DIGEST_INFO | VERSION_COMPONENT },
    { EventId::EVENT_UPGRADE_FAIL,     VERSION_DIGEST_INFO | VERSION_COMPONENT | ERROR_MESSAGE },
    { EventId::EVENT_AUTH_START,       VERSION_DIGEST_INFO | VERSION_COMPONENT | UPGRADE_STATUS },
    { EventId::EVENT_AUTH_SUCCESS,     VERSION_DIGEST_INFO | VERSION_COMPONENT | UPGRADE_STATUS },
    { EventId::EVENT_INITIALIZE,       UPGRADE_STATUS },
    { EventId::SYSTEM_BOOT_COMPLETE,   UPGRADE_STATUS }
};

class UpgradeAction {
public:
    static constexpr const char *UPGRADE = "upgrade";
    static constexpr const char *RECOVERY = "recovery";
    static constexpr const char *ROLLBACK = "rollback";
};

class BusinessVendor {
public:
    static constexpr const char *PUBLIC = "public";
};

struct BaseJsonStruct {
    virtual ~BaseJsonStruct() {}
    virtual JsonBuilder GetJsonBuilder() = 0;

    virtual std::string ToJson()
    {
        return GetJsonBuilder().ToJson();
    };
};

enum class DeviceType {
    UNKNOWN = 0,
    SMART_PHONE = 1,    // 手机
    SMART_PAD = 2,      // 平板
    SMART_TV = 4,       // 智能电视
    TWS = 6,            // 真无线耳机
    KEYBOARD = 7,       // 键盘
    PEN  = 8            // 手写笔
};

struct BusinessType : public BaseJsonStruct {
    std::string vendor; // BusinessVendor
    BusinessSubType subType = BusinessSubType::FIRMWARE;

    bool operator<(const BusinessType &businessType) const
    {
        if (vendor < businessType.vendor) return true;
        if (vendor > businessType.vendor) return false;

        if (CAST_INT(subType) < CAST_INT(businessType.subType)) return true;
        if (CAST_INT(subType) > CAST_INT(businessType.subType)) return false;

        return false;
    }

    JsonBuilder GetJsonBuilder() final;
};

const std::string LOCAL_UPGRADE_INFO = "LocalUpgradeInfo";
struct UpgradeInfo {
    std::string upgradeApp;
    BusinessType businessType;
    std::string upgradeDevId;
    std::string controlDevId;
    int32_t processId;
    DeviceType deviceType;

    bool operator<(const UpgradeInfo &r) const
    {
        if (upgradeApp < r.upgradeApp) return true;
        if (upgradeApp > r.upgradeApp) return false;

        if (businessType < r.businessType) return true;
        if (r.businessType < businessType) return false;

        if (upgradeDevId < r.upgradeDevId) return true;
        if (upgradeDevId > r.upgradeDevId) return false;

        if (controlDevId < r.controlDevId) return true;
        if (controlDevId > r.controlDevId) return false;

        if (processId < r.processId) return true;
        if (processId > r.processId) return false;

        if (deviceType < r.deviceType) return true;
        if (deviceType > r.deviceType) return false;

        return false;
    }

    std::string ToString() const;

    bool IsLocal() const
    {
        return upgradeApp == LOCAL_UPGRADE_INFO;
    }
};

struct SubscribeInfo : public BaseJsonStruct {
    std::string upgradeApp = OUC_PACKAGE_NAME;
    BusinessType businessType;
    std::string abilityName;
    std::string subscriberDevId;
    std::string upgradeDevId;
    DeviceType deviceType = DeviceType::UNKNOWN;
    std::string deviceName;

    explicit SubscribeInfo(BusinessSubType subType)
    {
        businessType.subType = subType;
    }
    SubscribeInfo() = default;

    JsonBuilder GetJsonBuilder() final;
};

struct VersionDigestInfo : public BaseJsonStruct {
    std::string versionDigest;

    JsonBuilder GetJsonBuilder() final;
};

struct DescriptionOptions {
    DescriptionFormat format = DescriptionFormat::STANDARD;
    std::string language;
};

struct DownloadOptions {
    NetType allowNetwork = NetType::WIFI;
    Order order = Order::DOWNLOAD;
};

struct ResumeDownloadOptions {
    NetType allowNetwork = NetType::WIFI;
};

struct PauseDownloadOptions {
    bool isAllowAutoResume = false;
};

struct UpgradeOptions {
    Order order = Order::INSTALL;
    OptionsAfterUpgrade optionsAfterUpgrade = OptionsAfterUpgrade::NORMAL;
};

struct ClearOptions {
    UpgradeStatus status = UpgradeStatus::INIT;
};

enum class InstallMode {
    NORMAL = 0,
    NIGHT,
    AUTO
};

enum class AutoUpgradeCondition {
    IDLE = 0,
};

struct AccessoryDeviceInfo {
    std::string macAddress;
    DeviceType deviceType;
    std::string deviceName;
};

struct ParamExtra : public BaseJsonStruct {
    std::string classify;
    std::string type;
    std::string subType;
    std::string maxRemindTime;

    JsonBuilder GetJsonBuilder() final;
};

struct DescriptionInfo : public BaseJsonStruct {
    DescriptionType descriptionType = DescriptionType::CONTENT;
    std::string content;

    JsonBuilder GetJsonBuilder() final;
};

enum class PolicyType {
    DEFAULT = 0,
    PROHIBIT,
    UPGRADE_TO_SPECIFIC_VERSION,
    WINDOWS,
    POSTPONE
};

struct FirmwareExtra : public BaseJsonStruct {
    ForceUpgradeType forceUpgradeType = ForceUpgradeType::NOT_SUPPORT;
    int64_t lastUpdateTime;                            // 行业定制最晚升级时间
    PolicyType customPolicyType = PolicyType::DEFAULT; // 行业定制策略类型
    bool patchHasOUC = false;
    std::string releaseRuleAttr;
    JsonBuilder GetJsonBuilder() final;
};

struct ComponentDescription {
    std::string componentId;
    DescriptionInfo descriptionInfo;
    DescriptionInfo notifyDescriptionInfo;
};

struct VersionComponent : public BaseJsonStruct {
    std::string componentId;
    int32_t componentType = CAST_INT(ComponentType::INVALID);
    std::string upgradeAction;
    std::string displayVersion;
    std::string innerVersion;
    size_t size = 0;
    size_t effectiveMode = static_cast<size_t>(EffectiveMode::COLD);
    DescriptionInfo descriptionInfo;
    std::string componentExtra;

    JsonBuilder GetJsonBuilder() final;
};

struct CurrentVersionInfo {
    std::string osVersion;
    std::string deviceName;
    std::vector<VersionComponent> versionComponents;
};

struct NewVersionInfo {
    VersionDigestInfo versionDigestInfo;
    std::vector<VersionComponent> versionComponents;
};

struct VersionDescriptionInfo {
    std::vector<ComponentDescription> componentDescriptions;
};

struct CheckResult {
    bool isExistNewVersion = false;
    NewVersionInfo newVersionInfo;
};

struct TaskBody {
    VersionDigestInfo versionDigestInfo;
    UpgradeStatus status = UpgradeStatus::INIT;
    int32_t subStatus = CAST_INT(UpgradeStatus::INIT);
    int32_t progress = 0;
    int32_t installMode = CAST_INT(InstallMode::NORMAL);
    std::vector<ErrorMessage> errorMessages;
    std::vector<VersionComponent> versionComponents;

    JsonBuilder GetJsonBuilder(EventId eventId);
};

struct TaskInfo {
    bool existTask;
    TaskBody taskBody;
};

struct Progress {
    uint32_t percent = 0;
    UpgradeStatus status = UpgradeStatus::INIT;
    std::string endReason;
};

struct UpgradeInterval {
    uint64_t timeStart = 0;
    uint64_t timeEnd = 0;
};

struct UpgradePeriod {
    uint32_t start = 0;
    uint32_t end = 0;
};

struct UpdateTime {
    int64_t lastUpdateTime = 0;
    int64_t installWindowStart = 0;
    int64_t installWindowEnd = 0;

    friend void to_json(nlohmann::json &jsonObj, const UpdateTime &updateTime)
    {
        jsonObj["lastUpdateTime"] = updateTime.lastUpdateTime;
        jsonObj["installWindowStart"] = updateTime.installWindowStart;
        jsonObj["installWindowEnd"] = updateTime.installWindowEnd;
    }

    friend void from_json(const nlohmann::json &jsonObj, UpdateTime &updateTime)
    {
        JsonUtils::GetValueAndSetTo(jsonObj, "lastUpdateTime", updateTime.lastUpdateTime);
        JsonUtils::GetValueAndSetTo(jsonObj, "installWindowStart", updateTime.installWindowStart);
        JsonUtils::GetValueAndSetTo(jsonObj, "installWindowEnd", updateTime.installWindowEnd);
    }
};

struct CustomPolicy {
    PolicyType type = PolicyType::DEFAULT;
    UpdateTime installTime;
    std::string version;

    friend void to_json(nlohmann::json &jsonObj, const CustomPolicy &policy)
    {
        jsonObj["type"] = policy.type;
        jsonObj["installTime"] = policy.installTime;
        jsonObj["version"] = policy.version;
    }

    friend void from_json(const nlohmann::json &jsonObj, CustomPolicy &policy)
    {
        int32_t typeInt = CAST_INT(PolicyType::DEFAULT);
        JsonUtils::GetValueAndSetTo(jsonObj, "type", typeInt);
        policy.type = static_cast<PolicyType>(typeInt);
        JsonUtils::GetValueAndSetTo(jsonObj, "installTime", policy.installTime);
        JsonUtils::GetValueAndSetTo(jsonObj, "version", policy.version);
    }
};

struct UpgradePolicy {
    bool downloadStrategy = false;
    bool autoUpgradeStrategy = false;
    UpgradePeriod autoUpgradePeriods[2];
};

struct UpgradeFile {
    ComponentType fileType = ComponentType::INVALID;
    std::string filePath;
};

struct EventClassifyInfo {
    EventClassify eventClassify = EventClassify::TASK;
    std::string extraInfo;

    EventClassifyInfo() : eventClassify(EventClassify::TASK) {}
    explicit EventClassifyInfo(EventClassify classify) : eventClassify(classify) {}
    EventClassifyInfo(EventClassify classify, const std::string &info) : eventClassify(classify), extraInfo(info) {}
};

struct EventInfo : public BaseJsonStruct {
    EventId eventId = EventId::EVENT_TASK_BASE;
    TaskBody taskBody;

    EventInfo() = default;
    EventInfo(EventId id, TaskBody body) : eventId(id), taskBody(std::move(body)) {}

    JsonBuilder GetJsonBuilder() final;
};

struct ConfigInfo {
    std::string businessDomain;
    uint32_t abInstallTimeout = 1800; // 1800s
    std::string moduleLibPath;
};

struct SystemUpdateInfo {
    std::string version;
    int64_t firstReceivedTime;
    std::string packageType;
};

enum class AuthType {
    PKI = 1,
    WD = 2
};

enum class UpgAuthSignVer {
    AUTH_SIGN_TYPE_HOTA = 0,
    AUTH_SIGN_TYPE_PKI = 1
};

using OnEvent = std::function<void(const EventInfo &eventInfo)>;

// 回调函数
struct UpdateCallbackInfo {
    OnEvent onEvent;
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_HELPER_H
