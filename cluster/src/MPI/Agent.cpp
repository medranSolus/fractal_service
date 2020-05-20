#include "Agent.h"

namespace MPI
{
    Agent::Agent() noexcept
    {
        MPI_Datatype rgb_channels_type;
        MPI_Type_contiguous(3, MPI_FLOAT, &rgb_channels_type);
        MPI_Type_commit(&rgb_channels_type);

        MPI_Datatype job_data_type;
        constexpr int job_data_block_lengths[] { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
        constexpr MPI_Aint job_data_offsets[]
        { 
            offsetof(Net::JobData, escape_r),
            offsetof(Net::JobData, zoom),
            offsetof(Net::JobData, offset_x),
            offsetof(Net::JobData, offset_y),
            offsetof(Net::JobData, width),
            offsetof(Net::JobData, height),
            offsetof(Net::JobData, power),
            offsetof(Net::JobData, color_offset),
            offsetof(Net::JobData, iterations),
            offsetof(Net::JobData, channels),
            offsetof(Net::JobData, token)
        };
        const MPI_Datatype job_data_types[]
        {
            MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_UINT32_T, MPI_UINT32_T,
            MPI_INT32_T, MPI_UINT16_T, MPI_UINT16_T, rgb_channels_type, MPI_UINT64_T
        };
        MPI_Type_create_struct(11, job_data_block_lengths, job_data_offsets, job_data_types, &job_data_type);
        MPI_Type_commit(&job_data_type);

        constexpr int job_request_block_lengths[] { 1, 1, 1, 1, 1};
        constexpr MPI_Aint job_request_offsets[]
        { 
            offsetof(JobRequest, data),
            offsetof(JobRequest, job_id),
            offsetof(JobRequest, min_y),
            offsetof(JobRequest, max_y),
            offsetof(JobRequest, id)
        };
        const MPI_Datatype job_request_types[]
        {
            job_data_type, MPI_UINT32_T, MPI_UINT32_T, MPI_UINT32_T, MPI_UINT8_T
        };
        MPI_Type_create_struct(5, job_request_block_lengths, job_request_offsets, job_request_types, &job_request_type);
        MPI_Type_commit(&job_request_type);

        MPI_Type_contiguous(5, MPI_UINT32_T, &image_header_type);
        MPI_Type_commit(&image_header_type);
        MPI_Type_contiguous(3, MPI_UINT8_T, &color_type);
        MPI_Type_commit(&color_type);
    }
}