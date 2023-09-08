#ifdef _MSC_VER 
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

//#define DBG_PRINT

#ifdef DBG_PRINT
#define DEBUG_MSG(str)  std::cout << str << std::endl;
#else
#define DEBUG_MSG(str)
#endif

//#define DEBUG_PRINT_PRETTY

#ifdef DEBUG_PRINT_PRETTY
	#define PRINT_PRETTY() std::cout << __PRETTY_FUNCTION__ << endl
#else
	#define PRINT_PRETTY()
#endif

#define UNUSED(arg) ((void)(arg))