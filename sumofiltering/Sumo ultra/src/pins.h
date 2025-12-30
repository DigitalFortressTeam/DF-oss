const int motorLEN = 7;
const int motorREN = 9;
const int motorL_PWM = 12;
const int motorR_PWM = 11;
const int GROUND_BL = 15;
const int GROUND_BR = A15;
const int GROUND_FL = 10;
const int GROUND_FR = 14;
int GROUND_BL_READ;
int GROUND_BR_READ;
int GROUND_FL_READ;
int GROUND_FR_READ;
int IR_L_READ;
int IR_R_READ;
int IR_FL_READ;
int IR_FR_READ;
const int buzzer = 13;
const int IR_L = A7;
const int IR_R = 16;
const int IR_FR = 25;
const int IR_FL = A1;

const int START = 0;
const int STOP = 4;
const int launching = 7;
const int ATTACK = 1;
const int GROUND = 2;
const int ROTATE_RIGHT = 3;
const int ROTATE_LEFT = 5;
const int SEARCH = 6;
const int IDLE = 9;
int robot_state = IDLE;

int remember_right = 0;
int remember_left = 0;

bool run1time = 1;
int fullspeed = 0;
bool accelerating = 0;
byte accelerating_speed = 129;

unsigned long t2 = 0;
unsigned long t3 = 0;

int amount = 0;
unsigned long t1 = 0;

bool countering = 0;

float current_pwm;

float smoothedpwm = 128;

unsigned long t4 = 0;

bool groundedr = 0;
bool groundedl = 0;

int advancedstate = 0;
#define Advanced_FirstSEARCh 1
#define Advanced_TornadoSEARCH 2
#define Advanced_RandomSEARCH_step2 3
#define Advanced_RandomSEARCH_step2 4
#define Advanced_DIRECTATTACKFORWARD 5
#define Advanced_DIRECTATTACKLEFT 6
#define Advanced_DIRECTATTACKRIGHT 7
#define Advanced_RotateSlowlyright 8
#define Advanced_RotateSlowlyleft 9
#define Advanced_APPROACH_step1 10
#define Advanced_APPROACH_step2 11
#define Advanced_GROUNDED 12

int flapsstate;
#define Flaps_LAUNCHER 1
#define Flaps_SEARCH 2
#define Flaps_ATTACK 3
#define Flaps_GROUND 4
#define Flaps_PRIOROTIZE_SIDE 5


#define Waiting 0

#define ACTIVE 7
#define STOP 8
int controlstate = Waiting;


bool use_strat1 = 0;
bool use_strat2 = 0;
bool use_strat3 = 0;
bool use_strat4 = 0;
bool use_strat5 = 0;
bool use_cleaning = 0;

#define Strat1 1
#define Strat2 2
#define Strat3 3
#define Strat4 4
#define Strat5 5
#define Cleaning 6
int stratstates = 0;

bool rightrotation = 0;
bool leftrotation = 0;


unsigned long tfirstsearch = 0;
unsigned long ttornadosearch = 0;
unsigned long trandomsearch1 = 0;
unsigned long trandomsearch2 = 0;
unsigned long t1approach1 = 0;
unsigned long t1approach2 = 0;

bool appr_from_right = 0;
bool appr_from_left = 0;
bool closer_from_right = 0;
bool closer_from_left = 0;
