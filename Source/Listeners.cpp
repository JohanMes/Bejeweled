#include "Listeners.h"
#include "JohanEngine\Dialogs.h"
#include "JohanEngine\Renderer.h"
#include "JohanEngine\Clock.h"
#include "resource.h"

void ToggleFullscreen(void* data) {
	renderer->ToggleFullScreen();
}
void ToggleConsole(void* data) {
	console->Toggle();
}
void ToggleOptions(void* data) {
	options->Toggle();
}
void OnRenderFrame(void* sender,float dt) {
	
	// Remember it only here
	static float time = 0;
	
	// Update frequency every second
	float elapsed = clock->GetTimeSec();
	if(elapsed > 0.1f) {
		time += elapsed;

		char buf[128];
		snprintf(buf,128,"Frequency: %g",game->score/time);
		freqlabel->SetCaption(buf);
	
		clock->Reset();
	}
}
void OnMouseMove(WPARAM wParam,LPARAM lParam,int dx,int dy) {
	if(!flashlight) {
		return;
	}
	
	float3 worldpos = camera->pos;
	float3 worlddir = renderer->GetPixelWorldRay(ui->mousepos).SetLength(1000);
	
	// Do collision with bg only
	if(background) {
		Collision groundintersect = background->IntersectModel(worldpos,worlddir);
		
		// We hit something!
		if(groundintersect.object) {
			flashlight->SetLookat(groundintersect.point);
			flashlight->SetPosition(groundintersect.point + float3(0,0,20));
		}
	}
}
void OnMouseDown(WPARAM wParam,LPARAM lParam,bool handled) {
	if(handled) {
		return;
	}
	
	switch(wParam) {
		case MK_LBUTTON: {
			float3 worldpos = camera->pos;
			float3 worlddir = renderer->GetPixelWorldRay(ui->mousepos).SetLength(1000); // reach 1000m
			Object* mouseobject = scene->IntersectScene(worldpos,worlddir).object;
			if(mouseobject && mouseobject->OnClick) {
				mouseobject->OnClick(mouseobject);
			}
			break;
		}
		case MK_MBUTTON:
		case MK_RBUTTON: {
			game->Deselect();
			break;
		}
	}
}
void OnMouseWheel(WPARAM wParam,LPARAM lParam) {
	signed short scroll = HIWORD(wParam);
	if(scroll > 0) {
		camera->Move(2*camera->dir);
	} else {
		camera->Move(-2*camera->dir);
	}
}
void RefillGame(Component* sender) {
	game->Refill();
}
void ResizeGame(Component* sender) {
	int size = 8;
	sscanf(sizeedit->GetCaption(),"%d",&size);
	game->Resize(size,size);
	game->Deselect();
}
