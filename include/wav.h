#ifndef __WAV_H__
#define __WAV_H__

//UINCODE环境
#ifndef UNICODE
#undef _UNICODE
#else
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct WAV_HEADER_S
{
	char            riffType[4];    //4byte,资源交换文件标志:RIFF
	unsigned int    riffSize;       //4byte,从下个地址到文件结尾的总字节数
	char            waveType[4];    //4byte,wave文件标志:WAVE
	char            formatType[4];  //4byte,波形文件标志:FMT
	unsigned int    formatSize;     //4byte,音频属性(compressionCode,numChannels,sampleRate,bytesPerSecond,blockAlign,bitsPerSample)所占字节数
	unsigned short  compressionCode;//2byte,编码格式(1-线性pcm-WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM)
	unsigned short  numChannels;    //2byte,通道数
	unsigned int    sampleRate;     //4byte,采样率
	unsigned int    bytesPerSecond; //4byte,传输速率
	unsigned short  blockAlign;     //2byte,数据块的对齐,每个采样的字节数,如果不是8的倍数的位深则为data块中填充bit数
	unsigned short  bitsPerSample;  //2byte,采样精度,位深
	char            dataType[4];    //4byte,数据标志:data
	unsigned int    dataSize;       //4byte,从下个地址到文件结尾的总字节数，即除了wav header以外的pcm data length
}WAV_HEADER;

typedef struct WAV_INFO_S
{
	WAV_HEADER    header;
	FILE          *fp;
	unsigned int  channelMask;
	fpos_t        dataPos;
}WAV_INFO;

#ifdef __cplusplus
extern "C" {//for this in mingw can't call as c++
#endif
WAV_INFO* wavInputOpen(const _TCHAR *filename);
#ifdef __cplusplus
}
#endif

#endif
