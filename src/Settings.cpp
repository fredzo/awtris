#include <Settings.h>
#include <AwtrisConf.h>

#define PREF_PREFIX         "Awtris"
#define HIGH_SCORE_KEY      "HighScore"

Settings *Settings::settingsInstance = nullptr;

void Settings::init()
{
    preferences.begin(PREF_PREFIX, false);
}

int Settings::getHighScore()
{
    return preferences.getInt(HIGH_SCORE_KEY,0);
}

void Settings::setHighScore(int highScore)
{
    preferences.putInt(HIGH_SCORE_KEY,highScore);
    dirty = true;
}

void Settings::save()
{
    if(dirty)
    {
        preferences.end();
        preferences.begin(PREF_PREFIX, false);
        dirty = false;
    }
}
