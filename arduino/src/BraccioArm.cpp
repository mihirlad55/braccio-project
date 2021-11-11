#include "Braccio.h"
#include "../include/util.hpp"
#include "../include/BraccioArm.hpp"

BraccioArm::BraccioArm(_Braccio &braccio)
    : braccio(braccio), current_joints({0, 15, 0, 0, 0, 10}) {}

void BraccioArm::begin() { braccio.begin(); }

bool BraccioArm::set_step_delay(int step_delay) {
  if (step_delay < 10 || step_delay > 30)
    return false;
  else
    this->step_delay = step_delay;
}

bool BraccioArm::move(JointList joints) {
  // If -1 is specified, don't change joint
  for (int i = 0; i < 6; i++) {
    if (joints[i] == -1)
      joints[i] = current_joints[i];
  }

  // Check all joints
  if (!validate_joints(joints))
    return false;

  // Update current joint positions
  current_joints = joints;

  // Move arm to reachable point
  braccio.ServoMovement(this->step_delay, joints[0], joints[1], joints[2],
                        joints[3], joints[4], joints[5]);
  return true;
}

bool BraccioArm::move_base(const int angle) {
  JointList new_joints = current_joints;
  new_joints.base = angle;

  return move(new_joints);
}

bool BraccioArm::move_shoulder(const int angle) {
  JointList new_joints = current_joints;
  new_joints.shoulder = angle;

  return move(new_joints);
}

bool BraccioArm::move_elbow(const int angle) {
  JointList new_joints = current_joints;
  new_joints.elbow = angle;

  return move(new_joints);
}

bool BraccioArm::move_wrist_ver(const int angle) {
  JointList new_joints = current_joints;
  new_joints.wrist_ver = angle;

  return move(new_joints);
}

bool BraccioArm::move_wrist_rot(const int angle) {
  JointList new_joints = current_joints;
  new_joints.wrist_rot = angle;

  return move(new_joints);
}

void BraccioArm::close_gripper() {
  current_joints.gripper = GRIPPER_LIMITS.higherLimit;
  move(current_joints);
}

void BraccioArm::open_gripper() {
  current_joints.gripper = GRIPPER_LIMITS.lowerLimit;
  move(current_joints);
}
