/*
typedef struct
{
    WORD  wFormatag; //编码格式，包括WAVE_FORMAT_PCM，WAVEFORMAT_ADPCM等
    WORD  nChannls; //声道数，单声道为1，双声道为2;

    DWORD  nSamplesPerSec; //采样频率；

    DWORD  nAvgBytesperSec；  //每秒的数据量；

    WORD  nBlockAlign; //块对齐；

    WORD  wBitsPerSample; //WAVE文件的采样大小；

    WORD  cbSize; // The count in bytes of the size of extra
                  // information(after cbSize). PCM中忽略此值
 } WAVEFORMATEX;

typedef struct
{
    WAVEFORMATEX  wfmt;

    WORD  nSamplesPerBlock;

} IMAADPCMWAVEFORMAT;

typedef struct{

    char[4]; //“fact”字符串

    DWORD chunksize;

    DWORD datafactsize; // 音频数据转换为PCM格式后的大小。

} factchunk;
*/

#include "wav.h"
#include "VA_OPT.h"

#ifdef UNICODE
#define _t "%ls"
#else
#define _t "%s"
#endif

/* func		: endian judge
 * return	: 0-big-endian othes-little-endian
 */
int IS_LITTLE_ENDIAN(void)
{
	int __dummy = 1;
	return ( *( (unsigned char*)(&(__dummy) ) ) );
}

unsigned int readHeader(void *dst, signed int size, signed int nmemb, FILE *fp)
{
	unsigned int n, s0, s1, err;
	unsigned char tmp, *ptr;

	if ((err = fread(dst, size, nmemb, fp)) != nmemb)
	{
		return err;
	}
	if (!IS_LITTLE_ENDIAN() && size > 1)
	{
		//printf("big-endian \n");
		ptr = (unsigned char*)dst;
		for (n=0; n<nmemb; n++)
		{
			for (s0=0, s1=size-1; s0 < s1; s0++, s1--)
			{
				tmp = ptr[s0];
				ptr[s0] = ptr[s1];
				ptr[s1] = tmp;
			}
			ptr += size;
		}
	}
	else
	{
		//printf("little-endian \n");
	}

	return err;
}


extern "C" WAV_INFO* wavInputOpen(const _TCHAR *filename)
{
	signed int offset;
	WAV_INFO *wav = (WAV_INFO*)malloc(sizeof(WAV_INFO)) ;

	if (wav == NULL)
	{
		_tprintf(_LT("Unable to allocate WAV struct.\n"));
		return NULL;
	}
	wav->fp = _tfopen(filename, _LT("rb"));
	if (wav->fp == NULL)
	{
		_tprintf(_LT("Unable to open wav file. ", _t, "\n"), filename);
		free(wav);
		return NULL;
	}

	/* RIFF标志符判断 */
	if (fread(&(wav->header.riffType), 1, 4, wav->fp) != 4)
	{
		_tprintf(_LT("couldn't read RIFF_ID\n"));
		fclose(wav->fp);
		free(wav);
		return NULL;
	}
	if (strncmp("RIFF", wav->header.riffType, 4))
	{
		_tprintf(_LT("RIFF descriptor not found.\n")) ;
		fclose(wav->fp);
		free(wav);
		return NULL;
	}

	/* Read RIFF size. Ignored. */
	readHeader(&(wav->header.riffSize), 4, 1, wav->fp);

	/* WAVE标志符判断 */
	if (fread(&wav->header.waveType, 1, 4, wav->fp) !=4)
	{
		_tprintf(_LT("couldn't read format\n"));
		fclose(wav->fp);
		free(wav);
		return NULL;  /* bad error "couldn't read format" */
	}
	if (strncmp("WAVE", wav->header.waveType, 4))
	{
		_tprintf(_LT("WAVE chunk ID not found.\n")) ;
		fclose(wav->fp);
		free(wav);
		return NULL;
	}

	/* fmt标志符判断 */
	if (fread(&(wav->header.formatType), 1, 4, wav->fp) != 4)
	{
		_tprintf(_LT("couldn't read format_ID\n"));
		fclose(wav->fp);
		free(wav);
		return NULL;  /* bad error "couldn't read format_ID" */
	}
	if (strncmp("fmt", wav->header.formatType, 3))
	{
		_tprintf(_LT("fmt chunk format not found.\n")) ;
		fclose(wav->fp);
		free(wav);
		return NULL;
	}

	readHeader(&wav->header.formatSize, 4, 1, wav->fp);  // Ignored

	/* read  info */
	readHeader(&(wav->header.compressionCode), 2, 1, wav->fp);
	readHeader(&(wav->header.numChannels), 2, 1, wav->fp);
	readHeader(&(wav->header.sampleRate), 4, 1, wav->fp);
	readHeader(&(wav->header.bytesPerSecond), 4, 1, wav->fp);
	readHeader(&(wav->header.blockAlign), 2, 1, wav->fp);
	readHeader(&(wav->header.bitsPerSample), 2, 1, wav->fp);

	offset = wav->header.formatSize - 16;

	/* Wav format extensible */
	if (wav->header.compressionCode == 0xFFFE)
	{
		static const unsigned char guidPCM[16] = {
			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
			0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71
		};
		unsigned short extraFormatBytes, validBitsPerSample;
		unsigned char guid[16];
		signed int i;

		/* read extra bytes */
		readHeader(&(extraFormatBytes), 2, 1, wav->fp);
		offset -= 2;

		if (extraFormatBytes >= 22)
		{
			readHeader(&(validBitsPerSample), 2, 1, wav->fp);
			readHeader(&(wav->channelMask), 4, 1, wav->fp);
			readHeader(&(guid), 16, 1, wav->fp);

			/* check for PCM GUID */
			for (i = 0; i < 16; i++) if (guid[i] != guidPCM[i]) break;
			if (i == 16) wav->header.compressionCode = 0x01;

			offset -= 22;
		}
	}

	/* Skip rest of fmt header if any. */
	for (;offset > 0; offset--)
	{
		fgetc(wav->fp);
	}

#if 1
	do
	{
		/* Read data chunk ID */
		if (fread(wav->header.dataType, 1, 4, wav->fp) != 4)
		{
			_tprintf(_LT("Unable to read data chunk ID.\n"));
			fclose(wav->fp);
			free(wav);
			return NULL;
		}
		/* Read chunk length. */
		readHeader(&offset, 4, 1, wav->fp);

		/* Check for data chunk signature. */
		if (strncmp("data", wav->header.dataType, 4) == 0)
		{
			wav->header.dataSize = offset;
			break;
		}

		/* Jump over non data chunk. */
		for (;offset > 0; offset--)
		{
			fgetc(wav->fp);
		}
	} while (!feof(wav->fp));
#endif

	fgetpos(wav->fp,&(wav->dataPos));
	fseek(wav->fp, 0, SEEK_END);
	fpos_t fpos;
	fgetpos(wav->fp, &fpos);
	if (fpos - wav->dataPos < wav->header.dataSize){
		_tprintf(_LT("pcm data size is less than dataSize!\n"));
		fclose(wav->fp);
		free(wav);
		return NULL;
	}
	fsetpos(wav->fp, &wav->dataPos);
	return wav;
}


