#ifndef JEWEL_H
#define JEWEL_H

#include "JohanEngine\Object.h"

enum JewelType {
	jtEmpty, // default type (-1)
	jtNone, // empty square (0)
	jtRed,
	jtBlue,
	jtGreen,
	jtYellow,
	jtPurple,
	jtCyan,
	jtWhite,
};

class Jewel {
	public:
		Jewel(JewelType type,int x,int y);
		~Jewel();
		
		Object* object;
		int x; // from bottom left, 0 based
		int y; // idem
		JewelType type;
		
		void SetPosition(int x,int y,bool animate);
		void SetType(JewelType type);
};

void ClickJewel(Object* sender);

#endif
