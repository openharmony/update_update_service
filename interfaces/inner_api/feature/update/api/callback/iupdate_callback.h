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

#ifndef IUPDATE_CALLBACK_H
#define IUPDATE_CALLBACK_H

#include <iostream>
#include "event_info.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"

namespace OHOS::UpdateEngine {
class IUpdateCallback : public OHOS::IRemoteBroker {
public:
    virtual ~IUpdateCallback() = default;
    enum {
        ON_EVENT = 1
    };

    DECLARE_INTERFACE_DESCRIPTOR(u"OHOS.Update.IUpdateCallback");
public:
    virtual void OnEvent(const EventInfo &eventInfo) = 0;
};
} // namespace OHOS::UpdateEngine
#endif // IUPDATE_CALLBACK_H
