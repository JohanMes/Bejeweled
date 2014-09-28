#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "Components.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Benchmark : public Window {
	bool running;
	
	static void StaticOnRenderFrame(void* sender,float dt);
	 
	public:
		Benchmark();
		~Benchmark();
		
    	// Teststarter en saver
    	Button* btnstart;
    	Button* btnsave;
    	Button* btnprint;
    	
     	// Resultaten
    	Label* minfpstext;
    	Label* minfpsfield;
    	Label* avgfpstext;
    	Label* avgfpsfield;
       	Label* maxfpstext;
    	Label* maxfpsfield;
       	Label* cputext;
    	Label* cpufield;
       	Label* gputext;
    	Label* gpufield;
    	
    	void Start();
    	bool IsRunning();
    	void Stop();
    	void OnRenderFrame(float dt);
};

#endif
