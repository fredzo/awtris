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

        void broadcastInvite();

        void sendJoin();

        void sendLevel(int level);

        void sendLine(int lines);

        void sendScore(int score);

        void sendGameOver(int score);


    private :

        MultiPlayer()
        {
        };

        ~MultiPlayer()
        {
        };
        unsigned long lastBroadcastTime;

        static MultiPlayer *multiPlayerInstance;

};

#endif 