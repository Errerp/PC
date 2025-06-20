#include "wav_lib.h"

int* wavread(const char* filename, WAVInfo& info)
{
	std::fstream	wavFile;
	int* data;
	__int16 tmp;

	wavFile.open(filename, std::ios::binary | std::ios::in);

	if (!wavFile)
		std::cout << "WAVREAD: Wav-file is not opened." << std::endl;

	wavFile.seekg(0, std::ios::beg);
	wavFile.read((char*)&info.Header, sizeof(info.Header));

	info.NumberOfSamples = info.Header.Subchunk2Size / info.Header.BlockAlign;

	data = new int[info.NumberOfSamples];
	memset(data, 0, info.NumberOfSamples);	//Zero array

	for (unsigned int i = 0; i < info.NumberOfSamples; i++) {
		wavFile.read((char*)&tmp, 2);
		data[i] = tmp;
	}
	return data;
}

int wavwrite(const char* filename, WAVInfo& info, int* data)
{
	std::fstream wavFile;

	wavFile.open(filename, std::ios::binary | std::ios::out);

	if (!wavFile) {
		std::cout << "WAVWRITE: Wav-file is not opened." << std::endl;
		return 1;
	}

	info.Header.Subchunk2Size = info.NumberOfSamples * info.Header.BlockAlign;

	wavFile.seekg(0, std::ios::beg);
	wavFile.write((char*)&info.Header, sizeof(info.Header));
	wavFile.seekg(0, std::ios::end);
	for (unsigned int i = 0; i < info.NumberOfSamples; i++) {
		__int16 tmp = data[i];
		wavFile.write((char*)&tmp, 2);
	}

	return 0;

}
