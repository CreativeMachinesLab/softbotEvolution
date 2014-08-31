#include "HCUBE_Defines.h"

namespace HCUBE
{
//#ifdef _DEBUG
#ifdef ONETHREAD
	const int NUM_THREADS = 1;
#else
	const int NUM_THREADS = 2;
#endif
}

