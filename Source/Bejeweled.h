#ifndef BEJEWELED_H
#define BEJEWELED_H

#include "Jewel.h"

#include <vector>
using std::vector;

class Bejeweled {
	public:
		Bejeweled();
		~Bejeweled();
		
		vector<vector<Jewel*>> grid;
		int xsize;
		int ysize;
		Jewel* selectiona;
		Jewel* selectionb;
		Object* selector; // placed out of sight when not in use
		int multiplier;
		int score;
		
		// Misc.
		JewelType GetRandomJewel();
		
		// Grid mangling
		void Clear();
		void Resize(int xsize,int ysize);
		void Click(int x,int y);
		bool SwapAllowed(Jewel* a,Jewel* b);
		void Swap(Jewel* a,Jewel* b);
		void SwapSwap(Jewel* a,Jewel* b);
		void Deselect();
		void HideCaret();
		
		// Score stuff...
		void SetScore(int value);
		void AddScore(int value);
		
		// Updating stuff
		void Update();
		bool FindCombos(); // returns true on change
		bool FindCombo(int x,int y); // pretend type is at [x,y], check for points
		void ApplyGravity();
		void Refill();
};

void TimerUpdate(void* data);

#endif
