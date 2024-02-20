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

#ifndef AUTO_REGISTER_H
#define AUTO_REGISTER_H

#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace OHOS::UpdateEngine {
template <class INTERFACE> class Container {
public:
    using FuncType = std::function<std::shared_ptr<INTERFACE>()>;

    ~Container() = default;
    // 单例模式
    static Container &Instance()
    {
        static Container instance; // c++11 静态局部变量保证线程安全
        return instance;
    }

    bool RegisterType(uint32_t functionType, FuncType type)
    {
        if (createMap_.find(functionType) != createMap_.end()) {
            return false;
        }
        return createMap_.emplace(functionType, type).second;
    }

    std::shared_ptr<INTERFACE> GetPtr(uint32_t functionType)
    {
        if (createMap_.find(functionType) == createMap_.end()) {
            return nullptr;
        }
        FuncType function = createMap_[functionType];
        // 获取容器中对象时实例化
        return function();
    }

private:
    Container() = default;
    Container(const Container &) = delete;
    Container(Container &&) = delete;

private:
    // 存储注入对象的回调函数
    std::unordered_map<uint32_t, FuncType> createMap_;
};

template <typename INTERFACE, typename T> class NapiAutoRegister {
public:
    explicit NapiAutoRegister(uint32_t FuncType)
    {
        Container<INTERFACE>::Instance().RegisterType(FuncType, []() { return std::make_shared<T>(); });
    }
};
} // namespace OHOS::UpdateEngine
#endif // AUTO_REGISTER_H