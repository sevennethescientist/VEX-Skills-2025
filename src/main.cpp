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

//&& BRAIN CONSTANT ---------------------------------------------
brain Brain;

// && SENSOR CONSTANTS -----------------------------------------
//TODO to be configured
inertial Inertial = inertial(PORT20);

// && SMART PORT CONSTANTS
//TODO CONFIGURE
digital_out moboClampPiston = digital_out(Brain.ThreeWirePort.A);


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
  Inertial.resetHeading();
  Inertial.resetRotation();
  Inertial.calibrate();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  //RESETS THE CLAMP 
  moboClampPiston.set(false);
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
  if (DRIVER.ButtonL1.pressing()) { //conveyor belt
    conveyorMotor.spin(forward, 50, percent);
  } else {
    conveyorMotor.stop();
  }

  if (DRIVER.ButtonL2.pressing()) { //piston
    moboClampPiston.set(true);
  } else {
    moboClampPiston.set(false);
  }

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
    //Sets sepeeds to inputs from driver contrller 
    int leftSpeed = DRIVER.Axis3.position();
    int rightSpeed = DRIVER.Axis2.position();

    // TODO Deadband Implementation
    // ^ SET MOTORSPEEDS 
    //TODO SEE IF NEEDS CHANGE
    // frontLeftDriveMotor.spin(forward, leftSpeed, percent);
    // frontRightDriveMotor.spin(forward, leftSpeed, percent);

    // backLeftDriveMotor.spin(forward, leftSpeed, percent);
    // backRightDriveMotor.spin(forward, rightSpeed, percent);

    


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
