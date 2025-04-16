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

#ifndef UPDATE_SERVICE_DOWNLOAD_OPTIONS_H
#define UPDATE_SERVICE_DOWNLOAD_OPTIONS_H

#include "network_type.h"
#include "order.h"
#include "parcel.h"
#include "update_log.h"

namespace OHOS::UpdateService {
struct DownloadOptions : public Parcelable {
    NetType allowNetwork = NetType::WIFI;
    Order order = Order::DOWNLOAD;

    bool ReadFromParcel(Parcel &parcel);
    bool Marshalling(Parcel &parcel) const override;
    static DownloadOptions *Unmarshalling(Parcel &parcel);
};
} // namespace OHOS::UpdateService
#endif // UPDATE_SERVICE_DOWNLOAD_OPTIONS_H
