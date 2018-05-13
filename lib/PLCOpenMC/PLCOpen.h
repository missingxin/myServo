#ifndef __PLCOPEN_MC_H__
#define __PLCOPEN_MC_H__
/*******************************************************************************
* File Name          : PLCOpen.h
* Author             : Joseph Lin
* Version            : V0.0.1
* Date               : 05/8/2018
* Description        : 建立可物件化的PLCOpen Middleware
********************************************************************************
* 說明：主要目標是建立一個可重覆使用在不同平台的 PLCOpen Middleware
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#if !defined(BOOL)
#define BOOL unsigned char
#endif

#if !defined(FALSE)
#define FALSE 0
#endif

#if !defined(TRUE)
#define TRUE 1
#endif

#if !defined(REAL)
#define REAL float
#endif

typedef enum{
  undefine, //0
  test
}MC_BUFFER_MODE; //See 2.4.2 Aborting versus Buffered modes

typedef enum{
  WORD_NO_ERROR, //0
  WORD_UNDEFINED
}WORD;

typedef enum{
  mcPositiveDirection,
  mcShortestWay,
  mcNegativeDirection,
  mcCurrentDirection
}MC_DIRECTION;
typedef enum {
  FA_DISABLED,  //0
  FA_STANDSTILL,
  FA_HOMING,
  FA_STOPPING,
  FA_DISCRETEMOTION,
  FA_CONTINUOUSMOTION,
  FA_SYNCHRONIZEDMOTION,
  FA_ERRORSTOP
}FA_STAT_NUM;


typedef union{
  BOOL FA[8];
  struct {
    BOOL FA_DISABLED;
    BOOL FA_STANDSTILL;
    BOOL FA_HOMING;
    BOOL FA_STOPPING;
    BOOL FA_DISCRETEMOTION;
    BOOL FA_CONTINUOUSMOTION;
    BOOL FA_SYNCHRONIZEDMOTION;
    BOOL FA_ERRORSTOP;
  };
}FA_STAT;


typedef struct AXIS_REF AXIS_REF;
void setStat(AXIS_REF *axis, FA_STAT_NUM stat);

struct AXIS_REF{
  FA_STAT stat;     // FA_Disabled;
  BOOL    power;    // FALSE;
  BOOL    positive; // FALSE;
  BOOL    negative; // FALSE;
  void *  DEVICE;
  void *  CurrentFB;
  void (*setStat)(AXIS_REF *axis, FA_STAT_NUM stat);
};


//立起指定stat, 倒下其他所有stat
void setStat(AXIS_REF *axis, FA_STAT_NUM stat){
  unsigned char i;
  for(i=0;i<8;i++){ (*axis).stat.FA[i] = 0; }
  (*axis).stat.FA[stat] = 1;
}

// ############################### Object Way ###############################



/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/




// ############################### function Way ###############################

/**3.1**************************************************************************
* Function Name  : MC_Power
* Description    : This Function Block controls the power stage (On or Off).
* In and Out     : @AXIS_REF Axis         //B Axis
* Input          : @BOOL Enable,          //B As long as ‘Enable’ is true, power is being enabled.
*                  @BOOL EnablePositive,  //E As long as ‘Enable’ is true, this permits motion in positive direction
*                  @BOOL EnableNegative   //E As long as ‘Enable’ is true, this permits motion in negative direction
* Output         : @BOOL *Status,         //B Effective state of the power stage
*                  @BOOL *Valid,          //E If true, a valid set of outputs is available at the FB
*                  @BOOL *Error,          //B Signals that an error has occurred within the Function Block
*                  @WORD ErrorID,         //E Error identification
* Return         : None
*******************************************************************************
* Notes:
*  The ‘Enable’ input enables the power stage in the drive and not the FB itself
*  If the MC_Power FB is called with the ‘Enable’ = true while being in ‘Disabled’, the axis state changes to ‘Standstill’.
*  It is possible to set an error variable when the Command is true for a while and the Status remains false with a Timer FB and an AND Function (with inverted Status input). It indicates that there is a hardware problem with the power stage.
*  If power fails (also during operation) it will generate a transition to the ‘ErrorStop’ state.
*  ‘EnablePositive’ and ‘EnableNegative’ are both level sensitive.
*  ‘EnablePositive’ & ‘EnableNegative’ can both be true.
*  Only 1 FB MC_Power should be issued per axis.
*******************************************************************************/
/*
void MC_Power(
  //VAR_IN_OUT
    AXIS_REF Axis,
  //VAR_OUTPUT
    BOOL *Status,          //B Effective state of the power stage
    BOOL *Valid,           //E If true, a valid set of outputs is available at the FB
    BOOL *Error,           //B Signals that an error has occurred within the Function Block
    WORD ErrorID,          //E Error identification
  //VAR_INPUT
    BOOL Enable,           //B As long as ‘Enable’ is true, power is being enabled.
    BOOL EnablePositive,   //E As long as ‘Enable’ is true, this permits motion in positive direction
    BOOL EnableNegative    //E As long as ‘Enable’ is true, this permits motion in negative direction
);
*/





//basic types

/*
struct MC_Parameter
{
  REAL CommandedPosition;
  REAL SWLimitPos;
  REAL SWLimitNeg;
  int EnableLimitPos;
  int EnableLimitNeg;
  int EnablePosLagMonitoring;
  REAL MaxPositionLag;
  REAL MaxVelocitySystem;
  REAL MaxVelocityAppl;
  REAL ActualVelocity;
  REAL CommandedVelocity;
  REAL MaxAccelerationSystem;
  REAL MaxAccelerationAppl;
  REAL MaxDecelerationSystem;
  REAL MaxDecelerationAppl;
  REAL MaxJerkSystem;
  REAL MaxJerkAppl;
}mc_parameter;

struct axisinfo
{
  int Valid;
  int Done;
  int Active;
  int Busy;
  int CommandAborted;
  int Error;
  int ErrorID;
  //int ErrorStop;
  int Status;
  //int Disabled;
  //int Stopping;
  //nt Homing;
  //int Standstill;
  //int DiscreteMotion;
  //int ContinuousMotion;
  //int SynchronizedMotion;
  int ConstantVelocity;
  int Accelerating;
  int Decelerating;
  int DirectionPositive;
  int DirectionNegative;
  int HomeAbsSwitch;
  int LimitSwitchPos;
  int LimitSwitchNeg;
  int Simulation;
  int CommunicationReady;
  int ReadyForPowerOn;
  int PowerOn;
  int IsHomed;
  int InVelocity;
  int InEndVelocity;
  int InTorque;
  int ProfileCompleted;
  int AxisErrorID;
  int AxisWarning;
  int InOperation;
  int Enabled;
  REAL Position;
  REAL Velocity;
  REAL Torque;
  REAL CoveredDistance;
  REAL Value;
  REAL RecordedPosition;
}axis;

*/


/* Private struct For Multi-Axis ---------------------------------------------*/
/*
struct multiAxisinfo
{
  int Done;
  int Busy;
  int Error;
  int ErrorID;
  int CamTableID;
  int InSync;
  int Active;
  int CommandAborted;
  int EndOfProfile;
  int InGear;
  int StartSync;
  REAL AbsolutePhaseShift;
  REAL ConveredPhaseShift;
}multiAxis;
*/



//PLC OPEN MOTION CONTROL FUNCTION


/*
//##################################################################
//3.2
//FB-Name: MC_Home
//This Function Block commands the axis to perform the «search home» sequence. The details of this sequence are manufacturer dependent and can be set by the axis’ parameters. The ‘Position’ input is used to set the absolute posi- tion when reference signal is detected. This Function Block completes at ‘Standstill’ if it was started in ‘Standstill’.
//Notes: 
//  MC_Home is a generic FB which does a system specified homing procedure which can be constructed by the StepHoming FBs as specified in Part 5 – Homing Procedures.
void MC_Home(   
  //VAR_IN_OUT
    AXIS_REF Axis,
  //VAR_OUTPUT
    BOOL Done,            //B Reference known and set sucessfully
    BOOL Busy,            //E The FB is not finished and new output values are to be expected
    BOOL Active,          //E Indicates that the FB has control on the axis
    BOOL CommandAborted,  //E ‘Command’ is aborted by another command
    BOOL Error,           //B Signals that an error has occurred within the Function Block
    WORD ErrorID,         //E Error identification
  //VAR_INPUT
    BOOL Execute,         //B Start the motion at rising edge
    REAL Position,        //B Absolute position when the reference signal is detected [u]
    MC_BufferMode BufferMode //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
  );

//##################################################################
//3.3
//FB-Name: MC_Stop
//This Function Block commands a controlled motion stop and transfers the axis to the state ‘Stopping’. It aborts any ongoing Function Block execution. While the axis is in state ‘Stopping’, no other FB can perform any motion on the same axis. After the axis has reached ‘Velocity’ zero, the ‘Done’ output is set to true immediately. The axis re- mains in the state ‘Stopping’ as long as ‘Execute’ is still true or ‘Velocity’ zero is not yet reached. As soon as ‘Done’ is SET and ‘Execute’ is false the axis goes to state ‘Standstill’.
//Notes: 
//  1. This FB is primarily intended for emergency stop functionality or exception situations
//  2. As long as ‘Execute’ is high, the axis remains in the state ‘Stopping’ and may not be executing any other motion command.
//  3. If ‘Deceleration’ = 0, the behavior of the function block is implementation specific
void MC_Stop(  BOOL Execute,           //B Start the action at rising edge
              REAL Deceleration,   //E Value of the ‘Deceleration’ [u/s^2]
              REAL Jerk            //E Value of the ‘Jerk’ [u/s^3]
            );

//##################################################################
//3.4
//FB-Name: MC_Halt
//This Function Block commands a controlled motion stop. The axis is moved to the state ‘DiscreteMotion’, until the velocity is zero. With the ‘Done’ output set, the state is transferred to ‘Standstill’.
//Notes:
// MC_Halt is used to stop the axis under normal operation conditions. In non-buffered mode it is possible to set another motion command during deceleration of the axis, which will abort the MC_Halt and will be executed immediately.
// If this command is active the next command can be issued. E.g. a driverless vehicle detects an obstacle and needs to stop. MC_Halt is issued. Before the ‘Standstill’ is reached the obstacle is removed and the motion can be continued by setting another motion command, so the vehicle does not stop.
void MC_Halt(  BOOL Execute,          //B Start the action at rising edge
               REAL Deceleration,   //E Value of the ‘Deceleration’ [u/s2]
               REAL Jerk,            //E Value of the ‘Jerk’ [u/s3]
               int BufferMode        //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
            );


//##################################################################
//3.5
//FB-Name: MC_MoveAbsolute
//This Function Block commands a controlled motion to a specified absolute position.
//Notes:
// This action completes with velocity zero if no further actions are pending
// If there is only one mathematical solution to reach the ‘CommandedPosition’ (like in linear systems), the value of the input ‘Direction’ is ignored
// For modulo axis - valid absolute position values are in the range of [0, 360[, (360 is excluded), or corresponding range. The application however may shift the ‘CommandedPosition’ of MC_MoveAbsolute into the corresponding modulo range.
// The Enum type ‘mcShortestWay’ is focused to a trajectory which will go through the shortest route. The decision which direction to go is based on the current position where the command is issued.
void MC_MoveAbsolute( BOOL Execute,          //B Start the motion at rising edge
                      int ContinuousUpdate, //E See 2.4.6 The input ‘ContinuousUpdate’
                      REAL Position,       //B Commanded ‘Position’ for the motion (in technical unit [u]) (negative or positive)
                      REAL Velocity,        //B Value of the maximum ‘Velocity’ (not necessarily reached) [u/s].
                      REAL Acceleration,    //E Value of the ‘Acceleration’ (always positive) (increasing energy of the motor) [u/s^2]
                      REAL Deceleration,    //E Value of the ‘Deceleration’ (always positive) (decreasing energy of the motor) [u/s^2]
                      REAL Jerk,            //E Value of the ‘Jerk’ [u/s^3]. (always positive)
                      int Direction,         //B Enum type (1-of-4 values: mcPositiveDirection, mcShort- estWay, mcNegativeDirection, mcCurrentDirection)
                      int BufferMode        //E Defines the chronological sequence of the FB. See 2.4.2 ‘Aborting versus Buffered modes’
                    );

//##################################################################
//3.6
//FB-Name: MC_MoveRelative
//This Function Block commands a controlled motion of a specified distance relative to the set position at the time of the execution.
//Notes: This action completes with velocity zero if no further actions are pending.
void MC_MoveRelative(  BOOL Execute,            //B Start the motion at rising edge
                      int ContinuousUpdate,  //E See 2.4.6 The input ‘ContinuousUpdate’
                      REAL Distance,         //B Relative distance for the motion (in technical unit [u])
                      REAL Velocity,         //E Value of the maximum velocity (not necessarily reached) [u/s]
                      REAL Acceleration,     //E Value of the acceleration (increasing energy of the motor) [u/s^2]
                      REAL Deceleration,     //E Value of the deceleration (decreasing energy of the motor) [u/s^2]
                      REAL Jerk,            //E Value of the Jerk [u/s^3]
                      int BufferMode          //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                    );



//##################################################################
//3.7
//FB-Name: MC_MoveAdditive
//This Function Block commands a controlled motion of a specified relative distance additional to the most recent com- manded position in the axis state ‘DiscreteMotion’. The most recent commanded position may be the result of a previ- ous MC_MoveAdditive motion which was aborted. If the FB is activated in the axis state ‘ContinuousMotion’, the specified relative distance is added to the set position at the time of the execution.
//Notes: --
void MC_MoveAdditive(  BOOL Execute,            //B Start the motion at rising edge
                      int ContinuousUpdate,  //E See 2.4.6 The input ‘ContinuousUpdate’
                      REAL Distance,         //B Relative distance for the motion (in technical unit [u])
                      REAL Velocity,         //E Value of the maximum velocity (not necessarily reached) [u/s]
                      REAL Acceleration,     //E Value of the acceleration (increasing energy of the motor) [u/s^2]
                      REAL Deceleration,     //E Value of the deceleration (decreasing energy of the motor) [u/s^2]
                      REAL Jerk,            //E Value of the Jerk [u/s^3]
                      int BufferMode          //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                    );

//##################################################################
//3.8
//FB-Name: MC_MoveSuperimposed
//This Function Block commands a controlled motion of a specified relative distance additional to an existing motion. The existing Motion is not interrupted, but is superimposed by the additional motion.
//Notes:
// If MC_MoveSuperimposed is active, then any other command in aborting mode except MC_MoveSuperimposed will abort both motion commands: both the MC_MoveSuperimposed and the underlying motion command. In any other mode, the underlying motion command is not aborted
// If MC_MoveSuperimposed is active and another MC_MoveSuperimposed is commanded, only the on-going MC_MoveSuperimposed command is aborted, and replaced by the new MC_MoveSuperimposed, but not the underlying motion command
// The FB MC_MoveSuperimposed causes a change of the velocity and, if applicable, the commanded position of an ongoing motion in all relevant states
// In the state ‘Standstill’ the FB MC_MoveSuperimposed acts like MC_MoveRelative
// The values of ‘Acceleration’, ‘Deceleration’, and ‘Jerk’ are additional values to the on-going motion, and not absolute ones. With this, the underlying FB always finishes its job in the same period of time regardless of whether a MC_MoveSuperimposed FB takes place concurrently.
// The output ‘Active’ has a different behavior as in buffered FBs.
void MC_MoveSuperimposed(  BOOL Execute,            //B Start the motion at rising edge
                          int ContinuousUpdate,  //E See 2.4.6 The input ‘ContinuousUpdate’
                          REAL Distance,         //B Additional distance that is to be superimposed (in technical unit [u])
                          REAL VelocityDiff,     //E Value of the velocity difference of the additional motion (not necessar- ily reached) [u/s]
                          REAL Acceleration,     //E Value of the acceleration (increasing energy of the motor) [u/s^2]
                          REAL Deceleration,     //E Value of the deceleration (decreasing energy of the motor) [u/s^2]
                          REAL Jerk              //E Value of the Jerk [u/s^3]
                          );

//##################################################################
//3.9
//FB-Name: MC_HaltSuperimposed
//This Function Block commands a halt to all superimposed motions of the axis. The underlying motion is not inter- rupted.
//Notes: --
void MC_HaltSuperimposed(  BOOL Execute,        //B Start the action at rising edge
                          REAL Deceleration, //E Value of the deceleration [u/s^2]
                          REAL Jerk          //E Value of the Jerk [u/s^3]
                        );

//##################################################################
//3.10
//FB-Name: MC_MoveVelocity
//This Function Block commands a never ending controlled motion at a specified velocity.
//Notes:
// To stop the motion, the FB has to be interrupted by another FB issuing a new command
// The signal ‘InVelocity’ has to be reset when the block is aborted by another block.
// Negative velocity * negative direction = positive velocity
// In combination with MC_MoveSuperimposed, the output ‘InVelocity’ is SET as long as the contribution of this FB (MC_MoveVelocity) to the set velocity is equal to the commanded velocity of this FB.
void MC_MoveVelocity(  BOOL Execute,            //B Start the motion at rising edge
                      int ContinuousUpdate,  //E See 2.4.6 The input ‘ContinuousUpdate’
                      REAL Velocity,         //B Value of the maximum velocity [u/s]. Can be a signed value.
                      REAL Acceleration,     //E Value of the acceleration (increasing energy of the motor) [u/s^2]
                      REAL Deceleration,     //E Value of the deceleration (decreasing energy of the motor) [u/s^2]
                      REAL Jerk,            //E  Value of the Jerk [u/s^3]
                      int Direction,          //E Enum type (1-of-3 values: mcPositiveDirection, mcNega- tiveDirection, and mcCurrentDirection. Note: shortest way not applicable)
                      int BufferMode          //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                    );


//##################################################################
//3.11
//FB-Name: MC_MoveContinuousAbsolute
//This Function Block commands a controlled motion to a specified absolute position ending with the specified velocity.
//Notes:
// If the commanded position is reached and no new motion command is put into the buffer, the axis continues to run with the specified ‘EndVelocity’.
// State ‘ContinuousMotion’ (meaning: it will not stop by itself).
// This FB can be replaced by the combination of MC_MoveAbsolute and MC_MoveVelocity if BufferMode is implemented on those FBs
void MC_MoveContinuousAbsolute(  BOOL Execute,            //B Start the motion at rising edge
                                int ContinuousUpdate,  //E See 2.4.6 The input ‘ContinuousUpdate’
                                REAL Position,         //B Commanded position for the motion (in technical unit [u]) (negative or positive)
                                REAL EndVelocity,      //B Value of the end velocity [u/s]. Signed value
                                REAL Velocity,          //B Value of the maximum velocity [u/s]
                                REAL Acceleration,     //E Value of the acceleration [u/s^2]
                                REAL Deceleration,      //E Value of the deceleration [u/s^2]
                                REAL Jerk,            //E Value of the Jerk [u/s^3]
                                int Direction,          //E Enum type (1-of-4 values: mcPositiveDirection, mcNegativeDirection, mcCurrentDirection and mcShort- estWay
                                int BufferMode          //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                              );

//##################################################################
//3.12
//FB-Name: MC_MoveContinuousRelative
//This Function Block commands a controlled motion of a specified relative distance ending with the specified velocity.
//Notes:
// If the commanded position is reached and no new motion command is put into the buffer, the axis continues to run with the specified ‘EndVelocity’.
// State ‘ContinuousMotion’ (meaning: it will not stop by itself).
// This FB is specified here for systems without the support for the ‘BufferMode’.
// This FB can be replaced by the combination of MC_MoveAbsolute and MC_MoveVelocity if BufferMode is implemented on those FBs
void MC_MoveContinuousRelative(  BOOL Execute,          //B Start the motion at rising edge
                                int ContinuousUpdate, //E See 2.4.6 The input ‘ContinuousUpdate’
                                REAL Distance,       //B Relative distance for the motion [u]
                                REAL EndVelocity,    //B Value of the end velocity [u/s]. Signed value
                                REAL Velocity,       //B Value of the maximum velocity [u/s]
                                REAL Acceleration,   //E Value of the acceleration [u/s2]
                                REAL Deceleration,    //E Value of the deceleration [u/s2]
                                REAL Jerk,            //E Value of the Jerk [u/s3]
                                int BufferMode        //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                              );

//##################################################################
//3.13
//FB-Name: MC_TorqueControl
//This Function Block continuously exerts a torque or force of the specified magnitude. This magnitude is ap- proached using a defined ramp (‘TorqueRamp’), and the Function Block sets the ‘InTorque’ output if the com- manded torque level is reached. This function block is applicable for force and torque. When there is no external load, force is applicable. Positive torque is in the positive direction of velocity.
//Notes:
//  1. The movement is limited by velocity, acceleration / deceleration, and jerk, or by the value of the torque, de- pending on the mechanical circumstances.
//  2. Specific additional tests are outside this FB. For instance, checking on the traveled distance could be done via tracing the actual positions during the action.
//  3. ‘Velocity’ is a limit input and is always a positive value. The direction is dependent on the torque and load.
//  4. The axis ceases to be in ‘Torque’ control mode when any motion control (not administrative) Function Block is accepted on the same axis.
void MC_TorqueControl(BOOL Execute,            //B Starts the motion on a rising edge
                      int ContinuousUpdate,  //E See 2.4.6 The input ‘ContinuousUpdate’
                      REAL Torque,           //B Value of the torque (Torque or force in technical unit [u])
                      REAL TorqueRamp,        //E The maximum time derivative of the set value of the torque or force (in technical unit per sec. [u/s])
                      REAL Velocity,         //E Absolute value of the maximum velocity
                      REAL Acceleration,     //E Value of the maximum acceleration (acceleration is appli- cable with same sign of torque and velocity)
                      REAL Deceleration,      //E Value of the maximum deceleration (deceleration is appli- cable with opposite signs of torque and velocity)
                      REAL Jerk,              //E Value of the maximum jerk
                      int Direction,          //E Enum type (1 of 2 values: mcPositiveDirection, mcNega- tiveDirection or mcCurrentDirection). Specifies the direc- tion of the torque. (Note: Torque input can be signed value).
                      int BufferMode          //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                      );

//##################################################################
//3.14
//FB-Name: MC_PositionProfile
//This Function Block commands a time-position locked motion profile
//Notes:
// MC_TP_REF is a supplier specific data type. An example for this datatype is given below:
//   The content of a Time/Position pair may be expressed in DeltaTime/Pos, where Delta could be the differ- ence in time between two successive points.
//   TYPE
//      MC_TP : STRUCT
//        DeltaTime : TIME;
//        Position : REAL;
//      END_STRUCT;
//    END_TYPE
//   TYPE
//      MC_TP_REF : STRUCT
//        NumberOfPairs : WORD;
//        IsAbsolute : BOOL;
//        MC_TP_Array : ARRAY [1..N] OF MC_TP;
//      END_STRUCT; 
//    END_TYPE
// This functionality does not mean it runs one profile over and over again: it can switch between different pro- files
// Alternatively to this FB, the FB MC_CamIn coupled to a virtual master can be used
void MC_PositionProfile(BOOL Execute,            //B Start the motion at rising edge
                        int ContinuousUpdate,  //E See 2.4.6 The input ‘ContinuousUpdate’
                        REAL TimeScale,         //E Overall time scaling factor of the profile
                        REAL PositionScale,    //E Overall Position scaling factor
                        REAL Offset,            //E Overall offset for profile [u]
                        int BufferMode          //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                        );

//##################################################################
//3.15
//FB-Name: MC_VelocityProfile
//This Function Block commands a time-velocity locked motion profile. The velocity in the final element in the profile should be maintained. The state remains ‘ContinuousMotion’.
//Notes:
// MC_TV_REF is a supplier specific datatype. An example for this datatype is given here below:
// The content of Time/Velocity pair may be expressed in DeltaTime/Velocity, where Delta could be the dif- ference in time between two successive points. Velocity can be a signed value.
// TYPE
//    MC_TV : STRUCT
//      DeltaTime : TIME;
//      Velocity : REAL;
//    END_STRUCT;
//  END_TYPE
// TYPE
//    MC_TV_REF : STRUCT
//      NumberOfPairs : WORD;
//      MC_TV_Array : ARRAY [1..N] of MC_TV; 
//    END_STRUCT;
//  END_TYPE
// This functionality does not mean it runs one profile over and over again: it can switch between different profiles
// Alternatively to this FB, the CAM FB coupled to a virtual master can be used
void MC_VelocityProfile(BOOL Execute,            //B Start the motion at rising edge
                        int ContinuousUpdate,  //E See 2.4.6 The input ‘ContinuousUpdate’
                        REAL TimeScale,        //E Overall time scaling factor of the profile
                        REAL VelocityScale,    //E Overall velocity scaling factor of the profile
                        REAL Offset,            //E Overall offset for profile [u/s]
                        int BufferMode          //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                        );

//##################################################################
//3.16
//FB-Name: MC_AccelerationProfile
//This Function Block commands a time-acceleration locked motion profile. After finalizing the acceleration pro- file, the acceleration goes to 0 (and typically the final velocity is maintained). It stays in the state ‘ContinuousMo- tion’.
//Notes:
// MC_TA_REF is a supplier specific datatype. An example for this datatype is given here below:
// The content of Time/Acceleration pair may be expressed in DeltaTime/Acceleration, where Delta could be the difference in time between two successive points.
// TYPE
//    MC_TA : STRUCT
//      DeltaTime : TIME;
//      Acceleration : REAL; 
//    END_STRUCT;
//  END_TYPE
// TYPE
//    MC_TA_REF : STRUCT
//      NumberOfPairs : WORD;
//      MC_TA_Array : ARRAY [1..N] of MC_TA;
//    END_STRUCT; 
//  END_TYPE
// alternatively to this FB, the CAM FB coupled to a virtual master can be used
void MC_AccelerationProfile(int TimeAcceleration,     //B Reference to Time / Acceleration. Description – see note below
                            BOOL Execute,              //B Start the motion at rising edge
                            int ContinuousUpdate,    //E See 2.4.6 The input ‘ContinuousUpdate’
                            REAL TimeScale,           //E Overall time scaling factor of the profile
                            REAL AccelerationScale,  //E Scale factor for acceleration amplitude
                            REAL Offset,              //E Overall offset for profile [u/s^2]
                            int BufferMode            //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                            );

//##################################################################
//3.17
//FB-Name: MC_SetPosition
//This Function Block shifts the coordinate system of an axis by manipulating both the set-point position as well as the actual position of an axis with the same value without any movement caused. (Re-calibration with same following er- ror). This can be used for instance for a reference situation. This Function Block can also be used during motion with- out changing the commanded position, which is now positioned in the shifted coordinate system.
//Notes: ‘Relative’ means that ‘Position’ is added to the actual position value of the axis at the time of execution. This results in a recalibration by a specified distance. ‘Absolute’ means that the actual position value of the axis is set to the value specified in the ‘Position’ parameter.
void MC_SetPosition(BOOL Execute,        //B Start setting position in axis
                    REAL Position,     //B Position unit [u] (Means ‘Distance’ if ‘Relative’= true)
                    int Relative,        //E ‘Relative’ distance if true, ‘Absolute’ position if false (= Default)
                    int ExecutionMode    //E ENUM. Defines the chronological sequence of the FB. 
                                        //        mcImmediately - the functionality is immediately valid and may influence the on-going motion but not the state (note: is the default behaviour) 
                                        //        mcQueued - Same functionality as buffer mode ‘Buffered’
                    );

//##################################################################
//3.18
//FB-Name: MC_SetOverride
//This Function Block sets the values of override for the whole axis, and all functions that are working on that axis. The override parameters contribute as a factor to the calculation of the commanded velocity, acceleration and jerk of the motion.
//Notes:
//1. The Input AccFactor acts on positive and negative acceleration (deceleration).
//2. This Function Block sets the factor. The override factor is valid until a new override is set.
//3. The default values of the override factor are 1.0.
//4. The value of the overrides can be between 0.0 and 1.0. The behavior of values > 1.0 is vendor specific. Values < 0.0 are not allowed. The value 0.0 is not allowed for ‘AccFactor’ and ‘JerkFactor’.
//5. The value 0.0 set to the ‘VelFactor’ stops the axis without bringing it to the state ‘Standstill’.
//6. Override does not act on slave axes. (Axes in the state synchronized motion).
//7. The Function Block does not influence the state diagram of the axis.
//8. ‘VelFactor’ can be changed at any time and acts directly on the ongoing motion.
//9. If in ‘Discrete’ motion, reducing the ‘AccFactor’ and/or ‘JerkFactor’ can lead to a position overshoot – a pos- sible cause of damage
//10. Activating this Function Block on an axis that is under control of MC_PositionProfile, MC_VelocityProfile, or MC_AccelerationProfile, is vendor specific.

void MC_SetOverride(BOOL Enable,        //B If SET, it writes the value of the override factor continuously. If RESET it should keep the last value.
                    REAL VelFactor,     //B New override factor for the velocity
                    REAL AccFactor,    //E New override factor for the acceleration/deceleration
                    int JerkFactor      //E New override factor for the jerk
                    );

//##################################################################
//3.19.1
//FB-Name: MC_ReadParameter
//This Function Block returns the value of a vendor specific parameter. The returned Value has to be converted to REAL if necessary. If not possible, the vendor has to supply a vendor specific FB to read the parameter.
//Note: The parameters are defined in the table below.
void MC_ReadParameter(BOOL Enable,          //B Get the value of the parameter continuously while enabled
                      int ParameterNumber    //E Number of the parameter. One can also use symbolic parameter names which are declared as VAR CONST.
                      );

//##################################################################
//3.19.2
//FB-Name: MC_ReadBOOLParameter
//This Function Block returns the value of a vendor specific parameter with datatype BOOL.
//Note: The parameters are defined in the table below


//(Note: PN is Parameter Number see FB MC_ReadParameter / MC_WriteParameter and BOOLean versions)
//PN   Name                  Datatype B/E R/W   Comments
// 1  CommandedPosition       REAL   B   R     Commanded position 
// 2  SWLimitPos               REAL   E   R/W   Positive Software limit switch position 
// 3  SWLimitNeg               REAL   E   R/W   Negative Software limit switch position 
// 4  EnableLimitPos           BOOL   E   R/W   Enable positive software limit switch 
// 5  EnableLimitNeg           BOOL   E   R/W   Enable negative software limit switch 
// 6  EnablePosLagMonitoring   BOOL   E   R/W   Enable monitoring of position lag 
// 7  MaxPositionLag           REAL   E   R/W   Maximal position lag 
// 8  MaxVelocitySystem       REAL   E   R     Maximal allowed velocity of the axis in the mo- tion system 
// 9  MaxVelocityAppl         REAL   B   R/W   Maximal allowed velocity of the axis in the appli- cation 
//10  ActualVelocity           REAL   B   R     Actual velocity 
//11  CommandedVelocity       REAL   B   R     Commanded velocity 
//12  MaxAccelerationSystem   REAL   E   R     Maximal allowed acceleration of the axis in the motion system 
//13  MaxAccelerationAppl     REAL   E   R/W   Maximal allowed acceleration of the axis in the application 
//14  MaxDecelerationSystem   REAL   E   R     Maximal allowed deceleration of the axis in the motion system 
//15  MaxDecelerationAppl     REAL   E   R/W   Maximal allowed deceleration of the axis in the application 
//16  MaxJerkSystem           REAL   E   R     Maximum allowed jerk of the axis in the motion system 
//17  MaxJerkAppl             REAL   E   R/W   Maximum allowed jerk of the axis in the applica- tion 

void MC_ReadBOOLParameter(BOOL Enable,          //B Get the value of the parameter continuously while enabled
                          int ParameterNumber    //B Number of the parameter. One can also use symbolic parameter names which are declared as VAR CONST.
                          );

//##################################################################
//3.20.1
//FB-Name: MC_WriteParameter
//This Function Block modifies the value of a vendor specific parameter.
//Notes: The parameters are defined in the table above (under MC_ReadParameter, writing allowed)
void MC_WriteParameter(  BOOL Execute,          //B Write the value of the parameter at rising edge
                        int ParameterNumber,  //B Number of the parameter (correspondence between number and parameter is specified in the table above)
                        REAL Value,          //B New value of the specified parameter
                        int ExecutionMode      //E Defines the chronological sequence of the FB.
                                              //        mcImmediately - the functionality is immediately valid and may influence the on-going motion but not the state (note: is the de- fault behaviour)
                                              //        mcQueued - Same functionality as buffer mode ‘Buffered’
                        );

//##################################################################
//3.20.2
//FB-Name: MC_WriteBOOLParameter
//This Function Block modifies the value of a vendor specific parameter of type BOOL.
//Notes: The parameters are defined in the table above (under MC_ReadParameter, writing allowed)
void MC_WriteBOOLParameter(  BOOL Execute,          //B Write the value of the parameter at rising edge
                            int ParameterNumber,  //B Number of the parameter (correspondence between number and parameter is specified in the table above)
                            int Value,            //B New value of the specified parameter
                            int ExecutionMode      //E Defines the chronological sequence of the FB.
                                                  //          mcImmediately - the functionality is immediately valid and may influence the on-going motion but not the state (note: is the de- fault behaviour)
                                                  //          mcQueued - Same functionality as buffer mode ‘Buffered’.
                            );

//##################################################################
//3.21
//FB-Name: MC_ReadDigitalInput
//This Function Block gives access to the value of the input, referenced by the datatype MC_INPUT_REF. It provides the value of the referenced input (BOOL)
//Note: It is not guaranteed that the digital signal will be seen by the FB: a short pulse on the digital input could be over before the next Function Block cycle occurs.
void MC_ReadDigitalInput(  BOOL Enable,      //B Get the value of the selected input signal continuously while enabled
                          int InputNumber    //E Selects the input. Can be part of MC_INPUT_REF, if only one single input is referenced.
                          );

//##################################################################
//3.22
//FB-Name: MC_ReadDigitalOutput
//This Function Block provides access to the value of a digital output, referenced by the datatype MC_OUTPUT_REF. It provides the value of the referenced output (BOOL).
//Note: It is not guaranteed that the digital signal will be seen by the FB: a short pulse on the digital output could be over before the next Function Block cycle occurs.
void MC_ReadDigitalOutput(BOOL Enable,        //B Get the value of the selected output signal continuously while enabled
                          int OutputNumber    //E Selects the output. Can be part of MC_OUTPUT_REF, if only one single output is referenced.
                          );

//##################################################################
//3.23
//FB-Name: MC_WriteDigitalOutput
//This Function Block writes a value to the output referenced by the argument ‘Output’ once (with rising edge of Exe- cute).
//Notes: --
void MC_WriteDigitalOutput(  BOOL Enable,        //B Write the value of the selected output
                            int OutputNumber,    //E Selects the output. Can be part of MC_OUTPUT_REF, if only one single input is referenced.
                            int Value,          //B The value of the selected output
                            int ExecutionMode    //E Defines the chronological sequence of the FB.
                                                //          mcImmediately - the functionality is immediately valid and may influence the on-going motion but not the state (note: is the default behaviour)
                                                //          mcQueued - Same functionality as buffer mode ‘Buffered’
                            );

//##################################################################
//3.24
//FB-Name: MC_ReadActualPosition
//This Function Block returns the actual position.
void MC_ReadActualPosition(int Enable);    //B Get the value of the parameter continuously while enabled

//##################################################################
//3.25
//FB-Name: MC_ReadActualVelocity
//This Function Block returns the value of the actual velocity as long as ‘Enable’ is set. ‘Valid’ is true when the data- output ‘Velocity’ is valid. If ‘Enable’ is Reset, the data loses its validity, and all outputs are reset, no matter if new data is available.
//Notes: The output ‘Velocity’ can be a signed value
void MC_ReadActualVelocity(int Enable);    //B Get the value of the parameter continuously while enabled

//##################################################################
//3.26
//FB-Name: MC_ReadActualTorque
//This Function Block returns the value of the actual torque or force as long as ‘Enable’ is set. ‘Valid’ is true when the data-output ‘Torque’ is valid. If ‘Enable’ is Reset, the data loses its validity, and ‘Valid’ is also reset, no matter if new data is available.
//Notes: The output ‘Torque’ can be a signed value
void MC_ReadActualTorque(int Enable);    //B  Get the value of the parameter continuously while enabled

//##################################################################
//3.27
//FB-Name: MC_ReadStatus
//This Function Block returns in detail the status of the state diagram of the selected axis.
//Notes: --
void MC_ReadStatus(int Enable);        //B Get the value of the parameter continuously while enabled

//##################################################################
//3.28
//FB-Name: MC_ReadMotionState
//This Function Block returns in detail the status of the axis with respect to the motion currently in progress.
//Notes: --
void MC_ReadMotionState(BOOL Enable,    //B Get the value of the parameter continuously while enabled
                        int Source    //E Defines the source of the relevant data: mcCommandedValue; mcSetValue, mcActualValue.
                        );

//##################################################################
//3.29
//FB-Name: MC_ReadAxisInfo
//This Function Block reads information concerning an axis, like modes, inputs directly related to the axis, and certain status information.
//Notes: --
void MC_ReadAxisInfo(int Enable);      //B Get the axis information constantly while enabled

//##################################################################
//3.30
//FB-Name: 
//This Function Block presents general axis errors not relating to the Function Blocks. (for instance axis errors, drive errors, communication errors)
//Notes: --
void MC_ReadAxisError(int Enable);    //B Get the value of the parameter continuously while enabled

//##################################################################
//3.31
//FB-Name: MC_Reset
//This Function Block makes the transition from the state ‘ErrorStop’ to ‘Standstill’ or ‘Disabled’ by resetting all inter- nal axis-related errors – it does not affect the output of the FB instances.
//Note: the application of MC_Reset in other states then the state ‘ErrorStop’ is vendor specific
void MC_Reset(int Enable);      //B Resets all internal axis-related errors

//##################################################################
//3.32
//FB-Name: MC_DigitalCamSwitch
//This Function Block is the analogy to switches on a motor shaft: it commands a group of discrete output bits to switch in analogy to a set of mechanical cam controlled switches connected to an axis. Forward and backward movements are allowed.
//Notes:
// MC_CAMSWITCH_REFis a vendor specific reference to the pattern data.
// MC_OUTPUT_REF is a vendor specific structure linked to the (physical) outputs
// MC_TRACK_REF is vendor specific structure containing the track properties, e.g. the compensation per track (A track is a set of switches related to one output). It can contain the reference to the output also.
// This functionality is sometimes called PLS – Phase or Position or Programmable Limit Switch
void MC_DigitalCamSwitch(  BOOL Enable,          //B Enables the ‘Switches’ outputs
                          REAL EnableMask,    //E 32 bits of BOOL. Enables the different tracks. Least sig- nificant data is related to the lowest TrackNumber. With data SET (to ‘1’ resp. true) the related TrackNumber is enabled.
                          int ValueSource      //E Defines the source for axis values (e.g. positions): 
                                              //          mcSetValue - Synchronization on set value 
                                              //          mcActualValue - Synchronization on actual value
                          );

//##################################################################
//3.33
//FB-Name: MC_TouchProbe
//This Function Block is used to record an axis position at a trigger event
//Notes:
//1. Intended for single shot operation, that is the first event after the rising edge at ‘Execute’ is valid for recording only. Possible following events are ignored
//2. One Function Block instance should represent exactly one probing command
//3. In case of multiple instances on the same probe and axis, the elements of MC_TRIGGER_REF should be ex- tended with TouchProbeID - Identification of a unique probing command – this can be linked to MC_AbortTrigger
void MC_TouchProbe(  BOOL Execute,            //B Starts touch probe recording at rising edge
                    int WindowOnly,          //E If SET, only use the window (defined hereunder) to accept trigger events
                    REAL FirstPosition,    //E Start position from where (positive direction) trigger events are accepted (in technical units [u]). Value included in win- dow.
                    REAL LastPosition      //E Stop position of the window (in technical units [u]). Value included in window.
                    );

//##################################################################
//3.34
//FB-Name: MC_AbortTrigger
//This Function Block is used to abort function blocks which are connected to trigger events (e.g. MC_TouchProbe)
//Notes: --
void MC_AbortTrigger(BOOL Execute);    //B Aborts trigger event at rising edge





//##################################################################
//####### PLC OPEN MOTION CONTROL FUNCTION For Multi-Axis ##########
//##################################################################


//##################################################################
//4.2
//FB-Name: MC_CamTableSelect
//This Function Block selects the CAM tables by setting the connections to the relevant tables
//Notes:
// A virtual axis can be used as master axis
// MC_CAM_REF is a supplier specific data type
// MC_CAM_ID is a supplier specific data type
// MC_CamTableSelect makes data available. This can include:
//  1. Starting point of a download of a profile
//  2. Start to generate a CAM profile
// When the Done output is SET, the CamTableID is valid and ready for use in a MC_CamIn FB.
// Possible parameters within the structure CAM_TABLE_REF are:
//    o E MasterPositions REAL, List of expressions of the MasterValues for the ‘CamTable’ 
//    o E SlavePositions REAL, List of expressions of the SlaveValues for the ‘CamTable’
void MC_CamTableSelect(  BOOL Execute,            //B Selection at rising edge
                        int Periodic,            //E 1 = periodic, 0 = non periodic (single-shot)
                        int MasterAbsolute,      //B 1 = absolute; 0 = relative coordinates
                        int SlaveAbsolute,      //E 1 = absolute; 0 = relative coordinates
                        int ExecutionMode        //E Defines the chronological sequence of the FB.
                                                //          mcImmediately - the functionality is immediately valid and may influence the on-going motion but not the state (note: is the default behaviour)
                                                //          mcQueued - Same functionality as buffer mode ‘Buffered’.
                        );

//##################################################################
//4.3
//FB-Name: MC_CamIn
//This Function Block engages the CAM
//Notes: --
void MC_CamIn(BOOL Execute,                    //B Start at rising edge
              int ContiunousUpdate,            //E See 2.4.6 The input ‘ContinuousUpdate’
              REAL MasterOffset,              //E Offset of the master shaft to cam.
              REAL SlaveOffset,              //E Offset of slave table.
              REAL MasterScaling,            //E Factor for the master profile (default = 1.0). From the slave point of view the master overall profile is multiplied by this factor
              REAL SlaveScaling,              //E Factor for the slave profile (default = 1.0). The overall slave profile is multiplied by this factor.
              REAL MasterStartDistance,      //E The master distance for the slave to start to synchronize to the master.
              REAL MasterSyncPosition,        //E The position of the master in the CAM profile where the slave is in-sync with the master. (if the ‘MasterSyncPosi- tion’ does not exist, at the first point of the CAM profile the master and slave are synchronized.) Note: the inputs acceleration, decelerations and jerk are not added here
              int StartMode,                  //E Start mode: mcAbsolute, mcRelative, or mcRampIn
              int MasterValueSource,          //E Defines the source for synchronization:
                                              //          mcSetValue - Synchronization on master set value 
                                              //          mcActualValue - Synchronization on master actual value
              int CamTableId,                  //E Identifier of CAM Table to be used, linked to output of MC_CamTableSelect
              int BufferMode                  //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
              );

//##################################################################
//4.4
//FB-Name: MC_CamOut
//This Function Block disengages the Slave axis from the Master axis immediately
//Notes:
// It is assumed that this command is followed by another command, for instance MC_Stop, MC_GearIn, or any other command. If there is no new command, the default condition should be: maintain last velocity.
// After issuing the FB there is no FB active on the slave axis till the next FB is issued (what can result in prob- lems because no motion command is controlling the axis). Alternatively one can read the actual velocity via MC_ReadActualVelocity and issue MC_MoveVelocity on the slave axis with the actual velocity as input. The FB is here because of compatibility reasons
void MC_CamOut(BOOL Execute);      //B Start to disengage the slave from the master

//##################################################################
//4.5
//FB-Name: MC_GearIn
//This Function Block commands a ratio between the VELOCITY of the slave and master axis.
//Notes:
//1. The slave ramps up to the ratio of the master velocity and locks in when this is reached. Any lost distance during synchronization is not caught up.
//2. The gearing ratio can be changed while MC_GearIn is running, using a consecutive MC_GearIn command without the necessity to MC_GearOut first
//3. After being ‘InGear’, a position locking or just a velocity locking is system specific.
void MC_GearIn(  BOOL Execute,              //B Start the gearing process at the rising edge
                int ContinuousUpdate,      //E See 2.4.6 The input ‘ContinuousUpdate
                int RatioNumberator,      //B Gear ratio Numerator
                int RatioDenominator,      //B Gear ratio Denominator
                int MasterValueSource,    //E Defines the source for synchronization:
                                          //          mcSetValue - Synchronization on master set value 
                                          //          mcActualValue - Synchronization on master actual value
                REAL Acceleration,        //E Acceleration for gearing in
                REAL Deceleration,        //E Deceleration for gearing in
                REAL Jerk,                //E Jerk of Gearing
                int BufferMode            //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                );

//##################################################################
//4.6
//FB-Name: MC_GearOut
//This Function Block disengages the Slave axis from the Master axis
//Notes:
// It is assumed that this command is followed by another command, for instance MC_Stop, MC_GearIn, or any other command. If there is no new command, the default condition should be: maintain last velocity.
// After issuing the FB there is no FB active on the slave axis till the next FB is issued (what can result in prob- lems because no motion command is controlling the axis). Alternatively one can read the actual velocity via MC_ReadActualVelocity and issue MC_MoveVelocity on the slave axis with the actual velocity as input. The FB is here because of compatibility reasons
void MC_GearOut(BOOL Execute);      //B Start disengaging process at the rising edge

//##################################################################
//4.7
//FB-Name: MC_GearInPos
//This Function Block commands a gear ratio between the position of the slave and master axes from the synchroniza- tion point onwards.
//Notes:
//1. If ‘MasterStartDistance’ is implemented, any previous motion is continued until master crosses ‘MasterSyncPosition’ – ‘MasterStartDistance’ in the correct direction (according to the sign of ‘MasterStartDis- tance’). At that point in time the output ‘StartSync’ is set. When a ‘Stop’ command is executed on the ‘Slave’ axis before the synchronization has happened, it inhibits the synchronization and the function block issues ‘CommandAborted’
//2. If the ‘MasterStartDistance’ is not specified, the system itself could calculate the set point for ‘StartSync’ based on the other relevant inputs.
//3. The difference between the ‘SyncModes’ ‘CatchUp’ and ‘SlowDown’ is in the energy needed to synchro- nize. ‘SlowDown’ costs the lowest energy vs. ‘CatchUp’.
void MC_GearInPos(BOOL Execute,                  //B Start the gearing process at the rising edge
                  int RatioNumberator,          //B Gear ratio Numerator
                  int RatioDenominator,          //B Gear ratio Denominator
                  int MasterValueSource,        //E Defines the source for synchronization:
                                                //          mcSetValue - Synchronization on master set value 
                                                //          mcActualValue - Synchronization on master actual value
                  REAL MasterSyncPosition,      //B The position of the master in the CAM profile where the slave is in-sync with the master. (if the ‘MasterSyncPosi- tion’ does not exist, at the first point of the CAM profile the master and slave are synchronized.) Note: the inputs acceleration, decelerations and jerk are not added here
                  REAL SlaveSyncPosition,      //B Slave Position at which the axes are running in sync
                  int SyncMode,                  //E Defines the way to synchronize (like ‘mcShortest’; ‘mcCatchUp’; ‘mcSlowDown’). Vendor specific
                  REAL MasterStartDistance,    //E Master Distance for gear in procedure (when the Slave axis is started to get into synchronization)
                  REAL Velocity,                //E Maximum Velocity during the time difference ‘StartSync’ and ‘InSync’
                  REAL Acceleration,            //E Maximum Acceleration during the time difference ‘Start- Sync’ and ‘InSync’
                  REAL Deceleration,            //E Maximum Deceleration during the time difference ‘Start- Sync’ and ‘InSync’
                  REAL Jerk,                    //E Maximum Jerk during the time difference ‘StartSync’ and ‘InSync’
                  int BufferMode                //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                  );

//##################################################################
//4.8
//FB-Name: MC_PhasingAbsolute
//This Function Block creates an absolute phase shift in the master position of a slave axis. The master position is shifted in relation to the REAL physical position. This is analogous to opening a coupling on the master shaft for a moment, and is used to delay or advance an axis to its master. The phase shift is seen from the slave. The master does not know that there is a phase shift experienced by the slave (MasterPos as seen from the SlaveAxis = Physi- calMasterPos + PhaseShiftValueSlaveAxis, the phase shift value has the character of a position offset) The phase shift remains until another ‘Phasing’ command changes it again.
//Note:
// ‘Phase’, ‘Velocity’, ‘Acceleration’, ‘Deceleration’ and ‘Jerk’ of a phase shift are controlled by the FB.
// For comparison: MC_MoveSuperimposed could also be used to act on the slave axis. MC_Phasing acts on the master side, as seen from the slave
void MC_PhasingAbsolute(BOOL Execute,          //B Start the phasing process at the rising edge
                        REAL PhaseShift,      //B Absolut phase difference in master position of the slave axis [u]
                        REAL Velocity,        //E Maximum Velocity to reach phase difference [u/s]
                        REAL Acceleration,    //E Maximum Acceleration to reach phase difference [u/s2]
                        REAL Deceleration,    //E Maximum Deceleration to reach phase difference [u/s2]
                        REAL Jerk,            //E Maximum Jerk to reach phase difference [u/s3]
                        int BufferMode        //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                        );

//##################################################################
//4.9
//FB-Name: MC_PhasingRelative
//This Function Block creates a phase shift in the master position of a slave axis relative to the existing phase shift. The master position is shifted in relation to the REAL physical position. This is analogous to opening a coupling on the master shaft for a moment, and is used to delay or advance an axis to its master. The phase shift is seen from the slave. The master does not know that there is a phase shift experienced by the slave. (MasterPos as seen from SlaveAxis = PhysicalMasterPos + PhaseShiftValueSlaveAxis, the phase shift value has the character of a position offset) The phase shift remains until another ‘Phasing’ command changes it again. Relative phase shifts are added to each other for the applicable phase shift
//Note:
// ‘Phase’, ‘Velocity’, ‘Acceleration’, ‘Deceleration’ and ‘Jerk’ of a phase shift are controlled by the FB.
// For comparison: MC_MoveSuperimposed could also be used to act on the slave axis. MC_Phasing acts on the master side, as seen from the slave
void MC_PhasingRelative(BOOL Execute,          //E Start the phasing process at the rising edge
                        REAL PhaseShift,      //E Additional phase difference in master position of the slave axis [u]
                        REAL Velocity,        //E Maximum Velocity to reach phase difference [u/s]
                        REAL Acceleration,    //E Maximum Acceleration to reach phase difference [u/s2]
                        REAL Deceleration,    //E Maximum Deceleration to reach phase difference [u/s2]
                        REAL Jerk,            //E Maximum Jerk to reach phase difference [u/s3]
                        int BufferMode        //E Defines the chronological sequence of the FB. See 2.4.2 Aborting versus Buffered modes
                        );

//##################################################################
//4.10
//FB-Name: MC_CombineAxes
//This Function Block combines the motion of 2 axes into a third axis with selectable combination method. Basically it is a calculation of a new position setpoint based on the 2 position setpoints of the input axes. This FB is reflected in the state diagram like a synchronized motion type. As application example one can work with a separate profile synchronized to an object on a moving belt, or a rotating knife with flexible covered distance to be cut.
//Note: To stop the motion, the FB has to be interrupted by another FB issuing a new command
void MC_CombineAxes(BOOL Execute,                  //B Start the combination process at the rising edge
                    int ContinuousUpdate,          //E See 2.4.6 The input ‘ContinuousUpdate’
                    int CombineMode,              //E Defines the type of combination applied to AxisOut : mcAddAxes : Addition of the 2 input axes positions mcSubAxes : Substraction of the 2 input axes positions
                    int GearRatioNumeratorM1,      //E Numerator for the gear factor for master axis 1 towards the slave
                    int GearRatioDenominatorM1,    //E Corresponding denominator for master axis 1
                    int GearRatioNumeratorM2,      //E Numerator for the gear factor for master axis 2 towards the slave
                    int GearRatioDenominatorM2,    //E Corresponding denominator for master axis 2
                    int mcMasterValueSourceM1,    //E Defines the source for synchronization for master axis 1: mcSetValue - Synchronization on master set value mcActualValue - Synchronization on master actual value
                    int mcMasterValueSourceM2,    //E Defines the source for synchronization for master axis 2: mcSetValue - Synchronization on master set value mcActualValue - Synchronization on master actual value
                    int BufferMode                //E Defines the behavior of the axis: modes are ‘Aborting’, ‘Buffered’, ‘Blending’
                    );



//defined by myself
void Turn(void);
void unTurn(void);
void transfer(REAL num);
void returnPeriod(void);
void initial(void);
void ErrorIDTable(int number);
void ReadParameterNumberTable(int number);
void ReadBOOLParameterNumberTable(int number);
void WriteRealParameterTable(int number,REAL realvalue);
void WriteBOOLParameterTable(int number,int BOOLvalue);
void AxisError(void);
void setMultiAxisinfo(void);



*/



#endif //__PLCOPEN_MC_H__