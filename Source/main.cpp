#include "JohanEngine\Renderer.h"
#include "JohanEngine\Dialogs.h"
#include "JohanEngine\Timer.h"
#include "JohanEngine\Options.h"
#include "Listeners.h"
#include "Bejeweled.h"
#include "resource.h"

void InitGame() {
	renderer->ShowTooltip(0);
	renderer->SetTimeMulti(1);
	
	ui->AddKey(new Key(VK_F2,ToggleFullscreen));
	ui->AddKey(new Key(VK_F11,ToggleConsole));
	ui->AddKey(new Key(VK_F12,ToggleOptions));
	
	ui->OnMouseMove = OnMouseMove;
	ui->OnMouseDown = OnMouseDown;
	ui->OnMouseWheel = OnMouseWheel;
	
	// Add a menu
	Window* menu = new Window(5,5,120,190,"Bejeweled");
	
	menu->AddChild(new Label(5,25,30,20,"Size:",true));
	sizeedit = new Edit(40,25,75,20,"8");
	menu->AddChild(sizeedit);
	
	scorelabel = new Label(5,55,110,20,"Score: 0",false);
	menu->AddChild(scorelabel);
	
	freqlabel = new Label(5,85,110,20,"Frequency: 0",false);
	menu->AddChild(freqlabel);
	
	Button* reset = new Button(5,110,110,35,"Refill!");
	reset->backcolor = float4(0.8,0,0,1);
	reset->hotcolor = float4(1,0,0,1);
	reset->OnClick = RefillGame;
	menu->AddChild(reset);
	
	Button* resize = new Button(5,150,110,35,"Resize!");
	resize->backcolor = float4(0,0.8,0,1);
	resize->hotcolor = float4(0,1,0,1);
	resize->OnClick = ResizeGame;
	menu->AddChild(resize);
	
	ui->AddComponent(menu);
	menu->Show(1);
	
//	// Add flash light
//	flashlight = new Spotlight(
//		float3(0,0,0), // aim it at the center of the field
//		float3(0,0,0),
//		float3(20,20,20),
//		45,
//		true);
//	flashlight->SetCastShadows(true);

	// Or sunlight
	Dirlight* dirlight = new Dirlight(
		float3(1,1,1), // aim it at the center of the field
		float3(1.4,1.4,1.2),
		true);
	dirlight->SetCastShadows(true);
	scene->lights->SetAmbientlight(0);
	
	// Add a background
	background = new Object("Background","Scene\\Background.obj","WoodBoards.mtl",0,0,1);
	background->castshadows = false;
		
	// Scan size for the first time
	game = new Bejeweled();
	ResizeGame(NULL);
	
	// Update frequency every second...
	renderer->OnRenderFrame->Add(NULL,OnRenderFrame);
	clock = new Clock(true);
	
	// Position light once
	OnMouseMove(0,0,0,0);
}

void DeleteGame() {
	delete game;
	delete clock;
}

// This is where Windows sends user input messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	// send message to 3D interface
	ui->OnMessage(hwnd,Message,wParam,lParam);
	
	// Perform more handling
	switch(Message) {
		case WM_DESTROY: {
			PostQuitMessage(0); // we are asked to close: kill main thread
			break;
		}
		default: {
			return DefWindowProc(hwnd, Message, wParam, lParam);
		}
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEXA wc = {0};
	MSG Msg = {0};

	// Create a window with these properties
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = "WindowClass";
	wc.hIcon         = LoadIcon(hInstance,"A"); // laad projecticoon
	wc.hIconSm       = LoadIcon(hInstance,"A");
	
	// Say hi to Windows
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	// Set up a window with 1024x768 usable pixels
	RECT result = {0,0,1024,768};
	AdjustWindowRect(&result,WS_VISIBLE|WS_OVERLAPPEDWINDOW,false);

	// Create a window with a border and 'client rect' of 1024x768
	hwnd = CreateWindow("WindowClass","Bejeweled",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		result.right - result.left, // width
		result.bottom - result.top, // height
		NULL,NULL,hInstance,NULL);
	if(hwnd == NULL) {
		MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	// Init render loop
	InitEngine();
	
	InitGame();
	
	// Handle user input. If done, render a frame. Goto 1
	while(Msg.message != WM_QUIT) {
		while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		
		if(!renderer->paused) {
			renderer->Begin(false);
			renderer->DrawScene(scene);
			renderer->End();
		} else {
			Sleep(100);
		}
	}
	
	DeleteGame();
	
	// Render loop stopped due to Alt+F4 etc? Delete everything
	DeleteEngine();

	return Msg.wParam;
}
