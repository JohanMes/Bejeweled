#ifndef OPTIONS_INCLUDE
#define OPTIONS_INCLUDE

#include "Components.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Options : public Window {
    static void OnShowDialog(Component* Sender);
    static void OnApplyClick(Component* Sender);
    public:
    	Options(const char* inifile);
    	~Options();
    	
    	Dropdown* cmbreso;
    	Edit* edshadowmapsize;
    	Edit* edshadowdistance;
    	Edit* edshadowbias;
    	Edit* edlodmultiplier;
    	Edit* edssaosamples;
    	Edit* edssaodistance;
    	// de rest van SSAO tweak je maar met de hand :D
    	Dropdown* cmbfilter;
    	Edit* edexposure;
    	// HDR bloom toevoegen?
    	Button* btnapply;
    	
		// Reso
		int backbufferwidth;
		int backbufferheight;
		int refreshrate;
		bool fullscreen;
		bool maxfsresolution;
		
		// Schaduwen
		int shadowmapsize; // 0 == disable
		float shadowdistance;
		float shadowbias;
		
		// Objects
		float lodmultiplier;
		float minviewdistance; // not tweakable
		float maxviewdistance; // idem
	
		// SSAO
		int ssaosamples; // 0 == disable
		float ssaodistance;
		float ssaorad;
		float ssaomulti;
		float ssaoscale;
		float ssaobias;
		float ssaodepth;
		
		// AA
		int aasamples; // TODO: remove?
		int aaquality; // TODO: remove?
		
		// Textures
		char texturefilter[128]; // random size guess
		int afsamples;
		
		// HDR
		bool enablehdr;
		float exposure;
		float bloomrampstart;
		float bloomrampend;
		float bloommultiplier;
		
		// Misc.
		bool enablevsync;
		bool usesoftware;  	
		
		// Instellingenlezers
		void LoadInterface(); // onshow
		void SaveInterface(); // onapplyclick, maar ook onclose?
		void SetToDefaults();
		void LoadFromFile(const char* inipath);
		void FindMaxResolution();
};

extern Options* options;

#endif
