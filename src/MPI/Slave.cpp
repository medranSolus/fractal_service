#include "Slave.h"

namespace MPI
{
    JobRequest Slave::ReceiveJob() const
    {
        static JobRequest request;
        MPI_Recv(&request, 1, job_request_type, 0, Channel::Jobs, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return request;
    }

    void Slave::SendImage(const Surface& surface, uint32_t token, uint32_t job_id) const
    {
        ImageHeader header { token, job_id, surface.GetWidth(), surface.GetHeight() };
        MPI_Send(&header, 1, image_header_type, 0, Channel::ImageHeaders, MPI_COMM_WORLD);
        MPI_Send(surface.GetData(0), header.width * header.height, color_type, 0, Channel::ImageData, MPI_COMM_WORLD);
    }

    int Slave::Run()
    {
        while (true)
        {
            JobRequest request = ReceiveJob();
            if (request.id == MessageID::Shutdown)
                break;
            SendImage(fractals.Mandelbrot(request.width, request.height, request.min_y, request.max_y,
                request.offset_x, request.offset_y, request.zoom, request.escape_r, request.iterations,
                request.color_offset, request.channels, request.power, request.id == MessageID::RequestIterative), request.token, request.job_id);
        }
        return 0;
    }
}