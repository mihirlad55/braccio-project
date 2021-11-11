#ifndef _JOINT_LIMITS_CPP_
#define _JOINT_LIMITS_CPP_

/**
 * Struct to hold joint limits
 */
typedef struct {
  int lowerLimit;
  int higherLimit;
} JointLimits;

/**
 * Limits for each joint
 */
const JointLimits BASE_LIMITS = {.lowerLimit = 0, .higherLimit = 180};
const JointLimits SHOULDER_LIMITS = {.lowerLimit = 15, .higherLimit = 165};
const JointLimits ELBOW_LIMITS = {.lowerLimit = 0, .higherLimit = 180};
const JointLimits WRIST_VER_LIMITS = {.lowerLimit = 0, .higherLimit = 180};
const JointLimits WRIST_ROT_LIMITS = {.lowerLimit = 0, .higherLimit = 180};
const JointLimits GRIPPER_LIMITS = {.lowerLimit = 10, .higherLimit = 73};

/**
 * Array of joint limits
 */
const JointLimits JOINT_LIMITS[] = {BASE_LIMITS,      SHOULDER_LIMITS,
                                    ELBOW_LIMITS,     WRIST_VER_LIMITS,
                                    WRIST_ROT_LIMITS, GRIPPER_LIMITS};

#endif // _JOINT_LIMITS_CPP_
