#pragma once
#include "Net/Server.h"
#include "Net/Data.h"
#include <QMainWindow>
#include <QImage>
#include <thread>
#include <stdint.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum Resolution : uint8_t { W16H9, W16H10, W21H9, W4H3, Equal };

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject* obj, QEvent* event);

private slots:
    void on_buttonType_clicked();
    void on_spinBoxEscapeR_editingFinished();
    void on_spinBoxPower_valueChanged(int arg1);
    void on_dialR_valueChanged(int value);
    void on_dialG_valueChanged(int value);
    void on_dialB_valueChanged(int value);
    void on_spinBoxColor_valueChanged(int arg1);
    void on_comboBoxResolution_currentIndexChanged(int index);
    void on_spinBoxResolution_valueChanged(int arg1);
    void on_horizontalSliderIterations_valueChanged(int value);

private:
    Ui::MainWindow* ui;
    Net::MessageID requestType = Net::MessageID::RequestClassic;
    Net::JobData requestData;
    Net::Server server;
    std::thread listenThread;
    int scrollDelta = 0;

    void SetImage(uint64_t token);
    void UpdateImage();
    void ListenJobs();
    void UpdateResolution(Resolution current, unsigned int scale);
};
