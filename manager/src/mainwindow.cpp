#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QImageReader>
#include <QColorSpace>
#include <QPixmap>
#include <QLabel>
#include <QObject>
#include <QWheelEvent>
#include <string>
#include <chrono>
#include <cstdio>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), server("/home/mpi_fractal/fractal_cluster/bin/fractal_server.soc"), listenThread([this]() { this->ListenJobs(); })
{
    ui->setupUi(this);
    ui->labelImage->installEventFilter(this);
    ui->comboBoxResolution->addItem("16:9");
    ui->comboBoxResolution->addItem("16:10");
    ui->comboBoxResolution->addItem("21:9");
    ui->comboBoxResolution->addItem("4:3");
    ui->comboBoxResolution->addItem("1:1");
    ui->comboBoxResolution->setCurrentIndex(4);
    UpdateImage();
}

MainWindow::~MainWindow()
{
    delete ui;
    listenThread.join();
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Wheel:
    {
        auto& wheelEvent = static_cast<QWheelEvent&>(*event);
        scrollDelta += wheelEvent.delta();
        wheelEvent.accept();
        if (scrollDelta >= 120)
        {
            scrollDelta -= 120;
            requestData.zoom *= 1.1;
            UpdateImage();
        }
        else if (scrollDelta <= -120)
        {
            scrollDelta += 120;
            requestData.zoom *= 0.9;
            UpdateImage();
        }
        return true;
    }
    case QEvent::KeyRelease:
    {
        const double step = 0.01 / requestData.zoom;
        auto& keyEvent = static_cast<QKeyEvent&>(*event);
        switch (keyEvent.key())
        {
        case Qt::Key_W:
        {
            requestData.offset_y += step;
            break;
        }
        case Qt::Key_S:
        {
            requestData.offset_y -= step;
            break;
        }
        case Qt::Key_A:
        {
            requestData.offset_x -= step;
            break;
        }
        case Qt::Key_D:
        {
            requestData.offset_x += step;
            break;
        }
        default:
            return false;
        }
        UpdateImage();
        return true;
    }
    default:
        return false;
    }
}

void MainWindow::SetImage(uint64_t token)
{
    const std::string token_string = std::to_string(token);
    const std::string file = "/home/mpi_fractal/fractal_cluster/bin/jobs/" + token_string + ".png";
    QImageReader reader(file.c_str());
    reader.setAutoTransform(true);
    QImage image = reader.read();
    if (image.isNull())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QString(token_string.c_str()), reader.errorString()));
        return;
    }

    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    ui->labelImage->setPixmap(QPixmap::fromImage(std::move(image)).scaled(ui->labelImage->width(), ui->labelImage->height(), Qt::KeepAspectRatio));
    //remove(file.c_str());
}

void MainWindow::UpdateImage()
{
    Net::Client socket("/home/mpi_fractal/fractal_cluster/bin/fractal_cluster.soc");
    uint8_t i = 5;
    while (!socket.Connect() && i != 0)
        --i;
    if (socket.IsConnected())
    {
        socket.Write(requestData);
        socket.Close();
    }
    else
        Logger::LogError("Cannot send JobRequest!");
    ++requestData.token;
}

void MainWindow::ListenJobs()
{
    server.Listen(100);
    uint64_t token;
    do
    {
        Net::Client socket = server.Accept();
        if (socket.IsConnected())
        {
            while (!socket.Read(token))
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            socket.Close();
            emit SetImage(token);
        }
    } while (true);
    server.Close();
}

void MainWindow::UpdateResolution(Resolution current, unsigned int scale)
{
    switch(current)
    {
    case Resolution::Equal:
    {
        requestData.height = requestData.width = scale;
        break;
    }
    case Resolution::W4H3:
    {
        requestData.height = 3U * scale;
        requestData.width = requestData.height + scale;
        break;
    }
    case Resolution::W21H9:
    {
        requestData.width = 21U * scale;
        requestData.height = 9U * scale;
        break;
    }
    case Resolution::W16H10:
    {
        requestData.width = 16U* scale;
        requestData.height = 10U * scale;
        break;
    }
    case Resolution::W16H9:
    default:
    {
        requestData.width = 16U * scale;
        requestData.height = 9U * scale;
        break;
    }
    }
    ui->labelResolution->setText(("Resolution scale [" + std::to_string(requestData.width) + "x" + std::to_string(requestData.height) + "]").c_str());
    UpdateImage();
}

void MainWindow::on_buttonType_clicked()
{
    if (requestData.id == MessageID::RequestClassic)
    {
        requestData.id = MessageID::RequestIterative;
        ui->buttonType->setText("Iterative");
    }
    else
    {
        requestData.id = MessageID::RequestClassic;
        ui->buttonType->setText("Classic");
    }
    UpdateImage();
}

void MainWindow::on_spinBoxEscapeR_editingFinished()
{
    requestData.escape_r = ui->spinBoxEscapeR->value();
    UpdateImage();
}

void MainWindow::on_spinBoxPower_valueChanged(int arg1)
{
    requestData.power = arg1;
    UpdateImage();
}

void MainWindow::on_dialR_valueChanged(int value)
{
    requestData.r = value / 10.0f;
    UpdateImage();
}

void MainWindow::on_dialG_valueChanged(int value)
{
    requestData.g = value / 10.0f;
    UpdateImage();
}

void MainWindow::on_dialB_valueChanged(int value)
{
    requestData.b = value / 10.0f;
    UpdateImage();
}

void MainWindow::on_spinBoxColor_valueChanged(int arg1)
{
    requestData.color_offset = arg1;
    UpdateImage();
}

void MainWindow::on_comboBoxResolution_currentIndexChanged(int index)
{
    UpdateResolution(static_cast<Resolution>(index), ui->spinBoxResolution->value());
}

void MainWindow::on_spinBoxResolution_valueChanged(int arg1)
{
    UpdateResolution(static_cast<Resolution>(ui->comboBoxResolution->currentIndex()), arg1);
}

void MainWindow::on_horizontalSliderIterations_valueChanged(int value)
{
    requestData.iterations = value;
    UpdateImage();
}
