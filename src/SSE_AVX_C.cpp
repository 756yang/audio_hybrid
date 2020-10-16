#include "SSE_AVX_C.h"


int InstructionIs::get_support_level()
{
	if (AVX512F()) return LAVX512;
	if (FMA()) return LFMA;
	if (AVX2()) return LAVX2;
	if(AVX()) return LAVX;
	if (SSE42()) return LSSE42;
	if (SSE41()) return LSSE41;
	if (SSSE3()) return LSSSE3;
	if (SSE3()) return LSSE3;
	if (SSE2()) return LSSE2;
	if (SSE()) return LSSE;
	return NO_SIMD;
}

int InstructionIs::get_crowd_level()
{
	if (AVX2()) return CLAVX2;
	if (SSE2()) return CLSSE2;
	return NO_SIMD;
}

// 静态成员变量必须在源文件声明
int InstructionIs::support_level;
int InstructionIs::crowd_level;
int InstructionIs::nIds_;
int InstructionIs::nExIds_;
std::string InstructionIs::vendor_;
std::string InstructionIs::brand_;
bool InstructionIs::isIntel_;
bool InstructionIs::isAMD_;
std::bitset<32> InstructionIs::f_1_ECX_;
std::bitset<32> InstructionIs::f_1_EDX_;
std::bitset<32> InstructionIs::f_7_EBX_;
std::bitset<32> InstructionIs::f_7_ECX_;
std::bitset<32> InstructionIs::f_81_ECX_;
std::bitset<32> InstructionIs::f_81_EDX_;
std::vector<std::array<int, 4>> InstructionIs::data_;
std::vector<std::array<int, 4>> InstructionIs::extdata_;

InstructionIs::InstructionIs()
{
	nIds_ = 0;
	nExIds_ = 0;
	data_ = {};
	extdata_ = {};
	isIntel_ = false;
	isAMD_ = false;
	f_1_ECX_ = { 0 };
	f_1_EDX_ = { 0 };
	f_7_EBX_ = { 0 };
	f_7_ECX_ = { 0 };
	f_81_ECX_ = { 0 };
	f_81_EDX_ = { 0 };
	//int cpuInfo[4] = {-1};
	std::array<int, 4> cpui;

	// Calling __cpuid with 0x0 as the function_id argument
	// gets the number of the highest valid function ID.
	__cpuid(cpui.data(), 0);
	nIds_ = cpui[0];

	for (int i = 0; i <= nIds_; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		data_.push_back(cpui);
	}

	// Capture vendor string
	char vendor[0x20];
	memset(vendor, 0, sizeof(vendor));
	*reinterpret_cast<int*>(vendor) = data_[0][1];
	*reinterpret_cast<int*>(vendor + 4) = data_[0][3];
	*reinterpret_cast<int*>(vendor + 8) = data_[0][2];
	vendor_ = vendor;
	if (vendor_ == "GenuineIntel")
	{
		isIntel_ = true;
	}
	else if (vendor_ == "AuthenticAMD")
	{
		isAMD_ = true;
	}

	// load bitset with flags for function 0x00000001
	if (nIds_ >= 1)
	{
		f_1_ECX_ = data_[1][2];
		f_1_EDX_ = data_[1][3];
	}

	// load bitset with flags for function 0x00000007
	if (nIds_ >= 7)
	{
		f_7_EBX_ = data_[7][1];
		f_7_ECX_ = data_[7][2];
	}

	// Calling __cpuid with 0x80000000 as the function_id argument
	// gets the number of the highest valid extended ID.
	__cpuid(cpui.data(), 0x80000000);
	nExIds_ = cpui[0];

	char brand[0x40];
	memset(brand, 0, sizeof(brand));

	for (int i = 0x80000000; i <= nExIds_; ++i)
	{
		__cpuidex(cpui.data(), i, 0);
		extdata_.push_back(cpui);
	}

	// load bitset with flags for function 0x80000001
	if (nExIds_ >= 0x80000001)
	{
		f_81_ECX_ = extdata_[1][2];
		f_81_EDX_ = extdata_[1][3];
	}

	// Interpret CPU brand string if reported
	if (nExIds_ >= 0x80000004)
	{
		memcpy(brand, extdata_[2].data(), sizeof(cpui));
		memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
		memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
		brand_ = brand;
	}

	support_level = get_support_level();
	crowd_level = get_crowd_level();
};

/*

#include <iostream>
#include <string>

INIT_INSTRUCTION_SET;

// Print out supported instruction set extensions
int main()
{
	auto& outstream = std::cout;

	auto support_message = [&outstream](std::string isa_feature, bool is_supported) {
		outstream << isa_feature << (is_supported ? " supported" : " not supported") << std::endl;
	};

	std::cout << InstructionIs::Vendor() << std::endl;
	std::cout << InstructionIs::Brand() << std::endl;

	support_message("3DNOW", InstructionIs::_3DNOW());
	support_message("3DNOWEXT", InstructionIs::_3DNOWEXT());
	support_message("ABM", InstructionIs::ABM());
	support_message("ADX", InstructionIs::ADX());
	support_message("AES", InstructionIs::AES());
	support_message("AVX", InstructionIs::AVX());
	support_message("AVX2", InstructionIs::AVX2());
	support_message("AVX512CD", InstructionIs::AVX512CD());
	support_message("AVX512ER", InstructionIs::AVX512ER());
	support_message("AVX512F", InstructionIs::AVX512F());
	support_message("AVX512PF", InstructionIs::AVX512PF());
	support_message("BMI1", InstructionIs::BMI1());
	support_message("BMI2", InstructionIs::BMI2());
	support_message("CLFSH", InstructionIs::CLFSH());
	support_message("CMPXCHG16B", InstructionIs::CMPXCHG16B());
	support_message("CX8", InstructionIs::CX8());
	support_message("ERMS", InstructionIs::ERMS());
	support_message("F16C", InstructionIs::F16C());
	support_message("FMA", InstructionIs::FMA());
	support_message("FSGSBASE", InstructionIs::FSGSBASE());
	support_message("FXSR", InstructionIs::FXSR());
	support_message("HLE", InstructionIs::HLE());
	support_message("INVPCID", InstructionIs::INVPCID());
	support_message("LAHF", InstructionIs::LAHF());
	support_message("LZCNT", InstructionIs::LZCNT());
	support_message("MMX", InstructionIs::MMX());
	support_message("MMXEXT", InstructionIs::MMXEXT());
	support_message("MONITOR", InstructionIs::MONITOR());
	support_message("MOVBE", InstructionIs::MOVBE());
	support_message("MSR", InstructionIs::MSR());
	support_message("OSXSAVE", InstructionIs::OSXSAVE());
	support_message("PCLMULQDQ", InstructionIs::PCLMULQDQ());
	support_message("POPCNT", InstructionIs::POPCNT());
	support_message("PREFETCHWT1", InstructionIs::PREFETCHWT1());
	support_message("RDRAND", InstructionIs::RDRAND());
	support_message("RDSEED", InstructionIs::RDSEED());
	support_message("RDTSCP", InstructionIs::RDTSCP());
	support_message("RTM", InstructionIs::RTM());
	support_message("SEP", InstructionIs::SEP());
	support_message("SHA", InstructionIs::SHA());
	support_message("SSE", InstructionIs::SSE());
	support_message("SSE2", InstructionIs::SSE2());
	support_message("SSE3", InstructionIs::SSE3());
	support_message("SSE4.1", InstructionIs::SSE41());
	support_message("SSE4.2", InstructionIs::SSE42());
	support_message("SSE4a", InstructionIs::SSE4a());
	support_message("SSSE3", InstructionIs::SSSE3());
	support_message("SYSCALL", InstructionIs::SYSCALL());
	support_message("TBM", InstructionIs::TBM());
	support_message("XOP", InstructionIs::XOP());
	support_message("XSAVE", InstructionIs::XSAVE());

	system("pause");
}
*/