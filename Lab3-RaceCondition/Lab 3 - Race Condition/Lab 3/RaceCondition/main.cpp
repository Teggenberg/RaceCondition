///////////////////////////////////////////////////////////////////////////////////
// TODO: #include any files need
///////////////////////////////////////////////////////////////////////////////////
#include <iostream>;
#include <thread>;
#include <vector>;
#include <mutex>;

// Include file and line numbers for memory leak detection for visual studio in debug mode
#if defined _MSC_VER && defined _DEBUG
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define ENABLE_LEAK_DETECTION() _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF)
#else
	#define ENABLE_LEAK_DETECTION()
#endif

//used for race condition 3
int turn = 0;

struct ThreadStruct
{
	// ID of the thread
	int id;
	// Length of the shared string
	int sharedStringLength;
	// Number of strings a single thread will generate
	int numberOfStringsToGenerate;
	// Shared string that will be generate in each thread. This memory is shared among all threads.
	char *sharedString;
	// variable to determine runtype
	int run_type;
	
	///////////////////////////////////////////////////////////////////////////////////
	// TODO: Add any extra variables needed by the threads here
	///////////////////////////////////////////////////////////////////////////////////	

};

///////////////////////////////////////////////////////////////////////////////////////////
// Prompts the user to press enter and waits for user input
///////////////////////////////////////////////////////////////////////////////////////////
void Pause()
{
	printf("Press enter to continue\n");
	getchar();
}

///////////////////////////////////////////////////////////////////////////////////
// Entry point for worker threads. 
//
// Arguments:
//   threadData - Pointer to per-thread data for this thread.
///////////////////////////////////////////////////////////////////////////////////
void ThreadEntryPoint(ThreadStruct *threadData)
{
	///////////////////////////////////////////////////////////////////////////////////
	// TODO: Add code to this function to make it run according to the run type.
	//		 However do NOT duplicate the following code.
	///////////////////////////////////////////////////////////////////////////////////	

	std::mutex m;
	//if (threadData->run_type == 1) m.lock();
	for(int i = 0; i < threadData->numberOfStringsToGenerate; i++, std::this_thread::sleep_for(std::chrono::milliseconds(10)))
	{
		if (threadData->run_type == 3) {
			
			while (threadData->id != turn) {
				continue;
			 }
			
			
		}
		for(int j = 0; j < threadData->sharedStringLength; j++)
		{
			
			if(threadData->run_type == 0)std::this_thread::sleep_for(std::chrono::milliseconds(1));
			
			if (threadData->run_type == 1 || threadData->run_type == 3) m.lock();
			threadData->sharedString[j] = 'A' + threadData->id;
			if (threadData->run_type == 1 || threadData->run_type == 3) m.unlock();
		}
		
		printf("Thread %d: %s\n", threadData->id, threadData->sharedString);
		
	}
	turn++;
	

	///////////////////////////////////////////////////////////////////////////////////
}

int main(int argc, char** argv)
{
	ENABLE_LEAK_DETECTION();

	int threadCount = 0;
	int sharedStringLength = 0;
	int numberOfStringsToGenerate = 0;
	int runType = 0;
	char *sharedString = nullptr;
	ThreadStruct *perThreadData = nullptr;

	///////////////////////////////////////////////////////////////////////////////////
	// TODO:: Handle the runType command line argument.
	//
	//		  The following code already handles the first 4 arguments by ignoring
	//		  the first argument (which is just the name of the program) and reading
	//		  in the next 3 (threadCount, sharedStringLength, and numberOfStringsToGenerate).
	//
	//		  You will need to add code to this section to read in the final command line
	//		  argument (the runType). 
	///////////////////////////////////////////////////////////////////////////////////

	
	
	if (argc != 5)
	{
		fprintf(stderr, "Error: missing or incorrect command line arguments\n\n"); 
		fprintf(stderr, "Usage: RaceCondition threadCount sharedStringLength numberOfStringsToGenerate runType\n\n");
		fprintf(stderr, "Arguments:\n");
		fprintf(stderr, "    threadCount                  Number of threads to create.\n");
		fprintf(stderr, "    sharedStringLength           Length of string to generate.\n");
		fprintf(stderr, "    numberOfStringsToGenerate    Number of strings to generate per thread.\n");
		fprintf(stderr, "    runType                      The run type.\n\n");
		Pause();
		return 1;
	}


	threadCount = atoi(argv[1]);
	sharedStringLength = atoi(argv[2]);
	numberOfStringsToGenerate = atoi(argv[3]);
	runType = atoi(argv[4]);
	//

	if(threadCount < 0 || sharedStringLength < 0 || numberOfStringsToGenerate < 0 || runType < 0)
	{
		fprintf(stderr, "Error: All arguments must be positive integer values.\n");
		Pause();
		return 1;
	}

	printf("%d thread(s), string sharedStringLength %d, %d iterations, %d runType\n",
		threadCount, sharedStringLength, numberOfStringsToGenerate, runType);
	
	sharedString = new char[sharedStringLength + 1];
	memset(sharedString, 0, sharedStringLength + 1);
	perThreadData = new ThreadStruct[threadCount];
	
	///////////////////////////////////////////////////////////////////////////////////
	// TODO:: You will need a container to store the thread class objects. It is up to you
	//   to decide how you want to store the threads.
	///////////////////////////////////////////////////////////////////////////////////	

	std::vector<std::thread*> t;
	// NOTE: Do NOT change this for loop header
	for (int i = threadCount - 1; i >= 0; i--)
	{
		perThreadData[i].id = i;
		perThreadData[i].sharedStringLength = sharedStringLength;
		perThreadData[i].numberOfStringsToGenerate = numberOfStringsToGenerate;
		perThreadData[i].sharedString = sharedString;
		perThreadData[i].run_type = runType;

		

		
		t.push_back(new std::thread(ThreadEntryPoint, &perThreadData[i]));

		///////////////////////////////////////////////////////////////////////////////////
		// TODO:: Setup any additional variables in perThreadData and start the threads.
		//			MUST be done in this for loop.
		///////////////////////////////////////////////////////////////////////////////////
	}


	///////////////////////////////////////////////////////////////////////////////////
	// TODO:: Wait for all of the threads to finish. Since we are using
	//   Joinable threads we must Join each one. Joining a thread will cause
	//   the calling thread (main in this case) to block until the thread being
	//   joined has completed executing.
	///////////////////////////////////////////////////////////////////////////////////	
	
	Pause();
	
	for (auto thrd : t) {

		thrd->join();
		delete thrd;
		
		
	}
	///////////////////////////////////////////////////////////////////////////////////
	// TODO: Clean up
	///////////////////////////////////////////////////////////////////////////////////
	delete sharedString;
	delete[] perThreadData;
	return 0;
}
