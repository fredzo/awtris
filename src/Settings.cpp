#include <Settings.h>
#include <AwtrisConf.h>

#define PREF_PREFIX         "Awtris"
#define HIGH_SCORE_KEY      "HighScore"
#define BG_EFFECT_KEY       "BgEffect"
#define VOLUME_KEY          "Volume"
#define BRIGHTNESS_KEY      "Brightness"

#define AUTO_SAVE_TIME      10000

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
    setDirty();
}

BgEffectManager::BackgroundEffect Settings::getBackgroundEffect()
{
    return (BgEffectManager::BackgroundEffect)preferences.getUChar(BG_EFFECT_KEY,BgEffectManager::BackgroundEffect::PLASMA);
}

void Settings::setBackgroundEffect(BgEffectManager::BackgroundEffect effect)
{
    preferences.putUChar(BG_EFFECT_KEY,(char)effect);
    setDirty();
}

byte Settings::getVolume()
{
    return preferences.getUChar(VOLUME_KEY,DEFAULT_VOLUME);
}

void Settings::setVolume(byte volume)
{
    preferences.putUChar(VOLUME_KEY,volume);
    setDirty();
}

byte Settings::getBrightness()
{
    return preferences.getUChar(BRIGHTNESS_KEY,DEFAULT_BRIGHTNESS);
}

void Settings::setBrigtness(byte brightness)
{
    preferences.putUChar(BRIGHTNESS_KEY,brightness);
    setDirty();
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

void Settings::setDirty()
{
    dirty = true;
    lastUpdateTime = millis();
}

void Settings::processAutoSave()
{
    if(dirty)
    {   // Wait 10 sec after last update
        if(millis() > (lastUpdateTime + AUTO_SAVE_TIME))
        {   // Perform autosave
            save();
        }
    }
}
