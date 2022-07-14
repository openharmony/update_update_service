/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef UPDATE_CALLBACK_PROXY_H
#define UPDATE_CALLBACK_PROXY_H

#include "iremote_proxy.h"
#include "iupdate_service.h"

namespace OHOS {
namespace UpdateEngine {
class UpdateCallbackProxy : public IRemoteProxy<IUpdateCallback> {
public:
    explicit UpdateCallbackProxy(const sptr<IRemoteObject>& impl) : IRemoteProxy<IUpdateCallback>(impl) {}

    virtual ~UpdateCallbackProxy() = default;

    void OnCheckVersionDone(const BusinessError &businessError, const CheckResultEx &checkResultEx) override;

    void OnEvent(const EventInfo &eventInfo) override;

private:
    static inline BrokerDelegator<UpdateCallbackProxy> delegator_;
};
} // namespace UpdateEngine
} // namespace OHOS
#endif // UPDATE_CALLBACK_PROXY_H