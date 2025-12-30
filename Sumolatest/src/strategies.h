// SEARCH STRATEGIES
#include <functions.h>

void simple()
{
  switch (robot_state)
  {
    // actions
    // transitions

  case START:
    starter();
    break;
  case SEARCH:
    

    if (IR_FR_READ == 0 || IR_FL_READ == 0 || IR_R_READ == 0 || IR_L_READ == 0)
    {
      robot_state = ATTACK;
    }

    if (GROUND_FR_READ)
    {
      groundedr = 1;
      t3 = millis();
    }
    if (GROUND_FL_READ)
    {
      groundedl = 1;
      t3 = millis();
    }
    if (groundedl)
    {
      if (t1 - t3 <= 500)
      {
        moveInstant(90, 90);
        t4 = millis();
      }
      else if (t1 - t4 <= 250)
      {
        rotate_right(160, false);
      }
      else
      {
        groundedl = 0;
      }
    }
    else if (groundedr)
    {
      if (t1 - t3 <= 500)
      {
        moveInstant(90, 90);
        t4 = millis();
      }
      else if (t1 - t4 <= 250)
      {
        rotate_left(160, false);
      }
      else
      {
        groundedr = 0;
      }
    }
    else
    {
      forward(155, false);
      t3 = millis();
      t4 = millis();
    }
    break;

  case ATTACK:
    
    if (IR_FR_READ == 0 && IR_FL_READ == 0)
    {
      forward(160, false);
    }
    else if (IR_FR_READ == 1 && IR_FL_READ == 0)
    {
      turn_L(160, 30, true);
    }
    else if (IR_FR_READ == 0 && IR_FL_READ == 1)
    {
      turn_R(160, 30, true);
    }
    else if (IR_R_READ == 0)
    {
      rotate_right(160, false);
    }
    else if (IR_L_READ == 0)
    {
      rotate_left(160, false);
    }
    else if (IR_FR_READ == 1 && IR_FL_READ == 1 && IR_R_READ == 1 && IR_L_READ == 1)
    {

      robot_state = SEARCH;
    }

    break;
  case STOP:
    brake();
    break;
  }
}
void smart()
{
  switch (robot_state)
  {
  case START:

    launcher();
    break;
  case SEARCH:
    rotate_right(150, true);

    if (IR_FR_READ == 0 || IR_FL_READ == 0)
    {
      robot_state = ATTACK;
    }
    else if (IR_R_READ == 0)
    {
      robot_state = ROTATE_RIGHT;
    }
    else if (IR_L_READ == 0)
    {
      robot_state = ROTATE_LEFT;
    }

    break;
  case ATTACK:

    if (IR_FR_READ == 0 && IR_FL_READ == 0)
    {
      forward(240, false);
    }
    else if (IR_FR_READ == 1 && IR_FL_READ == 0)
    {
      turn_L(240, 50, true);
      remember_left = 1;
    }
    else if (IR_FR_READ == 0 && IR_FL_READ == 1)
    {
      turn_R(240, 50, true);
      remember_right = 1;
    }
    else if (IR_FR_READ == 1 && IR_FL_READ == 1)
    {
      if (remember_left == 1)
      {
        robot_state = ROTATE_LEFT;
        remember_left = 0;
        remember_right = 0;
      }
      else if (remember_right == 1)
      {
        robot_state = ROTATE_RIGHT;

        remember_right = 0;
        remember_left = 0;
      }
      else
      {
        robot_state = ROTATE_LEFT;
      }
    }

    break;

  case GROUND:

    robot_state = SEARCH;
    break;
  case ROTATE_RIGHT:
    rotate_right(200, false);
    if (IR_FR_READ == 0 || IR_FL_READ == 0)
    {
      robot_state = ATTACK;
    }
    break;
  case ROTATE_LEFT:
    rotate_left(200, false);

    if (IR_FR_READ == 0 || IR_FL_READ == 0)
    {
      robot_state = ATTACK;
    }
    break;
  case STOP:
    brake();
    break;
  }
}
