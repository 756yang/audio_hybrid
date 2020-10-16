#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__)
#define __SIMD_LV__ V__NO_SIMD__
#else
//#define __SIMD_LV__ V__NO_SIMD__
#endif
#include "SSE_AVX_C.h"


#define MEM_SIZE (1<<20)


#if !defined(__SIMD_LV__) || __SIMD_LV__ >= V__AVX__
#define __ALIGN_SIZE__ 32
#else
#define __ALIGN_SIZE__ 16
#endif

#ifdef __NO_SIMD__
#if defined(__GNUC__)
#define ROUND(i,d) \
	asm(\
		"fldl %1 \n\t"\
		"fistpl %0 \n\t"\
		:"=m"(i)\
		:"m"(d)\
	)
#define FROUND(i,f) \
	asm(\
		"flds %1 \n\t"\
		"fistpl %0 \n\t"\
		:"=m"(i)\
		:"m"(f)\
	)
#elif defined(_MSC_VER)
#define ROUND(i,d) \
	__asm fld d \
	__asm fistp i
#define FROUND(i,f) ROUND(i,f)
#else
#error platform is unknown for that not ensure float environment!
#endif
#define FTURN(f,i) ((f)=(i))
#else
#if defined(__GNUC__)
//g++,gcc
#define ROUND(i,d) \
    asm(\
		"cvtsd2sil %1,%0 \n\t"\
		:"=r"(i)\
		:"x"(d)\
		)
// 注意，MinGW_W64由于FROUND,FTURN宏所用汇编指令不能用32位g++编译，所以请用64位模式编译。
#define FROUND(i,f) \
    asm(\
		"cvtss2sil %1,%0 \n\t"\
		:"=r"(i)\
		:"x"(f)\
	)
#define FTURN(f,i) \
    asm(\
		"cvtsi2ssl %1,%0 \n\t"\
		:"+x"(f)\
		:"r"(i)\
	)
#elif defined(_MSC_VER)
//msvc++
#include <xmmintrin.h>
#include <emmintrin.h>
#define ROUND(i,d) ((i)=_mm_cvtsd_si32(_mm_load_sd(&d)))
#define FROUND(i,f) ((i)=_mm_cvtss_si32(_mm_load_ss(&f)))
#define FTURN(f,i) ((f)=_mm_cvtss_f32(_mm_cvt_si2ss(_mm_load_ss(&f),i)))
#else
//其他
#define _NO__GNUC__NO_MSC_VER
#warning platform is unknown and you must ensure IEE environment for ROUND, FROUND and FTURN!
#include "typeconvert.h"//需要标准IEEE浮点环境
#define ROUND(i,d) ((i)=convert_to_int32(d))
#define FROUND(i,f) ((i)=convert_to_int32(f))
#define FTURN(f,i) ((f)=convert_to_float(i))
#endif
#endif


void _NO_correct_audio(int * __restrict dest, int * __restrict wav_src, int * __restrict main_src);
void _NO_hybrid_audio(int * __restrict dest, int * __restrict main_src, int * __restrict ext_src);
void _NO_float_int8(float * __restrict fpara);
void _NO_float_int16(float * __restrict fpara);
void _NO_float_int24(float * __restrict fpara);

void _NO_correct_audio_c(int * __restrict dest, int * __restrict wav_src, int * __restrict main_src, size_t count);
void _NO_hybrid_audio_c(int * __restrict dest, int * __restrict main_src, int * __restrict ext_src, size_t count);
void _NO_float_int8_c(float * __restrict fpara, size_t count);
void _NO_float_int16_c(float * __restrict fpara, size_t count);
void _NO_float_int24_c(float * __restrict fpara, size_t count);

#ifdef _MSC_VER//只实现了Intel语法的内嵌汇编
void _SSE2_correct_audio(int * __restrict dest, int * __restrict wav_src, int * __restrict main_src);
void _SSE2_hybrid_audio(int * __restrict dest, int * __restrict main_src, int * __restrict ext_src);
void _SSE2_float_int8(float * __restrict fpara);
void _SSE2_float_int16(float * __restrict fpara);
void _SSE2_float_int24(float * __restrict fpara);

void _SSE2_correct_audio_c(int * __restrict dest, int * __restrict wav_src, int * __restrict main_src, size_t count);
void _SSE2_hybrid_audio_c(int * __restrict dest, int * __restrict main_src, int * __restrict ext_src, size_t count);
void _SSE2_float_int8_c(float * __restrict fpara, size_t count);
void _SSE2_float_int16_c(float * __restrict fpara, size_t count);
void _SSE2_float_int24_c(float * __restrict fpara, size_t count);


void _AVX2_correct_audio(int * __restrict dest, int * __restrict wav_src, int * __restrict main_src);
void _AVX2_hybrid_audio(int * __restrict dest, int * __restrict main_src, int * __restrict ext_src);
void _AVX2_float_int8(float * __restrict fpara);
void _AVX2_float_int16(float * __restrict fpara);
void _AVX2_float_int24(float * __restrict fpara);

void _AVX2_correct_audio_c(int * __restrict dest, int * __restrict wav_src, int * __restrict main_src, size_t count);
void _AVX2_hybrid_audio_c(int * __restrict dest, int * __restrict main_src, int * __restrict ext_src, size_t count);
void _AVX2_float_int8_c(float * __restrict fpara, size_t count);
void _AVX2_float_int16_c(float * __restrict fpara, size_t count);
void _AVX2_float_int24_c(float * __restrict fpara, size_t count);
#endif

void* malloc_align(size_t n);
void free_align(void *p);
