/**@file    entry_point.cc
 * @brief   Auto-testing entry point of the SEL library
 * @date    Date Created:: 10/05/14 Dovgal Kostiantyn
 *          $Date:: 2014-10-05 15:32#$
 * @version $Revision:: 1#$
 * @author  Copyright (c) 2014
 */

#include <gtest\gtest.h>

int main(int argc, char* argv[]) {

  ::testing::InitGoogleTest(&argc, argv);

  //::testing::AddGlobalTestEnvironment( new SELEnvironment );
  return RUN_ALL_TESTS();
}
