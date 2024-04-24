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

#ifndef UPDATE_SERVICE_UPGRADE_STATUS_H
#define UPDATE_SERVICE_UPGRADE_STATUS_H

namespace OHOS::UpdateEngine {
enum class UpgradeStatus {
    ERROR = -1,
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
    AUTHING = 60,
    AUTH_FAIL,
    AUTH_SUCCESS,
    WAIT_TRANSFER = 68,
    TRANSFER_START = 69,
    PACKAGE_TRANSING = 70,
    PACKAGE_TRANS_FAIL,
    PACKAGE_TRANS_SUCCESS,
    INSTALLING = 80,
    INSTALL_FAIL,
    INSTALL_SUCCESS,
    UPDATING = 90,
    UPDATE_FAIL,
    UPDATE_SUCCESS,
    UPGRADE_REBOOT,
    UPGRADE_COUNT_DOWN = 100,
    UPGRADE_CANCEL
};
} // namespace OHOS::UpdateEngine
#endif // UPDATE_SERVICE_UPGRADE_STATUS_H
