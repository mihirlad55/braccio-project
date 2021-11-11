#include "Braccio.h"

class JointList;

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

/**
 * All Braccio servos.
 * Must be named this because they are defined as extern in Braccio.h
 */
Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

/**
 * Check if angle given is within range of the joint
 *
 * @param joint_angle  The value of the angle
 * @param limits  Reference to JointLimits struct defining limits
 *
 * @return True if joint is within range, otherwise false
 */
bool is_joint_in_range(int joint_angle, const JointLimits &limits) {
  if (joint_angle >= limits.lowerLimit && joint_angle <= limits.higherLimit)
    return true;

  return false;
}

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
            int gripper)
      : base(base), shoulder(shoulder), elbow(elbow), wrist_ver(wrist_ver),
        wrist_rot(wrist_rot), gripper(gripper) {}

  /**
   * Copy constructor
   *
   * @param old  The old JointList object
   *
   * @return New JointList object
   */
  JointList(const JointList &old) {
    this->base = old.base;
    this->shoulder = old.shoulder;
    this->elbow = old.elbow;
    this->wrist_ver = old.wrist_ver;
    this->wrist_rot = old.wrist_rot;
    this->gripper = old.gripper;
  }

  /**
   * Subscript operator override to access joints
   *
   * @param index  The index
   *
   * @return A reference to the corresponding joint variable
   */
  int &operator[](int index) {
    switch (index) {
    case 0:
      return base;
    case 1:
      return shoulder;
    case 2:
      return elbow;
    case 3:
      return wrist_ver;
    case 4:
      return wrist_rot;
    case 5:
      return gripper;
    }
  }

  // Joint angles
  int base;
  int shoulder;
  int elbow;
  int wrist_ver;
  int wrist_rot;
  int gripper;
};

/**
 * Check if JointList object has all joint angles specified within limits
 *
 * @param joint_list  The JointList object to check
 *
 * @return True if all joints are within limits, otherwise false
 */
bool validate_joints(const JointList& joint_list) {
  for (int i = 0; i < 6; i++) {
    if (!is_joint_in_range(joint_list[i], JOINT_LIMITS[i])) {
      return false;
    }
  }

  return true;
}

/**
 * Coerce joint angles to be within limits
 *
 * @param joint_list  JointList object
 */
void constrain_joints(JointList& joint_list) {
  for (int i = 0; i < 6; i++) {
    if (joint_list[i] < JOINT_LIMITS[i].lowerLimit)
      joint_list[i] = JOINT_LIMITS[i].lowerLimit;
    else if (joint_list[i] > JOINT_LIMITS[i].higherLimit)
      joint_list[i] = JOINT_LIMITS[i].higherLimit;
  }
}


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
  BraccioArm(_Braccio &braccio)
      : braccio(braccio), current_joints({0, 15, 0, 0, 0, 10}) {}

  /**
   * Initialize Braccio arm
   */
  void begin() { braccio.begin(); }

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
  bool set_step_delay(int step_delay) {
    if (step_delay < 10 || step_delay > 30)
      return false;
    else
      this->step_delay = step_delay;
  }

  /**
   * Move to a point within the arm's reachable workspace.
   *
   * @param joints  A JointList object specifying the joint angles. A value of
   *                -1 for the joint angle maintains the current joint angle
   *
   * @return True, if valid joint angles were specified, otherwise false
   */
  bool move(JointList joints) {
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

  /**
   * Move the base joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_base(const int angle) {
    JointList new_joints = current_joints;
    new_joints.base = angle;

    return move(new_joints);
  }

  /**
   * Move the shoulder joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_shoulder(const int angle) {
    JointList new_joints = current_joints;
    new_joints.shoulder = angle;

    return move(new_joints);
  }

  /**
   * Move the elbow joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_elbow(const int angle) {
    JointList new_joints = current_joints;
    new_joints.elbow = angle;

    return move(new_joints);
  }

  /**
   * Move the wrist vertical joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_wrist_ver(const int angle) {
    JointList new_joints = current_joints;
    new_joints.wrist_ver = angle;

    return move(new_joints);
  }

  /**
   * Move the wrist rotation joint of the robot
   *
   * @param angle  The base angle
   *
   * @param True if valid joint angle was specified, otherwise false
   */
  bool move_wrist_rot(const int angle) {
    JointList new_joints = current_joints;
    new_joints.wrist_rot = angle;

    return move(new_joints);
  }

  /**
   * Close the gripper of the robot
   */
  void close_gripper() {
    current_joints.gripper = GRIPPER_LIMITS.higherLimit;
    move(current_joints);
  }

  /**
   * Open the gripper of the robot
   */
  void open_gripper() {
    current_joints.gripper = GRIPPER_LIMITS.lowerLimit;
    move(current_joints);
  }

private:
  // Reference to _Braccio passed in
  const _Braccio braccio;
  // Current step_delay
  int step_delay = 10;
  // Current joint configuration
  JointList current_joints;
};


BraccioArm arm(Braccio);

/**
 * Check if an obstacle is detected
 *
 * @return True if obstacle is detected, otherwise false
 */
bool isObstacleDetected() {
  byte data = 0;

  // Seek to final byte and read
  while (Serial.available() > 0) {
    data = Serial.read();
  }

  // Debug
  Serial.write("Received data\n");

  return data == 1;
}

void setup() {
  // Initialize shield
  arm.begin();

  // Initialize serial
  Serial.begin(9600);
}

void movePrimaryPath() {
  // Turn around
  arm.move_base(180);
  delay(1000);
}

void moveSecondaryPath() {
  // Raise arm higher
  arm.move_wrist_ver(90);
  delay(1000);

  // Turn around
  arm.move_base(180);
  delay(1000);

  // Lower arm
  arm.move_wrist_ver(180);
  delay(1000);
}

void loop() {
  // Move to start position
  arm.move({0, 90, 90, 90, 90, 10});
  delay(1000);

  // Get ready to pick up
  arm.move({0, 45, 180, 180, 0, 10});
  delay(1000);

  // Reach down
  arm.move_shoulder(90);
  delay(1000);

  // Grab object
  arm.close_gripper();
  delay(1000);

  // Move up
  arm.move_shoulder(45);
  delay(1000);

  // Use collision-avoiding path
  if (!isObstacleDetected())
    movePrimaryPath();
  else
    moveSecondaryPath();

  // Reach down
  arm.move_shoulder(90);
  delay(1000);

  // Let go of object
  arm.open_gripper();
  delay(1000);

  // Move arm up
  arm.move_shoulder(45);
  delay(1000);
}
