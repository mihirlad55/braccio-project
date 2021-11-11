#ifndef _JOINT_LIST_HPP_
#define _JOINT_LIST_HPP_

#include "../src/joint_limits.cpp"

/**
 * The JointList class is used as a container for specifying the joint
 * configuration of the Braccio robot.
 * It simplifies passing joint angles.
 */
class JointList {
public:
  /**
   * Construct a JointList object
   *
   * @param base  Base joint angle
   * @param shoulder  Shoulder joint angle
   * @param elbow  Elbow joint angle
   * @param wrist_ver  Wrist vertical joint angle
   * @param wrist_rot  Wrist rotation joint angle
   * @param gripper  Gripper joint angle
   */
  JointList(int base, int shoulder, int elbow, int wrist_ver, int wrist_rot,
            int gripper);

  /**
   * Copy constructor
   *
   * @param old  The old JointList object
   *
   * @return New JointList object
   */
  JointList(const JointList &old);
  /**
   * Subscript operator override to access joints
   *
   * @param index  The index
   *
   * @return A reference to the corresponding joint variable
   */
  int &operator[](int index);

  // Joint angles
  int base;
  int shoulder;
  int elbow;
  int wrist_ver;
  int wrist_rot;
  int gripper;
};

#endif _JOINT_LIST_HPP_
