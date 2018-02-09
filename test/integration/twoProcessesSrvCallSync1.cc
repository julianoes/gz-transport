/*
 * Copyright (C) 2014 Open Source Robotics Foundation
 *
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
 *
*/

#include <chrono>
#include <cstdlib>
#include <string>
#include <ignition/msgs.hh>

#include "ignition/transport/Node.hh"
#include "gtest/gtest.h"
#include "ignition/transport/test_config.h"

using namespace ignition;

static std::string partition;
static std::string g_topic = "/foo";
static int data = 5;

//////////////////////////////////////////////////
/// \brief This test spawns a service responser and a service requester. The
/// synchronous requester uses a wrong service's name. The test should verify
/// that the service call does not succeed and the elapsed time was close to
/// the timeout.
TEST(twoProcSrvCallSync1, SrvTwoProcs)
{
  std::string responser_path = testing::portablePathUnion(
     PROJECT_BINARY_PATH,
     "test/integration/INTEGRATION_twoProcessesSrvCallReplier_aux");

  testing::forkHandlerType pi = testing::forkAndRun(responser_path.c_str(),
    partition.c_str());

  int64_t timeout = 500;
  ignition::msgs::Int32 req;
  ignition::msgs::Int32 rep;
  bool result;

  req.set_data(data);

  transport::Node node;

  // Make sure that the address of the service call provider is known.
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  ASSERT_TRUE(node.Request(g_topic, req, static_cast<unsigned int>(timeout),
    rep, result));
  EXPECT_EQ(req.data(), rep.data());
  EXPECT_TRUE(result);

  auto t1 = std::chrono::steady_clock::now();
  EXPECT_FALSE(node.Request("unknown_service", req,
      static_cast<unsigned int>(timeout), rep, result));
  auto t2 = std::chrono::steady_clock::now();

  int64_t elapsed =
    std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

  // Check if the elapsed time was close to the timeout.
  auto diff = std::max(elapsed, timeout) - std::min(elapsed, timeout);
  EXPECT_LT(diff, 200);

  // Wait for the child process to return.
  testing::waitAndCleanupFork(pi);
}

//////////////////////////////////////////////////
int main(int argc, char **argv)
{
  // Get a random partition name.
  partition = testing::getRandomNumber();

  // Set the partition name for this process.
  setenv("IGN_PARTITION", partition.c_str(), 1);

  // Enable verbose mode.
  setenv("IGN_VERBOSE", "1", 1);

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
