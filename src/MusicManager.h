#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <AwtrisConf.h>
#include <melody_player.h>
#include <melody_factory.h>

class MusicManager
{
    public :

        // Effets spéciaux
        enum SpecialEffect { NO_EFFECT = 0, SIREN = 1 };

        static MusicManager *getMusicManager()
        {
            if (musicManagerInstance == nullptr) {
                musicManagerInstance = new MusicManager();
            }
            return musicManagerInstance;
        }

        void init();

        void setVolume(byte volume);

        void startMelody();

        void stopMelody();

        void playLineSound();

        void playGameOverSound();

        void handleMusic();


    private :

        MelodyPlayer* player;

        Melody mainMelody;

        Melody gameOverMelody;

        MusicManager()
        {
            player = new MelodyPlayer(BUZZER_PIN,0,LOW);
        };

        ~MusicManager()
        {
            delete player;
        };

        static MusicManager *musicManagerInstance;
};

#endif 