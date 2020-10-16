//UINCODE环境
#ifndef UNICODE
#undef _UNICODE
#else
#ifndef _UNICODE
#define _UNICODE
#endif
#endif

/**
 * 请将解码器目录设置到环境变量Path中去，以方便本程序调用。
**/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string.h>
#include <locale.h>
#include <Windows.h>
#include "wav.h"
#include "instruction_func.h"
#include "VA_OPT.h"//该头文件仅是为了正确拼接宽字符串
#ifdef UNICODE
#define _t "%ls"
#else
#define _t "%s"
#endif


#define REPLACE(pos,s,d) while (*pos != 0) {if (*pos == s)*pos = d;pos++;}
#define STRCPY(dest,src) while(*src!=0)*(dest++)=*(src++)

//注意，有些编译器使用_T并不能正确拼接宽字符串，如：MSVC++（及配套的Intel C++），它们可能仅处理了一个宽字符转换，所以之後的字符串不能正确显示
//为了拼接宽字符串，可以在每个字符串前加L前缀，然後拼接，所以定义一个宏来实现这个功能。_LT能处理最多100个字符串拼接，返回类型是const _TCHAR*。
const _TCHAR HELP_INFO[] = _LT("*********** 有损无损混合编解码器 ******************\n",
	"有损无损混合编解码器，简称混合编解码器。\n",
	"构建日期：", __DATE__, "\n",
	"作者：杨玉军\n",
	"用于编解码有损音频和纠错音频，即混合编解码，可以自由搭配有损和无损的编解码器。\n",
	"This program is free software; you can redistribute it and/or modify\n",
	"it under the terms of the GNU General Public License.\n",
	"**************************************************************************\n",
	"\n",
	"命令行格式：\n",
	"帮助：ahc{[--help]|[-h]}\n",
	"外包编解码：ahc %s %d -w codec_params\n",
	"编码：ahc %s %d -e -p<n> main_params [-ahc_ep] -p<n> ext_params [-ahc_ep] [ -p<n> main_decode_params [-ahc_ep] ] filepath\n",
	"解码：ahc %s %d -d {2|[ -p<n> main_params [-ahc_ep] ] [ -p<n> ext_params [-ahc_ep] ] [--] [--src2]} filepath\n",
	"生成纠错文件1：ahc %s %d -c -p<n> ext_params [-ahc_ep] [\"-s<n>.ext path\"] {2|[ -p<n> wav_decode_params [-ahc_ep] ] [ -p<n> main_decode_params [-ahc_ep] ] [--[src1]] [--]} filepath\n",
	"生成纠错文件2：ahc %s %d -C -p<n> ext_params [-ahc_ep] [\"-s<n>.ext path\"] {2|[ -p<n> wav_decode_params [-ahc_ep] ] [ -p<n> main_decode_params [-ahc_ep] ] [--] [--[src2]]} filepath\n",
	"%s 输入文件名（不可作为变量使用）\n",
	"%d 输出文件名（不可作为变量使用）\n",
	"$s$ 输入文件名\n",
	"$d$ 输出文件名\n",
	"$sd$ 用于生成纠错文件的变量，是输入文件对应的无损/有损文件名，值为\"-s<n>.ext path\"\n",
	"$c$ 声道数\n",
	"$f$ 频率带宽\n",
	"$r$ 采样率\n",
	"$t$ 位深\n",
	"$n$ 临时文件名（包括filepath）\n",
	"$m$ 临时文件名（包括filepath）\n",
	"\n",
	"-p<n> 可选选项，n代表main_params等的参数个数，\"-p<n>\"可为：\"-p4\"\n",
	"[-ahc_ep] 可选选项，与\"-p<n>\"必择其一，用来分隔main_params、ext_params等编解码参数\n",
	"codec_params 编解码器参数\n",
	"main_params 主编码器参数\n",
	"ext_params 扩展编码器参数\n",
	"main_decode_params 主解码器参数\n",
	"wav_decode_params 无损音频解码参数\n",
	"filepath 缓存文件路径\n",
	"%s %d 是foobar2000转换器设置的字符串\n",
	"$d:~0,-3$ext 是得到替换%d的後缀（为3个字符）的字符串\n",
	"$s$ $d$ $sd$ $c$ $f$ $r$ $t$ $n$ $m$是本编解码器内置变量（不带引号）\n",
	"\n",
	"注：编解码器参数，第一个是编解码器名，解析後不带引号，其他则为编解码器传参，解析後带引号。\n",
	"\n",
	"以[]括起来的是可选选项，而用{}括起来的是可选参数组，要求从中选用指定的可选参数数量，如：{1|[src1] [src2]}，表示从src1,src2中选用一个，这也可以写为{[src1]|[src2]}\n",
	"\n",
	"\"-s<n>.ext path\" 参数是专为解决生成纠错文件中可能存在无损音频路径与有损音频不一样，通过替换有损/无损音频目录和後缀的方式来得到对应另一音频路径，n为$s$中保留的路径层数，path为替换的目录，ext为替换的後缀。\n",
	"\n",
	"编码步骤：\n",
	"wav输入，经主编码器得到有损音频。原wav音频与有损音频解码後的音频相减得到纠错音频存放到filepath，经扩展编码器得到无损音频。\n",
	"解码步骤：\n",
	"主音频输入，解码文件存放到filepath，扩展音频解码存放到filepath，两者相加，输出到%d即可。\n",
	"生成纠错文件步骤：\n",
	"主解码器解码得到wav或从文件中得到（已被调用方解码），再解码无损音频或从文件中得到（已被调用方解码），两者相差得到纠错音频，经扩展编码器得到纠错文件。\n",
	"\n",
	"几种参数设置：（建议创建一个2~4G大小的内存盘，作为filepath）\n",
	"faac外包编码：%s %d -w faac -q 500 -c $f$ -o $d$ $s$\n",
	"faac:takc编码：%s %d -e faac -q 500 -c $f$ -o $d$ $s$ -ahc_ep takc -e -pMax $m$ $d:~0,-3$tak -ahc_ep sfaad $d$ $n$ -ahc_ep V:\\\n",
	"sfaad:takc解码：%s %d -d sfaad $s$ $n$ -ahc_ep takc -d $s:~0,-3$tak $m$ -ahc_ep V:\\\n",
	"sfaad:takc[:ffmpeg]生成纠错文件1：%s %d -c takc -e -pMax $n$ $d$ -ahc_ep \"-s1.flac C:\\\\\" -- sfaad $s$ $m$ -ahc_ep V:\\\n",
	"sfaad:takc[:ffmpeg]生成纠错文件2：%s %d -C takc -e -pMax $n$ $d$ -ahc_ep -- sfaad $d:~0,-3$m4a $m$ -ahc_ep V:\\\n",
	"\n",
	"注意：命令行参数中（只有'\"'和'\\'是特殊字符，其他不变），以空格分开多个参数，如果参数中包含空格请用英文双引号括起来。如果英文双引号括起来的最後一个字符是'\\'，请用'\\\\'代替之。'\\'的作用是在参数中转义英文双引号，而'\\\\'的作用是使其後的英文双引号不被转义。\n",
	"\n"
);

_TCHAR strVchannels[4], strVfband[12], strVsample[12], strVbits[4];

INIT_SIMD_SET;

//以下五个函数的count参数应小于等于MEM_SIZE，因为在createCorrectAudio和createHybridAudio函数中只为之准备MEM_SIZE的缓冲区
//#define MEM_SIZE (1<<20)//定义于instruction_func.h
SET_SSE_AVX_FUNCTION(void, correct_audio, (int*, int*, int*));
SET_SSE_AVX_FUNCTION(void, hybrid_audio, (int*, int*, int*));
SET_SSE_AVX_FUNCTION(void, float_int8, (float*));
SET_SSE_AVX_FUNCTION(void, float_int16, (float*));
SET_SSE_AVX_FUNCTION(void, float_int24, (float*));

SET_SSE_AVX_FUNCTION(void, correct_audio_c, (int*, int*, int*, size_t));
SET_SSE_AVX_FUNCTION(void, hybrid_audio_c, (int*, int*, int*, size_t));
SET_SSE_AVX_FUNCTION(void, float_int8_c, (float*, size_t));
SET_SSE_AVX_FUNCTION(void, float_int16_c, (float*, size_t));
SET_SSE_AVX_FUNCTION(void, float_int24_c, (float*, size_t));

void set_variable(_TCHAR *wavFileName)
{//设置变量
	WAV_INFO *wav = wavInputOpen(wavFileName);
	if (wav == NULL)return;
	_swprintf(strVchannels, _LT("%d"), wav->header.numChannels);
	_swprintf(strVfband, _LT("%d"), wav->header.sampleRate/2);
	_swprintf(strVsample, _LT("%d"), wav->header.sampleRate);
	_swprintf(strVbits, _LT("%d"), wav->header.bitsPerSample);
	SetEnvironmentVariable(_LT("c"), strVchannels);//设置声道数变量
	SetEnvironmentVariable(_LT("f"), strVfband);//设置频率带宽变量
	SetEnvironmentVariable(_LT("r"), strVsample);//设置采样率变量
	SetEnvironmentVariable(_LT("t"), strVbits);//设置位深变量
	fclose(wav->fp);
	free(wav);
}

void (*float_int[4])(float*) = { NULL, float_int8, float_int16, float_int24 };
void (*float_int_c[4])(float*, size_t) = { NULL, float_int8_c, float_int16_c, float_int24_c };

void pcm_int2(char * __restrict pcm, int * __restrict data, int ms, size_t count) {
	int i;
	switch (ms)
	{
	case 2:
		while (count-- != 0){
			ms = *((short*)pcm);
			*(data++) = ms << 8; pcm += 2;
		}
		break;
	case 3:
		while (count-- != 0){
			ms = *((unsigned short*)pcm);
			pcm += 2;
			ms |= (int)(*(pcm++)) << 16;
			*(data++) = ms;
		}
		break;
	case 1:
		while (count-- != 0){
			ms = (char)(*(pcm++) ^ 0x80);
			*(data++) = ms << 16;
		}
		break;
	case 4:
		while (count-- != 0){
			ms = *(int*)pcm; pcm += 4;
			i = ms >> 31;
			ms = (ms ^ i) - i;
			ms += (ms & 0x100) >> 1;//向偶数舍入
			ms = (ms ^ i) - i;
			*(data++) = ms >> 8;
		}
		break;
	case 0:
	default:
		return;
	}
	/* 这是不周到的，没处理好8位和32位PCM数据
	pcm += ms * count;
	data += count;
	while (count-- != 0){
		int ret = (int)*(--pcm) << 16;
		if (ms > 1) ret |= (unsigned)(unsigned char)*(--pcm) << 8;
		if (ms > 2) ret |= (unsigned)(unsigned char)*(--pcm);
		*(--data) = ret;
	}*/
}
void pcm_int(char * __restrict pcm, int * __restrict data, int ms, size_t count) {
#if 1
	switch (ms){
	case 2:
		while (count-- != 0){
			*(data++) = *((short*)pcm);
			pcm += 2;
		}
		break;
	case 3:
		while (count-- != 0){
			ms = *((unsigned short*)pcm);
			pcm += 2;
			ms |= (int)(*(pcm++)) << 16;
			*(data++) = ms;
		}
		break;
	case 1://8位PCM是无符号的
		while (count-- != 0) *(data++) = (char)(*(pcm++) ^ 0x80);
		break;
	case 4:
		memcpy(pcm, data, count << 2);
		break;
	case 0:
	default:
		return;
	}
#else
	pcm += ms * count;
	data += count;
	while (count-- != 0){
		int i = ms;
		pcm -= ms;
		int ret = (int)pcm[--i];
		while (i > 0) ret = (ret << 8) | (unsigned char)pcm[--i];//char转为unsigned会先转为int再转unsigned，迷惑
		*(--data) = ret;
	}
#endif
}
void int_pcm(char * __restrict pcm, int * __restrict data, int ms, size_t count) {
	switch (ms){
	case 2:
		while (count-- != 0){
			*(short*)pcm = *(data++);
			pcm += 2;
		}
		break;
	case 3:
		while (count-- != 0){
			ms = *(data++);
			*(short*)pcm = ms;
			pcm += 2;
			*(pcm++) = ms >> 16;
		}
		break;
	case 1://8位PCM是无符号的
		while (count-- != 0) *(pcm++) = (char)(*(data++) ^ 0x80);
		break;
	case 4:
		memcpy(pcm, data, count << 2);
		break;
	case 0:
	default:
		return;
	}
}
int check_pcm(int * __restrict data, int ms, size_t count) {
	int m2s = 1 << (ms << 3);
	ms = m2s >> 1;
	while (count-- != 0){
		if ((unsigned)(*(data++) + ms) >= (unsigned)m2s) {
			return -1;
		}
	}
	return 0;
}

/**
 * originalAudioName - mainAudioName = correctAudioName
**/
int createCorrectAudio(_TCHAR *originalAudioName, _TCHAR *mainAudioName, _TCHAR *correctAudioName)
{//不考虑浮点数据超限，只要求最终结果不超限
	int orgbytes, mainbytes, crtbytes;
	size_t count;
	WAV_INFO *wav, *wav_m;
	wav = wavInputOpen(originalAudioName);
	if (wav == NULL) {
		_tprintf(_LT("can\'t open \"", _t, "\" for a wav file!\n"), originalAudioName);
		return -1;
	}
	wav_m= wavInputOpen(mainAudioName);
	if (wav_m == NULL) {
		_tprintf(_LT("can\'t open \"", _t, "\" for a wav file!\n"), mainAudioName);
		fclose(wav->fp);
		free(wav);
		return -1;
	}
	count = wav->header.dataSize;
	if (count == 0){
		_tprintf(_LT("the pcm data is null!: o=\"", _t, "\"\n"), originalAudioName);
		fclose(wav->fp);
		free(wav);
		fclose(wav_m->fp);
		free(wav_m);
		return -2;
	}
	orgbytes = wav->header.bitsPerSample >> 3;
	mainbytes = wav_m->header.bitsPerSample >> 3;
	count /= orgbytes;
	FILE *fp = _tfopen(correctAudioName, _LT("wb"));
	wav->header.formatSize = 16;
	wav->header.compressionCode = 1;
	if (orgbytes == 4) {//浮点类型需转换为定点类型
		wav->header.bitsPerSample = 24;
		wav->header.blockAlign = 6;
		_tprintf(_LT("this is a float wav file for that can\'t create correction file correctly!: ", _t, "\n"), originalAudioName);
		fwrite(&(wav->header), 1, sizeof(WAV_HEADER), fp);
		if (mainbytes == 4) {
			if (wav_m->header.dataSize >> 2 < count) {
				_tprintf(_LT("the time length not match!: o=\"", _t, "\" m=\"", _t, "\"\n"), originalAudioName, mainAudioName);
				fclose(fp);
				_tremove(correctAudioName);
				fclose(wav->fp);
				free(wav);
				fclose(wav_m->fp);
				free(wav_m);
				return -2;
			}
		}
		else {
			if (wav_m->header.dataSize / mainbytes < count) {
				_tprintf(_LT("the time length not match!: o=\"", _t, "\" m=\"", _t, "\"\n"), originalAudioName, mainAudioName);
				fclose(fp);
				_tremove(correctAudioName);
				fclose(wav->fp);
				free(wav);
				fclose(wav_m->fp);
				free(wav_m);
				return -2;
			}
		}
	}
	else {
		fwrite(&(wav->header), 1, sizeof(WAV_HEADER), fp);
		if (mainbytes == 4) {
			if (wav_m->header.dataSize >> 2 < count) {
				_tprintf(_LT("the time length not match!: o=\"", _t, "\" m=\"", _t, "\"\n"), originalAudioName, mainAudioName);
				fclose(fp);
				_tremove(correctAudioName);
				fclose(wav->fp);
				free(wav);
				fclose(wav_m->fp);
				free(wav_m);
				return -2;
			}
		}
		else {
			if (mainbytes != orgbytes) {
				_tprintf(_LT("the bitsPerSample not match!: o=\"", _t, "\" m=\"", _t, "\"\n"), originalAudioName, mainAudioName);
				fclose(fp);
				_tremove(correctAudioName);
				fclose(wav->fp);
				free(wav);
				fclose(wav_m->fp);
				free(wav_m);
				return -2;
			}
			if (wav_m->header.dataSize / mainbytes < count) {
				_tprintf(_LT("the time length not match!: o=\"", _t, "\" m=\"", _t, "\"\n"), originalAudioName, mainAudioName);
				fclose(fp);
				_tremove(correctAudioName);
				fclose(wav->fp);
				free(wav);
				fclose(wav_m->fp);
				free(wav_m);
				return -2;
			}
		}
	}
	int *data, *main_data;
	char *pcm, *main_pcm;
	pcm = (char*)malloc_align(orgbytes * (MEM_SIZE >> 2));
	main_pcm = (char*)malloc_align(mainbytes * (MEM_SIZE >> 2));
	if (orgbytes != 4){
		crtbytes = orgbytes;
		data = (int*)malloc_align(MEM_SIZE);
	}
	else{
		crtbytes = 3;
		data = (int*)pcm;
	}
	if (mainbytes != 4) main_data = (int*)malloc_align(MEM_SIZE);
	else main_data = (int*)main_pcm;
	//开始循环
	while (count > (MEM_SIZE >> 2)){
		fread(pcm, 1, orgbytes * (MEM_SIZE >> 2), wav->fp);
		fread(main_pcm, 1, mainbytes * (MEM_SIZE >> 2), wav_m->fp);
		if (orgbytes != 4) pcm_int(pcm, data, orgbytes, (MEM_SIZE >> 2));
		else float_int[3]((float*)data);
		if (mainbytes != 4){
			if (orgbytes == 4) pcm_int2(main_pcm, main_data, mainbytes, (MEM_SIZE >> 2));
			else pcm_int(main_pcm, main_data, mainbytes, (MEM_SIZE >> 2));
		}
		else float_int[crtbytes]((float*)main_data);
		correct_audio(data, data, main_data);
		if (check_pcm(data, crtbytes, (MEM_SIZE >> 2)) != 0){
			_tprintf(_LT("CorrectAudio data out of limits, c=\"", _t, "\"!\n"), correctAudioName);
			fclose(fp);
			_tremove(correctAudioName);
			goto CORRECT_FAIL;
		}
		int_pcm((char*)main_data, data, crtbytes, (MEM_SIZE >> 2));
		fwrite(main_data, 1, crtbytes * (MEM_SIZE >> 2), fp);
		count -= (MEM_SIZE >> 2);
	}//结束循环
	fread(pcm, 1, orgbytes * count, wav->fp);
	fread(main_pcm, 1, mainbytes * count, wav_m->fp);
	if (orgbytes != 4) pcm_int(pcm, data, orgbytes, count);
	else float_int_c[3]((float*)data, count);
	if (mainbytes != 4){
		if (orgbytes == 4) pcm_int2(main_pcm, main_data, mainbytes, count);
		else pcm_int(main_pcm, main_data, mainbytes, count);
	}
	else float_int_c[crtbytes]((float*)main_data, count);
	correct_audio_c(data, data, main_data, count);
	if (check_pcm(data, crtbytes, count) != 0){
		_tprintf(_LT("CorrectAudio data out of limits, c=\"", _t, "\"!\n"), correctAudioName);
		fclose(fp);
		_tremove(correctAudioName);
		goto CORRECT_FAIL;
	}
	int_pcm((char*)main_data, data, crtbytes, count);
	fwrite(main_data, 1, crtbytes * count, fp);
	fpos_t fpos;
	fgetpos(fp, &fpos);
	wav->header.riffSize = (*(uint64_t*)&fpos) - 8;
	wav->header.bytesPerSecond = (wav->header.bitsPerSample >> 3) * wav->header.sampleRate * wav->header.numChannels;
	wav->header.dataSize = (*(uint64_t*)&fpos) - sizeof(WAV_HEADER);
	fseek(fp, 0, SEEK_SET);
	fwrite(&(wav->header), sizeof(WAV_HEADER), 1, fp);
	fclose(fp);
	free_align(pcm);
	free_align(main_pcm);
	if (orgbytes != 4) free_align(data);
	if (mainbytes != 4) free_align(main_data);
	fclose(wav->fp);
	free(wav);
	fclose(wav_m->fp);
	free(wav_m);
	return 0;
CORRECT_FAIL:
	free_align(pcm);
	free_align(main_pcm);
	if (orgbytes != 4) free_align(data);
	if (mainbytes != 4) free_align(main_data);
	fclose(wav->fp);
	free(wav);
	fclose(wav_m->fp);
	free(wav_m);
	return 4;
}

/**
 * originalAudioName = mainAudioName + correctAudioName
**/
int createHybridAudio(_TCHAR *originalAudioName, _TCHAR *mainAudioName, _TCHAR *correctAudioName)
{
	int mainbytes, crtbytes;
	size_t count;
	WAV_INFO *wav, *wav_m;
	wav = wavInputOpen(correctAudioName);
	if (wav == NULL) {
		_tprintf(_LT("can\'t open ", _t, " for a wav file!\n"), correctAudioName);
		return -1;
	}
	wav_m = wavInputOpen(mainAudioName);
	if (wav_m == NULL) {
		_tprintf(_LT("can\'t open ", _t, " for a wav file!\n"), mainAudioName);
		fclose(wav->fp);
		free(wav);
		return -1;
	}
	count = wav->header.dataSize;
	crtbytes = wav->header.bitsPerSample >> 3;
	if (count == 0 || crtbytes == 4){
		_tprintf(_LT("the pcm data is null or bitsPerSample(32) is not suitable!: c=\"", _t, "\"\n"), correctAudioName);
		fclose(wav->fp);
		free(wav);
		fclose(wav_m->fp);
		free(wav_m);
		return -2;
	}
	mainbytes = wav_m->header.bitsPerSample >> 3;
	count /= crtbytes;
	FILE *fp = _tfopen(originalAudioName, _LT("wb+"));
	wav->header.formatSize = 16;
	wav->header.compressionCode = 1;
	if (count > wav_m->header.dataSize / mainbytes) {
		_tprintf(_LT("the time length not match!: c=\"", _t, "\" m=\"", _t, "\"\n"), correctAudioName, mainAudioName);
		fclose(fp);
		_tremove(originalAudioName);
		fclose(wav->fp);
		free(wav);
		fclose(wav_m->fp);
		free(wav_m);
		return -2;
	}
	fwrite(&(wav->header), sizeof(WAV_HEADER), 1, fp);
	if (crtbytes != 3){
		if (mainbytes != 4){
			if (crtbytes != mainbytes){
				_tprintf(_LT("the bitsPerSample not match!: c=\"", _t, "\" m=\"", _t, "\"\n"), correctAudioName, mainAudioName);
				fclose(fp);
				_tremove(originalAudioName);
				fclose(wav->fp);
				free(wav);
				fclose(wav_m->fp);
				free(wav_m);
				return -2;
			}
		}
	}
	int *data, *main_data;
	char *pcm, *main_pcm;
	pcm = (char*)malloc_align(crtbytes * (MEM_SIZE >> 2));
	main_pcm = (char*)malloc_align(mainbytes * (MEM_SIZE >> 2));
	data = (int*)malloc_align(MEM_SIZE);
	if (mainbytes != 4) main_data = (int*)malloc_align(MEM_SIZE);
	else main_data = (int*)main_pcm;
	//开始循环
	while (count > (MEM_SIZE >> 2)){
		fread(pcm, 1, crtbytes * (MEM_SIZE >> 2), wav->fp);
		fread(main_pcm, 1, mainbytes * (MEM_SIZE >> 2), wav_m->fp);
		pcm_int(pcm, data, crtbytes, (MEM_SIZE >> 2));
		if (mainbytes != 4){
			if (mainbytes != crtbytes) pcm_int2(main_pcm, main_data, mainbytes, (MEM_SIZE >> 2));
			else pcm_int(main_pcm, main_data, mainbytes, (MEM_SIZE >> 2));
		}
		else float_int[crtbytes]((float*)main_data);
		hybrid_audio(data, data, main_data);
		if (check_pcm(data, crtbytes, (MEM_SIZE >> 2)) != 0){
			_tprintf(_LT("HybridAudio data out of limits, o=\"", _t, "\"!\n"), originalAudioName);
			fclose(fp);
			_tremove(originalAudioName);
			goto HYBRID_FAIL;
		}
		int_pcm(pcm, data, crtbytes, (MEM_SIZE >> 2));
		fwrite(pcm, 1, crtbytes * (MEM_SIZE >> 2), fp);
		count -= (MEM_SIZE >> 2);
	}//结束循环
	fread(pcm, 1, crtbytes * count, wav->fp);
	fread(main_pcm, 1, mainbytes * count, wav_m->fp);
	pcm_int(pcm, data, crtbytes, count);
	if (mainbytes != 4){
		if (mainbytes != crtbytes) pcm_int2(main_pcm, main_data, mainbytes, count);
		else pcm_int(main_pcm, main_data, mainbytes, count);
	}
	else float_int_c[crtbytes]((float*)main_data, count);
	hybrid_audio_c(data, data, main_data, count);
	if (check_pcm(data, crtbytes, count) != 0){
		_tprintf(_LT("HybridAudio data out of limits, o=\"", _t, "\"!\n"), originalAudioName);
		fclose(fp);
		_tremove(originalAudioName);
		goto HYBRID_FAIL;
	}
	int_pcm(pcm, data, crtbytes, count);
	fwrite(pcm, 1, crtbytes * count, fp);
	fpos_t fpos;
	fgetpos(fp, &fpos);
	wav->header.riffSize = (*(uint64_t*)&fpos) - 8;
	fseek(fp, 0, SEEK_SET);
	fwrite(&(wav->header), sizeof(WAV_HEADER), 1, fp);
	fclose(fp);
	free_align(pcm);
	free_align(main_pcm);
	free_align(data);
	if (mainbytes != 4) free_align(main_data);
	fclose(wav->fp);
	free(wav);
	fclose(wav_m->fp);
	free(wav_m);
	return 0;
HYBRID_FAIL:
	free_align(pcm);
	free_align(main_pcm);
	free_align(data);
	if (mainbytes != 4) free_align(main_data);
	fclose(wav->fp);
	free(wav);
	fclose(wav_m->fp);
	free(wav_m);
	return 4;
}

void printcmd(_TCHAR *str) {
	_TCHAR cmd[8192];
	_stprintf(cmd, _LT("echo ", _t), str);
	_tsystem(cmd);
}

extern "C" int _tmain(int argc, _TCHAR *argv[])
{
	//_tfreopen(_LT("C:\\Users\\yang\\source\\repos\\audio_hybrid\\print.txt"), _LT("w"), stdout);
	//_tfreopen(_LT("C:\\Users\\yang\\source\\repos\\audio_hybrid\\print.txt"), _LT("w"), stderr);
	int i,j,k,ret;
	_TCHAR sourceName[4096], destName[4096], tempN[4096], tempM[4096], sourceDirex[4096], str1[8192], str2[8192], str3[8192], curDir[4096], *pos, *p, *pp;
	setlocale(LC_CTYPE, "chs");
	if (argc == 1) {
		goto TEST_HELP;
	}
	if (_tcscmp(argv[1], _LT("-h")) == 0 || _tcscmp(argv[1], _LT("--help")) == 0) {
TEST_HELP:
		_tprintf(_LT(_t),HELP_INFO);
		int level;
#if defined(__GNUC__)
#if defined(_WIN64) && defined(__GNUC__)
		level = 4;
#endif
#elif defined(_MSC_VER)
#ifndef __SIMD_LV__
		level = InstructionIs::crowd_level;
#else
		level = 0;
#if __SIMD_LV__ >= V__SSE2__
		level++;
#endif
#if __SIMD_LV__ >= V__AVX2__
		level++;
#endif
#endif
#endif
#ifdef _WIN64
		_tprintf(_LT("本程序是64位版本\n"));
#else
		_tprintf(_LT("本程序是32位版本\n"));
#endif
		switch (level)
		{
		case 0:
#ifdef _WIN64
			_tprintf(_LT("未使用特定指令集\n"));
#else
			_tprintf(_LT("未使用高级指令集\n"));
#endif
			break;
		case 1:
			_tprintf(_LT("使用SSE2指令集\n"));
			break;
		case 2:
			_tprintf(_LT("使用AVX2指令集\n"));
			break;
		case 4:
			_tprintf(_LT("使用MingGW_W64，使用SSE2指令集\n"));
			break;
		default:
			break;
		}
		return 0;
	}
	GetCurrentDirectory(4096, curDir);
	_tcscpy(sourceName, argv[1]);
	SetEnvironmentVariable(_LT("s"), sourceName);//设置输入文件变量
	_tcscpy(destName, argv[2]);
	SetEnvironmentVariable(_LT("d"), destName);//设置输出文件变量
	j = 0; i = 3;
	if (_tcscmp(argv[i], _LT("-w"))==0) {//外包编解码，无问题
		_tcscpy(str1, argv[++i]);
		while (++i < argc) {
			_tcscat(str1, _LT(" \""));
			_tcscat(str1, argv[i]);
			_tcscat(str1, _LT("\""));
		}
		set_variable(sourceName);
		pos = str1; REPLACE(pos, _LT('$'), _LT('%'));
		//printcmd(str1);
		SetCurrentDirectory(curDir);
		ret = _wsystem(str1);
		if (ret != 0) _tremove(destName);
		return ret;
	}
	if (_tcscmp(argv[i], _LT("-e")) == 0) {//编码
		if (_tcsncmp(argv[++i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
		if (j == 0) {
			_tcscpy(str1, argv[i++]);
			while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
				_tcscat(str1, _LT(" \""));
				_tcscat(str1, argv[i++]);
				_tcscat(str1, _LT("\""));
			}
			i++;
		}
		else {
			_tcscpy(str1, argv[i++]);
			while (--j != 0) {
				_tcscat(str1, _LT(" \""));
				_tcscat(str1, argv[i++]);
				_tcscat(str1, _LT("\""));
			}
			if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
		}
		if (_tcsncmp(argv[i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
		if (j == 0) {
			_tcscpy(str2, argv[i++]);
			while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
				_tcscat(str2, _LT(" \""));
				_tcscat(str2, argv[i++]);
				_tcscat(str2, _LT("\""));
			}
			i++;
		}
		else {
			_tcscpy(str2, argv[i++]);
			while (--j != 0) {
				_tcscat(str2, _LT(" \""));
				_tcscat(str2, argv[i++]);
				_tcscat(str2, _LT("\""));
			}
			if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
		}
		if (i < argc - 1) {
			if (_tcsncmp(argv[i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
			if (j == 0) {
				_tcscpy(str3, argv[i++]);
				while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
					_tcscat(str3, _LT(" \""));
					_tcscat(str3, argv[i++]);
					_tcscat(str3, _LT("\""));
				}
				i++;
			}
			else {
				_tcscpy(str3, argv[i++]);
				while (--j != 0) {
					_tcscat(str3, _LT(" \""));
					_tcscat(str3, argv[i++]);
					_tcscat(str3, _LT("\""));
				}
				if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
			}
			if (i != argc - 1) {
				_tprintf(_LT("Command line parameter error index %d!\n"), i);
				return -1;
			}
		}
		else *str3 = 0;
		pos = tempN;
		pp = tempM;
		p = argv[i];
		while (*p != 0)*(pp++) = *(pos++) = *(p++);//复制filepath
		if (pos[-1] != _LT('\\'))*(pp++) = *(pos++) = _LT('\\');
		p = _tcschr(sourceName, _LT('\\')) + 1;
		while ((k=*p) != 0) {
			if (k == _LT('\\')) k = _LT('.');
			*(pp++) = *(pos++) = k; p++;
		}
		_tcscpy(pos, _LT("_ahc_N.wav"));
		_tcscpy(pp, _LT("_ahc_M.wav"));
		SetEnvironmentVariable(_LT("n"), tempN);//设置临时文件N变量
		SetEnvironmentVariable(_LT("m"), tempM);//设置临时文件M变量
		set_variable(sourceName);
		pos = str1; REPLACE(pos, _LT('$'), _LT('%'));
		pos = str2; REPLACE(pos, _LT('$'), _LT('%'));
		pos = str3; REPLACE(pos, _LT('$'), _LT('%'));
		/* 不用进行文件锁操作
        //HANDLE fhnd = CreateFile(sourceName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD fileSizeHigh, fileSizeLow;
		OVERLAPPED lapped = { 0 };
        fileSizeLow = GetFileSize(fhnd, &fileSizeHigh);
        if (LockFileEx(fhnd, 0, 0, fileSizeLow, fileSizeHigh, &lapped) == FALSE) _tprintf(_LT("LockFileEx failure!\n"));
		*/
		SetCurrentDirectory(curDir);
		ret = _tsystem(str1);
		if (*str3 == 0)_stprintf(str3, _LT("ffmpeg -i \"", _t, "\" -f wav \"", _t, "\""), destName, tempN);
		ret |= _tsystem(str3);
		DWORD begin = GetTickCount();
		ret |= createCorrectAudio(sourceName,tempN,tempM);
		_tprintf(_LT("createHybridAudio cost %f sec.\n"), (GetTickCount() - begin) / 1000.0f);
		ret |= _tsystem(str2);
        //if (UnlockFileEx(fhnd, 0, fileSizeLow, fileSizeHigh, &lapped) == FALSE) _tprintf(_LT("UnlockFileEx failure!\n"));
        //CloseHandle(fhnd);
		_tremove(tempN); _tremove(tempM);
		if (ret != 0) _tremove(destName);
		return ret;
	}
	if (_tcscmp(argv[i], _LT("-d")) == 0) {//解码
		pos = tempN;
		pp = tempM;
		p = argv[argc - 1];
		while (*p != 0)*(pp++) = *(pos++) = *(p++);//复制filepath
		if (pos[-1] != _LT('\\'))*(pp++) = *(pos++) = _LT('\\');
		p = _tcschr(sourceName, _LT('\\')) + 1;
		while ((k = *p) != 0) {
			if (k == _LT('\\')) k = _LT('.');
			*(pp++) = *(pos++) = k; p++;
		}
		_tcscpy(pos, _LT("_ahc_N.wav"));
		_tcscpy(pp, _LT("_ahc_M.wav"));
		SetEnvironmentVariable(_LT("n"), tempN);//设置临时文件N变量
		SetEnvironmentVariable(_LT("m"), tempM);//设置临时文件M变量
		if (_tcsncmp(argv[++i], _LT("--"), 2) == 0) {
			_stprintf(str1, _LT("ffmpeg -i \"", _t, "\" -f wav \"", _t, "\""), sourceName, tempN);
			i++;
		}
		else {
			if (_tcsncmp(argv[i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
			if (j == 0) {
				_tcscpy(str1, argv[i++]);
				while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
					_tcscat(str1, _LT(" \""));
					_tcscat(str1, argv[i++]);
					_tcscat(str1, _LT("\""));
				}
				i++;
			}
			else {
				_tcscpy(str1, argv[i++]);
				while (--j != 0) {
					_tcscat(str1, _LT(" \""));
					_tcscat(str1, argv[i++]);
					_tcscat(str1, _LT("\""));
				}
				if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
			}
		}
		if (_tcsncmp(argv[i], _LT("--"), 2) == 0)_stprintf(str2, _LT("ffmpeg -i \"", _t, "\" -f wav \"", _t, "\""), argv[i++] + 2, tempM);
		else {
			if (_tcsncmp(argv[i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
			if (j == 0) {
				_tcscpy(str2, argv[i++]);
				while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
					_tcscat(str2, _LT(" \""));
					_tcscat(str2, argv[i++]);
					_tcscat(str2, _LT("\""));
				}
				i++;
			}
			else {
				_tcscpy(str2, argv[i++]);
				while (--j != 0) {
					_tcscat(str2, _LT(" \""));
					_tcscat(str2, argv[i++]);
					_tcscat(str2, _LT("\""));
				}
				if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
			}
		}
		if (i != argc - 1) {
			_tprintf(_LT("Command line parameter error index %d!\n"), i);
			return -1;
		}
		//set_variable(sourceName);//解码不用设置，不会改变当前目录
		pos = str1; REPLACE(pos, _LT('$'), _LT('%'));
		pos = str2; REPLACE(pos, _LT('$'), _LT('%'));
		ret = _tsystem(str1);
		ret |= _tsystem(str2);
		DWORD begin = GetTickCount();
		ret |= createHybridAudio(destName, tempN, tempM);
		_tprintf(_LT("createHybridAudio cost %f sec.\n"), (GetTickCount() - begin) / 1000.0f);
		_tremove(tempN); _tremove(tempM);
		if (ret != 0) _tremove(destName);
		return ret;
	}
	if (_tcscmp(argv[i], _LT("-c")) == 0) {//生成纠错文件1
		pos = tempN;
		pp = tempM;
		p = argv[argc - 1];
		while (*p != 0)*(pp++) = *(pos++) = *(p++);//复制filepath
		if (pos[-1] != _LT('\\'))*(pp++) = *(pos++) = _LT('\\');
		p = _tcschr(sourceName, _LT('\\')) + 1;
		while ((k = *p) != 0) {
			if (k == _LT('\\')) k = _LT('.');
			*(pp++) = *(pos++) = k; p++;
		}
		_tcscpy(pos, _LT("_ahc_N.wav"));
		_tcscpy(pp, _LT("_ahc_M.wav"));
		SetEnvironmentVariable(_LT("n"), tempN);//设置临时文件N变量
		SetEnvironmentVariable(_LT("m"), tempM);//设置临时文件M变量
		if (_tcsncmp(argv[++i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
		if (j == 0) {
			_tcscpy(str1, argv[i++]);
			while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
				_tcscat(str1, _LT(" \""));
				_tcscat(str1, argv[i++]);
				_tcscat(str1, _LT("\""));
			}
			i++;
		}
		else {
			_tcscpy(str1, argv[i++]);
			while (--j != 0) {
				_tcscat(str1, _LT(" \""));
				_tcscat(str1, argv[i++]);
				_tcscat(str1, _LT("\""));
			}
			if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
		}
		if (_tcsncmp(argv[i], _LT("-s"), 2) == 0) {//计算用于生成纠错文件的变量sourceDirex，是输入文件对应的无损文件名
			_TCHAR *tp,*mp;
			k = _tcstol(argv[i++] + 2, &pp, 0);
			p = pos = sourceName + _tcslen(sourceName);
			while (*(--p) != _LT('.'));
			while (k != 0) {
				while (*(--pos) != _LT('\\'));
				k--;
			}//往上溯得到多级文件名
			if (*pp == _LT('.'))pp++;
			tp = sourceDirex;
			mp = _tcschr(pp, _LT(' ')) + 1;
			STRCPY(tp, mp);
			if (tp[-1] == _LT('\\'))tp--;
			while (pos != p)*(tp++) = *(pos++);
			*(tp++) = *pos;
			while (*pp != _LT(' '))*(tp++) = *(pp++);
			*tp = 0;
			SetEnvironmentVariable(_LT("sd"), sourceDirex);//设置sourceDirex
		}
		if (_tcsncmp(argv[i], _LT("--"), 2) == 0) {
			if (argv[i][2] == 0)pos = sourceDirex;
			else pos = argv[i] + 2;
			i++;
			if (_tcscmp(pos + _tcslen(pos) - 4, _LT(".wav")) == 0) {
				*str2 = 0;
				_tcscpy(str2 + 1, pos);
			}
			else _stprintf(str2, _LT("ffmpeg -i \"", _t, "\" -f wav \"", _t, "\""), pos, tempN);
		}
		else {
			if (_tcsncmp(argv[i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
			if (j == 0) {
				_tcscpy(str2, argv[i++]);
				while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
					_tcscat(str2, _LT(" \""));
					_tcscat(str2, argv[i++]);
					_tcscat(str2, _LT("\""));
				}
				i++;
			}
			else {
				_tcscpy(str2, argv[i++]);
				while (--j != 0) {
					_tcscat(str2, _LT(" \""));
					_tcscat(str2, argv[i++]);
					_tcscat(str2, _LT("\""));
				}
				if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
			}
		}
		if (_tcsncmp(argv[i], _LT("--"), 2) == 0) {
			i++;
			if (_tcscmp(sourceName + _tcslen(sourceName) - 4, _LT(".wav")) == 0) {
				*str3 = 0;
				_tcscpy(str3 + 1, sourceName);
			}
			else _stprintf(str3, _LT("ffmpeg -i \"", _t, "\" -f wav \"", _t, "\""), sourceName, tempM);
		}
		else {
			if (_tcsncmp(argv[i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
			if (j == 0) {
				_tcscpy(str3, argv[i++]);
				while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
					_tcscat(str3, _LT(" \""));
					_tcscat(str3, argv[i++]);
					_tcscat(str3, _LT("\""));
				}
				i++;
			}
			else {
				_tcscpy(str3, argv[i++]);
				while (--j != 0) {
					_tcscat(str3, _LT(" \""));
					_tcscat(str3, argv[i++]);
					_tcscat(str3, _LT("\""));
				}
				if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
			}
		}
		if (i != argc - 1) {
			_tprintf(_LT("Command line parameter error index %d!\n"), i);
			return -1;
		}
		if (*str2 != 0) {
			pos = str2; REPLACE(pos, _LT('$'), _LT('%'));
			ret = _tsystem(str2);
		}
		else {
			_tcscpy(tempN, str2 + 1);
			ret = 0;
		}
		set_variable(tempN);
		SetCurrentDirectory(curDir);
		DWORD begin = GetTickCount();
		if (*str3 != 0) {
			pos = str3; REPLACE(pos, _LT('$'), _LT('%'));
			ret |= _tsystem(str3);
			_tcscpy(curDir, tempM);
			curDir[_tcslen(curDir) - 5] = _LT('C');
			SetEnvironmentVariable(_LT("n"), curDir);//重新设置临时文件N变量
			ret |= createCorrectAudio(tempN, tempM, curDir);
		}
		else {
			ret |= createCorrectAudio(tempN, str3 + 1, tempM);
			SetEnvironmentVariable(_LT("n"), tempM);//设置临时文件N变量
		}
		_tprintf(_LT("createHybridAudio cost %f sec.\n"), (GetTickCount() - begin) / 1000.0f);
		pos = str1; REPLACE(pos, _LT('$'), _LT('%'));
		ret |= _tsystem(str1);
		if (*str2 != 0)_tremove(tempN);
		if (*str3 != 0) { _tremove(tempM); _tremove(curDir); }
		if (ret != 0) _tremove(destName);
		return ret;
	}
	if (_tcscmp(argv[i], _LT("-C")) == 0) {//生成纠错文件2
		pos = tempN;
		pp = tempM;
		p = argv[argc - 1];
		while (*p != 0)*(pp++) = *(pos++) = *(p++);//复制filepath
		if (pos[-1] != _LT('\\'))*(pp++) = *(pos++) = _LT('\\');
		p = _tcschr(sourceName, _LT('\\')) + 1;
		while ((k = *p) != 0) {
			if (k == _LT('\\')) k = _LT('.');
			*(pp++) = *(pos++) = k; p++;
		}
		_tcscpy(pos, _LT("_ahc_N.wav"));
		_tcscpy(pp, _LT("_ahc_M.wav"));
		SetEnvironmentVariable(_LT("n"), tempN);//设置临时文件N变量
		SetEnvironmentVariable(_LT("m"), tempM);//设置临时文件M变量
		if (_tcsncmp(argv[++i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
		if (j == 0) {
			_tcscpy(str1, argv[i++]);
			while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
				_tcscat(str1, _LT(" \""));
				_tcscat(str1, argv[i++]);
				_tcscat(str1, _LT("\""));
			}
			i++;
		}
		else {
			_tcscpy(str1, argv[i++]);
			while (--j != 0) {
				_tcscat(str1, _LT(" \""));
				_tcscat(str1, argv[i++]);
				_tcscat(str1, _LT("\""));
			}
			if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
		}
		if (_tcsncmp(argv[i], _LT("-s"), 2) == 0) {//计算用于生成纠错文件的变量sourceDirex，是输入文件对应的有损文件名
			_TCHAR *tp, *mp;
			k = _tcstol(argv[i++] + 2, &pp, 0);
			p = pos = sourceName + _tcslen(sourceName);
			while (*(--p) != _LT('.'));
			while (k != 0) {
				while (*(--pos) != _LT('\\'));
				k--;
			}//往上溯得到多级文件名
			if (*pp == _LT('.'))pp++;
			tp = sourceDirex;
			mp = _tcschr(pp, _LT(' ')) + 1;
			STRCPY(tp, mp);
			if (tp[-1] == _LT('\\'))tp--;
			while (pos != p)*(tp++) = *(pos++);
			*(tp++) = *pos;
			while (*pp != _LT(' '))*(tp++) = *(pp++);
			*tp = 0;
			SetEnvironmentVariable(_LT("sd"), sourceDirex);//设置sourceDirex
		}
		if (_tcsncmp(argv[i], _LT("--"), 2) == 0) {
			i++;
			if (_tcscmp(sourceName + _tcslen(sourceName) - 4, _LT(".wav")) == 0) {
				*str2 = 0;
				_tcscpy(str2 + 1, sourceName);
			}
			else _stprintf(str2, _LT("ffmpeg -i \"", _t, "\" -f wav \"", _t, "\""), sourceName, tempN);
		}
		else {
			if (_tcsncmp(argv[i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
			if (j == 0) {
				_tcscpy(str2, argv[i++]);
				while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
					_tcscat(str2, _LT(" \""));
					_tcscat(str2, argv[i++]);
					_tcscat(str2, _LT("\""));
				}
				i++;
			}
			else {
				_tcscpy(str2, argv[i++]);
				while (--j != 0) {
					_tcscat(str2, _LT(" \""));
					_tcscat(str2, argv[i++]);
					_tcscat(str2, _LT("\""));
				}
				if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
			}
		}
		if (_tcsncmp(argv[i], _LT("--"), 2) == 0) {
			if (argv[i][2] == 0)pos = sourceDirex;
			else pos = argv[i] + 2;
			i++;
			if (_tcscmp(pos + _tcslen(pos) - 4, _LT(".wav")) == 0) {
				*str3 = 0;
				_tcscpy(str3 + 1, pos);
			}
			else _stprintf(str3, _LT("ffmpeg -i \"", _t, "\" -f wav \"", _t, "\""), pos, tempM);
		}
		else {
			if (_tcsncmp(argv[i], _LT("-p"), 2) == 0)j = _tcstol(argv[i++] + 2, NULL, 0);
			if (j == 0) {
				_tcscpy(str3, argv[i++]);
				while (_tcscmp(argv[i], _LT("-ahc_ep")) != 0) {
					_tcscat(str3, _LT(" \""));
					_tcscat(str3, argv[i++]);
					_tcscat(str3, _LT("\""));
				}
				i++;
			}
			else {
				_tcscpy(str3, argv[i++]);
				while (--j != 0) {
					_tcscat(str3, _LT(" \""));
					_tcscat(str3, argv[i++]);
					_tcscat(str3, _LT("\""));
				}
				if (_tcscmp(argv[i], _LT("-ahc_ep")) == 0)i++;
			}
		}
		if (i != argc - 1) {
			_tprintf(_LT("Command line parameter error index %d!\n"), i);
			return -1;
		}
		if (*str2 != 0) {
			pos = str2; REPLACE(pos, _LT('$'), _LT('%'));
			ret = _tsystem(str2);
		}
		else {
			_tcscpy(tempN, str2 + 1);
			ret = 0;
		}
		set_variable(tempN);
		SetCurrentDirectory(curDir);
		DWORD begin = GetTickCount();
		if (*str3 != 0) {
			pos = str3; REPLACE(pos, _LT('$'), _LT('%'));
			ret |= _tsystem(str3);
			_tcscpy(curDir, tempM);
			curDir[_tcslen(curDir) - 5] = _LT('C');
			SetEnvironmentVariable(_LT("n"), curDir);//重新设置临时文件N变量
			ret |= createCorrectAudio(tempN, tempM, curDir);
		}
		else {
			ret |= createCorrectAudio(tempN, str3 + 1, tempM);
			SetEnvironmentVariable(_LT("n"), tempM);//设置临时文件N变量
		}
		_tprintf(_LT("createHybridAudio cost %f sec.\n"), (GetTickCount() - begin) / 1000.0f);
		pos = str1; REPLACE(pos, _LT('$'), _LT('%'));
		ret |= _tsystem(str1);
		if (*str2 != 0)_tremove(tempN);
		if (*str3 != 0) { _tremove(tempM); _tremove(curDir); }
		if (ret != 0) _tremove(destName);
		return ret;
	}
	//_tfreopen(_LT("CON"), _LT("w"), stdout); _tfreopen(_LT("CON"), _LT("w"), stderr);
	return 8;
}
