#include "include/JointList.hpp"
#include "include/BraccioArm.hpp"
#include "include/util.hpp"
#include "Braccio.h"

class JointList;

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

// Braccio Arm
BraccioArm arm(Braccio);

// Delay between steps in routine in ms
const int ROUTINE_STEP_DELAY = 1000;

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

  // Move to start position
  arm.move({0, 90, 90, 90, 90, 10});
  delay(ROUTINE_STEP_DELAY);

  // Get ready to pick up
  arm.move({0, 45, 180, 180, 0, 10});
  delay(ROUTINE_STEP_DELAY);

  // Reach down
  arm.move_shoulder(90);
  delay(ROUTINE_STEP_DELAY);
}

void movePrimaryPath() {
  // Turn around
  arm.move_base(180);
  delay(ROUTINE_STEP_DELAY);
}

void movePrimaryPathReturn() {
  // Turn around
  arm.move_base(0);
  delay(ROUTINE_STEP_DELAY);
}

void moveSecondaryPath() {
  // Raise arm higher
  arm.move_wrist_ver(90);
  delay(ROUTINE_STEP_DELAY);

  // Turn around
  arm.move_base(180);
  delay(ROUTINE_STEP_DELAY);

  // Lower arm
  arm.move_wrist_ver(180);
  delay(ROUTINE_STEP_DELAY);
}

void moveSecondaryPathReturn() {
  // Raise arm higher
  arm.move_wrist_ver(90);
  delay(ROUTINE_STEP_DELAY);

  // Turn around
  arm.move_base(0);
  delay(ROUTINE_STEP_DELAY);

  // Lower arm
  arm.move_wrist_ver(180);
  delay(ROUTINE_STEP_DELAY);
}

void loop() {
  // Grab object
  arm.close_gripper();
  delay(ROUTINE_STEP_DELAY);

  // Move up
  arm.move_shoulder(45);
  delay(ROUTINE_STEP_DELAY);

  // Use collision-avoiding path
  if (!isObstacleDetected())
    movePrimaryPath();
  else
    moveSecondaryPath();

  // Reach down
  arm.move_shoulder(90);
  delay(ROUTINE_STEP_DELAY);

  // Let go of object
  arm.open_gripper();
  delay(ROUTINE_STEP_DELAY);

  // Grab object again
  arm.close_gripper();
  delay(ROUTINE_STEP_DELAY);

  // Move up
  arm.move_shoulder(45);
  delay(ROUTINE_STEP_DELAY);

  // Use collision-avoiding path
  if (!isObstacleDetected())
    movePrimaryPathReturn();
  else
    moveSecondaryPathReturn();

  // Reach down
  arm.move_shoulder(90);
  delay(ROUTINE_STEP_DELAY);

  // Let go of object
  arm.open_gripper();
  delay(ROUTINE_STEP_DELAY);
}
