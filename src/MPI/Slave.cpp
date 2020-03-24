#include "Slave.h"
#include "Fractal.h"

namespace MPI
{
    inline JobRequest Slave::ReceiveJob() const
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
        JobRequest request = ReceiveJob();
        SendImage(Fractal::Mandelbrot(request.width, request.height, request.work_space,
            { -2.0, 1.0, -1.5, 1.5 }, request.escape_r, request.iterations, request.color_offset), request.token, request.job_id);
        return 0;
    }
}