#include "instruction_func.h"

#if defined(_WIN64) && defined(__GNUC__)
#include <xmmintrin.h>
#include <emmintrin.h>
#endif

#ifdef _WIN64
#define R(ax) r##ax
#define R8 r8
#define R9 r9
#else
#define R(ax) e##ax
#define R8 esi
#define R9 edi
#endif


void _NO_correct_audio(int * __restrict dest, int * __restrict wav_src, int * __restrict main_src) {
	_NO_correct_audio_c(dest, wav_src, main_src , MEM_SIZE >> 2);
}
void _NO_hybrid_audio(int * __restrict dest, int * __restrict main_src, int * __restrict ext_src) {
	_NO_hybrid_audio_c(dest, main_src, ext_src, MEM_SIZE >> 2);
}
void _NO_float_int8(float * __restrict fpara) {
	_NO_float_int8_c(fpara, MEM_SIZE >> 2);
}
void _NO_float_int16(float * __restrict fpara) {
	_NO_float_int16_c(fpara, MEM_SIZE >> 2);
}
void _NO_float_int24(float * __restrict fpara) {
	_NO_float_int24_c(fpara, MEM_SIZE >> 2);
}

void _NO_correct_audio_c(int * __restrict dest, int * __restrict wav_src, int * __restrict main_src, size_t count) {
#if defined(_WIN64) && defined(__GNUC__)
	while(count >= 16){
		__m128i x = _mm_load_si128((__m128i*)wav_src); wav_src += 4;
		__m128i x1 = _mm_load_si128((__m128i*)wav_src); wav_src += 4;
		__m128i x2 = _mm_load_si128((__m128i*)wav_src); wav_src += 4;
		__m128i x3 = _mm_load_si128((__m128i*)wav_src); wav_src += 4;
		x = _mm_sub_epi32(x, _mm_load_si128((__m128i*)main_src)); main_src += 4;
		x1 = _mm_sub_epi32(x1, _mm_load_si128((__m128i*)main_src)); main_src += 4;
		x2 = _mm_sub_epi32(x2, _mm_load_si128((__m128i*)main_src)); main_src += 4;
		x3 = _mm_sub_epi32(x3, _mm_load_si128((__m128i*)main_src)); main_src += 4;
		_mm_stream_si128((__m128i*)dest, x); dest += 4;
		_mm_stream_si128((__m128i*)dest, x1); dest += 4;
		_mm_stream_si128((__m128i*)dest, x2); dest += 4;
		_mm_stream_si128((__m128i*)dest, x3); dest += 4;
		count-=16;
	}
#endif
	while (count-- != 0) *(dest++) = *(wav_src++) - *(main_src++);
}
void _NO_hybrid_audio_c(int * __restrict dest, int * __restrict main_src, int * __restrict ext_src, size_t count) {
#if defined(_WIN64) && defined(__GNUC__)
	while (count >= 16){
		__m128i x = _mm_load_si128((__m128i*)main_src); main_src += 4;
		__m128i x1 = _mm_load_si128((__m128i*)main_src); main_src += 4;
		__m128i x2 = _mm_load_si128((__m128i*)main_src); main_src += 4;
		__m128i x3 = _mm_load_si128((__m128i*)main_src); main_src += 4;
		x = _mm_add_epi32(x, _mm_load_si128((__m128i*)ext_src)); ext_src += 4;
		x1 = _mm_add_epi32(x1, _mm_load_si128((__m128i*)ext_src)); ext_src += 4;
		x2 = _mm_add_epi32(x2, _mm_load_si128((__m128i*)ext_src)); ext_src += 4;
		x3 = _mm_add_epi32(x3, _mm_load_si128((__m128i*)ext_src)); ext_src += 4;
		_mm_stream_si128((__m128i*)dest, x); dest += 4;
		_mm_stream_si128((__m128i*)dest, x1); dest += 4;
		_mm_stream_si128((__m128i*)dest, x2); dest += 4;
		_mm_stream_si128((__m128i*)dest, x3); dest += 4;
		count -= 16;
	}
#endif
	while (count-- != 0) *(dest++) = *(main_src++) + *(ext_src++);
}
#if defined(__GNUC__)
alignas(__ALIGN_SIZE__)
#elif defined(_MSC_VER)
__declspec(align(__ALIGN_SIZE__))
#endif
float c_float_int8[] = { 98304.0f, 98304.0f, 98304.0f, 98304.0f, 98304.0f, 98304.0f, 98304.0f, 98304.0f };
void _NO_float_int8_c(float * __restrict fpara, size_t count) {
#if defined(_WIN64) && defined(__GNUC__)
	__m128 c = _mm_load_ps(c_float_int8);
	while (count >= 16){
		__m128 x = _mm_load_ps(fpara);
		__m128 x1 = _mm_load_ps(fpara + 4);
		__m128 x2 = _mm_load_ps(fpara + 8);
		__m128 x3 = _mm_load_ps(fpara + 12);
		x = _mm_add_ps(x, c);
		x1 = _mm_add_ps(x1, c);
		x2 = _mm_add_ps(x2, c);
		x3 = _mm_add_ps(x3, c);
		__m128i d = _mm_slli_epi32(_mm_castps_si128(x), 16);
		__m128i d1 = _mm_slli_epi32(_mm_castps_si128(x1), 16);
		__m128i d2 = _mm_slli_epi32(_mm_castps_si128(x2), 16);
		__m128i d3 = _mm_slli_epi32(_mm_castps_si128(x3), 16);
		d = _mm_srai_epi32(d, 16);
		d1 = _mm_srai_epi32(d1, 16);
		d2 = _mm_srai_epi32(d2, 16);
		d3 = _mm_srai_epi32(d3, 16);
		_mm_stream_si128((__m128i*)fpara, d);
		_mm_stream_si128((__m128i*)(fpara + 4), d1);
		_mm_stream_si128((__m128i*)(fpara + 8), d2);
		_mm_stream_si128((__m128i*)(fpara + 12), d3);
		fpara += 16;
		count -= 16;
	}
	while (count-- != 0) {//其实这里可以自动向量化
		*fpara += 98304.0f;
		*(int*)(fpara++) = (*(int*)fpara) << 16 >> 16;
	}
#else
	while (count-- != 0) {
		float f = *fpara * 0x80; int i;
		FROUND(i, f);
		*(int*)(fpara++) = i;
	}
#endif
}
#if defined(__GNUC__)
alignas(__ALIGN_SIZE__)
#elif defined(_MSC_VER)
__declspec(align(__ALIGN_SIZE__))
#endif
float c_float_int16[] = { 384.0f, 384.0f, 384.0f, 384.0f, 384.0f, 384.0f, 384.0f, 384.0f };
void _NO_float_int16_c(float * __restrict fpara, size_t count) {
#if defined(_WIN64) && defined(__GNUC__)
	__m128 c = _mm_load_ps(c_float_int16);
	while (count >= 16){
		__m128 x = _mm_load_ps(fpara);
		__m128 x1 = _mm_load_ps(fpara + 4);
		__m128 x2 = _mm_load_ps(fpara + 8);
		__m128 x3 = _mm_load_ps(fpara + 12);
		x = _mm_add_ps(x, c);
		x1 = _mm_add_ps(x1, c);
		x2 = _mm_add_ps(x2, c);
		x3 = _mm_add_ps(x3, c);
		__m128i d = _mm_slli_epi32(_mm_castps_si128(x), 12);
		__m128i d1 = _mm_slli_epi32(_mm_castps_si128(x1), 12);
		__m128i d2 = _mm_slli_epi32(_mm_castps_si128(x2), 12);
		__m128i d3 = _mm_slli_epi32(_mm_castps_si128(x3), 12);
		d = _mm_srai_epi32(d, 12);
		d1 = _mm_srai_epi32(d1, 12);
		d2 = _mm_srai_epi32(d2, 12);
		d3 = _mm_srai_epi32(d3, 12);
		_mm_stream_si128((__m128i*)fpara, d);
		_mm_stream_si128((__m128i*)(fpara + 4), d1);
		_mm_stream_si128((__m128i*)(fpara + 8), d2);
		_mm_stream_si128((__m128i*)(fpara + 12), d3);
		fpara += 16;
		count -= 16;
	}
	while (count-- != 0) {//其实这里可以自动向量化
		*fpara += 384.0f;
		*(int*)(fpara++) = (*(int*)fpara) << 12 >> 12;
	}
#else
	while (count-- != 0) {
		float f = *fpara * 0x8000; int i;
		FROUND(i, f);
		*(int*)(fpara++) = i;
	}
#endif
}
#if defined(__GNUC__)
alignas(__ALIGN_SIZE__)
#elif defined(_MSC_VER)
__declspec(align(__ALIGN_SIZE__))
#endif
double c_float_int24[] = { 805306368.0, 805306368.0, 805306368.0, 805306368.0 };
void _NO_float_int24_c(float * __restrict fpara, size_t count) {
#if defined(_WIN64) && defined(__GNUC__)
	__m128d c = _mm_load_pd(c_float_int24);
	while (count >= 8){
		__m128 x = _mm_load_ps(fpara);
		__m128 x1 = _mm_load_ps(fpara + 4);
		__m128d a = _mm_cvtps_pd(x);
		__m128d a2 = _mm_cvtps_pd(x1);
		x = _mm_movehl_ps(x, x);
		x1 = _mm_movehl_ps(x1, x1);
		__m128d a1 = _mm_cvtps_pd(x);
		__m128d a3 = _mm_cvtps_pd(x1);
		a = _mm_add_pd(a, c);
		a1 = _mm_add_pd(a1, c);
		a2 = _mm_add_pd(a2, c);
		a3 = _mm_add_pd(a3, c);
		x = _mm_shuffle_ps(_mm_castpd_ps(a), _mm_castpd_ps(a), 232);
		x1 = _mm_shuffle_ps(_mm_castpd_ps(a1), _mm_castpd_ps(a1), 232);
		__m128 x2 = _mm_shuffle_ps(_mm_castpd_ps(a2), _mm_castpd_ps(a2), 232);
		__m128 x3 = _mm_shuffle_ps(_mm_castpd_ps(a3), _mm_castpd_ps(a3), 232);
		x = _mm_movelh_ps(x, x1);
		x2 = _mm_movelh_ps(x2, x3);
		_mm_stream_ps(fpara, x);
		_mm_stream_ps(fpara + 4, x2);
		fpara += 8;
		count -= 8;
	}
	while (count-- != 0) {//这里由于使用不一致类型和指针而不能向量化
		double d = *fpara + 805306368.0;
		*(int*)(fpara++) = *(int*)&d;
	}
#else
	while (count-- != 0) {
		float f = *fpara * 0x800000; int i;
		FROUND(i, f);
		*(int*)(fpara++) = i;
	}
#endif
}


#ifdef _MSC_VER

//AVX2实现是正确的
__declspec(naked) void _AVX2_correct_audio(int *dest, int *wav_src, int *main_src) {
	_asm {//dest=rcx, wav_src=rdx, main_src=r8
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
		push esi;
		mov esi, DWORD PTR[esp + 16];
#endif
		mov R(ax), MEM_SIZE;
		add R(cx), R(ax);
		add R(dx), R(ax);
		add R8, R(ax);
		neg R(ax);
	ALC1://不应该用vmovntdqa读取，因为该内存不是WC（写联合），而是WB（写回）类型。也不用预读因为是连续读取
		vmovdqa ymm0, YMMWORD PTR[R(dx) + R(ax)];
		vmovdqa ymm1, YMMWORD PTR[R(dx) + R(ax) + 32];
		vpsubd ymm0, ymm0, YMMWORD PTR[R8 + R(ax)];
		vpsubd ymm1, ymm1, YMMWORD PTR[R8 + R(ax) + 32];
		vmovntdq YMMWORD PTR[R(cx) + R(ax)], ymm0;//WB类型可以用WC方式写内存
		vmovntdq YMMWORD PTR[R(cx) + R(ax) + 32], ymm1;
		add R(ax), 64;//一次读取64字节，即使缓存行是128字节也一样
		jnz ALC1;
#ifndef _WIN64
		pop esi;
#endif
		ret;
	}
}
__declspec(naked) void _AVX2_hybrid_audio(int *dest, int *main_src, int *ext_src) {
	_asm {//dest=rcx, main_src=rdx, ext_src=r8
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
		push esi;
		mov esi, DWORD PTR[esp + 16];
#endif
		mov R(ax), MEM_SIZE;
		add R(cx), R(ax);
		add R(dx), R(ax);
		add R8, R(ax);
		neg R(ax);
	ALH1://不应该用vmovntdqa读取，因为该内存不是WC（写联合），而是WB（写回）类型。也不用预读因为是连续读取
		vmovdqa ymm0, YMMWORD PTR[R(dx) + R(ax)];
		vmovdqa ymm1, YMMWORD PTR[R(dx) + R(ax) + 32];
		vpaddd ymm0, ymm0, YMMWORD PTR[R8 + R(ax)];
		vpaddd ymm1, ymm1, YMMWORD PTR[R8 + R(ax) + 32];
		vmovntdq YMMWORD PTR[R(cx) + R(ax)], ymm0;//WB类型可以用WC方式写内存
		vmovntdq YMMWORD PTR[R(cx) + R(ax) + 32], ymm1;
		add R(ax), 64;//一次读取64字节，即使缓存行是128字节也一样
		jnz ALH1;
#ifndef _WIN64
		pop esi;
#endif
		ret;
	}
}
__declspec(naked) void _AVX2_float_int8(float *fpara) {
	_asm {//fpara=rcx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
#endif
		lea R(ax), [R(cx) + MEM_SIZE];
		vmovaps ymm2, YMMWORD PTR c_float_int8[0];
	ALI8:
		vaddps ymm0, ymm2, YMMWORD PTR[R(cx)];
		vaddps ymm1, ymm2, YMMWORD PTR[R(cx) + 32];
		vpslld ymm0, ymm0, 16;
		vpslld ymm1, ymm1, 16;
		vpsrad ymm0, ymm0, 16;
		vpsrad ymm1, ymm1, 16;
		vmovntdq YMMWORD PTR[R(cx)], ymm0;
		vmovntdq YMMWORD PTR[R(cx) + 32], ymm1;
		add R(cx), 64;
		cmp R(cx), R(ax);
		jne ALI8;
		ret;
	}
}
__declspec(naked) void _AVX2_float_int16(float *fpara) {
	_asm {//fpara=rcx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
#endif
		lea R(ax), [R(cx) + MEM_SIZE];
		vmovaps ymm2, YMMWORD PTR c_float_int16[0];
	ALI16:
		vaddps ymm0, ymm2, YMMWORD PTR[R(cx)];
		vaddps ymm1, ymm2, YMMWORD PTR[R(cx) + 32];
		vpslld ymm0, ymm0, 12;
		vpslld ymm1, ymm1, 12;
		vpsrad ymm0, ymm0, 12;
		vpsrad ymm1, ymm1, 12;
		vmovntdq YMMWORD PTR[R(cx)], ymm0;
		vmovntdq YMMWORD PTR[R(cx) + 32], ymm1;
		add R(cx), 64;
		cmp R(cx), R(ax);
		jne ALI16;
		ret;
	}
}
__declspec(naked) void _AVX2_float_int24(float *fpara) {
	_asm {//fpara=rcx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
#endif
		lea R(ax), [R(cx) + MEM_SIZE];
		vmovapd ymm4, YMMWORD PTR c_float_int24[0];
	ALI24:
		vcvtps2pd ymm0, XMMWORD PTR[R(cx)];
		vcvtps2pd ymm1, XMMWORD PTR[R(cx) + 16];
		vcvtps2pd ymm2, XMMWORD PTR[R(cx) + 32];
		vcvtps2pd ymm3, XMMWORD PTR[R(cx) + 48];
		vaddpd ymm0, ymm0, ymm4;
		vaddpd ymm1, ymm1, ymm4;
		vaddpd ymm2, ymm2, ymm4;
		vaddpd ymm3, ymm3, ymm4;
		vpermilps ymm0, ymm0, 232;
		vextractf128 xmm5, ymm0, 1;
		vshufpd xmm0, xmm0, xmm5, 0;
		vpermilps ymm1, ymm1, 232;
		vextractf128 xmm5, ymm1, 1;
		vshufpd xmm1, xmm1, xmm5, 0;
		vpermilps ymm2, ymm2, 232;
		vextractf128 xmm5, ymm2, 1;
		vshufpd xmm2, xmm2, xmm5, 0;
		vpermilps ymm3, ymm3, 232;
		vextractf128 xmm5, ymm3, 1;
		vshufpd xmm3, xmm3, xmm5, 0;
		vmovntdq XMMWORD PTR[R(cx)], xmm0;
		vmovntdq XMMWORD PTR[R(cx) + 16], xmm1;
		vmovntdq XMMWORD PTR[R(cx) + 32], xmm2;
		vmovntdq XMMWORD PTR[R(cx) + 48], xmm3;
		add R(cx), 64;//为了优化，循环代码不能太大
		cmp R(cx), R(ax);
		jne ALI24;
		ret;
	}
}

__declspec(naked) void _AVX2_correct_audio_c(int *dest, int *wav_src, int *main_src, size_t count) {
	_asm {//dest=rcx, wav_src=rdx, main_src=r8, count=r9
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
		push esi;
		push edi;
		mov esi, DWORD PTR[esp + 20];
		mov edi, DWORD PTR[esp + 24];
#endif
		mov R(ax), R9;
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz ALC2;
		lea R(cx), [R(cx) + R(ax) * 4];
		lea R(dx), [R(dx) + R(ax) * 4];
		lea R8, [R8 + R(ax) * 4];
		neg R(ax);
	ALC1://不应该用vmovntdqa读取，因为该内存不是WC（写联合），而是WB（写回）类型。也不用预读因为是连续读取
		vmovdqa ymm0, YMMWORD PTR[R(dx) + R(ax) * 4];
		vmovdqa ymm1, YMMWORD PTR[R(dx) + R(ax) * 4 + 32];
		vpsubd ymm0, ymm0, YMMWORD PTR[R8 + R(ax) * 4];
		vpsubd ymm1, ymm1, YMMWORD PTR[R8 + R(ax) * 4 + 32];
		vmovntdq YMMWORD PTR[R(cx) + R(ax) * 4], ymm0;//WB类型可以用WC方式写内存
		vmovntdq YMMWORD PTR[R(cx) + R(ax) * 4 + 32], ymm1;
		add R(ax), 16;//一次读取64字节，即使缓存行是128字节也一样
		jnz ALC1;
	ALC2:
		and R9,0xF;
		jz ALC4;
		dec R9;
	ALC3:
		mov eax, DWORD PTR[R(dx) + R9 * 4];
		sub eax, DWORD PTR[R8 + R9 * 4];
		mov DWORD PTR[R(cx) + R9 * 4], eax;
		dec R9;
		jns ALC3;
	ALC4:
#ifndef _WIN64
		pop edi;
		pop esi;
#endif
		ret;
	}
}
__declspec(naked) void _AVX2_hybrid_audio_c(int *dest, int *main_src, int *ext_src, size_t count) {
	_asm {//dest=rcx, main_src=rdx, ext_src=r8, count=r9
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
		push esi;
		push edi;
		mov esi, DWORD PTR[esp + 20];
		mov edi, DWORD PTR[esp + 24];
#endif
		mov R(ax), R9;
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz ALH2;
		lea R(cx), [R(cx) + R(ax) * 4];
		lea R(dx), [R(dx) + R(ax) * 4];
		lea R8, [R8 + R(ax) * 4];
		neg R(ax);
	ALH1://不应该用vmovntdqa读取，因为该内存不是WC（写联合），而是WB（写回）类型。也不用预读因为是连续读取
		vmovdqa ymm0, YMMWORD PTR[R(dx) + R(ax) * 4];
		vmovdqa ymm1, YMMWORD PTR[R(dx) + R(ax) * 4 + 32];
		vpaddd ymm0, ymm0, YMMWORD PTR[R8 + R(ax) * 4];
		vpaddd ymm1, ymm1, YMMWORD PTR[R8 + R(ax) * 4 + 32];
		vmovntdq YMMWORD PTR[R(cx) + R(ax) * 4], ymm0;//WB类型可以用WC方式写内存
		vmovntdq YMMWORD PTR[R(cx) + R(ax) * 4 + 32], ymm1;
		add R(ax), 16;//一次读取64字节，即使缓存行是128字节也一样
		jnz ALH1;
	ALH2:
		and R9,0xF;
		jz ALH4;
		dec R9;
	ALH3:
		mov eax, DWORD PTR[R(dx) + R9 * 4];
		add eax, DWORD PTR[R8 + R9 * 4];
		mov DWORD PTR[R(cx) + R9 * 4], eax;
		dec R9;
		jns ALH3;
	ALH4:
#ifndef _WIN64
		pop edi;
		pop esi;
#endif
		ret;
	}
}
__declspec(naked) void _AVX2_float_int8_c(float *fpara, size_t count) {
	_asm {//fpara=rcx, count=rdx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
#endif
		vmovaps ymm2, YMMWORD PTR c_float_int8[0];
		mov R(ax), R(dx);
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz AL8I2;
		lea R(cx), [R(cx) + R(ax) * 4];
		neg R(ax);
	AL8I1:
		vaddps ymm0, ymm2, YMMWORD PTR[R(cx) + R(ax) * 4];
		vaddps ymm1, ymm2, YMMWORD PTR[R(cx) + R(ax) * 4 + 32];
		vpslld ymm0, ymm0, 16;
		vpslld ymm1, ymm1, 16;
		vpsrad ymm0, ymm0, 16;
		vpsrad ymm1, ymm1, 16;
		vmovntdq YMMWORD PTR[R(cx) + R(ax) * 4], ymm0;
		vmovntdq YMMWORD PTR[R(cx) + R(ax) * 4 + 32], ymm1;
		add R(ax), 16;
		jnz AL8I1;
	AL8I2:
		and R(dx), 0xF;
		jz AL8I4;
		dec R(dx);
	AL8I3:
		vaddss xmm0, xmm2, DWORD PTR[R(cx) + R(dx) * 4];
		vmovd eax, xmm0;
		movsx eax, ax;
		mov DWORD PTR[R(cx) + R(dx) * 4], eax;
		dec R(dx);
		jns AL8I3;
	AL8I4:
		ret;
	}
}
__declspec(naked) void _AVX2_float_int16_c(float *fpara, size_t count) {
	_asm {//fpara=rcx, count=rdx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
#endif
		vmovaps ymm2, YMMWORD PTR c_float_int16[0];
		mov R(ax), R(dx);
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz AL16I2;
		lea R(cx), [R(cx) + R(ax) * 4];
		neg R(ax);
	AL16I1:
		vaddps ymm0, ymm2, YMMWORD PTR[R(cx) + R(ax) * 4];
		vaddps ymm1, ymm2, YMMWORD PTR[R(cx) + R(ax) * 4 + 32];
		vpslld ymm0, ymm0, 12;
		vpslld ymm1, ymm1, 12;
		vpsrad ymm0, ymm0, 12;
		vpsrad ymm1, ymm1, 12;
		vmovntdq YMMWORD PTR[R(cx) + R(ax) * 4], ymm0;
		vmovntdq YMMWORD PTR[R(cx) + R(ax) * 4 + 32], ymm1;
		add R(ax), 16;
		jnz AL16I1;
	AL16I2:
		and R(dx), 0xF;
		jz AL16I4;
		dec R(dx);
	AL16I3:
		vaddss xmm0, xmm2, DWORD PTR[R(cx) + R(dx) * 4];
		vmovd eax, xmm0;
		shl eax, 12;
		sar eax, 12;
		mov DWORD PTR[R(cx) + R(dx) * 4], eax;
		dec R(dx);
		jns AL16I3;
	AL16I4:
		ret;
	}
}
__declspec(naked) void _AVX2_float_int24_c(float *fpara, size_t count) {
	_asm {//fpara=rcx, count=rdx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
#endif
		vmovapd ymm4, YMMWORD PTR c_float_int24[0];
		mov R(ax), R(dx);
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz AL24I2;
		lea R(cx), [R(cx) + R(ax) * 4];
		neg R(ax);
	AL24I1:
		vcvtps2pd ymm0, XMMWORD PTR[R(cx) + R(ax) * 4];
		vcvtps2pd ymm1, XMMWORD PTR[R(cx) + R(ax) * 4 + 16];
		vcvtps2pd ymm2, XMMWORD PTR[R(cx) + R(ax) * 4 + 32];
		vcvtps2pd ymm3, XMMWORD PTR[R(cx) + R(ax) * 4 + 48];
		vaddpd ymm0, ymm0, ymm4;
		vaddpd ymm1, ymm1, ymm4;
		vaddpd ymm2, ymm2, ymm4;
		vaddpd ymm3, ymm3, ymm4;
		vpermilps ymm0, ymm0, 232;
		vextractf128 xmm5, ymm0, 1;
		vshufpd xmm0, xmm0, xmm5, 0;
		vpermilps ymm1, ymm1, 232;
		vextractf128 xmm5, ymm1, 1;
		vshufpd xmm1, xmm1, xmm5, 0;
		vpermilps ymm2, ymm2, 232;
		vextractf128 xmm5, ymm2, 1;
		vshufpd xmm2, xmm2, xmm5, 0;
		vpermilps ymm3, ymm3, 232;
		vextractf128 xmm5, ymm3, 1;
		vshufpd xmm3, xmm3, xmm5, 0;
		vmovntdq XMMWORD PTR[R(cx) + R(ax) * 4], xmm0;
		vmovntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 16], xmm1;
		vmovntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 32], xmm2;
		vmovntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 48], xmm3;
		add R(ax), 16;//为了优化，循环代码不能太大
		jnz AL24I1;
	AL24I2:
		and R(dx), 0xF;
		jz AL24I4;
		dec R(dx);
	AL24I3:
		vmovss xmm0, DWORD PTR[R(cx) + R(dx) * 4];
		vcvtps2pd xmm0, xmm0;
		vaddsd xmm0, xmm0, xmm4;
		vmovd DWORD PTR[R(cx) + R(dx) * 4], xmm0;
		dec R(dx);
		jns AL24I3;
	AL24I4:
		ret;
	}
}


//SSE2实现是正确的
__declspec(naked) void _SSE2_correct_audio(int *dest, int *wav_src, int *main_src) {
	_asm {//dest=rcx, wav_src=rdx, main_src=r8
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
		push esi;
		mov esi, DWORD PTR[esp + 16];
#endif
		mov R(ax), MEM_SIZE;
		add R(cx), R(ax);
		add R(dx), R(ax);
		add R8, R(ax);
		neg R(ax);
	SLC1:
		movdqa xmm0, XMMWORD PTR[R(dx) + R(ax)];
		movdqa xmm1, XMMWORD PTR[R(dx) + R(ax) + 16];
		movdqa xmm2, XMMWORD PTR[R(dx) + R(ax) + 32];
		movdqa xmm3, XMMWORD PTR[R(dx) + R(ax) + 48];
		psubd xmm0, XMMWORD PTR[R8 + R(ax)];
		psubd xmm1, XMMWORD PTR[R8 + R(ax) + 16];
		psubd xmm2, XMMWORD PTR[R8 + R(ax) + 32];
		psubd xmm3, XMMWORD PTR[R8 + R(ax) + 48];
		movntdq XMMWORD PTR[R(cx) + R(ax)], xmm0;
		movntdq XMMWORD PTR[R(cx) + R(ax) + 16], xmm1;
		movntdq XMMWORD PTR[R(cx) + R(ax) + 32], xmm2;
		movntdq XMMWORD PTR[R(cx) + R(ax) + 48], xmm3;
		add R(ax), 64;
		jnz SLC1;
#ifndef _WIN64
		pop esi;
#endif
		ret;
	}
}
__declspec(naked) void _SSE2_hybrid_audio(int *dest, int *main_src, int *ext_src) {
	_asm {//dest=rcx, main_src=rdx, ext_src=r8
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
		push esi;
		mov esi, DWORD PTR[esp + 16];
#endif
		mov R(ax), MEM_SIZE;
		add R(cx), R(ax);
		add R(dx), R(ax);
		add R8, R(ax);
		neg R(ax);
	SLH1:
		movdqa xmm0, XMMWORD PTR[R(dx) + R(ax)];
		movdqa xmm1, XMMWORD PTR[R(dx) + R(ax) + 16];
		movdqa xmm2, XMMWORD PTR[R(dx) + R(ax) + 32];
		movdqa xmm3, XMMWORD PTR[R(dx) + R(ax) + 48];
		paddd xmm0, XMMWORD PTR[R8 + R(ax)];
		paddd xmm1, XMMWORD PTR[R8 + R(ax) + 16];
		paddd xmm2, XMMWORD PTR[R8 + R(ax) + 32];
		paddd xmm3, XMMWORD PTR[R8 + R(ax) + 48];
		movntdq XMMWORD PTR[R(cx) + R(ax)], xmm0;
		movntdq XMMWORD PTR[R(cx) + R(ax) + 16], xmm1;
		movntdq XMMWORD PTR[R(cx) + R(ax) + 32], xmm2;
		movntdq XMMWORD PTR[R(cx) + R(ax) + 48], xmm3;
		add R(ax), 64;
		jnz SLH1;
#ifndef _WIN64
		pop esi;
#endif
		ret;
	}
}
//float c_float_int8[] = { 98304.0f ,98304.0f ,98304.0f ,98304.0f };
__declspec(naked) void _SSE2_float_int8(float *fpara) {
	_asm {//fpara=rcx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
#endif
		lea R(ax), [R(cx) + MEM_SIZE];
		movaps xmm4, XMMWORD PTR c_float_int8[0];
	SLI8:
		movaps xmm0, XMMWORD PTR[R(cx)];
		movaps xmm1, XMMWORD PTR[R(cx) + 16];
		movaps xmm2, XMMWORD PTR[R(cx) + 32];
		movaps xmm3, XMMWORD PTR[R(cx) + 48];
		addps xmm0, xmm4;
		addps xmm1, xmm4;
		addps xmm2, xmm4;
		addps xmm3, xmm4;
		pslld xmm0, 16;
		pslld xmm1, 16;
		pslld xmm2, 16;
		pslld xmm3, 16;
		psrad xmm0, 16;
		psrad xmm1, 16;
		psrad xmm2, 16;
		psrad xmm3, 16;
		movntdq XMMWORD PTR[R(cx)], xmm0;
		movntdq XMMWORD PTR[R(cx) + 16], xmm1;
		movntdq XMMWORD PTR[R(cx) + 32], xmm2;
		movntdq XMMWORD PTR[R(cx) + 48], xmm3;
		add R(cx), 64;
		cmp R(cx), R(ax);
		jne SLI8;
		ret;
	}
}
//float c_float_int16[] = { 384.0f ,384.0f ,384.0f ,384.0f };
__declspec(naked) void _SSE2_float_int16(float *fpara) {
	_asm {//fpara=rcx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
#endif
		lea R(ax), [R(cx) + MEM_SIZE];
		movaps xmm4, XMMWORD PTR c_float_int16[0];
	SLI16:
		movaps xmm0, XMMWORD PTR[R(cx)];
		movaps xmm1, XMMWORD PTR[R(cx) + 16];
		movaps xmm2, XMMWORD PTR[R(cx) + 32];
		movaps xmm3, XMMWORD PTR[R(cx) + 48];
		addps xmm0, xmm4;
		addps xmm1, xmm4;
		addps xmm2, xmm4;
		addps xmm3, xmm4;
		pslld xmm0, 12;
		pslld xmm1, 12;
		pslld xmm2, 12;
		pslld xmm3, 12;
		psrad xmm0, 12;
		psrad xmm1, 12;
		psrad xmm2, 12;
		psrad xmm3, 12;
		movntdq XMMWORD PTR[R(cx)], xmm0;
		movntdq XMMWORD PTR[R(cx) + 16], xmm1;
		movntdq XMMWORD PTR[R(cx) + 32], xmm2;
		movntdq XMMWORD PTR[R(cx) + 48], xmm3;
		add R(cx), 64;
		cmp R(cx), R(ax);
		jne SLI16;
		ret;
	}
}
//double c_float_int24[] = { 805306368.0 ,805306368.0 };
__declspec(naked) void _SSE2_float_int24(float *fpara) {
	_asm {//fpara=rcx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
#endif
		lea R(ax), [R(cx) + MEM_SIZE];
		movapd xmm4, XMMWORD PTR c_float_int24[0];
	SLI24:
		cvtps2pd xmm0, QWORD PTR[R(cx)];
		cvtps2pd xmm1, QWORD PTR[R(cx) + 8];
		cvtps2pd xmm2, QWORD PTR[R(cx) + 16];
		cvtps2pd xmm3, QWORD PTR[R(cx) + 24];
		addpd xmm0, xmm4;
		addpd xmm1, xmm4;
		addpd xmm2, xmm4;
		addpd xmm3, xmm4;
		shufps xmm0, xmm0, 232;
		shufps xmm1, xmm1, 232;
		shufps xmm2, xmm2, 232;
		shufps xmm3, xmm3, 232;
		movlhps xmm0, xmm1;
		movlhps xmm2, xmm3;
		movntdq XMMWORD PTR[R(cx)], xmm0;
		movntdq XMMWORD PTR[R(cx) + 16], xmm2;
		add R(cx), 32;
		cmp R(cx), R(ax);
		jne SLI24;
		ret;
	}
}

__declspec(naked) void _SSE2_correct_audio_c(int *dest, int *wav_src, int *main_src, size_t count) {
	_asm {//dest=rcx, wav_src=rdx, main_src=r8, count=r9
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
		push esi;
		push edi;
		mov esi, DWORD PTR[esp + 20];
		mov edi, DWORD PTR[esp + 24];
#endif
		mov R(ax), R9;
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz SLC2;
		lea R(cx), [R(cx) + R(ax) * 4];
		lea R(dx), [R(dx) + R(ax) * 4];
		lea R8, [R8 + R(ax) * 4];
		neg R(ax);
	SLC1:
		movdqa xmm0, XMMWORD PTR[R(dx) + R(ax) * 4];
		movdqa xmm1, XMMWORD PTR[R(dx) + R(ax) * 4 + 16];
		movdqa xmm2, XMMWORD PTR[R(dx) + R(ax) * 4 + 32];
		movdqa xmm3, XMMWORD PTR[R(dx) + R(ax) * 4 + 48];
		psubd xmm0, XMMWORD PTR[R8 + R(ax) * 4];
		psubd xmm1, XMMWORD PTR[R8 + R(ax) * 4 + 16];
		psubd xmm2, XMMWORD PTR[R8 + R(ax) * 4 + 32];
		psubd xmm3, XMMWORD PTR[R8 + R(ax) * 4 + 48];
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4], xmm0;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 16], xmm1;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 32], xmm2;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 48], xmm3;
		add R(ax), 16;//一次读取64字节，即使缓存行是128字节也一样
		jnz SLC1;
	SLC2:
		and R9,0xF;
		jz SLC4;
		dec R9;
	SLC3:
		mov eax, DWORD PTR[R(dx) + R9 * 4];
		sub eax, DWORD PTR[R8 + R9 * 4];
		mov DWORD PTR[R(cx) + R9 * 4], eax;
		dec R9;
		jns SLC3;
	SLC4:
#ifndef _WIN64
		pop edi;
		pop esi;
#endif
		ret;
	}
}
__declspec(naked) void _SSE2_hybrid_audio_c(int *dest, int *main_src, int *ext_src, size_t count) {
	_asm {//dest=rcx, main_src=rdx, ext_src=r8, count=r9
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
		push esi;
		push edi;
		mov esi, DWORD PTR[esp + 20];
		mov edi, DWORD PTR[esp + 24];
#endif
		mov R(ax), R9;
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz SLH2;
		lea R(cx), [R(cx) + R(ax) * 4];
		lea R(dx), [R(dx) + R(ax) * 4];
		lea R8, [R8 + R(ax) * 4];
		neg R(ax);
	SLH1:
		movdqa xmm0, XMMWORD PTR[R(dx) + R(ax) * 4];
		movdqa xmm1, XMMWORD PTR[R(dx) + R(ax) * 4 + 16];
		movdqa xmm2, XMMWORD PTR[R(dx) + R(ax) * 4 + 32];
		movdqa xmm3, XMMWORD PTR[R(dx) + R(ax) * 4 + 48];
		paddd xmm0, XMMWORD PTR[R8 + R(ax) * 4];
		paddd xmm1, XMMWORD PTR[R8 + R(ax) * 4 + 16];
		paddd xmm2, XMMWORD PTR[R8 + R(ax) * 4 + 32];
		paddd xmm3, XMMWORD PTR[R8 + R(ax) * 4 + 48];
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4], xmm0;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 16], xmm1;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 32], xmm2;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 48], xmm3;
		add R(ax), 16;//一次读取64字节，即使缓存行是128字节也一样
		jnz SLH1;
	SLH2:
		and R9,0xF;
		jz SLH4;
		dec R9;
	SLH3:
		mov eax, DWORD PTR[R(dx) + R9 * 4];
		add eax, DWORD PTR[R8 + R9 * 4];
		mov DWORD PTR[R(cx) + R9 * 4], eax;
		dec R9;
		jns SLH3;
	SLH4:
#ifndef _WIN64
		pop edi;
		pop esi;
#endif
		ret;
	}
}
//float c_float_int8[] = { 98304.0f ,98304.0f ,98304.0f ,98304.0f };
__declspec(naked) void _SSE2_float_int8_c(float *fpara, size_t count) {
	_asm {//fpara=rcx//fpara=rcx, count=rdx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
#endif
		movaps xmm4, XMMWORD PTR c_float_int8[0];
		mov R(ax), R(dx);
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz SL8I2;
		lea R(cx), [R(cx) + R(ax) * 4];
		neg R(ax);
	SL8I1:
		movaps xmm0, XMMWORD PTR[R(cx) + R(ax) * 4];
		movaps xmm1, XMMWORD PTR[R(cx) + R(ax) * 4 + 16];
		movaps xmm2, XMMWORD PTR[R(cx) + R(ax) * 4 + 32];
		movaps xmm3, XMMWORD PTR[R(cx) + R(ax) * 4 + 48];
		addps xmm0, xmm4;
		addps xmm1, xmm4;
		addps xmm2, xmm4;
		addps xmm3, xmm4;
		pslld xmm0, 16;
		pslld xmm1, 16;
		pslld xmm2, 16;
		pslld xmm3, 16;
		psrad xmm0, 16;
		psrad xmm1, 16;
		psrad xmm2, 16;
		psrad xmm3, 16;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4], xmm0;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 16], xmm1;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 32], xmm2;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 48], xmm3;
		add R(ax), 16;
		jnz SL8I1;
	SL8I2:
		and R(dx), 0xF;
		jz SL8I4;
		dec R(dx);
	SL8I3:
		movss xmm0, DWORD PTR[R(cx) + R(dx) * 4];
		addss xmm0, xmm4;
		movd eax, xmm0;
		movsx eax, ax;
		mov DWORD PTR[R(cx) + R(dx) * 4], eax;
		dec R(dx);
		jns SL8I3;
	SL8I4:
		ret;
	}
}
//float c_float_int16[] = { 384.0f ,384.0f ,384.0f ,384.0f };
__declspec(naked) void _SSE2_float_int16_c(float *fpara, size_t count) {
	_asm {//fpara=rcx, count=rdx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
#endif
		movaps xmm4, XMMWORD PTR c_float_int16[0];
		mov R(ax), R(dx);
		and R(ax), 0xFFFFFFFFFFFFFFF0LL;
		jz SL16I2;
		lea R(cx), [R(cx) + R(ax) * 4];
		neg R(ax);
	SL16I1:
		movaps xmm0, XMMWORD PTR[R(cx) + R(ax) * 4];
		movaps xmm1, XMMWORD PTR[R(cx) + R(ax) * 4 + 16];
		movaps xmm2, XMMWORD PTR[R(cx) + R(ax) * 4 + 32];
		movaps xmm3, XMMWORD PTR[R(cx) + R(ax) * 4 + 48];
		addps xmm0, xmm4;
		addps xmm1, xmm4;
		addps xmm2, xmm4;
		addps xmm3, xmm4;
		pslld xmm0, 12;
		pslld xmm1, 12;
		pslld xmm2, 12;
		pslld xmm3, 12;
		psrad xmm0, 12;
		psrad xmm1, 12;
		psrad xmm2, 12;
		psrad xmm3, 12;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4], xmm0;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 16], xmm1;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 32], xmm2;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 48], xmm3;
		add R(ax), 16;
		jnz SL16I1;
	SL16I2:
		and R(dx), 0xF;
		jz SL16I4;
		dec R(dx);
	SL16I3:
		movss xmm0, DWORD PTR[R(cx) + R(dx) * 4];
		addss xmm0, xmm4;
		movd eax, xmm0;
		shl eax, 12;
		sar eax, 12;
		mov DWORD PTR[R(cx) + R(dx) * 4], eax;
		dec R(dx);
		jns SL16I3;
	SL16I4:
		ret;
	}
}
//double c_float_int24[] = { 805306368.0 ,805306368.0 };
__declspec(naked) void _SSE2_float_int24_c(float *fpara, size_t count) {
	_asm {//fpara=rcx, count=rdx
#ifndef _WIN64
		mov ecx, DWORD PTR[esp + 4];
		mov edx, DWORD PTR[esp + 8];
#endif
		movapd xmm4, XMMWORD PTR c_float_int24[0];
		mov R(ax), R(dx);
		and R(ax), 0xFFFFFFFFFFFFFFF8LL;
		jz SL24I2;
		lea R(cx), [R(cx) + R(ax) * 4];
		neg R(ax);
	SL24I1:
		cvtps2pd xmm0, QWORD PTR[R(cx) + R(ax) * 4];
		cvtps2pd xmm1, QWORD PTR[R(cx) + R(ax) * 4 + 8];
		cvtps2pd xmm2, QWORD PTR[R(cx) + R(ax) * 4 + 16];
		cvtps2pd xmm3, QWORD PTR[R(cx) + R(ax) * 4 + 24];
		addpd xmm0, xmm4;
		addpd xmm1, xmm4;
		addpd xmm2, xmm4;
		addpd xmm3, xmm4;
		shufps xmm0, xmm0, 232;
		shufps xmm1, xmm1, 232;
		shufps xmm2, xmm2, 232;
		shufps xmm3, xmm3, 232;
		movlhps xmm0, xmm1;
		movlhps xmm2, xmm3;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4], xmm0;
		movntdq XMMWORD PTR[R(cx) + R(ax) * 4 + 16], xmm2;
		add R(ax), 8;
		jnz SL24I1;
	SL24I2:
		and R(dx), 0x7;
		jz SL24I4;
		dec R(dx);
	SL24I3:
		movss xmm0, DWORD PTR[R(cx) + R(dx) * 4];
		cvtps2pd xmm0, xmm0;
		addsd xmm0, xmm4;
		movd DWORD PTR[R(cx) + R(dx) * 4], xmm0;
		dec R(dx);
		jns SL24I3;
	SL24I4:
		ret;
	}
}

#endif

void* malloc_align(size_t n){
	ptrdiff_t *ptr;
	ptrdiff_t p = (ptrdiff_t)malloc(n + __ALIGN_SIZE__);
	ptr = (ptrdiff_t*)((p & (ptrdiff_t)(-__ALIGN_SIZE__)) + __ALIGN_SIZE__);
	ptr[-1] = p;
	return ptr;
}
void free_align(void *p){
	free((void*)(((ptrdiff_t*)p)[-1]));
}

#if 0
#define RDSTC2L(t) \
	__asm rdtsc \
	__asm mov DWORD PTR t[0], eax \
	__asm mov DWORD PTR t[4], edx

#define rand(r) (r=r*1664525+1013904223)
int rnd = 123456;

INIT_SIMD_SET;

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


int main()
{//浮点计算依赖于平台，请尽量少用，或明确是否支持
	long long ct, et, at, st = 0;
	int *data, *data2, *data3;
	int i, ret;
	double fscale = 0x80000000;
	ret = 0;
	rnd = 123456;
	data = (int*)malloc_align(1 << 20);
	data2 = (int*)malloc_align(1 << 20);
	data3 = (int*)malloc_align(1 << 20);
	for (i = 1 << 20 >> 2; i-- != 0;) data2[i] = rand(rnd);
	for (i = 1 << 20 >> 2; i-- != 0;) data3[i] = rand(rnd);
	for (i = 1 << 20 >> 2; i-- != 0;) data[i] = rand(rnd);
	for (i = 1 << 20 >> 2; i-- != 0;) ret ^= data[i];
	correct_audio(data, data2, data3);
	for (i = 1 << 20 >> 2; i-- != 0;) ret ^= data[i];
	if (ret != -146800640)printf("correct_audio error ret = %d\n", ret);
	else printf("correct_audio success!\n");
	hybrid_audio(data, data2, data3);
	for (i = 1 << 20 >> 2; i-- != 0;) ret ^= data[i];
	if (ret != -1310720000)printf("hybrid_audio error ret = %d\n", ret);
	else printf("hybrid_audio success!\n");
	{//__SIMD_LV__宏会影响该区域编译，具体未知
		for (i = 1 << 20 >> 2; i-- != 0;) ((float*)data)[i] = (double)(data[i]) / fscale;
		for (i = 1 << 20 >> 2; i-- != 0;) ((float*)data2)[i] = (double)(data2[i]) / fscale;
		for (i = 1 << 20 >> 2; i-- != 0;) ((float*)data3)[i] = (double)(data3[i]) / fscale;
	}
	float_int8((float*)data);
	for (i = 1 << 20 >> 2; i-- != 0;) ret ^= data[i];
	if (ret != -1310719781)printf("float_int8 error ret = %d\n", ret);
	else printf("float_int8 success!\n");
	float_int16((float*)data2);
	for (i = 1 << 20 >> 2; i-- != 0;) ret ^= data2[i];
	if (ret != 1310681856)printf("float_int16 error ret = %d\n", ret);
	else printf("float_int16 success!\n");
	float_int24((float*)data3);
	for (i = 1 << 20 >> 2; i-- != 0;) ret ^= data3[i];
	if (ret != 1314753420)printf("float_int24 error ret = %d\n\n", ret);
	else printf("float_int24 success!\n\n");
	ret = 0;
	rnd = 654321;
	for (i = 0x317a9; i-- != 0;) data2[i] = rand(rnd);
	for (i = 0x317a9; i-- != 0;) data3[i] = rand(rnd);
	for (i = 0x317a9; i-- != 0;) data[i] = rand(rnd);
	for (i = 0x317a9; i-- != 0;) ret ^= data[i];
	correct_audio_c(data, data2, data3, 0x317a9);
	for (i = 0x317a9; i-- != 0;) ret ^= data[i];
	if (ret != -600282041)printf("correct_audio_c error ret = %d\n", ret);
	else printf("correct_audio_c success!\n");
	hybrid_audio_c(data, data2, data3, 0x317a9);
	for (i = 0x317a9; i-- != 0;) ret ^= data[i];
	if (ret != -1416820472)printf("hybrid_audio_c error ret = %d\n", ret);
	else printf("hybrid_audio_c success!\n");
	{//__SIMD_LV__宏会影响该区域编译，具体未知
		for (i = 0x317a9; i-- != 0;) ((float*)data)[i] = (double)(data[i]) / fscale;
		for (i = 0x317a9; i-- != 0;) ((float*)data2)[i] = (double)(data2[i]) / fscale;
		for (i = 0x317a9; i-- != 0;) ((float*)data3)[i] = (double)(data3[i]) / fscale;
	}
	float_int8_c((float*)data, 0x317a9);
	for (i = 0x317a9; i-- != 0;) ret ^= data[i];
	if (ret != -1416820305)printf("float_int8_c error ret = %d\n", ret);
	else printf("float_int8_c success!\n");
	float_int16_c((float*)data2, 0x317a9);
	for (i = 0x317a9; i-- != 0;) ret ^= data2[i];
	if (ret != 1416774556)printf("float_int16_c error ret = %d\n", ret);
	else printf("float_int16_c success!\n");
	float_int24_c((float*)data3, 0x317a9);
	for (i = 0x317a9; i-- != 0;) ret ^= data3[i];
	if (ret != 1411314831)printf("float_int24_c error ret = %d\n\n", ret);
	else printf("float_int24_c success!\n\n");
	free_align(data);
	free_align(data2);
	free_align(data3);
	for (int t = 0; t < 8; t++){
		ret = 0;
		rnd = 123456;
		data = (int*)malloc_align(1 << 30);
		data2 = (int*)malloc_align(1 << 30);
		data3 = (int*)malloc_align(1 << 30);
		for (i = 1 << 30 >> 2; i-- != 0;) data2[i] = rand(rnd);
		for (i = 1 << 30 >> 2; i-- != 0;) data3[i] = rand(rnd);
		for (i = 1 << 30 >> 2; i-- != 0;) data[i] = rand(rnd);
		for (i = 1 << 30 >> 2; i-- != 0;) ret ^= data[i];
		RDSTC2L(ct);
		correct_audio_c(data, data2, data3, 1 << 30 >> 2);
		RDSTC2L(et);
		printf("cost=%lld\n", et -= ct); at = et;
		for (i = 1 << 30 >> 2; i-- != 0;) ret ^= data[i];
		RDSTC2L(ct);
		hybrid_audio_c(data, data2, data3, 1 << 30 >> 2);
		RDSTC2L(et);
		printf("cost=%lld\n", et -= ct); at += et;
		for (i = 1 << 30 >> 2; i-- != 0;) ret ^= data[i];
		{//__SIMD_LV__宏会影响该区域编译，具体未知
			for (i = 1 << 30 >> 2; i-- != 0;) ((float*)data)[i] = (double)(data[i]) / fscale;
			for (i = 1 << 30 >> 2; i-- != 0;) ((float*)data2)[i] = (double)(data2[i]) / fscale;
			for (i = 1 << 30 >> 2; i-- != 0;) ((float*)data3)[i] = (double)(data3[i]) / fscale;
		}
		RDSTC2L(ct);
		float_int8_c((float*)data, 1 << 30 >> 2);
		RDSTC2L(et);
		printf("cost=%lld\n", et -= ct); at += et;
		for (i = 1 << 30 >> 2; i-- != 0;) ret ^= data[i];
		RDSTC2L(ct);
		float_int16_c((float*)data2, 1 << 30 >> 2);
		RDSTC2L(et);
		printf("cost=%lld\n", et -= ct); at += et;
		for (i = 1 << 30 >> 2; i-- != 0;) ret ^= data2[i];
		RDSTC2L(ct);
		float_int24_c((float*)data3, 1 << 30 >> 2);
		RDSTC2L(et);
		printf("cost=%lld\n", et -= ct); at += et;
		for (i = 1 << 30 >> 2; i-- != 0;) ret ^= data3[i];
		free_align(data);
		free_align(data2);
		free_align(data3);
		printf("all cost %lld\n", at);
		if (at < 3500000000)st += at;
		else t--;
		printf("ret=%d\n\n", ret);
	}
	if (ret != 1075319609)printf("error ret = %d\n", ret);
	else printf("success!\n");
	printf("averge cost %lf\n", (double)st / 8);
	return ret;
}
#endif