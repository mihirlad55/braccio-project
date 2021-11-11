#include "../include/util.hpp"

bool is_joint_in_range(int joint_angle, const JointLimits &limits) {
  if (joint_angle >= limits.lowerLimit && joint_angle <= limits.higherLimit)
    return true;

  return false;
}

bool validate_joints(const JointList& joint_list) {
  for (int i = 0; i < 6; i++) {
    if (!is_joint_in_range(joint_list[i], JOINT_LIMITS[i])) {
      return false;
    }
  }

  return true;
}

void constrain_joints(JointList& joint_list) {
  for (int i = 0; i < 6; i++) {
    if (joint_list[i] < JOINT_LIMITS[i].lowerLimit)
      joint_list[i] = JOINT_LIMITS[i].lowerLimit;
    else if (joint_list[i] > JOINT_LIMITS[i].higherLimit)
      joint_list[i] = JOINT_LIMITS[i].higherLimit;
  }
}
