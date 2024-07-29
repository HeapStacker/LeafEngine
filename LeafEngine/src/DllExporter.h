#pragma once
#ifdef LF_DYNAMIC
	#ifdef LF_WINDOWS
		#ifdef LF_DLL_EXPORT
			#define LF_LIB __declspec(dllexport)
		#else
			#define LF_LIB __declspec(dllimport)
		#endif
	#elif LF_LINUX
		#error Linux not implemented yet!
	#else
		#error We dont suport this platform!
	#endif
#else
	#define LF_LIB
#endif
