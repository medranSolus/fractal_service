#include "Slave.h"

namespace MPI
{
    JobRequest Slave::ReceiveJob() const
    {
        static JobRequest request;
        MPI_Recv(&request, 1, job_request_type, master_rank, Channel::Jobs, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        return request;
    }

    void Slave::SendImage(const Surface& surface, uint64_t token, uint32_t job_id) const
    {
        ImageHeader header { token, job_id, surface.GetWidth(), surface.GetHeight() };
        MPI_Send(&header, 1, image_header_type, master_rank, Channel::ImageHeaders, MPI_COMM_WORLD);
        MPI_Send(surface.GetData(0), header.width * header.height, color_type, master_rank, Channel::ImageData, MPI_COMM_WORLD);
    }

    int Slave::Run()
    {
        while (true)
        {
            JobRequest request = ReceiveJob();
            if (request.id == Net::MessageID::Shutdown)
                break;
            SendImage(fractals.Mandelbrot(request.data, request.min_y, request.max_y,
                request.id == Net::MessageID::RequestIterative), request.data.token, request.job_id);
        }
        Logger::LogInfo("Worker " + std::to_string(rank) + " shutting down.");
        return 0;
    }
}