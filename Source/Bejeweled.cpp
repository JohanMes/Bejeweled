#include "Bejeweled.h"
#include "JohanEngine\Camera.h"
#include "JohanEngine\Messagebox.h"
#include "JohanEngine\Animation.h"
#include "resource.h"

Bejeweled::Bejeweled() {
	selector = new Object("Selector","SelectionBox.obj","Glass.mtl",float3(-1000,0,0),0,1);
}
Bejeweled::~Bejeweled() {
	Clear();
}

void Bejeweled::Refill() {
	int oldscore = score;
	for(int x = 0;x < xsize;x++) {
		for(int y = 0;y < ysize;y++) {
			delete grid[x][y];
			grid[x][y] = new Jewel(GetRandomJewel(),x,y+ysize);
			grid[x][y]->SetPosition(x,y,true);
		}
	}
	Deselect();
	Update();
	SetScore(oldscore); // do NOT give points for random combos
}
void Bejeweled::Resize(int xsize,int ysize) {
	Clear();
	
	this->xsize = xsize;
	this->ysize = ysize;

	grid.resize(xsize);
	for(int x = 0;x < xsize;x++) {
		grid[x].resize(ysize);
	}
	
	for(int x = 0;x < xsize;x++) {
		for(int y = 0;y < ysize;y++) {
			grid[x][y] = new Jewel(GetRandomJewel(),x,y+ysize);
			grid[x][y]->SetPosition(x,y,true);
		}
	}
	
	// Center camera
	float3 focus = float3(2 * (xsize-1)/2,2 * (ysize-1)/2,0);

	// Make sure the camera views everything
	float distance = 2.5f * xsize/(2 * tan(camera->fov/2)); // allow 80% extra vision
	
	camera->SetPos(focus + float3(0.001,0.001,distance));
	camera->SetLookAt(focus);
	
	// Center background
	background->SetTranslation(float3(xsize-1,ysize-1 - 128,-1));
		
	// Update field
	Update();
	SetScore(0);
}
void Bejeweled::Clear() {
	for(unsigned int x = 0;x < grid.size();x++) {
		for(unsigned int y = 0;y < grid[x].size();y++) {
			delete grid[x][y];
		}
	}
	grid.clear();
	
	xsize = 0;
	ysize = 0;
	
	score = 0;
	multiplier = 0;
}
void Bejeweled::Deselect() {
	selectiona = NULL;
	selectionb = NULL;
	HideCaret();
}
void Bejeweled::HideCaret() {
	selector->SetTranslation(float3(-1000,0,0)); // undo selection
}
void Bejeweled::Click(int x,int y) {
	if(!selectiona) { // start action
		selectiona = grid[x][y];
		selector->SetTranslation(float3(2*x,2*y,0.1));
	} else { // end action
		selectionb = grid[x][y];
		HideCaret();
		if(SwapAllowed(selectiona,selectionb)) {
			Swap(selectiona,selectionb);
		} else {
			SwapSwap(selectiona,selectionb);
		}
		Deselect();
	}
}
bool Bejeweled::SwapAllowed(Jewel* a,Jewel* b) {
	// only allow horizontal movement
	if(a->y != b->y && a->x != b->x) { 
		return false;
	}

	// Only allow swapping adjacent items
	if(abs(a->x - b->x) > 1) { // horizontal
		return false;
	}
	if(abs(a->y - b->y) > 1) { // vertical
		return false;
	}
	
	// Exchange types temporarily
	std::swap(grid[a->x][a->y]->type,grid[b->x][b->y]->type);
	
	// Now demand a combination of the new positions
	bool result = (FindCombo(a->x,a->y) || FindCombo(b->x,b->y));
	
	// And exchange back
	std::swap(grid[a->x][a->y]->type,grid[b->x][b->y]->type);
	
	return result;
}
void Bejeweled::Swap(Jewel* a,Jewel* b) {
	
	// tmp for swap
	int ax = a->x;
	int ay = a->y;

	a->SetPosition(b->x,b->y,true);
	b->SetPosition(ax,ay,true);
	
	// Swap in grid
	grid[a->x][a->y] = a;
	grid[b->x][b->y] = b;
	
	// Check if swapping is allowed when animation is done
	new Timer(AnimateSpeed * 1200,true,TimerUpdate);
}
void Bejeweled::SwapSwap(Jewel* a,Jewel* b) {
	
	// Move back and forth
	Animation* ania = new Animation(a->object,atLinear);
	ania->AddKeyframe(0*AnimateSpeed,a->object->GetTranslation());
	ania->AddKeyframe(1*AnimateSpeed,b->object->GetTranslation());
	ania->AddKeyframe(2*AnimateSpeed,a->object->GetTranslation());
	
	// Idem
	Animation* anib = new Animation(b->object,atLinear);
	anib->AddKeyframe(0*AnimateSpeed,b->object->GetTranslation());
	anib->AddKeyframe(1*AnimateSpeed,a->object->GetTranslation());
	anib->AddKeyframe(2*AnimateSpeed,b->object->GetTranslation());
}
void Bejeweled::Update() {
	if(FindCombos()) {
		multiplier++;
		ApplyGravity();
		
		// Update when everyone is done moving
		new Timer(AnimateSpeed * 1200,true,TimerUpdate);
	} else {
		multiplier = 1;
	}
}
bool Bejeweled::FindCombo(int x,int y) { // COPY FROM FindCombos

	// Check each separate tile for combinations
	JewelType type = grid[x][y]->type;
	if(type == jtNone) {
		return false;
	}
	
	// Left
	int xstart = x;
	while(xstart > 0 && grid[xstart-1][y]->type == type) {
		xstart--; // don't step onto the different color
	}
	
	// Right
	int xend = x;
	while(xend < xsize-1 && grid[xend+1][y]->type == type) {
		xend++; // don't step onto the different color
	}
	
	// Down
	int ystart = y;
	while(ystart > 0 && grid[x][ystart-1]->type == type) {
		ystart--; // don't step onto the different color
	}
	
	// Up
	int yend = y;
	while(yend < ysize-1 && grid[x][yend+1]->type == type) {
		yend++; // don't step onto the different color
	}
	
	int xstreak = xend - xstart + 1;
	int ystreak = yend - ystart + 1;
	if(xstreak >= 3 or ystreak >= 3) {
		return true;
	} else {
		return false;
	}
}
bool Bejeweled::FindCombos() {
	
	bool changed = false;
	
	// Check all rows for horizontal matches
	for(int x = 0;x < xsize;x++) {
		for(int y = 0;y < ysize;y++) {
			
			// Check each separate tile for combinations
			JewelType type = grid[x][y]->type;
			if(type == jtNone) {
				continue;
			}
			
			// Left
			int xstart = x;
			while(xstart > 0 && grid[xstart-1][y]->type == type) {
				xstart--; // don't step onto the different color
			}
			
			// Right
			int xend = x;
			while(xend < xsize-1 && grid[xend+1][y]->type == type) {
				xend++; // don't step onto the different color
			}
			
			// Down
			int ystart = y;
			while(ystart > 0 && grid[x][ystart-1]->type == type) {
				ystart--; // don't step onto the different color
			}
			
			// Up
			int yend = y;
			while(yend < ysize-1 && grid[x][yend+1]->type == type) {
				yend++; // don't step onto the different color
			}
			
			int xstreak = xend - xstart + 1;
			int ystreak = yend - ystart + 1;
			
			// Remove horizontal streak
			if(xstreak >= 3) {
				changed = true;
				for(int i = xstart;i <= xend;i++) {
					grid[i][y]->SetType(jtNone); // flag for deletion
				}
				AddScore(xstreak * 10 * multiplier);
			}

			// Vertical too
			if(ystreak >= 3) {
				changed = true;
				for(int i = ystart;i <= yend;i++) {
					grid[x][i]->SetType(jtNone);
				}
				AddScore(ystreak * 10 * multiplier);
			}
		}
	}
	
	return changed;
}
void Bejeweled::ApplyGravity() {
	for(int x = 0;x < xsize;x++) {
		for(int y = 0;y < ysize;y++) {
			if(grid[x][y]->type == jtNone) {// a hole we left behind
				
				// Determine how far we are falling
				int ystart = y;
				int ywalk = y;
				while(ywalk < ysize-1 && grid[x][ywalk+1]->type == jtNone) {
					ywalk++;
				}
				int yfall = ywalk - ystart + 1;
				
				// Delete empty items
				for(int i = ystart;i <= ywalk;i++) {
					delete grid[x][i];
				}
				
				// Apply falldown animation
				for(int i = ywalk+1;i < ysize;i++) {
					grid[x][i-yfall] = grid[x][i]; // fill with items from above
					grid[x][i-yfall]->SetPosition(x,i-yfall,true); // animate
				}
				
				// Create new items above
				for(int i = ysize-yfall;i < ysize;i++) {
					grid[x][i] = new Jewel(GetRandomJewel(),x,i+yfall); // fill gap above
					grid[x][i]->SetPosition(x,i,true);
				}
			}
		}
	}
}
void Bejeweled::SetScore(int value) {
	score = value;
	
	char buf[128];
	snprintf(buf,128,"Score: %d",score);
	scorelabel->SetCaption(buf);
}
void Bejeweled::AddScore(int value) {
	SetScore(score + value);
}
JewelType Bejeweled::GetRandomJewel() {
	float random = RandomRange(0,7);
	if(random < 1) {
		return jtRed;
	} else if(random < 2) {
		return jtGreen;
	} else if(random < 3) {
		return jtBlue;
	} else if(random < 4) {
		return jtYellow;
	} else if(random < 5) {
		return jtPurple;
	} else if(random < 6) {
		return jtCyan;
	} else {
		return jtWhite;
	}
}
void TimerUpdate(void* data) {
	game->Update();
}
