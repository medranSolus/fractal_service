#pragma once
#include "Server.h"
#include <QMainWindow>
#include <QImage>
#include <thread>
#include <stdint.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class MessageID : uint8_t { Shutdown = 66, RequestClassic = 42, RequestIterative = 43 };
enum Resolution : uint8_t { W16H9, W16H10, W21H9, W4H3, Equal };

#pragma pack(push, 1)
struct JobRequest
{
    MessageID id = MessageID::RequestClassic;
    double escape_r = 2.0;
    double zoom = 1.0;
    double offset_x = 0.0;
    double offset_y = 0.0;
    uint32_t width = 1000;
    uint32_t height = 1000;
    int32_t power = 2;
    uint16_t color_offset = 5;
    uint16_t iterations = 550;
    float r = 9.0f;
    float g = 15.0f;
    float b = 8.5f;
    uint64_t token = 0;
};
#pragma pack(pop)

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
    JobRequest requestData;
    Net::Server server;
    std::thread listenThread;
    int scrollDelta = 0;

    void SetImage(uint64_t token);
    void UpdateImage();
    void ListenJobs();
    void UpdateResolution(Resolution current, unsigned int scale);
};
