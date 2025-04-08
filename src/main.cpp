/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Robotics                                                  */
/*    Created:      3/13/2025, 4:56:54 PM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"
#include <stdio.h> 
#include <stdlib.h>

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

//&& NUMERAL CONSTANTS 
int CONVEYOR_MAX_SPEED = 100;
int CONVEYOR_MIN_SPEED = 0;


//&& BRAIN CONSTANT ---------------------------------------------
brain Brain;

// && SENSOR CONSTANTS -----------------------------------------
//TODO to be configured
//https://api.vex.com/v5/home/cpp/Inertial.html
inertial Inertial = inertial(PORT20);

// && SMART PORT CONSTANTS
//TODO CONFIGURE
digital_out leftMoboClampPiston = digital_out(Brain.ThreeWirePort.A);
digital_out rightMoboClampPiston = digital_out(Brain.ThreeWirePort.A);


//&& CONTROLLER CONSTANTS ----------------------------------------
controller DRIVER = controller(primary);
controller OPERATOR = controller(partner);

int Channel1;
int Channel2;
int threshold;

//&& DRIVE MOTOR CONSTANTS ---------------------------------------------
// ** Parameters: port, ratio, reversed boolean
//TODO update ports, update which side should be inversed
motor frontLeftDriveMotor = motor(PORT3, ratio18_1, false);
motor frontRightDriveMotor = motor(PORT4, ratio18_1, true);
motor backLeftDriveMotor = motor(PORT5, ratio18_1, false);
motor backRightDriveMotor = motor(PORT6, ratio18_1, true);

motor_group leftMotorGroup = motor_group(frontLeftDriveMotor, backRightDriveMotor);
motor_group rightMotorGroup = motor_group(frontLeftDriveMotor, backRightDriveMotor);

//TODO VERIFY THESE VALUES
smartdrive DriveTrain = smartdrive(leftMotorGroup, rightMotorGroup,
                                  Inertial, 0, 0, 0, mm, 1);
// view: https://api.vex.com/v5/home/cpp/SmartDrive.html#smartdrive

//&& DRIVE ENCODER CONSTANTS

// && SCORING MOTOR CONSTANTS 
motor conveyorMotor = motor(PORT11, ratio18_1, false);
motor intakeMotor = motor(PORT12, ratio18_1, false);

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
void pre_auton(void) {
  //RESETS THE GYROSCOPE FOR THE SMART DRIVETRAIN
  Inertial.calibrate();
  Inertial.resetHeading();
  Inertial.resetRotation();

  while (Inertial.isCalibrating()) {
    Brain.Screen.clearScreen();
    Brain.Screen.print("Inertial Calibrating");
    wait(0.5, seconds);
  }
  
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  //RESETS THE CLAMP 
  leftMoboClampPiston.set(false);
  rightMoboClampPiston.set(false);
}



void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  //TO BE TESTED
  DriveTrain.driveFor(forward, 6, inches);
  //TODO VERIFY
  //DriveTrain.turnToHeading();
}


void configureDriveBindings(){

  // * CONVEYOR CONTROL
  if (DRIVER.ButtonL1.pressing()) { //conveyor belt
    conveyorMotor.spin(reverse, CONVEYOR_MAX_SPEED, percent);
  } 
  else if (DRIVER.ButtonL2.pressing()) {
    conveyorMotor.spin(forward, CONVEYOR_MAX_SPEED, percent);
  }
  else {
    conveyorMotor.stop();
  }

  // * CLAMP CONTROL
  if (DRIVER.ButtonR1.pressing()) { //piston
    leftMoboClampPiston.set(true);
    rightMoboClampPiston.set(true);
  } else {
    leftMoboClampPiston.set(false);
    rightMoboClampPiston.set(false);
  }

    //Sets sepeeds to inputs from driver contrller 
    int leftSpeed = DRIVER.Axis3.position();
    int rightSpeed = DRIVER.Axis2.position();

    // TODO Deadband Implementation?
    frontLeftDriveMotor.spin(forward, leftSpeed, percent);
    frontRightDriveMotor.spin(forward, leftSpeed, percent);

    backLeftDriveMotor.spin(forward, leftSpeed, percent);
    backRightDriveMotor.spin(forward, rightSpeed, percent);


}

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    configureDriveBindings();


    


    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//todo for now put everything in here


//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
