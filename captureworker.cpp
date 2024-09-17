#include "captureworker.h"

#include <QApplication>
#include <QPixmap>
#include <QScreen>
#include <chrono>
#include <ranges>

CaptureWorker::CaptureWorker(QObject* parent)
    : QThread { parent }
    , _fps(30)
{
}

void CaptureWorker::run()
{

    // 开始采集

    QImage data;
    while (true) {
        auto start = std::chrono::high_resolution_clock::now();
        std::chrono::duration<long long, std::ratio<1, 1000>> dur;
        // 等一些时间
        do {
            QApplication::processEvents();
            auto end = std::chrono::high_resolution_clock::now();

            dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        } while (dur.count() < 1000 / _fps);

        for (auto const [index, scr] : QGuiApplication::screens() | std::views::enumerate) {
            auto bmp = scr->grabWindow(0);
            emit capture_screen(static_cast<uint32_t>(index), bmp);
        }
    }
}
