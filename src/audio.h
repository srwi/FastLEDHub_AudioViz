#pragma once

#include <iostream>
#include <math.h>
#include <bass.h>
#include <basswasapi.h>
#include <vector>
#include <functional>

#define SAMPLE_RATE 44100
#define SPECTRUM_LINES 16
#define FFT_RANGE_BIT 10
#define FFT_SIZE 2048
#define FFT_RANGE 1024
#define FFT_TYPE BASS_DATA_FFT2048

class AudioController
{
public:
	~AudioController();
	bool begin(int device, float beta, float interval, std::function<void(const std::vector<uint8_t>)> callback);
	float* getSpectrum();

private:
	static DWORD CALLBACK tick(void* buffer, DWORD length, void* user);

	float m_spectrum[SPECTRUM_LINES];
	float m_beta;
	float m_interval;
	int m_device;
	std::function<void(const std::vector<uint8_t>)> m_callback;
};
