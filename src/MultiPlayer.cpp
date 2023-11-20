#include <MultiPlayer.h>

#include <WifiManager.h>
#include <Tetris.h>

MultiPlayer* MultiPlayer::multiPlayerInstance = nullptr;

#define INVITE_MESSAGE_PREFIX       "Invite"
#define JOIN_MESSAGE_PREFIX         "Join"
#define LEVEL_MESSAGE_PREFIX        "Level"
#define LINE_MESSAGE_PREFIX         "Line"
#define GAME_OVER_MESSAGE_PREFIX    "GameOver"
#define SCORE_MESSAGE_PREFIX        "GameOver"


void MultiPlayer::init()
{
}

static int extractParameter(String message)
{
    int result = 0;
    int sepIndex = message.indexOf(':');
    if(sepIndex >= 0)
    {
        result = atoi(message.substring(sepIndex+1).c_str());
    }
    return result;
}

void MultiPlayer::processMultiPlayer()
{
    String message = consumeMessage();
    if(message.length()>0)
    {   // We have a new message
        if(message.startsWith(INVITE_MESSAGE_PREFIX))
        {
            inviteCallback();
        }
        else if(message.startsWith(JOIN_MESSAGE_PREFIX))
        {
            joinCallback();
        }
        else if(message.startsWith(LEVEL_MESSAGE_PREFIX))
        {
            levelCallback(extractParameter(message));
        }
        else if(message.startsWith(LINE_MESSAGE_PREFIX))
        {
            addLineCallback(extractParameter(message));
        }
        else if(message.startsWith(GAME_OVER_MESSAGE_PREFIX))
        {
            gameOverCallback(extractParameter(message));
        }
        else if(message.startsWith(SCORE_MESSAGE_PREFIX))
        {
            scoreCallback(extractParameter(message));
        }
    }
}


