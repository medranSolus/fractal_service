#include "Net/Server.h"
#include "Net/Data.h"
#include <map>
#include <utility>
#include <iostream>

int main(int argc, char* argv[])
{
    constexpr uint64_t test_size = 100;
    Net::JobData request;
    request.height = request.width = 2000U;
    uint64_t sum = 0;
    sleep(10); // Max time for cluster to start
    Net::Server serv("fractal_server.soc");
    serv.Listen(1);
    for (request.token = 0; request.token < test_size;)
    {
        Net::Client soc("fractal_cluster.soc");
        uint8_t i = 0;
        while (!soc.Connect() && i < 10U)
            ++i;
        if (soc.IsConnected())
        {
            struct timespec start;
            soc.Write(Net::MessageID::RequestIterative);
            soc.Write(request);
            soc.Close();
            clock_gettime(CLOCK_MONOTONIC, &start);
            do
            {
                Net::Client soc = serv.Accept();
                if (soc.IsConnected())
                {
                    soc.Read(request.token);
                    soc.Close();
                    struct timespec end;
                    clock_gettime(CLOCK_MONOTONIC, &end);
                    sum += (end.tv_nsec / 1000000ULL + end.tv_sec * 1000ULL) - (start.tv_nsec / 1000000ULL + start.tv_sec * 1000ULL);
                    break;
                }
            } while (true);
            ++request.token;
        }
        else
            Logger::LogWarning("Trying to connect to cluster in next iteration. Token = " + std::to_string(request.token));
    }
    Net::Client soc("fractal_cluster.soc");
    soc.Connect();
    soc.Write(Net::MessageID::Shutdown);
    soc.Close();
    std::cout << "Avg time: " << sum / test_size << " ms" << std::endl;
    return 0;
}