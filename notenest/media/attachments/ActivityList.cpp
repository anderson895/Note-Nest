#include <windows.h>

#define ID_BTN_LAND     101
#define ID_BTN_MENU     102
#define ID_BTN_CHOOSE   103
#define ID_BTN_EXIT     104

#define ID_BTN_ACT1     201
#define ID_BTN_ACT2     202
#define ID_BTN_ACT3     203
#define ID_BTN_ACT4     204
#define ID_BTN_ACT5     205
#define ID_BTN_QUIZ     206
#define ID_BTN_BACK     207

HINSTANCE hInst;
HFONT hFont;

LPCWSTR szMainClass = L"MainWin";
LPCWSTR szActivityClass = L"ActivityWin";
LPCWSTR szLandClass = L"LandWin";
LPCWSTR szMenuClass = L"MenuWin";
LPCWSTR szContentWinClass = L"ActivityContentWin";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ActivityWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK LandWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MenuWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ContentWndProc(HWND, UINT, WPARAM, LPARAM);

void CreateActivityWindow();
void CreateLandWindow();
void CreateMainWindow();
void CreateMenuWindow();
void CreateActivityContentWindow(int activityId);

bool openLandAfterClose = false;
bool openActivityAfterClose = false;
bool openMenuAfterClose = false;

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    WNDCLASSW wc = {};
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = szMainClass;
    RegisterClassW(&wc);

    wc.lpfnWndProc = ActivityWndProc;
    wc.lpszClassName = szActivityClass;
    RegisterClassW(&wc);

    wc.lpfnWndProc = LandWndProc;
    wc.lpszClassName = szLandClass;
    RegisterClassW(&wc);

    wc.lpfnWndProc = MenuWndProc;
    wc.lpszClassName = szMenuClass;
    RegisterClassW(&wc);

    wc.lpfnWndProc = ContentWndProc;
    wc.lpszClassName = szContentWinClass;
    RegisterClassW(&wc);

    CreateMainWindow();

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

void CreateMainWindow() {
    HWND hWnd = CreateWindowW(
        szMainClass, L"Main Menu",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        nullptr, nullptr, hInst, nullptr
    );

    if (hWnd) {
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }
}

void CreateMenuWindow() {
    HWND hWnd = CreateWindowW(
        szMenuClass, L"Menu Page",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 720,
        nullptr, nullptr, hInst, nullptr
    );

    if (hWnd) {
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        hFont = CreateFontW(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
            VARIABLE_PITCH, L"Segoe UI");

        CreateWindowW(L"BUTTON", L"> Land Page", WS_CHILD | WS_VISIBLE,
            50, 50, 280, 35, hWnd, (HMENU)ID_BTN_LAND, hInst, NULL);
        CreateWindowW(L"BUTTON", L"> Menu Page", WS_CHILD | WS_VISIBLE,
            50, 95, 280, 35, hWnd, (HMENU)ID_BTN_MENU, hInst, NULL);
        CreateWindowW(L"BUTTON", L"> Choose from the list", WS_CHILD | WS_VISIBLE,
            50, 140, 280, 35, hWnd, (HMENU)ID_BTN_CHOOSE, hInst, NULL);
        CreateWindowW(L"BUTTON", L"> Exit Page", WS_CHILD | WS_VISIBLE,
            50, 185, 280, 35, hWnd, (HMENU)ID_BTN_EXIT, hInst, NULL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BTN_LAND:
            openLandAfterClose = true;
            DestroyWindow(hWnd);
            break;
        case ID_BTN_MENU:
            openMenuAfterClose = true;
            DestroyWindow(hWnd);
            break;
        case ID_BTN_CHOOSE:
            openActivityAfterClose = true;
            DestroyWindow(hWnd);
            break;
        case ID_BTN_EXIT:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBrush = CreateSolidBrush(RGB(178, 73, 50));
        FillRect(hdc, &ps.rcPaint, hBrush);
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        DeleteObject(hFont);
        if (openLandAfterClose) {
            CreateLandWindow();
            openLandAfterClose = false;
        }
        else if (openActivityAfterClose) {
            CreateActivityWindow();
            openActivityAfterClose = false;
        }
        else if (openMenuAfterClose) {
            CreateMenuWindow();
            openMenuAfterClose = false;
        }
        else {
            PostQuitMessage(0);
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateActivityWindow() {
    HWND hActivity = CreateWindowW(
        szActivityClass, L"List of Activities",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        nullptr, nullptr, hInst, nullptr
    );
    if (hActivity) {
        ShowWindow(hActivity, SW_SHOW);
        UpdateWindow(hActivity);
    }
}

void CreateActivityContentWindow(int activityId) {
    HWND hContent = CreateWindowW(
        szContentWinClass, L"Activity Content",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInst, (LPVOID)(INT_PTR)activityId
    );

    if (hContent) {
        ShowWindow(hContent, SW_SHOW);
        UpdateWindow(hContent);
    }
}

LRESULT CALLBACK ActivityWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        CreateWindowW(L"BUTTON", L"< Back", WS_CHILD | WS_VISIBLE,
            10, 10, 100, 30, hWnd, (HMENU)ID_BTN_BACK, hInst, NULL);
        CreateWindowW(L"BUTTON", L"1. Act 1", WS_CHILD | WS_VISIBLE,
            50, 60, 280, 30, hWnd, (HMENU)ID_BTN_ACT1, hInst, NULL);
        CreateWindowW(L"BUTTON", L"2. Act 2", WS_CHILD | WS_VISIBLE,
            50, 100, 280, 30, hWnd, (HMENU)ID_BTN_ACT2, hInst, NULL);
        CreateWindowW(L"BUTTON", L"3. Act 3", WS_CHILD | WS_VISIBLE,
            50, 140, 280, 30, hWnd, (HMENU)ID_BTN_ACT3, hInst, NULL);
        CreateWindowW(L"BUTTON", L"4. Act 4", WS_CHILD | WS_VISIBLE,
            50, 180, 280, 30, hWnd, (HMENU)ID_BTN_ACT4, hInst, NULL);
        CreateWindowW(L"BUTTON", L"5. Act 5", WS_CHILD | WS_VISIBLE,
            50, 220, 280, 30, hWnd, (HMENU)ID_BTN_ACT5, hInst, NULL);
        CreateWindowW(L"BUTTON", L"Q. Quiz", WS_CHILD | WS_VISIBLE,
            50, 260, 280, 30, hWnd, (HMENU)ID_BTN_QUIZ, hInst, NULL);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BTN_BACK) {
            CreateMainWindow();
            DestroyWindow(hWnd);
        }
        else {
            CreateActivityContentWindow(LOWORD(wParam));
            DestroyWindow(hWnd);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBrush = CreateSolidBrush(RGB(178, 73, 50));
        FillRect(hdc, &ps.rcPaint, hBrush);
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateLandWindow() {
    HWND hLand = CreateWindowW(
        szLandClass, L"Land Page",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 720,
        nullptr, nullptr, hInst, nullptr
    );
    if (hLand) {
        ShowWindow(hLand, SW_SHOW);
        UpdateWindow(hLand);
    }
}

HBITMAP hCoverImage = NULL;

LRESULT CALLBACK LandWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        CreateWindowW(L"BUTTON", L"< Back", WS_CHILD | WS_VISIBLE,
            10, 10, 100, 30, hWnd, (HMENU)ID_BTN_BACK, hInst, NULL);

        hCoverImage = (HBITMAP)LoadImageW(NULL, L"landing.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BTN_BACK) {
            CreateMainWindow();
            DestroyWindow(hWnd);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBrush = CreateSolidBrush(RGB(128, 200, 255));
        FillRect(hdc, &ps.rcPaint, hBrush);

        if (hCoverImage) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hCoverImage);
            BITMAP bmp;
            GetObject(hCoverImage, sizeof(BITMAP), &bmp);
            BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);
        }

        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        if (hCoverImage) {
            DeleteObject(hCoverImage);
            hCoverImage = NULL;
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK MenuWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        CreateWindowW(L"BUTTON", L"< Back", WS_CHILD | WS_VISIBLE,
            10, 10, 100, 30, hWnd, (HMENU)ID_BTN_BACK, hInst, NULL);
        hCoverImage = (HBITMAP)LoadImageW(NULL, L"menu.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BTN_BACK) {
            CreateMainWindow();
            DestroyWindow(hWnd);
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBrush = CreateSolidBrush(RGB(128, 200, 255));
        FillRect(hdc, &ps.rcPaint, hBrush);

        if (hCoverImage) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            SelectObject(hdcMem, hCoverImage);
            BITMAP bmp;
            GetObject(hCoverImage, sizeof(BITMAP), &bmp);
            BitBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hdcMem, 0, 0, SRCCOPY);
            DeleteDC(hdcMem);
        }

        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        if (hCoverImage) {
            DeleteObject(hCoverImage);
            hCoverImage = NULL;
        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



LRESULT CALLBACK ContentWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hScrollChild, hText, hImage, hBackBtn;
    static HBITMAP hBitmap = NULL;
    static int activityId;
    static int yOffset = 0;

    switch (message) {
    case WM_CREATE:
    {
        activityId = (int)(INT_PTR)((LPCREATESTRUCT)lParam)->lpCreateParams;

        LPCWSTR content = L"", imagePath = L"";

        switch (activityId) {
        case ID_BTN_ACT1:
            content = L"Activity 1: 1. Draw initials First, middle, last using asterisks 2. Draw ur favorite cartoon character";
            imagePath = L"act1.bmp";
            break;
        case ID_BTN_ACT2:
            content =
                L"Activity 2: C-Code and C++\n\n"

                L"// C - CODE\n"
                L"#include <stdio.h>\n"
                L"#include <math.h>\n"
                L"int main() {\n"
                L"    float base_length = 10, slant_height = 15, height = 12;\n"
                L"    float volume, surface_area;\n"
                L"    volume = (1.0 / 3) * pow(base_length, 2) * height;\n"
                L"    surface_area = pow(base_length, 2) + 2 * base_length * slant_height;\n"
                L"    printf(\"Base Length = %.2f\\n\", base_length);\n"
                L"    printf(\"Height = %.2f\\n\", height);\n"
                L"    printf(\"Slant Height = %.2f\\n\", slant_height);\n"
                L"    printf(\"Volume = %.2f cubic units\\n\", volume);\n"
                L"    printf(\"Surface Area = %.2f square units\\n\", surface_area);\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// C++\n"
                L"#include <iostream>\n"
                L"#include <cmath>\n"
                L"using namespace std;\n"
                L"int main() {\n"
                L"    float base_length = 10, slant_height = 15, height = 12;\n"
                L"    float volume, surface_area;\n"
                L"    volume = (1.0 / 3) * pow(base_length, 2) * height;\n"
                L"    surface_area = pow(base_length, 2) + 2 * base_length * slant_height;\n"
                L"    cout << \"Base Length = \" << base_length << endl;\n"
                L"    cout << \"Height = \" << height << endl;\n"
                L"    cout << \"Slant Height = \" << slant_height << endl;\n"
                L"    cout << \"Volume = \" << volume << \" cubic units\" << endl;\n"
                L"    cout << \"Surface Area = \" << surface_area << \" square units\" << endl;\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// 2) C - CODE (Law of Cosines)\n"
                L"#include <stdio.h>\n"
                L"#include <math.h>\n"
                L"int main() {\n"
                L"    float a = 8, b = 10, angle = 60;\n"
                L"    float c;\n"
                L"    c = sqrt(pow(a, 2) + pow(b, 2) - 2 * a * b * cos(angle * M_PI / 180));\n"
                L"    printf(\"Side A = %.2f\\n\", a);\n"
                L"    printf(\"Side B = %.2f\\n\", b);\n"
                L"    printf(\"Angle (degrees) = %.2f\\n\", angle);\n"
                L"    printf(\"Third Side (C) = %.2f\\n\", c);\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// C++ (Law of Cosines)\n"
                L"#include <iostream>\n"
                L"#include <cmath>\n"
                L"using namespace std;\n"
                L"int main() {\n"
                L"    float a = 8, b = 10, angle = 60;\n"
                L"    float c;\n"
                L"    c = sqrt(pow(a, 2) + pow(b, 2) - 2 * a * b * cos(angle * M_PI / 180));\n"
                L"    cout << \"Side A = \" << a << endl;\n"
                L"    cout << \"Side B = \" << b << endl;\n"
                L"    cout << \"Angle (degrees) = \" << angle << endl;\n"
                L"    cout << \"Third Side (C) = \" << c << endl;\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// 3) C++ (Free fall)\n"
                L"#include <iostream>\n"
                L"#include <cmath>\n"
                L"using namespace std;\n"
                L"int main() {\n"
                L"    float g = 9.81, h = 50, time = 3;\n"
                L"    float distance, velocity;\n"
                L"    distance = 0.5 * g * pow(time, 2);\n"
                L"    velocity = sqrt(2 * g * h);\n"
                L"    cout << \"Height = \" << h << \" meters\" << endl;\n"
                L"    cout << \"Time Elapsed = \" << time << \" seconds\" << endl;\n"
                L"    cout << \"Distance Fallen = \" << distance << \" meters\" << endl;\n"
                L"    cout << \"Final Velocity = \" << velocity << \" m/s\" << endl;\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// 4) C - CODE (Temperature and Speed Conversion)\n"
                L"#include <stdio.h>\n"
                L"int main() {\n"
                L"    float celsius = 100, fahrenheit, kelvin;\n"
                L"    float meters_per_second = 10, km_per_hour, miles_per_hour;\n"
                L"    fahrenheit = (celsius * 9 / 5) + 32;\n"
                L"    kelvin = celsius + 273.15;\n"
                L"    km_per_hour = meters_per_second * 3.6;\n"
                L"    miles_per_hour = meters_per_second * 2.237;\n"
                L"    printf(\"Temperature in Celsius: %.2f\\n\", celsius);\n"
                L"    printf(\"Converted to Fahrenheit: %.2f\\n\", fahrenheit);\n"
                L"    printf(\"Converted to Kelvin: %.2f\\n\", kelvin);\n"
                L"    printf(\"Speed in m/s: %.2f\\n\", meters_per_second);\n"
                L"    printf(\"Converted to km/h: %.2f\\n\", km_per_hour);\n"
                L"    printf(\"Converted to mph: %.2f\\n\", miles_per_hour);\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// C++ (Temperature and Speed Conversion)\n"
                L"#include <iostream>\n"
                L"using namespace std;\n"
                L"int main() {\n"
                L"    float celsius = 100, fahrenheit, kelvin;\n"
                L"    float meters_per_second = 10, km_per_hour, miles_per_hour;\n"
                L"    fahrenheit = (celsius * 9 / 5) + 32;\n"
                L"    kelvin = celsius + 273.15;\n"
                L"    km_per_hour = meters_per_second * 3.6;\n"
                L"    miles_per_hour = meters_per_second * 2.237;\n"
                L"    cout << \"Temperature in Celsius: \" << celsius << endl;\n"
                L"    cout << \"Converted to Fahrenheit: \" << fahrenheit << endl;\n"
                L"    cout << \"Converted to Kelvin: \" << kelvin << endl;\n"
                L"    cout << \"Speed in m/s: \" << meters_per_second << endl;\n"
                L"    cout << \"Converted to km/h: \" << km_per_hour << endl;\n"
                L"    cout << \"Converted to mph: \" << miles_per_hour << endl;\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// 5) C - CODE (Loan Payment)\n"
                L"#include <stdio.h>\n"
                L"#include <math.h>\n"
                L"int main() {\n"
                L"    float principal = 500000, annual_rate = 5, years = 10;\n"
                L"    float monthly_rate, fixed_payment, remaining_balance, declining_payment;\n"
                L"    int months = years * 12;\n"
                L"    monthly_rate = annual_rate / 100 / 12;\n"
                L"    fixed_payment = (principal * monthly_rate) / (1 - pow((1 + monthly_rate), -months));\n"
                L"    remaining_balance = principal;\n"
                L"    printf(\"Yearly Payments (Declining Balance Method):\\n\");\n"
                L"    for (int year = 1; year <= years; year++) {\n"
                L"        declining_payment = remaining_balance * monthly_rate * 12;\n"
                L"        remaining_balance -= (declining_payment - (remaining_balance * annual_rate / 100));\n"
                L"        printf(\"Year %d: Payment = %.2f, Remaining Balance = %.2f\\n\", year, declining_payment, remaining_balance);\n"
                L"    }\n"
                L"    printf(\"\\nFixed Monthly Payment = %.2f\\n\", fixed_payment);\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// C++ (Loan Payment)\n"
                L"#include <iostream>\n"
                L"#include <cmath>\n"
                L"using namespace std;\n"
                L"int main() {\n"
                L"    float principal = 500000, annual_rate = 5, years = 10;\n"
                L"    float monthly_rate, fixed_payment, remaining_balance, declining_payment;\n"
                L"    int months = years * 12;\n"
                L"    monthly_rate = annual_rate / 100 / 12;\n"
                L"    fixed_payment = (principal * monthly_rate) / (1 - pow((1 + monthly_rate), -months));\n"
                L"    remaining_balance = principal;\n"
                L"    cout << \"Yearly Payments (Declining Balance Method):\\n\";\n"
                L"    for (int year = 1; year <= years; year++) {\n"
                L"        declining_payment = remaining_balance * monthly_rate * 12;\n"
                L"        remaining_balance -= (declining_payment - (remaining_balance * annual_rate / 100));\n"
                L"        cout << \"Year \" << year << \": Payment = \" << declining_payment << \", Remaining Balance = \" << remaining_balance << endl;\n"
                L"    }\n"
                L"    cout << \"\\nFixed Monthly Payment = \" << fixed_payment << endl;\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// 6) C - CODE (Quadratic Formula)\n"
                L"#include <stdio.h>\n"
                L"#include <math.h>\n"
                L"int main() {\n"
                L"    float a = 1, b = 5, c = 6;\n"
                L"    float root1, root2, discriminant;\n"
                L"    discriminant = b * b - 4 * a * c;\n"
                L"    root1 = (-b + sqrt(discriminant)) / (2 * a);\n"
                L"    root2 = (-b - sqrt(discriminant)) / (2 * a);\n"
                L"    printf(\"Quadratic Equation: %.2fx² + %.2fx + %.2f = 0\\n\", a, b, c);\n"
                L"    printf(\"Root 1 = %.2f\\n\", root1);\n"
                L"    printf(\"Root 2 = %.2f\\n\", root2);\n"
                L"    return 0;\n"
                L"}\n\n"

                L"// C++ (Quadratic Formula)\n"
                L"#include <iostream>\n"
                L"#include <cmath>\n"
                L"using namespace std;\n"
                L"int main() {\n"
                L"    float a = 1, b = 5, c = 6;\n"
                L"    float root1, root2, discriminant;\n"
                L"    discriminant = b * b - 4 * a * c;\n"
                L"    root1 = (-b + sqrt(discriminant)) / (2 * a);\n"
                L"    root2 = (-b - sqrt(discriminant)) / (2 * a);\n"
                L"    cout << \"Quadratic Equation: \" << a << \"x² + \" << b << \"x + \" << c << \" = 0\" << endl;\n"
                L"    cout << \"Root 1 = \" << root1 << endl;\n"
                L"    cout << \"Root 2 = \" << root2 << endl;\n"
                L"    return 0;\n"
                L"}\n";

            break;
        case ID_BTN_ACT3:
            content = L"Activity 3: C - Code and C++\n\n"
                L"C - CODE\n\n"
                L"// Online C compiler to run C program online\n"
                L"#include <stdio.h>\n"
                L"#include <math.h>\n\n"
                L"int main() {\n"
                L"    float n1 = 4, n2 = 9, n3 = 5, n4 = 7, result;\n\n"
                L"    // a) Check if 2nd value is higher than the 3rd value\n"
                L"    if (n2 > n3) {\n"
                L"        result = (n1 + n3) / 2;\n"
                L"        printf(\"Average of 1st and 3rd value: %.2f\\n\", result);\n"
                L"    }\n\n"
                L"    // b) Check if 4th value is not equal to 8\n"
                L"    if (n4 != 8) {\n"
                L"        result = 3 * (n4 - n2);\n"
                L"        printf(\"Thrice the difference of 2nd from 4th value: %.2f\\n\", result);\n"
                L"    }\n\n"
                L"    // c) Check if the sum of 2nd, 3rd, and 4th value is <= 10\n"
                L"    if ((n2 + n3 + n4) <= 10) {\n"
                L"        result = cbrt(n1 * n2 * n3 * n4);\n"
                L"        printf(\"Cube root of the product of all inputted values: %.2f\\n\", result);\n"
                L"    }\n"
                L"    return 0;\n"
                L"}\n\n\n"

                L"C++ CODE\n\n"
                L"// Online C++ compiler to run C++ program\n"
                L"#include <iostream>\n"
                L"#include <cmath>\n"
                L"using namespace std;\n\n"
                L"int main() {\n"
                L"    float n1 = 4, n2 = 9, n3 = 5, n4 = 7, result;\n\n"
                L"    // a) Check if 2nd value is higher than the 3rd value\n"
                L"    if (n2 > n3) {\n"
                L"        result = (n1 + n3) / 2;\n"
                L"        cout << \"Average of 1st and 3rd value: \" << result << endl;\n"
                L"    }\n\n"
                L"    // b) Check if 4th value is not equal to 8\n"
                L"    if (n4 != 8) {\n"
                L"        result = 3 * (n4 - n2);\n"
                L"        cout << \"Thrice the difference of 2nd from 4th value: \" << result << endl;\n"
                L"    }\n\n"
                L"    // c) Check if the sum of 2nd, 3rd, and 4th value is <= 10\n"
                L"    if ((n2 + n3 + n4) <= 10) {\n"
                L"        result = cbrt(n1 * n2 * n3 * n4);\n"
                L"        cout << \"Cube root of the product of all inputted values: \" << result << endl;\n"
                L"    }\n"
                L"    return 0;\n"
                L"}\n\n\n"

                L"(2) C - CODE (Calendar)\n\n"
                L"// Online C compiler to run C program online\n"
                L"#include <stdio.h>\n\n"
                L"void printCalendar(int month) {\n"
                L"    char* monthName[] = { \"\", \"September\", \"October\", \"November\", \"December\" };\n"
                L"    int daysInMonth[] = { 0, 30, 31, 30, 31 };\n\n"
                L"    if (month < 9 || month > 12) {\n"
                L"        printf(\"Invalid Input! Please enter a number between 9 to 12.\\n\");\n"
                L"        return;\n"
                L"    }\n\n"
                L"    printf(\"\\n|----------------------------------|\\n\");\n"
                L"    printf(\"|      %s 2025      |\\n\", monthName[month - 8]);\n"
                L"    printf(\"| S M T W T F S |\\n\");\n"
                L"    for (int i = 1; i <= daysInMonth[month - 8]; i++) {\n"
                L"        printf(\"%2d \", i);\n"
                L"        if (i % 7 == 0) printf(\"\\n\");\n"
                L"    }\n"
                L"    printf(\"\\n|----------------------------------|\\n\");\n"
                L"}\n\n"
                L"int main() {\n"
                L"    int month;\n"
                L"    printf(\"Enter a number (9-12) corresponding to a month: \");\n"
                L"    scanf(\"%d\", &month);\n"
                L"    printCalendar(month);\n"
                L"    return 0;\n"
                L"}\n\n\n"

                L"C++ CODE (Calendar)\n\n"
                L"#include <iostream>\n"
                L"using namespace std;\n\n"
                L"void printCalendar(int month) {\n"
                L"    string monthName[] = { \"\", \"September\", \"October\", \"November\", \"December\" };\n"
                L"    int daysInMonth[] = { 0, 30, 31, 30, 31 };\n\n"
                L"    if (month < 9 || month > 12) {\n"
                L"        cout << \"Invalid Input! Please enter a number between 9 to 12.\" << endl;\n"
                L"        return;\n"
                L"    }\n\n"
                L"    cout << \"\\n|----------------------------------|\\n\";\n"
                L"    cout << \"|      \" << monthName[month - 8] << \" 2025      |\\n\";\n"
                L"    cout << \"| S M T W T F S |\\n\";\n"
                L"    for (int i = 1; i <= daysInMonth[month - 8]; i++) {\n"
                L"        cout.width(2);\n"
                L"        cout << i << \" \";\n"
                L"        if (i % 7 == 0) cout << endl;\n"
                L"    }\n"
                L"    cout << \"\\n|----------------------------------|\\n\";\n"
                L"}\n\n"
                L"int main() {\n"
                L"    int month;\n"
                L"    cout << \"Enter a number (9-12) corresponding to a month: \";\n"
                L"    cin >> month;\n"
                L"    printCalendar(month);\n"
                L"    return 0;\n"
                L"}\n\n\n"

                L"(3) C - CODE (Initials)\n\n"
                L"#include <stdio.h>\n\n"
                L"int main() {\n"
                L"    // My Initials (Full name: Alicia Heart D. Feraren)\n"
                L"    printf(\"  *    *   *  ****   *****\\n\");\n"
                L"    printf(\" * *   *   *  *   *  *    \\n\");\n"
                L"    printf(\"*****  *****  *   *  *****\\n\");\n"
                L"    printf(\"*   *  *   *  *   *  *    \\n\");\n"
                L"    printf(\"*   *  *   *  ****   *    \\n\");\n"
                L"    return 0;\n"
                L"}\n\n\n"

                L"C++ CODE (Initials)\n\n"
                L"#include <iostream>\n"
                L"using namespace std;\n\n"
                L"int main() {\n"
                L"    // My Initials (Full name: Alicia Heart D. Feraren)\n"
                L"    cout << \"  *    *   *  ****   *****\" << endl;\n"
                L"    cout << \" * *   *   *  *   *  *    \" << endl;\n"
                L"    cout << \"**********  *   *  *****\" << endl;\n"
                L"    cout << \"*   *  *   *  *   *  *    \" << endl;\n"
                L"    cout << \"*   *  *   *  ****   *    \" << endl;\n"
                L"    return 0;\n"
                L"}\n";


            break;
        case ID_BTN_ACT4:
            content = L"Activity 4: ITERATION\n";
            imagePath = L"act5-7.bmp";


            break;
        case ID_BTN_ACT5:
            content = L"Activity 5: Recap and summary.";
            imagePath = L"act5-1.bmp";
            break;
        case ID_BTN_QUIZ:
            content = L"Quiz Time!";
            imagePath = L"quiz.bmp";
            break;
        default:
            content = L"Unknown activity.";
            break;
        }

        

        hText = CreateWindowW(L"STATIC", content, WS_CHILD | WS_VISIBLE,
            10, 10, 800, 2000, hWnd, NULL, hInst, NULL);


        hImage = CreateWindowW(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP,
            10, 90, 400, 300, hWnd, NULL, hInst, NULL);

        if (wcslen(imagePath) > 0) {
            hBitmap = (HBITMAP)LoadImageW(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            if (hBitmap) {
                SendMessageW(hImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
            }
            else {
                MessageBoxW(hWnd, L"Image failed to load!", imagePath, MB_OK | MB_ICONERROR);
                ShowWindow(hImage, SW_HIDE);
            }
        }

        else {
            // Optional: hide the image control if no path is provided
            ShowWindow(hImage, SW_HIDE);
        }


        hBackBtn = CreateWindowW(L"BUTTON", L"< Back", WS_CHILD | WS_VISIBLE,
            20, 520, 100, 30, hWnd, (HMENU)ID_BTN_BACK, hInst, NULL);


        // Set scroll info
        SCROLLINFO si = { sizeof(si), SIF_RANGE | SIF_PAGE };
        si.nMin = 0;
        si.nMax = 600;
        si.nPage = 500;
        SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
        break;
    }

    case WM_VSCROLL:
    {
        SCROLLINFO si = { sizeof(si), SIF_ALL };
        GetScrollInfo(hWnd, SB_VERT, &si);
        int yNewOffset = yOffset;

        switch (LOWORD(wParam)) {
        case SB_LINEUP: yNewOffset -= 20; break;
        case SB_LINEDOWN: yNewOffset += 20; break;
        case SB_PAGEUP: yNewOffset -= 100; break;
        case SB_PAGEDOWN: yNewOffset += 100; break;
        case SB_THUMBTRACK: yNewOffset = HIWORD(wParam); break;
        }

        yNewOffset = max(0, min(yNewOffset, si.nMax - (int)si.nPage));
        if (yNewOffset != yOffset) {
            int delta = yOffset - yNewOffset;
            yOffset = yNewOffset;

            // Move controls manually
            SetWindowPos(hText, NULL, 10, 10 - yOffset, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            SetWindowPos(hImage, NULL, 10, 90 - yOffset, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

            // Update scrollbar
            si.fMask = SIF_POS;
            si.nPos = yOffset;
            SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    }


    case WM_COMMAND:
        if (LOWORD(wParam) == ID_BTN_BACK) {
            CreateActivityWindow();
            DestroyWindow(hWnd);
        }
        break;

    case WM_DESTROY:
        if (hBitmap) {
            DeleteObject(hBitmap);
            hBitmap = NULL;
        }
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &ps.rcPaint, hBrush);
        DeleteObject(hBrush);
        EndPaint(hWnd, &ps);
    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

