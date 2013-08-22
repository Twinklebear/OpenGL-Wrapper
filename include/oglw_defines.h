#ifndef OGLW_DEF_H
#define OGLW_DEF_H

#ifdef _WIN32
	#ifdef OGLW_EXPORT
		#define OGLW_API __declspec(dllexport)
		#define OGLW_TEMPLATE 
	#else
		#define OGLW_API __declspec(dllimport)
		#define OGLW_TEMPLATE extern
	#endif
#else
	#define OGLW_API
#endif

#endif
