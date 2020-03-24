#include "Agent.h"

namespace MPI
{
    Agent::Agent() noexcept
    {
        MPI_Type_contiguous(4, MPI_UINT32_T, &work_space_type);
        MPI_Type_commit(&work_space_type);
        constexpr int block_lengths[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        constexpr MPI_Aint offsets[]
        { 
            offsetof(JobRequest, escape_r),
            offsetof(JobRequest, zoom),
            offsetof(JobRequest, offset_x),
            offsetof(JobRequest, offset_y),
            offsetof(JobRequest, work_space),
            offsetof(JobRequest, width),
            offsetof(JobRequest, height),
            offsetof(JobRequest, token),
            offsetof(JobRequest, job_id),
            offsetof(JobRequest, power),
            offsetof(JobRequest, color_offset),
            offsetof(JobRequest, iterations),
            offsetof(JobRequest, id)
        };
        const MPI_Datatype types[]
        {
            MPI_LONG_DOUBLE, MPI_LONG_DOUBLE, MPI_LONG_DOUBLE, MPI_LONG_DOUBLE,
            work_space_type,MPI_UINT32_T, MPI_UINT32_T, MPI_UINT32_T, MPI_UINT32_T,
            MPI_INT, MPI_UINT16_T, MPI_UINT16_T, MPI_UINT8_T
        };
        MPI_Type_create_struct(13, block_lengths, offsets, types, &job_request_type);
        MPI_Type_commit(&job_request_type);
        MPI_Type_contiguous(4, MPI_UINT32_T, &image_header_type);
        MPI_Type_commit(&image_header_type);
        MPI_Type_contiguous(3, MPI_UINT8_T, &color_type);
        MPI_Type_commit(&color_type);
    }
}