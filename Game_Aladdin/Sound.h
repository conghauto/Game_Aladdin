#pragma once
#define AUDIO_FORMAT_TAG		WAVE_FORMAT_PCM
#define	AUDIO_NUM_OF_CHANNEL	2
#define AUDIO_SAMPLE_SPEED		22050
#define AUDIO_BITS_PER_SAMPLE	16
#define AUDIO_FLAGS				0
#define AUDIO_BUFFER_SIZE		640000
#define AUDIO_GUID				GUID_NULL
#define BACKGROUND_MUSIC "sound\\Ending.wav"
#define GAME1_MUSIC "sound\\man1.wav"
#define GAME2_MUSIC "sound\\bosstheme.wav"
#define CUT_MUSIC "sound\\Low Sword.wav"
#define THROW_MUSIC "sound\\Apple Splat.wav"
#define CAMEL_MUSIC "sound\\Camel Spit.wav"
#define HURT_MUSIC "sound\\Aladdin Hurt.wav"
#define APPLE_MUSIC "sound\\Apple Collect.wav"
#define APPLE_POP_MUSIC "sound\\Balloon Pop.wav"
#define SWORD_SWORD_MUSIC "sound\\Sword Ching.wav"
#define DIE_MUSIC "sound\\chet.wav"
#define JAFAR_HURT_MUSIC "sound\\Jafar Snake.wav"
#define SHOOT_MUSIC "sound\\shoot.wav"
#define NEXT_MUSIC "sound\\level_complete.wav"
#define JAFAR_HURT_MUSIC "sound\\Jafar Snake.wav"
#define GUARD_HIT_MUSIC "sound\\Guard Hit 1.wav"
#define THEEND_MUSIC "sound\\theend.wav"
#define POT_MUSIC "sound\\Clay Pot.wav"
#define COMEOUT_MUSIC "sound\\Coming Out.wav"
#define COMEIN_MUSIC "sound\\Continue Point.wav"
#define FIRE_MUSIC "sound\\Fire From Coal.wav"
#define LAMP_MUSIC "sound\\Splash.wav"
#define WOW_MUSIC "sound\\Wow!.wav"

#define AUDIO_BLOCK_ALIGN(bitPerSample, nChannels)		(WORD)(bitPerSample / 8 * nChannels)
#define AUDIO_AVERAGE_BPS(samplesPerSec, blockAlign)	(DWORD)(samplesPerSec * blockAlign)

#include "dsound.h"
#include "windows.h"
#include <map>
#include <string>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

class Sound
{
public:
	struct WaveHeaderStruct
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};
	float volume;
	void static create(HWND hWnd);
	void setVolume(float percentage, std::string name = "");
	void loadSound(const char* fileName, std::string name);
	void play(std::string name, bool infiniteLoop, int times);
	void stop(std::string name = "");
	float getVolume();
	~Sound();
	static Sound* getInstance();
	void mute();
	void unMute();
	void cleanUp();
private:
	Sound(HWND hWnd);
	static Sound* instance;
	IDirectSound8* pDevice;
	IDirectSoundBuffer* primaryBuffer;
	std::map<std::string, IDirectSoundBuffer8*> soundBufferMap;
	bool isMute;

};