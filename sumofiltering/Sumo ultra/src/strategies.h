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
    expFilterRatio = 0.1;
    expFilterPeriod = 10;

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
    expFilterPeriod = 30;
    expFilterRatio = 0.07;

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
void advancedright()
{
  switch (advancedstate)
  {
  case START:
    starter();
    tfirstsearch = millis();
    break;
  case Advanced_FirstSEARCh:
    // actions
    turn_L(200, 50, false);

    // transitions
    if (t1 - tfirstsearch >= 2000)
    {
      advancedstate = Advanced_TornadoSEARCH;
      ttornadosearch = millis();
      break;
    }
    if (IR_FR_READ == 0 || IR_FL_READ == 0)
    {
      advancedstate = Advanced_DIRECTATTACK;
    }
    else if (IR_R_READ == 0)
    {
      advancedstate = Advanced_RotateSlowly;
      rightrotation = 1;
      leftrotation = 0;
    }
    else if (IR_L_READ == 0)
    {
      advancedstate = Advanced_RotateSlowly;
      leftrotation = 1;
      rightrotation = 0;
    }
    else if (GROUND_FR_READ)
    {
      groundedr = 1;
      t4 = millis();
    }
    else if (GROUND_FL_READ)
    {
      groundedl = 1;
      t3 = millis();
    }

    break;
  case Advanced_TornadoSEARCH:
    // actions
    rotate_left(180, false);
    // transitions
    if (t1 - ttornadosearch >= 2000)
    {
      advancedstate = Advanced_RandomSEARCH;
      trandomsearch1 = millis();
    }
    
    if (IR_FR_READ == 0 || IR_FL_READ == 0)
    {
      advancedstate = Advanced_DIRECTATTACK;
    }
    else if (IR_R_READ == 0)
    {
      advancedstate = Advanced_RotateSlowly;
      rightrotation = 1;
      leftrotation = 0;
    }
    else if (IR_L_READ == 0)
    {
      advancedstate = Advanced_RotateSlowly;
      leftrotation = 1;
      rightrotation = 0;
    }
    else if (GROUND_FR_READ)
    {
      groundedr = 1;
      t4 = millis();
    }
    else if (GROUND_FL_READ)
    {
      groundedl = 1;
      t3 = millis();
    }

    break;
  case Advanced_RandomSEARCH:
    // actions
    if (t1 - trandomsearch1 <= 600)
    {
      forward(200, false);
      trandomsearch2 = millis();
    }
    else if (t1 - trandomsearch2 <= 300)
    {
      rotate_left(200, false);
    }
    else
    {
      trandomsearch1 = millis();
    }
    // transitions
    if (IR_FR_READ == 0)
    {
      advancedstate = Advanced_DIRECTATTACK;
      appr_from_left = 1;
      appr_from_right = 0;
      break;
    }
    else if (IR_FL_READ == 0)
    {
      advancedstate = Advanced_DIRECTATTACK;
      appr_from_left = 0;
      appr_from_right = 1;
      break;
    }
    else if (IR_R_READ == 0)
    {
      advancedstate = Advanced_RotateSlowly;
      rightrotation = 1;
      leftrotation = 0;
    }
    else if (IR_L_READ == 0)
    {
      advancedstate = Advanced_RotateSlowly;
      leftrotation = 1;
      rightrotation = 0;
    }

    break;
  case Advanced_DIRECTATTACKFORWARD:
    // actions
    forward(150, false);

    // transitions
    if (IR_FR_READ == 1 && IR_FL_READ == 1)
    {
      advancedstate = Advanced_TornadoSEARCH;
    }
    break;
  case Advanced_DIRECTATTACKLEFT:
  break;
  case Advanced_RotateSlowlyright:
    // actions
    if (rightrotation == 1)
    {
      rotate_right(160, false);
    }
    else if (leftrotation == 1)
    {
      rotate_left(160, false);
    }
    // transitions
    if (IR_FR_READ == 0 || IR_FL_READ == 0)
    {
      advancedstate = Advanced_DIRECTATTACK;
    }
    else if (IR_R_READ == 0)
    {

      rightrotation = 0;
      leftrotation = 1;
    }
    else if (IR_L_READ == 0)
    {
      leftrotation = 0;
      rightrotation = 1;
    }

    break;
  case Advanced_APPROACH:
    // actions
    // if (appr_from_left == 1)
    // {
    //   if (t1 - t1approach1 <= 500)
    //   {
    //     turn_L(200, 50, false);
    //     t1approach2 = millis();
    //   }
    // }
    // else if (appr_from_right == 1)
    // {
    //   if (t1 - t1approach1 <= 500)
    //   {
    //     turn_R(200, 70, false);
    //     t1approach2 = millis();
    //   }
    //   else { 
    //     advancedstate = Advanced_DIRECTATTACK;
    //     appr_from_left = 0;
    //     appr_from_right = 0;
    //   }
    //   if (IR_FL_READ == 1)
    //   {
    //     closer_from_right = 1;
    //     appr_from_left = 0;
    //     appr_from_right = 0;

    //   }
      

    //   if(closer_from_right == 1)
    //   {
    //     if (t1 - t1approach2 <= 500)
    //     {
    //       turn_L(200, 60, false);
    //       t1approach2 = millis();
    //     }
    //   }
    // }

    // transitions

    break;
  case Advanced_GROUNDED:
    // actions
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
    // transitions

    break;
  }
}

void advancedleft()
{
  switch (advancedstate)
  {
  case Advanced_FirstSEARCh:
    // actions
    turn_R(200, 50, false);

    // transitions
    if (t1 - tfirstsearch >= 2000)
    {
      advancedstate = Advanced_TornadoSEARCH;
      ttornadosearch = millis();
      break;
    }
    if (IR_FR_READ == 0 || IR_FL_READ == 0)
    {
      advancedstate = Advanced_DIRECTATTACK;
    }
    else if (IR_R_READ == 0)
    {
      advancedstate = Advanced_RotateSlowly;
      rightrotation = 1;
      leftrotation = 0;
    }
    else if (IR_L_READ == 0)
    {
      advancedstate = Advanced_RotateSlowly;
      leftrotation = 1;
      rightrotation = 0;
    }
    else if (GROUND_FR_READ)
    {
      groundedr = 1;
      t4 = millis();
    }
    else if (GROUND_FL_READ)
    {
      groundedl = 1;
      t3 = millis();
    }

    break;
  case Advanced_TornadoSEARCH:
    // actions

    // transitions

    break;
  case Advanced_RandomSEARCH:
    // actions

    // transitions

    break;
  case Advanced_DIRECTATTACKFORWARD:
    // actions
    forward(150, false);

    // transitions
    if (IR_FR_READ == 1 && IR_FL_READ == 1)
    {
      advancedstate = Advanced_TornadoSEARCH;
    }
    break;
  case Advanced_RotateSlowly:
    // actions

    // transitions

    break;
  case Advanced_APPROACH:
    // actions

    // transitions

    break;
  case Advanced_GROUNDED:
    // actions

    // transitions

    break;
  }
}