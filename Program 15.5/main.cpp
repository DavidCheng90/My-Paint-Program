// Program 15.5 "Paint Drawing Program" by David Cheng (11/18/2018)

#include <Windows.h>
#include <string>
#include <vector>
#include "Shape.h"
#include "resource.h"
using namespace std;

// ==================== Globals ====================

const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF ORANGE = RGB(255, 100, 0);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF PURPLE = RGB(150, 0, 255);

HWND ghMainWnd = 0;
HINSTANCE ghAppInst = 0;
HMENU ghMenu = 0;
vector<Shape*> gShapes;
Shape* gShape = 0;

bool gMouseDown = false;

int gCurrPrimSel = ID_PRIMITIVE_LINE;
int gCurrPenColSel = ID_PENCOLOR_BLACK;
int gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
int gCurrPenStyleSel = ID_PENSTYLE_SOLID;
int gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;

LOGPEN gLogPen;
LOGBRUSH gLogBrush;

// =================================================

// Step 1: Define and implement the window procedure.
LRESULT CALLBACK
WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Objects for painting.
	HDC hdc = 0;
	PAINTSTRUCT ps;

	POINT p0;
	POINT p1;

	switch (msg)
	{
	case WM_CREATE:
		CheckMenuItem(ghMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(ghMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);
		return 0;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			// =================================================
			// File Menu
			// =================================================
		case ID_FILE_UNDO:
			gShapes.pop_back();
			delete gShape;
			gShape = 0;
			InvalidateRect(hWnd, 0, true);
			return 0;
		case ID_FILE_EXIT:
			DestroyWindow(ghMainWnd);
			return 0;
		// =================================================
		// Primitive Types (Shape Types)
		// =================================================
		case ID_PRIMITIVE_LINE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_LINE;
			return 0;
		case ID_PRIMITIVE_RECTANGLE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_RECTANGLE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_RECTANGLE;
			return 0;
		case ID_PRIMITIVE_ELLIPSE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_ELLIPSE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_ELLIPSE;
			return 0;
		case ID_PRIMITIVE_CUBE:
			CheckMenuItem(ghMenu, ID_PRIMITIVE_CUBE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPrimSel, MF_UNCHECKED);
			gCurrPrimSel = ID_PRIMITIVE_CUBE;
			return 0;
		// =================================================
		// Pen Colors
		// =================================================
		case ID_PENCOLOR_BLACK:
			CheckMenuItem(ghMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_BLACK;
			gLogPen.lopnColor = BLACK;
			return 0;
		case ID_PENCOLOR_WHITE:
			CheckMenuItem(ghMenu, ID_PENCOLOR_WHITE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_WHITE;
			gLogPen.lopnColor = WHITE;
			return 0;
		case ID_PENCOLOR_RED:
			CheckMenuItem(ghMenu, ID_PENCOLOR_RED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_RED;
			gLogPen.lopnColor = RED;
			return 0;
		case ID_PENCOLOR_ORANGE:
			CheckMenuItem(ghMenu, ID_PENCOLOR_ORANGE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_ORANGE;
			gLogPen.lopnColor = ORANGE;
			return 0;
		case ID_PENCOLOR_YELLOW:
			CheckMenuItem(ghMenu, ID_PENCOLOR_YELLOW, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_YELLOW;
			gLogPen.lopnColor = YELLOW;
			return 0;
		case ID_PENCOLOR_GREEN:
			CheckMenuItem(ghMenu, ID_PENCOLOR_GREEN, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_GREEN;
			gLogPen.lopnColor = GREEN;
			return 0;
		case ID_PENCOLOR_BLUE:
			CheckMenuItem(ghMenu, ID_PENCOLOR_BLUE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_BLUE;
			gLogPen.lopnColor = BLUE;
			return 0;
		case ID_PENCOLOR_PURPLE:
			CheckMenuItem(ghMenu, ID_PENCOLOR_PURPLE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenColSel, MF_UNCHECKED);
			gCurrPenColSel = ID_PENCOLOR_PURPLE;
			gLogPen.lopnColor = PURPLE;
			return 0;
		// =================================================
		// Brush Colors
		// =================================================
		case ID_BRUSHCOLOR_BLACK:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
			gLogBrush.lbColor = BLACK;
			return 0;
		case ID_BRUSHCOLOR_WHITE:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_WHITE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_WHITE;
			gLogBrush.lbColor = WHITE;
			return 0;
		case ID_BRUSHCOLOR_RED:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_RED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_RED;
			gLogBrush.lbColor = RED;
			return 0;
		case ID_BRUSHCOLOR_ORANGE:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_ORANGE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_ORANGE;
			gLogBrush.lbColor = ORANGE;
			return 0;
		case ID_BRUSHCOLOR_YELLOW:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_YELLOW, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_YELLOW;
			gLogBrush.lbColor = YELLOW;
			return 0;
		case ID_BRUSHCOLOR_GREEN:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_GREEN, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_GREEN;
			gLogBrush.lbColor = GREEN;
			return 0;
		case ID_BRUSHCOLOR_BLUE:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_BLUE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_BLUE;
			gLogBrush.lbColor = BLUE;
			return 0;
		case ID_BRUSHCOLOR_PURPLE:
			CheckMenuItem(ghMenu, ID_BRUSHCOLOR_PURPLE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushColSel, MF_UNCHECKED);
			gCurrBrushColSel = ID_BRUSHCOLOR_PURPLE;
			gLogBrush.lbColor = PURPLE;
			return 0;
		// =================================================
		// Pen Styles
		// =================================================
		case ID_PENSTYLE_SOLID:
			CheckMenuItem(ghMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_SOLID;
			gLogPen.lopnStyle = PS_SOLID;
			gLogPen.lopnWidth.x = 1;
			return 0;
		case ID_PENSTYLE_DOTTED:
			CheckMenuItem(ghMenu, ID_PENSTYLE_DOTTED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_DOTTED;
			gLogPen.lopnStyle = PS_DOT;
			gLogPen.lopnWidth.x = 1;
			return 0;
		case ID_PENSTYLE_DASHED:
			CheckMenuItem(ghMenu, ID_PENSTYLE_DASHED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_DASHED;
			gLogPen.lopnStyle = PS_DASH;
			gLogPen.lopnWidth.x = 1;
			return 0;
		case ID_PENSTYLE_DOTDASH:
			CheckMenuItem(ghMenu, ID_PENSTYLE_DOTDASH, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_DOTDASH;
			gLogPen.lopnStyle = PS_DASHDOT;
			gLogPen.lopnWidth.x = 1;
			return 0;
		case ID_PENSTYLE_DOUBLED:
			CheckMenuItem(ghMenu, ID_PENSTYLE_DOUBLED, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrPenStyleSel, MF_UNCHECKED);
			gCurrPenStyleSel = ID_PENSTYLE_DOUBLED;
			gLogPen.lopnStyle = PS_INSIDEFRAME;
			gLogPen.lopnWidth.x = 5;
			return 0;
		// =================================================
		// Brush Styles
		// =================================================
		case ID_BRUSHSTYLE_SOLID:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;
			gLogBrush.lbStyle = BS_SOLID;
			return 0;
		case ID_BRUSHSTYLE_NULL:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_NULL, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_NULL;
			gLogBrush.lbStyle = BS_NULL;
			return 0;
		case ID_BRUSHSTYLE_DIAGONAL:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_DIAGONAL, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_DIAGONAL;
			gLogBrush.lbStyle = BS_HATCHED;
			gLogBrush.lbHatch = HS_BDIAGONAL;
			return 0;
		case ID_BRUSHSTYLE_CROSS:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_CROSS, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_CROSS;
			gLogBrush.lbStyle = BS_HATCHED;
			gLogBrush.lbHatch = HS_CROSS;
			return 0;
		case ID_BRUSHSTYLE_WAFFLE:
			CheckMenuItem(ghMenu, ID_BRUSHSTYLE_WAFFLE, MF_CHECKED);
			CheckMenuItem(ghMenu, gCurrBrushStyleSel, MF_UNCHECKED);
			gCurrBrushStyleSel = ID_BRUSHSTYLE_WAFFLE;
			gLogBrush.lbStyle = BS_HATCHED;
			gLogBrush.lbHatch = HS_DIAGCROSS;
			return 0;
		}

	case WM_LBUTTONDOWN:
		// Capture the mouse even after the mouse cursor moves off the client area.
		SetCapture(hWnd);
		gMouseDown = true;

		// Point that was clicked is stored in the lParam.
		p0.x = LOWORD(lParam);
		p0.y = HIWORD(lParam);

		// We don't know the end point yet, so set to zero.
		p1.x = 0;
		p1.y = 0;

		// Create the shape based on what shape the user has selected in the menu.
		switch (gCurrPrimSel)
		{
		case ID_PRIMITIVE_LINE:
			gShape = new LineShape(p0, p1, gLogPen, gLogBrush);
			break;
		case ID_PRIMITIVE_RECTANGLE:
			gShape = new RectShape(p0, p1, gLogPen, gLogBrush);
			break;
		case ID_PRIMITIVE_ELLIPSE:
			gShape = new EllipseShape(p0, p1, gLogPen, gLogBrush);
			break;
		case ID_PRIMITIVE_CUBE:
			gShape = new CubeShape(p0, p1, gLogPen, gLogBrush);
			break;
		};

		return 0;

	case WM_MOUSEMOVE:
		if (gMouseDown)
		{
			// Current mouse position is stored in the lParam.
			p1.x = LOWORD(lParam);
			p1.y = HIWORD(lParam);

			// Update the end point of the current temporary shape based on the mouse position.
			gShape->setEndPt(p1);

			// Repaint the window so the temporary shape is redrawn interactively as the mouse moves.
			InvalidateRect(hWnd, 0, true);
		}

		return 0;

	case WM_LBUTTONUP:
		// Release the captured mouse when the left mouse button is lifted.
		ReleaseCapture();
		gMouseDown = false;

		// Current mouse position is stored in the lParam.
		p1.x = LOWORD(lParam);
		p1.y = HIWORD(lParam);

		// Update the end point of the current temporary shape based on the mouse position.
		gShape->setEndPt(p1);

		// The user lifted the left mouse button, so the shape becomes permanent, so add it to the shape container.
		gShapes.push_back(gShape);

		// Repaint the window so the new permanent shape will be displayed.
		InvalidateRect(hWnd, 0, true);

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Only draw temporary shape if the mouse is down.
		if (gMouseDown)
			gShape->draw(hdc);

		// Draw all the permanent shapes.
		for (size_t i = 0; i < gShapes.size(); ++i)
			gShapes[i]->draw(hdc);

		EndPaint(hWnd, &ps);

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(ghMainWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	// Forward any other messages we didn't handle to the default window procedure.
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

// WinMain: Entry point for a Windows application.
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	// Save handle to application instance.
	ghAppInst = hInstance;

	// Step 2: Fill out a WNDCLASS instance.
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghAppInst;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MyWndClassName";

	// Step 3: Register the WNDCLASS instance with Windows.
	RegisterClass(&wc);

	// Step 4: Create the window, and save handle in global window handle variable ghMainWnd.
	ghMenu = LoadMenu(ghAppInst, MAKEINTRESOURCE(IDR_MENU1));
	ghMainWnd = ::CreateWindow(L"MyWndClassName", L"My Paint Program", WS_OVERLAPPEDWINDOW, 200, 200, 640, 480, 0, ghMenu, ghAppInst, 0);

	if (ghMainWnd == 0)
	{
		::MessageBox(0, L"CreateWindow - Failed", 0, 0);
		return false;
	}

	// Step 5: Show and update the window.
	ShowWindow(ghMainWnd, showCmd);
	UpdateWindow(ghMainWnd);

	// Step 6: Enter the message loop and don't quit until a WM_QUIT message is received.
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	for (size_t i = 0; i < gShapes.size(); ++i)
		delete gShapes[i];

	// Return exit code back to the operating system.
	return (int)msg.wParam;
}