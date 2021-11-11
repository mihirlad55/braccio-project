#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include "../src/joint_limits.cpp"
#include "JointList.hpp"

/**
 * Check if angle given is within range of the joint
 *
 * @param joint_angle  The value of the angle
 * @param limits  Reference to JointLimits struct defining limits
 *
 * @return True if joint is within range, otherwise false
 */
bool is_joint_in_range(int joint_angle, const JointLimits &limits);

/**
 * Check if JointList object has all joint angles specified within limits
 *
 * @param joint_list  The JointList object to check
 *
 * @return True if all joints are within limits, otherwise false
 */
bool validate_joints(const JointList& joint_list);

/**
 * Coerce joint angles to be within limits
 *
 * @param joint_list  JointList object
 */
void constrain_joints(JointList& joint_list);

#endif // _UTIL_HPP_
