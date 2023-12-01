#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <AwtrisConf.h>
#include <melody_player.h>
#include <melody_factory.h>

#define INITIAL_TEMPO     120
#define TEMPO_INCREMENT   2

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

        void playLineSound(int numlines);

        void playPenaltySound(int numlines);

        void playCountDownSound(bool last);

        void increaseTempo(int increment);

        void playGameOverSound();

        void unmute();


    private :

        MelodyPlayer* player;
        MelodyPlayer* effectPlayer;

        Melody mainMelody;
        Melody gameOverMelody;
        Melody oneLineEffect;
        Melody twoThreeLinesEffect;
        Melody tetrisEffect;
        Melody onePenaltyEffect;
        Melody twoPenaltiesEffect;
        Melody tetrisPenaltyEffect;
        Melody countdownHighEffect;
        Melody countdownLowEffect;


        int tempo = INITIAL_TEMPO;

        MusicManager()
        {
            player = new MelodyPlayer(BUZZER_PIN,0,LOW);
            effectPlayer = new MelodyPlayer(BUZZER_PIN,0,LOW);
        };

        ~MusicManager()
        {
            delete player;
            delete effectPlayer;
        };

        static MusicManager *musicManagerInstance;
};

#endif 