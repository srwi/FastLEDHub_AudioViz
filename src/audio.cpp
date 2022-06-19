#include "audio.h"

AudioController::~AudioController()
{
	BASS_WASAPI_Free();
	BASS_WASAPI_Stop(true);
}

void AudioController::begin(int device, float beta, float interval, std::function<void(const std::vector<uint8_t>)> callback)
{
	m_device = device;
	m_beta = beta;
	m_interval = interval;
	m_callback = callback;

	memset(m_spectrum, 0, sizeof(m_spectrum));

	if (!BASS_Init(0, SAMPLE_RATE, NULL, 0, NULL))
	{
		throw std::runtime_error("Initializing 'no sound' device failed with error code: " + BASS_ErrorGetCode());  // TODO: raise errors correctly
	}
	if (!BASS_WASAPI_Init(m_device, SAMPLE_RATE, 2, (BASS_WASAPI_BUFFER | BASS_WASAPI_AUTOFORMAT), 0.5, m_interval, tick, this))
	{
		std::cout << "Initializing audio device failed with error code: " << BASS_ErrorGetCode() << std::endl;
	}
	if (!BASS_WASAPI_Start())
	{
		std::cout << "Starting Bass WASAPI failed with error code: " << BASS_ErrorGetCode() << std::endl;
	}
}

void AudioController::listDevices()
{
	BASS_WASAPI_DEVICEINFO device;
	for (int i = 0; BASS_WASAPI_GetDeviceInfo(i, &device); i++)
	{
		if (device.flags & BASS_DEVICE_INPUT && device.flags & BASS_DEVICE_ENABLED)
		{
			// TODO
		}
	}
}

float* AudioController::getSpectrum()
{
	return m_spectrum;
}

DWORD CALLBACK AudioController::tick(void* buffer, DWORD length, void* user)
{
	AudioController* this_ = static_cast<AudioController*>(user);

	float fft[FFT_RANGE];
	BASS_WASAPI_GetData(fft, FFT_TYPE);

	int b0 = 0;
	for (int x = 0; x < SPECTRUM_LINES; ++x)
	{
		int b1 = round(pow(2, (float)FFT_RANGE_BIT * x / (SPECTRUM_LINES - 1)) / FFT_RANGE * (FFT_RANGE - 1));

		float binPeak = 0;
		for (; b0 < b1; ++b0)
		{
			if (fft[b0 + 1] > binPeak)
			{
				binPeak = fft[b0 + 1];
			}
		}

		int y = sqrt(binPeak) * 255.0;
		if (y > 255) y = 255;
		if (y < 0) y = 0;

		// Low pass filter
		this_->m_spectrum[x] = this_->m_spectrum[x] - this_->m_beta * (this_->m_spectrum[x] - y);
	}

	std::vector<uint8_t> bytes(SPECTRUM_LINES);
	for (int i = 0; i < SPECTRUM_LINES; ++i)
	{
		bytes[i] = (unsigned char)(this_->m_spectrum[i]);
	}
	this_->m_callback(bytes);

	return length;
}
