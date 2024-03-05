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

#include "update_log.h"

using namespace testing::ext;
using namespace testing;

namespace OHOS::UpdateEngine {
class UpdateLogTest : public testing::Test {
public:
    static void SetupTestCase(void) {};
    static void TearDownTestCase(void) {};
    void SetUp() final {};
    void TearDown() final {};
};

/**
* @tc.name: TestJudgeLevel
* @tc.desc: Test judge level.
* @tc.type: FUNC
*/
HWTEST_F(UpdateLogTest, TestJudgeLevel, TestSize.Level1)
{
    ASSERT_TRUE(UpdateLog().JudgeLevel(UpdateLogLevel::UPDATE_INFO));
}

/**
* @tc.name: TestSetLogLevel
* @tc.desc: Test set log level.
* @tc.type: FUNC
*/
HWTEST_F(UpdateLogTest, TestSetLogLevel, TestSize.Level1)
{
    UpdateLog log;
    UpdateLogLevel level = UpdateLogLevel::UPDATE_DEBUG;
    log.SetLogLevel(level);
    ASSERT_TRUE(true);
}

/**
* @tc.name: TestGetLogLevel
* @tc.desc: Test get log level.
* @tc.type: FUNC
*/
HWTEST_F(UpdateLogTest, TestGetLogLevel, TestSize.Level1)
{
    UpdateLog log;
    UpdateLogLevel level = UpdateLogLevel::UPDATE_ERROR;
    log.SetLogLevel(level);
    ASSERT_EQ(level, log.GetLogLevel());
}

/**
* @tc.name: TestGetBriefFileName
* @tc.desc: Test get brief file name.
* @tc.type: FUNC
*/
HWTEST_F(UpdateLogTest, TestGetBriefFileName, TestSize.Level1)
{
    UpdateLog log;
    ASSERT_EQ("test.cpp", log.GetBriefFileName("test.cpp"));
    ASSERT_EQ("test.cpp", log.GetBriefFileName("log/test.cpp"));
    ASSERT_EQ("test.cpp", log.GetBriefFileName("log\\test.cpp"));
}

/**
* @tc.name: TestPrintLongLog
* @tc.desc: Test print long log.
* @tc.type: FUNC
*/
HWTEST_F(UpdateLogTest, TestPrintLongLog, TestSize.Level1)
{
    constexpr const char *longLogBuffer =
        "------------------------------string test for long log printf---------------------------------------"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
        "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    ENGINE_LONG_LOGD("log test %{public}s", longLogBuffer);
    ENGINE_LONG_LOGI("log test %{public}s", longLogBuffer);
    ENGINE_LONG_LOGE("log test %{public}s", longLogBuffer);
    ENGINE_LONG_LOGI("log test %s", longLogBuffer);
    ENGINE_LONG_LOGI("log test %{private}s", longLogBuffer);
    ENGINE_LONG_LOGI("log test %{public}s", longLogBuffer);
    ASSERT_TRUE(true);
}
} // namespace OHOS::UpdateEngine