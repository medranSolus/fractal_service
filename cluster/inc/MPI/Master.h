#pragma once
#include "Agent.h"
#include "Net/Server.h"
#include <unordered_map>
#include <shared_mutex>
#include <deque>
#include <atomic>

namespace MPI
{
    class Master : public Agent
    {
        typedef std::unique_ptr<std::thread> unique_thread;

        struct JobEntry
        {
            uint32_t count;
            std::unordered_map<uint32_t, std::shared_ptr<Surface>> done;
        };

        int master_rank;
        uint32_t stations;
        const uint64_t minimal_node_size;
        std::atomic_bool cluster_running = false;
        Net::Server request_server;

        std::deque<int> free_stations;
        std::deque<JobRequest> pending_jobs;
        std::unordered_map<uint64_t, JobEntry> finished_jobs;
        std::deque<std::thread> worker_threads;

        std::shared_mutex free_stations_mutex;
        std::shared_mutex pending_jobs_mutex;
        std::shared_mutex finished_jobs_mutex;
        std::shared_mutex worker_threads_mutex;

        unique_thread job_scheduler_thread = nullptr;
        unique_thread job_receiver_thread = nullptr;
        unique_thread worker_threads_clean_thread = nullptr;

        inline void SendJob(int rank, const JobRequest& request) const { MPI_Send(&request, 1, job_request_type, rank, Channel::Jobs, MPI_COMM_WORLD); }
        
        void SaveImage(const JobEntry& job, uint64_t token);
        void InsertJob(const JobRequest& request);
        void DispatchJob(JobRequest& request);
        void ReceiveRequests();
        void ScheduleJobs();
        MPI_Request ReceiveHeader(ImageHeader& header) const;
        void ReceiveImage(const ImageHeader& header, int source);
        void ReceiveJobs();
        void CleanWorkThreads();
        void Stop();

    public:
        Master(int master_rank, uint32_t stations, uint64_t minimal_node_size);
        ~Master();

        int Run(int connection_queue_size);
    };
}