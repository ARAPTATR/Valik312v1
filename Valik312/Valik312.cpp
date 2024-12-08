#include <iostream>
#include <Windows.h>
#include <thread>
#include <fstream>
#include <string>
#include <ctime>

bool checkColor(COLORREF color, COLORREF targetColor, int threshold) {
    int redDiff = GetRValue(color) - GetRValue(targetColor);
    int greenDiff = GetGValue(color) - GetGValue(targetColor);
    int blueDiff = GetBValue(color) - GetBValue(targetColor);
    int colorThreshold = threshold * 255 / 100; 

    return (abs(redDiff) <= colorThreshold) && (abs(greenDiff) <= colorThreshold) && (abs(blueDiff) <= colorThreshold);
    if (color == RGB(210, 143, 176)) {
        return true; // Игнорируем цвет RGB(100, 50, 200)
    }
    if (color == RGB(208, 128, 176)) {
        return true; // Игнорируем цвет RGB(100, 50, 200)
    }
    if (color == RGB(185, 143, 156)) {
        return true; // Игнорируем цвет RGB(100, 50, 200)
    }
}

void drawCircle(HDC hdc, int centerX, int centerY, int radius, COLORREF color, int thickness) {
    HPEN hPen = CreatePen(PS_SOLID, thickness, color);
    SelectObject(hdc, hPen);
    Ellipse(hdc, centerX - radius, centerY - radius, centerX + radius, centerY + radius);
    DeleteObject(hPen);
}

int main() {

    std::ofstream logfile("log.txt", std::ios::app); // Открытие файла логов для добавления

    if (!logfile.is_open()) {
        std::cerr << "Ошибка открытия файла логов!" << std::endl;
        return 1;
    }
        // Получение текущей даты и времени
    time_t now = time(0);
    tm localtm;
    localtime_s(&localtm, &now);
    char timestamp[50];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %X", &localtm);

    // Запись сообщения в лог файл
    try {
        int result = 10 / 1; // Генерация ошибки деления на ноль
    }
    catch (const std::exception& e) {
        // Запись сообщения об ошибке в лог файл
        std::string errorMessage = timestamp;
        errorMessage += " - Ошибка: ";
        errorMessage += e.what();
        errorMessage += "\n";
        logfile << errorMessage;
    }

    // Закрытие файла
    logfile.close();

    HWND hwnd = GetConsoleWindow(); // Получаем дескриптор окна консоли

    if (hwnd != NULL) {
        ShowWindow(hwnd, SW_HIDE); // Скрываем окно по дескриптору
    }

    DWORD dwProcessId = GetCurrentProcessId(); // Получаем ID текущего процесса
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId); // Открываем процесс
    if (hProcess != NULL) {
        SetPriorityClass(hProcess, IDLE_PRIORITY_CLASS); // Устанавливаем приоритет процесса на самый низкий
        CloseHandle(hProcess); // Закрываем дескриптор процесса
    }

    std::cout << "Trigger on\n";
    Beep(500, 36); // Воспроизвести звук частотой 500 и длительностью 36
    Sleep(0); // Подождать немного между звуками
    Beep(1700, 14); // Воспроизвести звук частотой 1700 и длительностью 14

    int centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
    int centerY = GetSystemMetrics(SM_CYSCREEN) / 2;

    int radius = 2;
    COLORREF outlineColor = RGB(128, 0, 0);

    //HWND hwnd = GetDesktopWindow(); я так понимаю
    HDC hdc = GetDC(hwnd);

    int monitorRefreshRate = GetDeviceCaps(GetDC(NULL), VREFRESH);
    int reactionTime = 0; 

        ReleaseDC(NULL, hdc);


    while (true) {
       
        if (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) {
            HDC hdc = GetDC(NULL);

            

            for (int x = centerX - radius; x <= centerX + radius; x++) {
                for (int y = centerY - radius; y <= centerY + radius; y++) {
                    COLORREF color = GetPixel(hdc, x, y);
                    if (checkColor(color, RGB(254, 103, 254), 26) && !checkColor(color, outlineColor, 0)) {
                        keybd_event('K', 0, 0, 0);
                        keybd_event('K', 0, KEYEVENTF_KEYUP, 0);
                        Beep(1920, 36);

                        break;
                    }
                }
            }

            ReleaseDC(NULL, hdc);

        }

        if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
            Beep(920, 36); // Воспроизвести звук частотой 920 и длительностью 36
            Sleep(200); // Подождать немного между звуками
            Beep(320, 14); // Воспроизвести звук частотой 320 и длительностью 14
            break;
            break;
        }

        Sleep(reactionTime);
    }

    return 0;
}
