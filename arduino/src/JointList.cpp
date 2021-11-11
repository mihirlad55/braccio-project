#include "../include/JointList.hpp"

JointList::JointList(int base, int shoulder, int elbow, int wrist_ver,
                     int wrist_rot, int gripper)
    : base(base), shoulder(shoulder), elbow(elbow), wrist_ver(wrist_ver),
      wrist_rot(wrist_rot), gripper(gripper) {}

JointList::JointList(const JointList &old) {
  this->base = old.base;
  this->shoulder = old.shoulder;
  this->elbow = old.elbow;
  this->wrist_ver = old.wrist_ver;
  this->wrist_rot = old.wrist_rot;
  this->gripper = old.gripper;
}

int &JointList::operator[](int index) {
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
