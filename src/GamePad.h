#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <AwtrisConf.h>
#include <ESP32Wiimote.h>

class GamePad
{
    public :
        class Command {
            public :
                Command();
                bool a = false;
                bool b = false;
                bool up = false;
                bool down = false;
                bool left = false;
                bool right = false;
                bool hasCommand();
        };
        static Command NO_COMMAND;

        void init();
        void processTasks();
        Command getCommand();

    private :
        /// Wiimote
        ESP32Wiimote wiimote;
        bool logging = false;
        long last_ms = 0;
        int num_run = 0, num_updates = 0;


};

#endif 