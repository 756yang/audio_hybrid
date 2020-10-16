#pragma once

// SSE_AVX_C.cpp 
// processor: x86, x64
// Uses the __cpuid intrinsic to get information about 
// CPU extended instruction set support.
// 用于条件编译各种指令集函数

#include <string>
#include <vector>
#include <bitset>
#include <array>
#include <intrin.h>


/*
Intel C++是先对预处理宏按顺序进行展开，再替换到代码中去，它不会嵌套替换，所以请保证该宏可以完全展开。VC++会嵌套替换，但是还是不要定义这种复杂的宏。
请使用：//对应crowd_level
DECL_CSIMD_FUNCTION(ret,x,para);//可以自己声明而不用宏以之更灵活
SET_CSIMD_FUNCTION(ret,x,para);
或者：//同上，但固定了声明
SET_SSE_AVX_FUNCTION(ret,x,para);
或者：//对应support_level
DECL_SIMD_FUNCTION(ret,x,para);//可以自己声明而不用宏以之更灵活
SET_SIMD_FUNCTION(ret,x,para);
*/

#define V__AVX512__ 512
#define V__FMA__ 256
#define V__AVX2__ 128
#define V__AVX__ 64
#define V__SSE42__ 32
#define V__SSE41__ 16
#define V__SSSE3__ 8
#define V__SSE3__ 4
#define V__SSE2__ 2
#define V__SSE__ 1
#define V__NO_SIMD__ 0

#undef __AVX512__
#undef __FMA__
#undef __AVX2__
#undef __AVX__
#undef __SSE42__
#undef __SSE41__
#undef __SSSE3__
#undef __SSE3__
#undef __SSE2__
#undef __SSE__

#ifndef __SIMD_LV__
#undef __NO_SIMD__
#else
#if __SIMD_LV__ & V__SSE__
#define __SSE__
#endif
#if __SIMD_LV__ & V__SSE2__
#define __SSE2__
#endif
#if __SIMD_LV__ & V__SSE3__
#define __SSE3__
#endif
#if __SIMD_LV__ & V__SSSE3__
#define __SSSE3__
#endif
#if __SIMD_LV__ & V__SSE41__
#define __SSE41__
#endif
#if __SIMD_LV__ & V__SSE42__
#define __SSE42__
#endif
#if __SIMD_LV__ & V__AVX__
#define __AVX__
#endif
#if __SIMD_LV__ & V__AVX2__
#define __AVX2__
#endif
#if __SIMD_LV__ & V__FMA__
#define __FMA__
#endif
#if __SIMD_LV__ & V__AVX512__
#define __AVX512__
#endif
#if __SIMD_LV__ == V__NO_SIMD__
#define __NO_SIMD__
#endif
#endif

#ifdef __AVX512__
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_AVX512_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_AVX512_##x
#elif defined(__FMA__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_FMA_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_FMA_##x
#elif defined(__AVX2__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_AVX2_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_AVX2_##x
#elif defined(__AVX__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_AVX_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_AVX_##x
#elif defined(__SSE42__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE42_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE42_##x
#elif defined(__SSE41__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE41_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE41_##x
#elif defined(__SSSE3__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_SSSE3_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_SSSE3_##x
#elif defined(__SSE3__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE3_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE3_##x
#elif defined(__SSE2__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE2_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE2_##x
#elif defined(__SSE__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_SSE_##x
#elif defined(__NO_SIMD__)
#define DECL_CSIMD_FUNCTION(ret,x,para)
#define DECL_SMID_FUNCTION(ret,x,para)
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = &_NO_##x
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = &_NO_##x
#else
#define DECL_CSIMD_FUNCTION(ret,x,para) \
ret (*f_##x[3])para = { &_NO_##x, &_SSE2_##x, &_AVX2_##x }
#define DECL_SMID_FUNCTION(ret,x,para) \
ret (*f_##x[11])para = { &_NO_##x, &_SSE_##x, &_SSE2_##x, &_SSE3_##x, &_SSSE3_##x, &_SSE41_##x, &_SSE42_##x, &_AVX_##x, &_AVX2_##x, &_FMA_##x, &_AVX512_##x }
//注意，在G++/GCC中不能写(*x)##para，因为连接符##前不能是非标识符字符
#define SET_CSIMD_FUNCTION(ret,x,para) \
ret (*x)para = f_##x[InstructionIs::crowd_level]
#define SET_SMID_FUNCTION(ret,x,para) \
ret (*x)para = f_##x[InstructionIs::support_level]
#endif
// 设置crowd simd function (_NO_,_SSE2_,_AVX2_前缀函数)
#define SET_SSE_AVX_FUNCTION(ret,x,para) \
DECL_CSIMD_FUNCTION(ret,x,para); \
SET_CSIMD_FUNCTION(ret,x,para)

// Initialize const type data
#define INIT_INSTRUCTION_SET InstructionIs CPU_Rep;

#if !defined(__SIMD_LV__) || (!(__SIMD_LV__ & 1023) && __SIMD_LV__ != 0)
#define INIT_SIMD_SET INIT_INSTRUCTION_SET
#else
#define INIT_SIMD_SET
#endif

class InstructionIs
{
public:
	InstructionIs();

	static int get_support_level();
	static int get_crowd_level();

	// getters
	static std::string Vendor(void) { return vendor_; }
	static std::string Brand(void) { return brand_; }

	static bool SSE3(void) { return f_1_ECX_[0]; }
	static bool PCLMULQDQ(void) { return f_1_ECX_[1]; }
	static bool MONITOR(void) { return f_1_ECX_[3]; }
	static bool SSSE3(void) { return f_1_ECX_[9]; }
	static bool FMA(void) { return f_1_ECX_[12]; }
	static bool CMPXCHG16B(void) { return f_1_ECX_[13]; }
	static bool SSE41(void) { return f_1_ECX_[19]; }
	static bool SSE42(void) { return f_1_ECX_[20]; }
	static bool MOVBE(void) { return f_1_ECX_[22]; }
	static bool POPCNT(void) { return f_1_ECX_[23]; }
	static bool AES(void) { return f_1_ECX_[25]; }
	static bool XSAVE(void) { return f_1_ECX_[26]; }
	static bool OSXSAVE(void) { return f_1_ECX_[27]; }
	static bool AVX(void) { return f_1_ECX_[28]; }
	static bool F16C(void) { return f_1_ECX_[29]; }
	static bool RDRAND(void) { return f_1_ECX_[30]; }

	static bool MSR(void) { return f_1_EDX_[5]; }
	static bool CX8(void) { return f_1_EDX_[8]; }
	static bool SEP(void) { return f_1_EDX_[11]; }
	static bool CMOV(void) { return f_1_EDX_[15]; }
	static bool CLFSH(void) { return f_1_EDX_[19]; }
	static bool MMX(void) { return f_1_EDX_[23]; }
	static bool FXSR(void) { return f_1_EDX_[24]; }
	static bool SSE(void) { return f_1_EDX_[25]; }
	static bool SSE2(void) { return f_1_EDX_[26]; }

	static bool FSGSBASE(void) { return f_7_EBX_[0]; }
	static bool BMI1(void) { return f_7_EBX_[3]; }
	static bool HLE(void) { return isIntel_ && f_7_EBX_[4]; }
	static bool AVX2(void) { return f_7_EBX_[5]; }
	static bool BMI2(void) { return f_7_EBX_[8]; }
	static bool ERMS(void) { return f_7_EBX_[9]; }
	static bool INVPCID(void) { return f_7_EBX_[10]; }
	static bool RTM(void) { return isIntel_ && f_7_EBX_[11]; }
	static bool AVX512F(void) { return f_7_EBX_[16]; }
	static bool RDSEED(void) { return f_7_EBX_[18]; }
	static bool ADX(void) { return f_7_EBX_[19]; }
	static bool AVX512PF(void) { return f_7_EBX_[26]; }
	static bool AVX512ER(void) { return f_7_EBX_[27]; }
	static bool AVX512CD(void) { return f_7_EBX_[28]; }
	static bool SHA(void) { return f_7_EBX_[29]; }

	static bool PREFETCHWT1(void) { return f_7_ECX_[0]; }

	static bool LAHF(void) { return f_81_ECX_[0]; }
	static bool LZCNT(void) { return isIntel_ && f_81_ECX_[5]; }
	static bool ABM(void) { return isAMD_ && f_81_ECX_[5]; }
	static bool SSE4a(void) { return isAMD_ && f_81_ECX_[6]; }
	static bool XOP(void) { return isAMD_ && f_81_ECX_[11]; }
	static bool TBM(void) { return isAMD_ && f_81_ECX_[21]; }

	static bool SYSCALL(void) { return isIntel_ && f_81_EDX_[11]; }
	static bool MMXEXT(void) { return isAMD_ && f_81_EDX_[22]; }
	static bool RDTSCP(void) { return isIntel_ && f_81_EDX_[27]; }
	static bool _3DNOWEXT(void) { return isAMD_ && f_81_EDX_[30]; }
	static bool _3DNOW(void) { return isAMD_ && f_81_EDX_[31]; }

	enum level {
		NO_SIMD = 0,
		LSSE,
		LSSE2,
		LSSE3,
		LSSSE3,
		LSSE41,
		LSSE42,
		LAVX,
		LAVX2,
		LFMA,
		LAVX512
	};

	enum crowd_lv {
		CLSSE2 = 1,
		CLAVX2
	};

	static int support_level;
	static int crowd_level;
	static int nIds_;
	static int nExIds_;
	static std::string vendor_;
	static std::string brand_;
	static bool isIntel_;
	static bool isAMD_;
	static std::bitset<32> f_1_ECX_;
	static std::bitset<32> f_1_EDX_;
	static std::bitset<32> f_7_EBX_;
	static std::bitset<32> f_7_ECX_;
	static std::bitset<32> f_81_ECX_;
	static std::bitset<32> f_81_EDX_;
	static std::vector<std::array<int, 4>> data_;
	static std::vector<std::array<int, 4>> extdata_;
};
