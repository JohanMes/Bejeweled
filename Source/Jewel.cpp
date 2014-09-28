#include "Jewel.h"
#include "JohanEngine\Scene.h"
#include "JohanEngine\Messagebox.h"
#include "resource.h"

Jewel::Jewel(JewelType type,int x,int y) {
	
	// Do not throw this away
	object = new Object("Jewel");
	object->OnClick = ClickJewel;
	
	// Load model and materia;
	this->type = jtEmpty;
	SetType(type);
	
	// Do not animate to initial position
	SetPosition(x,y,false);
}

Jewel::~Jewel() {
	delete object;
}

void Jewel::SetPosition(int x,int y,bool animate) {
	this->x = x;
	this->y = y;
	
	// move object using animation
	if(object) {
		
		// Swap 3D models, using an animation
		if(animate) {
			Animation* swap = new Animation(object,atLinear);
			swap->AddKeyframe(0,object->GetTranslation());
			swap->AddKeyframe(AnimateSpeed*RandomRange(0.8,1.2),float3(2*x,2*y,0));			
		} else {
			object->SetTranslation(float3(2*x,2*y,0));
		}

		// Hide pos info inside name
		char name[128];
		snprintf(name,128,"Jewel %d %d",x,y);
		object->SetName(name);
	}
}

void Jewel::SetType(JewelType type) {
	
	if(this->type == type) {
		return;
	}
	
	// Remove models
	object->ClearDetailLevels();
	
	// Set new one
	this->type = type;
	char typestring[128];
	switch(type) {
		case jtRed: {
			strncpy(typestring,"Jewels\\Red",128);
			break;
		}
		case jtGreen: {
			strncpy(typestring,"Jewels\\Green",128);
			break;
		}
		case jtBlue: {
			strncpy(typestring,"Jewels\\Blue",128);
			break;
		}
		case jtYellow: {
			strncpy(typestring,"Jewels\\Yellow",128);
			break;
		}
		case jtPurple: {
			strncpy(typestring,"Jewels\\Purple",128);
			break;
		}
		case jtCyan: {
			strncpy(typestring,"Jewels\\Cyan",128);
			break;
		}
		case jtWhite: {
			strncpy(typestring,"Jewels\\White",128);
			break;
		}
		default: {
			return; // don't load anything
		}
	}
	
	// Load model
	char modelfilename[256];
	snprintf(modelfilename,256,"%s.obj",typestring);
	object->AddDetailLevel(modelfilename);

	// Load material
	char materialfilename[256];
	snprintf(materialfilename,256,"%s.mtl",typestring);
	object->material->LoadFromFile(materialfilename);
}

void ClickJewel(Object* sender) { // fake scope
	int x,y;
	
	// We hid type info inside name
	if(sscanf(sender->GetName(),"Jewel %d %d",&x,&y) == 2) {
		game->Click(x,y);
	}
}
