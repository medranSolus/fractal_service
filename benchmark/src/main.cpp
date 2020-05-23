#include "Net/Server.h"
#include "Net/Data.h"
#include <map>
#include <utility>
#include <iostream>

int main(int argc, char* argv[])
{
    const uint64_t test_size = 100;
    std::map<uint64_t, std::pair<uint64_t, uint64_t>> results;
    std::thread listen_thread([test_size, &results]()
    {
        Net::Server serv("fractal_server.soc");
        serv.Listen(test_size);
        uint64_t token = 0;
        do
        {
            Net::Client soc = serv.Accept();
            if (soc.IsConnected())
            {
                soc.Read(token);
                soc.Close();
                struct timespec end;
                clock_gettime(CLOCK_MONOTONIC, &end);
                results.at(token).second = end.tv_nsec / 1000000ULL + end.tv_sec * 1000ULL;
            }
        } while (token < test_size);
        
    });
    Net::JobData request;
    request.height = 2000U;
    request.width = 2000U;
    for (request.token = 1; request.token <= test_size;)
    {
        Net::Client soc("fractal_cluster.soc");
        uint8_t i = 0;
        while (!soc.Connect() && i < 10U)
            ++i;
        if (soc.IsConnected())
        {
            struct timespec start;
            soc.Write(Net::MessageID::RequestClassic);
            soc.Write(request);
            soc.Close();
            clock_gettime(CLOCK_MONOTONIC, &start);
            results.emplace(request.token++, std::make_pair<uint64_t, uint64_t>(start.tv_nsec / 1000000ULL + start.tv_sec * 1000ULL, 0ULL));
        }
        else
            Logger::LogWarning("Trying to connect to cluster in next iteration. Token = " + std::to_string(request.token));
    }
    listen_thread.join();
    Net::Client soc("fractal_cluster.soc");
    soc.Connect();
    soc.Write(Net::MessageID::Shutdown);
    soc.Close();
    uint64_t sum = 0;
    for (uint64_t i = 1; i <= test_size; ++i)
        sum += results.at(i).second - results.at(i).first;
    std::cout << "Avg time: " << sum / test_size << " ms" << std::endl;
    return 0;
}