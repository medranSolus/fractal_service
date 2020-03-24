#include "Master.h"
#include "Fractal.h"

namespace MPI
{
    JobResponse Master::ReceiveImage() const
    {
        MPI_Status status;
        ImageHeader header;
        MPI_Recv(&header, 1, image_header_type, MPI_ANY_SOURCE, Channel::ImageHeaders, MPI_COMM_WORLD, &status);
        JobResponse response;
        response.token = header.token;
        response.job_id = header.job_id;
        response.image = Surface(header.width, header.height);
        MPI_Recv(response.image.GetData(0), header.width * header.height, color_type, status.MPI_SOURCE, Channel::ImageData, MPI_COMM_WORLD, &status);
        return response;
    }
    
    int Master::Run()
    {
        JobRequest request;
        request.iterations = 550;
        request.color_offset = 5;
        request.power = 2;
        request.token = 15;
        request.job_id = 0;
        request.width = 1000;
        request.height = 1000;
        request.zoom = 0.01;
        request.offset_x = -1.75;
        request.offset_y = 0.0;
        request.work_space = { 0, request.width, 0, request.height };
        SendJob(1, request);
        JobResponse response = ReceiveImage();
        response.image.Save("bin/" + std::to_string(response.token) + "_" + std::to_string(response.job_id));
        return 0;
    }
}