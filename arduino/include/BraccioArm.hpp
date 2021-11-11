#ifndef _BRACCIO_ARM_HPP_
#define _BRACCIO_ARM_HPP_

#include "JointList.hpp"
#include "Braccio.h"

/**
 * The BraccioArm class simplifies controlling the Braccio arm
 */
class BraccioArm {
public:
  /**
   * Construct a BraccioArm
   *
   * @param braccio  Reference to _Braccio object
   */
  BraccioArm(_Braccio &braccio);

  /**
   * Initialize Braccio arm
   */
  void begin();

  /**
   * Set delay between steps to control joint angles. Joint angles are changed
   * 1 degree at a time, this delay is how long the program sleeps before
   * stepping the joint angle again. As a result, the step delay controls the
   * speed of the robot.
   *
   * @param step_delay  The delay between 10 and 30ms
   *
   * @return True if a valid step delay was specified, otherwise false
   */
  bool set_step_delay(int step_delay);

  /**
   * Move to a point within the arm's reachable workspace.
   *
   * @param joints  A JointList object specifying the joint angles. A value of
   *                -1 for the joint angle maintains the current joint angle
   *
   * @return True, if valid joint angles were specified, otherwise false
   */
  bool move(JointList joints);

  /**
   * Move the base joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_base(const int angle);

  /**
   * Move the shoulder joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_shoulder(const int angle);

  /**
   * Move the elbow joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_elbow(const int angle);

  /**
   * Move the wrist vertical joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_wrist_ver(const int angle);

  /**
   * Move the wrist rotation joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_wrist_rot(const int angle);

  /**
   * Close the gripper of the robot
   */
  void close_gripper();

  /**
   * Open the gripper of the robot
   */
  void open_gripper();

private:
  // Reference to _Braccio passed in
  const _Braccio braccio;

  // Current step_delay
  int step_delay = 10;

  // Current joint configuration
  JointList current_joints;
};

#endif // _BRACCIO_ARM_HPP_
