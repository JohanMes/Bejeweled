#ifndef EVENT_H
#define EVENT_H

#include <vector>
using std::vector;

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class EventPointer {
	public:
		EventPointer(void* sender,void (*function)(void* sender,float param)) {
			this->sender = sender;
			this->function = function;
		}
		void* sender;
		void (*function)(void* sender,float param);
};

class DLLIMPORT Event {
	std::vector<EventPointer*> list;
	int persistentcount;
	public:
		Event();
		~Event();

		void AddPersistent(void* sender,void (*function)(void* sender,float param));
		void Add(void* sender,void (*function)(void* sender,float param));
		void Delete(EventPointer* value);
		void Execute(float param);
		void Clear(); // user function, keep 
};

#endif
