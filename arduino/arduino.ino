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
