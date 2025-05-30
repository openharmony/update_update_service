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

#ifndef UPDATE_NO_CONSTRUCTOR_H
#define UPDATE_NO_CONSTRUCTOR_H

namespace OHOS {
namespace UpdateService {
class NoConstructor {
public:
    // 禁止默认构造
    NoConstructor() = delete;

    // 禁止拷贝构造
    NoConstructor(const NoConstructor &) = delete;
};
} // namespace UpdateService
} // namespace OHOS
#endif // UPDATE_NO_CONSTRUCTOR_H