#include "Master.h"
#include "Fractal.h"
#include "Logger.h"
#include "Net/Server.h"
#include <algorithm>
#include <chrono>

namespace MPI
{
    void Master::SaveImage(const JobEntry& job, uint64_t token)
    {
        auto& image = job.done.at(0);
        for (uint32_t i = 1; i < job.count; ++i)
            image->Append(std::move(*job.done.at(i)));
        image->Save("jobs/" + std::to_string(token));
        Net::Client socket("fractal_server.soc");
        uint8_t i = 5;
        while (!socket.Connect() && i != 0)
            --i;
        if (socket.IsConnected())
        {
            socket.Write(token);
            socket.Close();
        }
        else
            Logger::LogError("Cannot send finish signal to fractal server about job " + std::to_string(token));
    }

    void Master::InsertJob(const JobRequest& request)
    {
        pending_jobs_mutex.lock();
        pending_jobs.emplace_back(request);
        pending_jobs_mutex.unlock();
    }

    void Master::DispatchJob(JobRequest& request)
    {
        if (request.height >= 2 * minimal_node_size)
        {
            uint32_t step = request.height / stations;
            if (step < minimal_node_size)
                step = minimal_node_size;
            request.job_id = 0;
            request.work_space = { 0, request.width, 0, step };
            finished_jobs_mutex.lock();
            finished_jobs.emplace(request.token, JobEntry{ 0 });
            finished_jobs_mutex.unlock();
            while (request.work_space.max_y + step < request.height)
            {
                InsertJob(request);
                ++request.job_id;
                request.work_space.min_y = request.work_space.max_y;
                request.work_space.max_y += step;
            }
            if (request.height - request.work_space.max_y >= minimal_node_size * 2 / 3)
            {
                request.work_space.max_y = request.work_space.min_y + (request.height - request.work_space.min_y) / 2;
                InsertJob(request);
                ++request.job_id;
                request.work_space.min_y = request.work_space.max_y;
            }
            request.work_space.max_y = request.height;
            InsertJob(request);
            finished_jobs_mutex.lock();
            finished_jobs.at(request.token).count = request.job_id + 1;
            finished_jobs_mutex.unlock();
        }
        else
        {
            finished_jobs_mutex.lock();
            finished_jobs.emplace(request.token, JobEntry{ 1 });
            finished_jobs_mutex.unlock();
            request.job_id = 0;
            request.work_space = { 0, request.width, 0, request.height };
            InsertJob(request);
        }
    }

    void Master::ReceiveRequests()
    {
        do
        {
            Net::Client socket = request_server.Accept();
            if (socket.IsConnected())
            {
                JobRequest request;
                socket.Read(request.id);
                if (request.id == MessageID::Shutdown)
                {
                    socket.Close();
                    Stop();
                }
                else
                {
                    worker_threads_mutex.lock();
                    worker_threads.emplace_back([this](JobRequest request, Net::Client socket)
                    { 
                        socket.Read(request);
                        DispatchJob(request);
                        socket.Close();
                    }, std::move(request), std::move(socket));
                    worker_threads_mutex.unlock();
                }
            }
        } while (cluster_running);
    }

    void Master::ScheduleJobs()
    {
        do
        {
            pending_jobs_mutex.lock_shared();
            size_t count = pending_jobs.size();
            pending_jobs_mutex.unlock_shared();
            if (count)
            {
                pending_jobs_mutex.lock();
                JobRequest job = std::move(pending_jobs.front());
                pending_jobs.pop_front();
                pending_jobs_mutex.unlock();
                do
                {
                    free_stations_mutex.lock_shared();
                    count = free_stations.size();
                    free_stations_mutex.unlock_shared();
                    if (count)
                    {
                        free_stations_mutex.lock();
                        int rank = free_stations.front();
                        free_stations.pop_front();
                        free_stations_mutex.unlock();
                        SendJob(rank, job);
                    }
                    else
                        std::this_thread::sleep_for(std::chrono::microseconds(1));
                } while (count == 0 && cluster_running);
            }
            else
                std::this_thread::sleep_for(std::chrono::microseconds(1));
        } while (cluster_running);
        
    }

    MPI_Request Master::ReceiveHeader(ImageHeader& header) const
    {
        static MPI_Request request;
        MPI_Irecv(&header, 1, image_header_type, MPI_ANY_SOURCE, Channel::ImageHeaders, MPI_COMM_WORLD, &request);
        return request;
    }

    void Master::ReceiveImage(const ImageHeader& header, int source)
    {
        static JobResponse response;
        response.token = header.token;
        response.job_id = header.job_id;
        response.image = std::make_shared<Surface>(header.width, header.height);
        MPI_Recv(response.image->GetData(0), header.width * header.height, color_type,
            source, Channel::ImageData, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        free_stations_mutex.lock();
        free_stations.push_back(source);
        free_stations_mutex.unlock();

        finished_jobs_mutex.lock();
        auto& current_job = finished_jobs.at(response.token);
        current_job.done.emplace(response.job_id, response.image);
        if (current_job.count == current_job.done.size())
        {
            auto finished_job = std::move(finished_jobs.at(response.token));
            finished_jobs.erase(response.token);
            finished_jobs_mutex.unlock();
            worker_threads_mutex.lock();
            worker_threads.emplace_back(&Master::SaveImage, this, std::move(finished_job), response.token);
            worker_threads_mutex.unlock();
        }
        else
            finished_jobs_mutex.unlock();
    }

    void Master::ReceiveJobs()
    {
        ImageHeader header;
        MPI_Request request;
        MPI_Status status; 
        do
        {
            request = ReceiveHeader(header);
            int flag = false;
            do 
            {
                MPI_Test(&request, &flag, &status);
                if (flag)
                    ReceiveImage(header, status.MPI_SOURCE);
                else
                    std::this_thread::sleep_for(std::chrono::microseconds(1));
            } while (flag == false && cluster_running);
        } while (cluster_running);
        free_stations_mutex.lock();
        while (free_stations.size() < stations)
        {
            request = ReceiveHeader(header);
            MPI_Wait(&request, &status);
            ReceiveImage(header, status.MPI_SOURCE);
        }
        free_stations_mutex.unlock();
    }

    void Master::CleanWorkThreads()
    {
        size_t count = 0;
        do
        {
            worker_threads_mutex.lock_shared();
            count = worker_threads.size();
            worker_threads_mutex.unlock_shared();
            if (count)
            {
                worker_threads_mutex.lock();
                auto thread = std::move(worker_threads.front());
                worker_threads.pop_front();
                worker_threads_mutex.unlock();
                thread.join();
            }
            else
                std::this_thread::sleep_for(std::chrono::seconds(1));
        } while (count > 0 || cluster_running);
    }
    
    void Master::Stop()
    {
        cluster_running = false;
        JobRequest request;
        request.id = MessageID::Shutdown;
        MPI_Request* status_requests = new MPI_Request[stations];
        for (uint32_t i = 0; i < stations; ++i)
            MPI_Isend(&request, 1, job_request_type, i + 1, Channel::Jobs, MPI_COMM_WORLD, &status_requests[i]);
        for (uint32_t i = 0; i < stations; ++i)
            MPI_Wait(&status_requests[i], MPI_STATUS_IGNORE);
        request_server.Close();
    }

    Master::Master(uint32_t stations, uint64_t minimal_node_size)
        : stations(stations), minimal_node_size(minimal_node_size), request_server("fractal_cluster.soc")
    {
        free_stations.resize(stations);
        int i = 1;
        std::generate(free_stations.begin(), free_stations.end(), [&i]() { return i++; });
    }

    Master::~Master()
    {
        job_scheduler_thread->join();
        job_receiver_thread->join();
        worker_threads_clean_thread->join();
        for (auto& thread : worker_threads)
            thread.join();
    }

    int Master::Run(int connection_queue_size)
    {
        request_server.Listen(connection_queue_size);
        cluster_running = true;
        job_receiver_thread = std::make_unique<std::thread>(&Master::ReceiveJobs, this);
        job_scheduler_thread = std::make_unique<std::thread>(&Master::ScheduleJobs, this);
        worker_threads_clean_thread = std::make_unique<std::thread>(&Master::CleanWorkThreads, this);
        ReceiveRequests();
        return 0;
    }
}