/*!*********************************************************************************************************************
@file user_app1.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1RunActiveState(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                          /*!< @brief Global state flags */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */
extern volatile u32 G_u32ApplicationFlags;                /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_<type>" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_pfStateMachine;               /*!< @brief The state machine function pointer */
static struct vehicle car;
static bool Start = FALSE;
static bool Pause = FALSE;
//static u32 UserApp1_u32Timeout;                           /*!< @brief Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserApp1Initialize(void)

@brief
Initializes the State Machine and its variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- NONE

*/
void UserApp1Initialize(void)
{
  //1/10 ms for LCD update
  LcdCommand(LCD_CLEAR_CMD);
  u8 u8message1[] = "Press Buttons 1 or 2";
  u8 u8message2[] = "L     R     P";
  car.u8Character = u8message1;
  car.u32PositionA = LINE1_START_ADDR;
  LcdMessage(car.u32PositionA, car.u8Character);
  LcdMessage(LINE2_START_ADDR + 7, u8message2);
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_pfStateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_pfStateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserApp1RunActiveState(void)

@brief Selects and runs one iteration of the current state in the state machine.

All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
- State machine function pointer points at current state

Promises:
- Calls the function to pointed by the state machine function pointer

*/
void UserApp1RunActiveState(void)
{
  UserApp1_pfStateMachine();

} /* end UserApp1RunActiveState */


/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
/*-------------------------------------------------------------------------------------------------------------------*/
/* What does this state do? */
static void UserApp1SM_Idle(void)
{     
  GameState();
} /* end UserApp1SM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */

void ButtonCheck(void){
  
  static u32 u32delayCount = 500;
  static bool Button1Pressed = FALSE;
  static bool Button2Pressed = FALSE;

  if(WasButtonPressed(BUTTON1)  && !Pause){
    ButtonAcknowledge(BUTTON1);
    
    Button1Pressed = TRUE;
    if(Button2Pressed == TRUE){
      LcdClearChars(car.u32PositionA, 2);
      Button2Pressed = FALSE;
    }
    
    for(u32 i = 0; i < u32delayCount; i++){
      
    }
    
    LedOff(BLUE);
    LedOn(PURPLE);
    car.u32PositionA  = car.u32PositionA - 1;
    u8 u8newCar[] = "0 ";
    car.u8Character = u8newCar;
    LcdMessage(car.u32PositionA, car.u8Character);
  }
  
  else if (WasButtonPressed(BUTTON2)  && !Pause){
    ButtonAcknowledge(BUTTON2);
    
    Button2Pressed = TRUE;
    if(Button1Pressed == TRUE){
      LcdClearChars(car.u32PositionA, 2);
      Button1Pressed = FALSE;
    }
    
    for(u32 i = 0; i < u32delayCount; i++){
      
    }
    
    LedOff(PURPLE);
    LedOn(BLUE);
    car.u32PositionA  = car.u32PositionA + 1;
    u8 u8newCar[] = " 0";
    car.u8Character = u8newCar;
    LcdMessage(car.u32PositionA, car.u8Character);
  }
  
  //Add pause button Later 
  else if (WasButtonPressed(BUTTON3) && !Pause){
    ButtonAcknowledge(BUTTON3);
    Pause = TRUE;
  }
  
}

void GameState(void){
  
  if(!Start){
    GameStart();
  }
  
  else if(Start){
    ButtonCheck();
  }
  
  else if(Pause){
    PauseGame();
  }
  
}

void GameStart(void){
  if(WasButtonPressed(BUTTON2) || WasButtonPressed(BUTTON1)){
    LcdClearChars(LINE1_START_ADDR, 40);
    LcdClearChars(LINE2_START_ADDR, 40);
    car.u32PositionA = LINE2_START_ADDR + 8;
    Start = TRUE;
    Pause = FALSE;
  }
}

void PauseGame(){
  Start = FALSE;
  LcdClearChars(car.u32PositionA, 4);
  u8 u8newMessage1[] = "Game is Paused";
  u8 u8newMessage2[] = "LED    #     S";
  LcdMessage(LINE1_START_ADDR, u8newMessage1);
  LcdMessage(LINE2_START_ADDR + 6, u8newMessage2);
   
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
