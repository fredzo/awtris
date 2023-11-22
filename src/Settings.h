#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <Preferences.h>
#include <BgEffectManager.h>

class Settings
{
    public :
        static Settings *getSettings()
        {
            if (settingsInstance == nullptr) {
                settingsInstance = new Settings();
            }
            return settingsInstance;
        }

        void init();

        void setAutoSave(bool autoSave);

        int getHighScore();
        void setHighScore(int highScore);
        BgEffectManager::BackgroundEffect getBackgroundEffect();
        void setBackgroundEffect(BgEffectManager::BackgroundEffect effect);
        byte getVolume();
        void setVolume(byte volume);
        byte getBrightness();
        void setBrigtness(byte brightness);


        void save();

        void processAutoSave();

    private :
        Preferences preferences;
        bool dirty = false; // True when save is needed
        bool autoSave = false;
        unsigned long lastUpdateTime = 0;

        Settings()
        {
        };

        ~Settings()
        {
        };

        void setDirty();

        static Settings *settingsInstance;
};

#endif 