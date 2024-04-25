#include <iostream>
#include <windows.h>
#include <cmath>
#include <vector>
using namespace std;

# define POINTS_NUM 3

struct MyPoint {
    int x;
    int y;
};


void Swap(int &x1, int &y1, int &x2, int &y2);
void Swap(MyPoint& point1,MyPoint& point2);
void drawLine(MyPoint start, MyPoint end, HDC hdc, COLORREF color);
void drawTriangle(MyPoint point1 ,MyPoint point2 ,MyPoint point3, HDC hdc, COLORREF color);
void lineMidpointX(int xs, int ys, int xe, int ye, HDC hdc, COLORREF color);
void lineMidpointY(int xs, int ys, int xe, int ye, HDC hdc, COLORREF color);


vector<MyPoint> points;
LRESULT WINAPI WndProc(HWND hwnd, UINT m, WPARAM wp, LPARAM lp) {
    HDC hdc;
    int x, y;
    switch (m) {
        case WM_LBUTTONDOWN:
            hdc = GetDC(hwnd);
            x = LOWORD(lp);
            y = HIWORD(lp);
            points.push_back(MyPoint{x,y});
            if(points.size() == POINTS_NUM){
                drawTriangle(points[0], points[1], points[2], hdc, RGB(0,0,0));
                points.clear();
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, m, wp, lp);
    }
    return 0;
}


int APIENTRY WinMain(HINSTANCE h, HINSTANCE p, LPSTR cmd, int csh) {
    WNDCLASS wc;
    wc.lpszClassName = "MyClass";
    wc.lpszMenuName = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hbrBackground = (HBRUSH) GetStockObject(LTGRAY_BRUSH);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = h;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "Triangle Drawer", WS_OVERLAPPEDWINDOW, 0, 0, 800, 800, NULL, NULL, h, NULL);
    ShowWindow(hwnd, csh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}


// ************************* Core-Algorithm *************************

void drawTriangle(MyPoint point1 ,MyPoint point2 ,MyPoint point3, HDC hdc, COLORREF color){
    drawLine(point1,point2, hdc, color);
    drawLine(point1,point3, hdc, color);
    drawLine(point2,point3, hdc, color);
}

void drawLine(MyPoint start, MyPoint end, HDC hdc, COLORREF color) {
    int dx = end.x - start.x;
    int dy = end.y - start.y;
    if (abs(dx) > abs(dy)) {
        if(start.x > end.x){
            Swap(start,end);
        }
        lineMidpointX(start.x, start.y, end.x, end.y, hdc, color);
    }
    else {
        if(start.y > end.y){
            Swap(start,end);
        }
        lineMidpointY(start.x, start.y, end.x, end.y, hdc, color);
    }
}

void lineMidpointX(int xs, int ys, int xe, int ye, HDC hdc, COLORREF color) {
    int dx = xe - xs;
    int dy = ye - ys;
    int yChange = 1;

    if (dy < 0)
    {
        yChange = -1;
        dy = -dy;
    }

    int d = (2 * dy) - dx;
    int change1 = 2 * (dy - dx);
    int change2 = 2 * dy;
    int y = ys;

    for (int x = xs; x < xe; x++)
    {
        SetPixel(hdc, x, y, color);
        if (d > 0)
        {
            d += change1;
            y += yChange;
        }
        else {
            d += change2;
        }
    }
}


void lineMidpointY(int xs, int ys, int xe, int ye, HDC hdc, COLORREF color) {
    int dx = xe - xs;
    int dy = ye - ys;
    int xChange = 1;

    if (dx < 0)
    {
        xChange = -1;
        dx = -dx;
    }

    int d = (2 * dx) - dy;
    int change1 = 2 * (dx - dy);
    int change2 = 2 * dx;
    int x = xs;

    for (int y = ys; y < ye; y++)
    {
        SetPixel(hdc, x, y, color);
        if (d > 0)
        {
            d += change1;
            x += xChange;
        }
        else {
            d += change2;
        }
    }
}

// ************************* Helper-Functions *************************

void Swap(int &x1, int &y1, int &x2, int &y2) {
    x1 = x1 ^ x2;
    x2 = x1 ^ x2;
    x1 = x1 ^ x2;

    y1 = y1 ^ y2;
    y2 = y1 ^ y2;
    y1 = y1 ^ y2;
}

void Swap(MyPoint& point1, MyPoint& point2){
    Swap(point1.x,point1.y,point2.x,point2.y);
}