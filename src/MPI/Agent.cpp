#include "Agent.h"

namespace MPI
{
    Agent::Agent() noexcept
    {
        MPI_Type_contiguous(4, MPI_UINT32_T, &work_space_type);
        MPI_Type_commit(&work_space_type);
        constexpr int block_lengths[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        constexpr MPI_Aint offsets[]
        { 
            offsetof(JobRequest, id),
            offsetof(JobRequest, iterations),
            offsetof(JobRequest, color_offset),
            offsetof(JobRequest, token),
            offsetof(JobRequest, job_id),
            offsetof(JobRequest, width),
            offsetof(JobRequest, height),
            offsetof(JobRequest, work_space),
            offsetof(JobRequest, escape_r),
            offsetof(JobRequest, zoom),
            offsetof(JobRequest, offset)
        };
        const MPI_Datatype types[]
        {
            MPI_UINT8_T, MPI_UINT16_T, MPI_UINT16_T, MPI_UINT32_T, MPI_UINT32_T,
            MPI_UINT32_T, MPI_UINT32_T, work_space_type, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE
        };
        MPI_Type_create_struct(11, block_lengths, offsets, types, &job_request_type);
        MPI_Type_commit(&job_request_type);
        MPI_Type_contiguous(4, MPI_UINT32_T, &image_header_type);
        MPI_Type_commit(&image_header_type);
        MPI_Type_contiguous(3, MPI_UINT8_T, &color_type);
        MPI_Type_commit(&color_type);
    }
}