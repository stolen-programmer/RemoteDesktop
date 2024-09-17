#ifndef CAPTUREWORKER_H
#define CAPTUREWORKER_H

#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QThread>

class CaptureWorker : public QThread {
    Q_OBJECT
public:
    explicit CaptureWorker(QObject* parent = nullptr);

    void run() override;

signals:

    void capture_screen(uint32_t screen, QPixmap data);

private:
    uint32_t _fps = 0;
};

#endif // CAPTUREWORKER_H
