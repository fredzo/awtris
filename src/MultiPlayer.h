#ifndef MULTI_PLAYER_H
#define MULTI_PLAYER_H

#include <AwtrisConf.h>


class MultiPlayer
{
    public :

        static MultiPlayer *getMultiPlayer()
        {
            if (multiPlayerInstance == nullptr) {
                multiPlayerInstance = new MultiPlayer();
            }
            return multiPlayerInstance;
        }

        void init();

        void processMultiPlayer();


    private :

        MultiPlayer()
        {
        };

        ~MultiPlayer()
        {
        };

        static MultiPlayer *multiPlayerInstance;
};

#endif 