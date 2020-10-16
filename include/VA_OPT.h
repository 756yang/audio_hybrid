#ifndef __VA_OPT_H__
#define __VA_OPT_H__

//连接
#define _TC_CAT(x,y) x##y
//宏前有'#'则不会展开，这是为了让一层能够展开
#define TC_CAT(x,y) _TC_CAT(x,y)

//为获取参数个数而设的宏
#define TC_OPT(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,_65,_66,_67,_68,_69,_70,_71,_72,_73,_74,_75,_76,_77,_78,_79,_80,_81,_82,_83,_84,_85,_86,_87,_88,_89,_90,_91,_92,_93,_94,_95,_96,_97,_98,_99,_100,N,...) \
	N
//为阻止__VA_ARGS__只作为一个参数，需将其用括号保护
#define TC_OPT_(x) TC_OPT x

//计算宏参数个数，但在MSVC++中不能检测参数为空的情况
#define _TC_CAL1(...) \
	TC_OPT_((0,##__VA_ARGS__,100,99,98,97,96,95,94,93,92,91,90,89,88,87,86,85,84,83,82,81,80,79,78,77,76,75,74,73,72,71,70,69,68,67,66,65,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0))

/* 以下选择预编译，定义TC_CAL和TC_BOOL */
#ifdef _MSC_VER/* MSVC++平台 */
//__VA_ARGS__只会传给一个宏参数
#define _TC_OPT(_0,_1,N,...) N
//返回参数是否为空（在MSVC++中，##操作其实会失效）
#define TC_BOOL(...) _TC_OPT(0,##__VA_ARGS__,1,0)

#define _TC_CAL0(...) 0
//计算宏参数个数，可以检测参数为空的情况
#define TC_CAL(...) TC_CAT(_TC_CAL,TC_BOOL(__VA_ARGS__))(__VA_ARGS__)
//调用M##_N的函数，_N由参数个数确定，如：_TC(M,1,0)则为M2(1,0)，注意，定义宏函数M2的时候只能有一个参数，因为__VA_ARGS__作为一个整体传送。

#else/* 其他标准平台 */
//计算宏参数个数
#define TC_CAL _TC_CAL1
//返回参数是否为空
#define TC_BOOL(...) \
	TC_OPT_((0,##__VA_ARGS__,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0))

#endif /* 选择预编译结束 */


#define _TC(M,...) TC_CAT(M,TC_CAL(__VA_ARGS__))(__VA_ARGS__)

#define VA_OPT__0(_0,_1) _0
#define VA_OPT__1(_0,_1) _1
//类似__VA_OPT__，只是x不能是英文逗号，且需__VA_ARGS__做参数。__VA_ARGS__为空则返回_0，否则返回_1
#define VA_OPT(_0,_1,...) TC_CAT(VA_OPT__,TC_BOOL(__VA_ARGS__))(_0,_1)


#include <tchar.h>

#define __100__LT(x) _T(x)
#define __99__LT(x,...) __100__LT(x) VA_OPT(,__100__LT(__VA_ARGS__),__VA_ARGS__)
#define __98__LT(x,...) __100__LT(x) VA_OPT(,__99__LT(__VA_ARGS__),__VA_ARGS__)
#define __97__LT(x,...) __100__LT(x) VA_OPT(,__98__LT(__VA_ARGS__),__VA_ARGS__)
#define __96__LT(x,...) __100__LT(x) VA_OPT(,__97__LT(__VA_ARGS__),__VA_ARGS__)
#define __95__LT(x,...) __100__LT(x) VA_OPT(,__96__LT(__VA_ARGS__),__VA_ARGS__)
#define __94__LT(x,...) __100__LT(x) VA_OPT(,__95__LT(__VA_ARGS__),__VA_ARGS__)
#define __93__LT(x,...) __100__LT(x) VA_OPT(,__94__LT(__VA_ARGS__),__VA_ARGS__)
#define __92__LT(x,...) __100__LT(x) VA_OPT(,__93__LT(__VA_ARGS__),__VA_ARGS__)
#define __91__LT(x,...) __100__LT(x) VA_OPT(,__92__LT(__VA_ARGS__),__VA_ARGS__)
#define __90__LT(x,...) __100__LT(x) VA_OPT(,__91__LT(__VA_ARGS__),__VA_ARGS__)
#define __89__LT(x,...) __100__LT(x) VA_OPT(,__90__LT(__VA_ARGS__),__VA_ARGS__)
#define __88__LT(x,...) __100__LT(x) VA_OPT(,__89__LT(__VA_ARGS__),__VA_ARGS__)
#define __87__LT(x,...) __100__LT(x) VA_OPT(,__88__LT(__VA_ARGS__),__VA_ARGS__)
#define __86__LT(x,...) __100__LT(x) VA_OPT(,__87__LT(__VA_ARGS__),__VA_ARGS__)
#define __85__LT(x,...) __100__LT(x) VA_OPT(,__86__LT(__VA_ARGS__),__VA_ARGS__)
#define __84__LT(x,...) __100__LT(x) VA_OPT(,__85__LT(__VA_ARGS__),__VA_ARGS__)
#define __83__LT(x,...) __100__LT(x) VA_OPT(,__84__LT(__VA_ARGS__),__VA_ARGS__)
#define __82__LT(x,...) __100__LT(x) VA_OPT(,__83__LT(__VA_ARGS__),__VA_ARGS__)
#define __81__LT(x,...) __100__LT(x) VA_OPT(,__82__LT(__VA_ARGS__),__VA_ARGS__)
#define __80__LT(x,...) __100__LT(x) VA_OPT(,__81__LT(__VA_ARGS__),__VA_ARGS__)
#define __79__LT(x,...) __100__LT(x) VA_OPT(,__80__LT(__VA_ARGS__),__VA_ARGS__)
#define __78__LT(x,...) __100__LT(x) VA_OPT(,__79__LT(__VA_ARGS__),__VA_ARGS__)
#define __77__LT(x,...) __100__LT(x) VA_OPT(,__78__LT(__VA_ARGS__),__VA_ARGS__)
#define __76__LT(x,...) __100__LT(x) VA_OPT(,__77__LT(__VA_ARGS__),__VA_ARGS__)
#define __75__LT(x,...) __100__LT(x) VA_OPT(,__76__LT(__VA_ARGS__),__VA_ARGS__)
#define __74__LT(x,...) __100__LT(x) VA_OPT(,__75__LT(__VA_ARGS__),__VA_ARGS__)
#define __73__LT(x,...) __100__LT(x) VA_OPT(,__74__LT(__VA_ARGS__),__VA_ARGS__)
#define __72__LT(x,...) __100__LT(x) VA_OPT(,__73__LT(__VA_ARGS__),__VA_ARGS__)
#define __71__LT(x,...) __100__LT(x) VA_OPT(,__72__LT(__VA_ARGS__),__VA_ARGS__)
#define __70__LT(x,...) __100__LT(x) VA_OPT(,__71__LT(__VA_ARGS__),__VA_ARGS__)
#define __69__LT(x,...) __100__LT(x) VA_OPT(,__70__LT(__VA_ARGS__),__VA_ARGS__)
#define __68__LT(x,...) __100__LT(x) VA_OPT(,__69__LT(__VA_ARGS__),__VA_ARGS__)
#define __67__LT(x,...) __100__LT(x) VA_OPT(,__68__LT(__VA_ARGS__),__VA_ARGS__)
#define __66__LT(x,...) __100__LT(x) VA_OPT(,__67__LT(__VA_ARGS__),__VA_ARGS__)
#define __65__LT(x,...) __100__LT(x) VA_OPT(,__66__LT(__VA_ARGS__),__VA_ARGS__)
#define __64__LT(x,...) __100__LT(x) VA_OPT(,__65__LT(__VA_ARGS__),__VA_ARGS__)
#define __63__LT(x,...) __100__LT(x) VA_OPT(,__64__LT(__VA_ARGS__),__VA_ARGS__)
#define __62__LT(x,...) __100__LT(x) VA_OPT(,__63__LT(__VA_ARGS__),__VA_ARGS__)
#define __61__LT(x,...) __100__LT(x) VA_OPT(,__62__LT(__VA_ARGS__),__VA_ARGS__)
#define __60__LT(x,...) __100__LT(x) VA_OPT(,__61__LT(__VA_ARGS__),__VA_ARGS__)
#define __59__LT(x,...) __100__LT(x) VA_OPT(,__60__LT(__VA_ARGS__),__VA_ARGS__)
#define __58__LT(x,...) __100__LT(x) VA_OPT(,__59__LT(__VA_ARGS__),__VA_ARGS__)
#define __57__LT(x,...) __100__LT(x) VA_OPT(,__58__LT(__VA_ARGS__),__VA_ARGS__)
#define __56__LT(x,...) __100__LT(x) VA_OPT(,__57__LT(__VA_ARGS__),__VA_ARGS__)
#define __55__LT(x,...) __100__LT(x) VA_OPT(,__56__LT(__VA_ARGS__),__VA_ARGS__)
#define __54__LT(x,...) __100__LT(x) VA_OPT(,__55__LT(__VA_ARGS__),__VA_ARGS__)
#define __53__LT(x,...) __100__LT(x) VA_OPT(,__54__LT(__VA_ARGS__),__VA_ARGS__)
#define __52__LT(x,...) __100__LT(x) VA_OPT(,__53__LT(__VA_ARGS__),__VA_ARGS__)
#define __51__LT(x,...) __100__LT(x) VA_OPT(,__52__LT(__VA_ARGS__),__VA_ARGS__)
#define __50__LT(x,...) __100__LT(x) VA_OPT(,__51__LT(__VA_ARGS__),__VA_ARGS__)
#define __49__LT(x,...) __100__LT(x) VA_OPT(,__50__LT(__VA_ARGS__),__VA_ARGS__)
#define __48__LT(x,...) __100__LT(x) VA_OPT(,__49__LT(__VA_ARGS__),__VA_ARGS__)
#define __47__LT(x,...) __100__LT(x) VA_OPT(,__48__LT(__VA_ARGS__),__VA_ARGS__)
#define __46__LT(x,...) __100__LT(x) VA_OPT(,__47__LT(__VA_ARGS__),__VA_ARGS__)
#define __45__LT(x,...) __100__LT(x) VA_OPT(,__46__LT(__VA_ARGS__),__VA_ARGS__)
#define __44__LT(x,...) __100__LT(x) VA_OPT(,__45__LT(__VA_ARGS__),__VA_ARGS__)
#define __43__LT(x,...) __100__LT(x) VA_OPT(,__44__LT(__VA_ARGS__),__VA_ARGS__)
#define __42__LT(x,...) __100__LT(x) VA_OPT(,__43__LT(__VA_ARGS__),__VA_ARGS__)
#define __41__LT(x,...) __100__LT(x) VA_OPT(,__42__LT(__VA_ARGS__),__VA_ARGS__)
#define __40__LT(x,...) __100__LT(x) VA_OPT(,__41__LT(__VA_ARGS__),__VA_ARGS__)
#define __39__LT(x,...) __100__LT(x) VA_OPT(,__40__LT(__VA_ARGS__),__VA_ARGS__)
#define __38__LT(x,...) __100__LT(x) VA_OPT(,__39__LT(__VA_ARGS__),__VA_ARGS__)
#define __37__LT(x,...) __100__LT(x) VA_OPT(,__38__LT(__VA_ARGS__),__VA_ARGS__)
#define __36__LT(x,...) __100__LT(x) VA_OPT(,__37__LT(__VA_ARGS__),__VA_ARGS__)
#define __35__LT(x,...) __100__LT(x) VA_OPT(,__36__LT(__VA_ARGS__),__VA_ARGS__)
#define __34__LT(x,...) __100__LT(x) VA_OPT(,__35__LT(__VA_ARGS__),__VA_ARGS__)
#define __33__LT(x,...) __100__LT(x) VA_OPT(,__34__LT(__VA_ARGS__),__VA_ARGS__)
#define __32__LT(x,...) __100__LT(x) VA_OPT(,__33__LT(__VA_ARGS__),__VA_ARGS__)
#define __31__LT(x,...) __100__LT(x) VA_OPT(,__32__LT(__VA_ARGS__),__VA_ARGS__)
#define __30__LT(x,...) __100__LT(x) VA_OPT(,__31__LT(__VA_ARGS__),__VA_ARGS__)
#define __29__LT(x,...) __100__LT(x) VA_OPT(,__30__LT(__VA_ARGS__),__VA_ARGS__)
#define __28__LT(x,...) __100__LT(x) VA_OPT(,__29__LT(__VA_ARGS__),__VA_ARGS__)
#define __27__LT(x,...) __100__LT(x) VA_OPT(,__28__LT(__VA_ARGS__),__VA_ARGS__)
#define __26__LT(x,...) __100__LT(x) VA_OPT(,__27__LT(__VA_ARGS__),__VA_ARGS__)
#define __25__LT(x,...) __100__LT(x) VA_OPT(,__26__LT(__VA_ARGS__),__VA_ARGS__)
#define __24__LT(x,...) __100__LT(x) VA_OPT(,__25__LT(__VA_ARGS__),__VA_ARGS__)
#define __23__LT(x,...) __100__LT(x) VA_OPT(,__24__LT(__VA_ARGS__),__VA_ARGS__)
#define __22__LT(x,...) __100__LT(x) VA_OPT(,__23__LT(__VA_ARGS__),__VA_ARGS__)
#define __21__LT(x,...) __100__LT(x) VA_OPT(,__22__LT(__VA_ARGS__),__VA_ARGS__)
#define __20__LT(x,...) __100__LT(x) VA_OPT(,__21__LT(__VA_ARGS__),__VA_ARGS__)
#define __19__LT(x,...) __100__LT(x) VA_OPT(,__20__LT(__VA_ARGS__),__VA_ARGS__)
#define __18__LT(x,...) __100__LT(x) VA_OPT(,__19__LT(__VA_ARGS__),__VA_ARGS__)
#define __17__LT(x,...) __100__LT(x) VA_OPT(,__18__LT(__VA_ARGS__),__VA_ARGS__)
#define __16__LT(x,...) __100__LT(x) VA_OPT(,__17__LT(__VA_ARGS__),__VA_ARGS__)
#define __15__LT(x,...) __100__LT(x) VA_OPT(,__16__LT(__VA_ARGS__),__VA_ARGS__)
#define __14__LT(x,...) __100__LT(x) VA_OPT(,__15__LT(__VA_ARGS__),__VA_ARGS__)
#define __13__LT(x,...) __100__LT(x) VA_OPT(,__14__LT(__VA_ARGS__),__VA_ARGS__)
#define __12__LT(x,...) __100__LT(x) VA_OPT(,__13__LT(__VA_ARGS__),__VA_ARGS__)
#define __11__LT(x,...) __100__LT(x) VA_OPT(,__12__LT(__VA_ARGS__),__VA_ARGS__)
#define __10__LT(x,...) __100__LT(x) VA_OPT(,__11__LT(__VA_ARGS__),__VA_ARGS__)
#define __9__LT(x,...) __100__LT(x) VA_OPT(,__10__LT(__VA_ARGS__),__VA_ARGS__)
#define __8__LT(x,...) __100__LT(x) VA_OPT(,__9__LT(__VA_ARGS__),__VA_ARGS__)
#define __7__LT(x,...) __100__LT(x) VA_OPT(,__8__LT(__VA_ARGS__),__VA_ARGS__)
#define __6__LT(x,...) __100__LT(x) VA_OPT(,__7__LT(__VA_ARGS__),__VA_ARGS__)
#define __5__LT(x,...) __100__LT(x) VA_OPT(,__6__LT(__VA_ARGS__),__VA_ARGS__)
#define __4__LT(x,...) __100__LT(x) VA_OPT(,__5__LT(__VA_ARGS__),__VA_ARGS__)
#define __3__LT(x,...) __100__LT(x) VA_OPT(,__4__LT(__VA_ARGS__),__VA_ARGS__)
#define __2__LT(x,...) __100__LT(x) VA_OPT(,__3__LT(__VA_ARGS__),__VA_ARGS__)
#define __1__LT(x,...) __100__LT(x) VA_OPT(,__2__LT(__VA_ARGS__),__VA_ARGS__)
//在MSVC++环境中，以上定义涉及到了二次宏展开，所以__2__LT(__VA_ARGS__)能正确展开，即__VA_ARGS__在二次宏展开被分为多个参数，一次宏展开其整体作为一个参数
//在GCC/G++中，__VA_ARGS__直接被展开为多参数
//用于对__VA_ARGS__每一个参数进行同一操作，默认是宽字符转换拼接，最多能处理100个参数
#define _LT __1__LT

#endif