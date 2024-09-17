#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QNetworkInterface>
#include <QStringListModel>

extern "C" {
#include <libavformat/avformat.h>
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QStringList ifs;

    QSet<QNetworkInterface::InterfaceType> ty = {
        QNetworkInterface::Wifi,
        QNetworkInterface::Ethernet,
        QNetworkInterface::Loopback,
    };
    for (auto& _if : QNetworkInterface::allInterfaces()) {

        if (!ty.contains(_if.type())) {
            continue;
        }
        for (auto& ip : _if.addressEntries()) {
            if (ip.ip().protocol() == QAbstractSocket::NetworkLayerProtocol::IPv4Protocol)
                ifs << ip.ip().toString();
        }
    }
    ui->comboBox->setModel(new QStringListModel(ifs));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);

    // updateLayout(*this, geometry());
}
void MainWindow::updateLayout(QWidget& root, QRect rc)
{
    auto queryAttr = [](const QWidget& w, const char* attr) { return w.property(attr); };
    auto query = [](const QWidget& w) { return w.property("__origin_rect"); };
    auto set = [](QWidget& w, const QRect rc) { w.setProperty("__origin_rect", rc); };

    auto queryGeo = [&query, &root, &set](QWidget& w) {
        if (auto rc = query(root); rc.isNull()) {
            set(root, root.geometry());
        }

        return query(root).toRect();
    };

    auto queryAnchor = [](QWidget& w) {
        return w.property("anchor");
    };

    // auto rc = queryGeo(root);
    // 获取子项5

    for (auto w : root.children()) {
        if (!w->inherits("QWidget")) {
            continue;
        }

        updateLayout(*qobject_cast<QWidget*>(w), root.geometry());
    }
}
