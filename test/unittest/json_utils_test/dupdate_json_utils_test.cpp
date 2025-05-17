/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "cJSON.h"
#include "dupdate_json_utils.h"

using namespace testing::ext;
using namespace testing;

namespace OHOS::UpdateService {
class JsonUtilsTest : public testing::Test {
public:
    static void SetupTestCase(void) {};
    static void TearDownTestCase(void) {};
    void SetUp() final {};
    void TearDown() final {};
};

/**
* @tc.name: ParseAndGetJsonObjectTrue
* @tc.desc: Test json string is true
* @tc.type: FUNC
*/
HWTEST_F(JsonUtilsTest, ParseAndGetJsonObjectTrue, TestSize.Level1)
{
    std::string jsonStr = "{\"name\":\"Alice\", \"age\":25}";
    cJSON *root = JsonUtils::ParseAndGetJsonObject(jsonStr);
    EXPECT_NE(nullptr, root);
    cJSON_Delete(root);
}

/**
* @tc.name: ParseAndGetJsonObjectFalse
* @tc.desc: Test json string is false
* @tc.type: FUNC
*/
HWTEST_F(JsonUtilsTest, ParseAndGetJsonObjectFalse, TestSize.Level1)
{
    std::string jsonStr = "{\"name\":\"Alice\", \"age\":25, abc}";
    cJSON *root = JsonUtils::ParseAndGetJsonObject(jsonStr);
    EXPECT_EQ(nullptr, root);
}

/**
* @tc.name: GetValueAndSetToInt
* @tc.desc: Test json string, get int
* @tc.type: FUNC
*/
HWTEST_F(JsonUtilsTest, GetValueAndSetToInt, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"file\": \"a.txt\"}";
    int ageNumber = 0;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    EXPECT_NE(nullptr, root);
    int ret = JsonUtils::GetValueAndSetTo(root, "age", ageNumber);
    EXPECT_EQ(ageNumber, 54);
    EXPECT_EQ(0, ret);
    cJSON_Delete(root);
}

/**
* @tc.name: GetValueAndSetToUint64
* @tc.desc: Test json string, get uint64
* @tc.type: FUNC
*/
HWTEST_F(JsonUtilsTest, GetValueAndSetToUint64, TestSize.Level1)
{
    std::string jsonStr = "{\"time\": 123456789, \"file\": \"a.txt\"}";
    uint64_t timeNumber = 0;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    EXPECT_NE(nullptr, root);
    int ret = JsonUtils::GetValueAndSetTo(root, "time", timeNumber);
    EXPECT_EQ(timeNumber, 123456789);
    EXPECT_EQ(0, ret);
    cJSON_Delete(root);
}

/**
* @tc.name: GetValueAndSetToString
* @tc.desc: Test json string, get string
* @tc.type: FUNC
*/
HWTEST_F(JsonUtilsTest, GetValueAndSetToString, TestSize.Level1)
{
    std::string jsonStr = "{\"age\": 54, \"file\": \"a.txt\"}";
    std::string fileName;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    EXPECT_NE(nullptr, root);
    int ret = JsonUtils::GetValueAndSetTo(root, "file", fileName);
    EXPECT_EQ(fileName, "a.txt");
    EXPECT_EQ(0, ret);
    cJSON_Delete(root);
}

/**
* @tc.name: GetValueAndSetToBool
* @tc.desc: Test json string, get bool
* @tc.type: FUNC
*/
HWTEST_F(JsonUtilsTest, GetValueAndSetToBool, TestSize.Level1)
{
    std::string jsonStr = "{\"exist\": true, \"file\": \"a.txt\"}";
    bool  existYN = false;
    cJSON *root = cJSON_Parse(jsonStr.c_str());
    EXPECT_NE(nullptr, root);
    int ret = JsonUtils::GetValueAndSetTo(root, "exist", existYN);
    EXPECT_EQ(existYN, true);
    EXPECT_EQ(0, ret);
    cJSON_Delete(root);
}
} // namespace OHOS::UpdateService