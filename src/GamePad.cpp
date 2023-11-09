#include <GamePad.h>


GamePad::Command GamePad::NO_COMMAND = GamePad::Command();

bool GamePad::Command::hasCommand()
{
    return (a || b || up || down || left || right);
}

void GamePad::init()
{
  last_ms = millis();

  wiimote.init();
  if (! logging)
  {
    wiimote.addFilter(ACTION_IGNORE, FILTER_ACCEL); // optional
  }
}

void GamePad::processTasks()
{
   wiimote.task();
   num_run++;
}

GamePad::Command GamePad::getCommand()
{
    if (wiimote.available() > 0) 
    {
        ButtonState  button  = wiimote.getButtonState();
        bool a     = (button & BUTTON_A);
        bool b     = (button & BUTTON_B);
        bool c     = (button & BUTTON_C);
        bool z     = (button & BUTTON_Z);
        bool b1     = (button & BUTTON_ONE);
        bool b2     = (button & BUTTON_TWO);
        bool minus = (button & BUTTON_MINUS);
        bool plus  = (button & BUTTON_PLUS);
        bool home  = (button & BUTTON_HOME);
        bool left  = (button & BUTTON_LEFT);
        bool right = (button & BUTTON_RIGHT);
        bool up    = (button & BUTTON_UP);
        bool down  = (button & BUTTON_DOWN);
        if(a || b || c || z || b1 || b2 || minus || plus || home || left || right || up || down)
        {
            currentCommand = Command();
            currentCommand.a = a || b1 || c || minus || home;
            currentCommand.b = a || b2 || z || plus;
            // Change for horizontal orientation
            currentCommand.left = up;
            currentCommand.right = down;
            currentCommand.up = right;
            currentCommand.down = left;
            num_updates++;
            if (logging)
            {
                AccelState   accel   = wiimote.getAccelState();
                NunchukState nunchuk = wiimote.getNunchukState();

                char ca     = (button & BUTTON_A)     ? 'A' : '.';
                char cb     = (button & BUTTON_B)     ? 'B' : '.';
                char cc     = (button & BUTTON_C)     ? 'C' : '.';
                char cz     = (button & BUTTON_Z)     ? 'Z' : '.';
                char c1     = (button & BUTTON_ONE)   ? '1' : '.';
                char c2     = (button & BUTTON_TWO)   ? '2' : '.';
                char cminus = (button & BUTTON_MINUS) ? '-' : '.';
                char cplus  = (button & BUTTON_PLUS)  ? '+' : '.';
                char chome  = (button & BUTTON_HOME)  ? 'H' : '.';
                char cleft  = (button & BUTTON_LEFT)  ? '<' : '.';
                char cright = (button & BUTTON_RIGHT) ? '>' : '.';
                char cup    = (button & BUTTON_UP)    ? '^' : '.';
                char cdown  = (button & BUTTON_DOWN)  ? 'v' : '.';
        
                Serial.printf("button: %05x = ", (int)button);
                Serial.print(ca);
                Serial.print(cb);
                Serial.print(cc);
                Serial.print(cz);
                Serial.print(c1);
                Serial.print(c2);
                Serial.print(cminus);
                Serial.print(chome);
                Serial.print(cplus);
                Serial.print(cleft);
                Serial.print(cright);
                Serial.print(cup);
                Serial.print(cdown);
                Serial.printf(", wiimote.axis: %3d/%3d/%3d", accel.xAxis, accel.yAxis, accel.zAxis);
                Serial.printf(", nunchuk.axis: %3d/%3d/%3d", nunchuk.xAxis, nunchuk.yAxis, nunchuk.zAxis);
                Serial.printf(", nunchuk.stick: %3d/%3d\n", nunchuk.xStick, nunchuk.yStick);
            }
        }
        else
        {
            currentCommand = NO_COMMAND;
        }
    }
/*
    if (! logging)
    {
        long ms = millis();
        if (ms - last_ms >= 1000)
        {
            Serial.printf("Run %d times per second with %d updates\n", num_run, num_updates);
            num_run = num_updates = 0;
            last_ms += 1000;
        }
    }*/
    return currentCommand;
}
