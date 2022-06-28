//------ memory leaks detection --------

#ifdef _DEBUG

	#include <stdlib.h>
	#include <crtdbg.h>
	#include <iostream>

	#ifndef DBG_NEW					//detect new allocations leaks
		#define _CRTDBG_MAP_ALLOC		//detect malloc allocations leaks
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif

	#define meInitMemoryCheck() \
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)


	//class for checking memory leaks
	class MemoryCheckInitializer
	{
	public:
		MemoryCheckInitializer()
		{
			//_CrtSetBreakAlloc(23464); //paste here number from output
			meInitMemoryCheck();
		}

		//uncomment if you wan dump memory leaks to file
		//~MemoryCheckInitializer()
		//{
		//	FILE *pFile;
		//	freopen_s( &pFile, "memory_leaks.txt", "w", stdout);
		//	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		//	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		//	_CrtDumpMemoryLeaks();
		//	fclose(pFile);
		//}
	};
	static MemoryCheckInitializer mci_;

#endif // _DEBUG