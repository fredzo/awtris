#include <MultiPlayer.h>

#include <WifiManager.h>
#include <Tetris.h>

MultiPlayer* MultiPlayer::multiPlayerInstance = nullptr;

#define INVITE_MESSAGE              "Invite"
#define JOIN_MESSAGE                "Join"
#define LEVEL_MESSAGE_PREFIX        "Level"
#define LINE_MESSAGE_PREFIX         "Line"
#define GAME_OVER_MESSAGE_PREFIX    "GameOver"
#define SCORE_MESSAGE_PREFIX        "Score"

#define BRAODCAST_DELAY             2000

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

static String formatMessage(String message, int parameter)
{
    return (message + ':' + parameter);
}

void MultiPlayer::broadcastInvite()
{
    if(millis()>=(lastBroadcastTime + BRAODCAST_DELAY))
    {
        lastBroadcastTime = millis();
        broadcastMessage(INVITE_MESSAGE);
    }
}

void MultiPlayer::sendJoin()
{
    broadcastMessage(JOIN_MESSAGE);
}

void MultiPlayer::sendLevel(int level)
{
    broadcastMessage(formatMessage(LEVEL_MESSAGE_PREFIX,level));
}

void MultiPlayer::sendLine(int lines)
{
    broadcastMessage(formatMessage(LINE_MESSAGE_PREFIX,lines));
}

void MultiPlayer::sendScore(int score)
{
    broadcastMessage(formatMessage(SCORE_MESSAGE_PREFIX,score));
}

void MultiPlayer::sendGameOver(int score)
{
    broadcastMessage(formatMessage(GAME_OVER_MESSAGE_PREFIX,score));
}

void MultiPlayer::processMultiPlayer()
{
    String message = consumeMessage();
    if(message.length()>0)
    {   // We have a new message
        if(message.startsWith(INVITE_MESSAGE))
        {
            inviteCallback();
        }
        else if(message.startsWith(JOIN_MESSAGE))
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


