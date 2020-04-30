#include "Net/Server.h"

int main(int argc, char* argv[])
{
    MPI::JobRequest request;
    request.token = argc >= 2 ? abs(atoll(argv[1])) : 9999;
    request.height = 2000;
    request.width = 2000;
    request.zoom = 2.0;
    request.offset_x = -1.0;
    uint64_t tok = 0;
    Net::Server serv("fractal_server.soc");
    serv.Listen(1);
    Net::Client soc("fractal_cluster.soc");
    sleep(5);
    soc.Connect();
    soc.Write(MPI::MessageID::RequestClassic);
    soc.Write(request);
    soc.Close();
    sleep(5);
    do
    {
        soc = serv.Accept();
    } while (!soc.IsConnected());
    soc.Read(tok);
    soc.Close();
    serv.Close();
    Net::Client soc1("fractal_cluster.soc");
    soc1.Connect();
    soc1.Write(MPI::MessageID::Shutdown);
    soc1.Close();
    return 0;
}