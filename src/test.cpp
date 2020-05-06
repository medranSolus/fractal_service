#include "Net/Server.h"

// args: TYPE(0=Classic, 1=Iterative) TOKEN HEIGHT WIDTH ZOOM OFFSET_X OFFSET_Y COLOR_OFFSET POWER ITERATIONS R G B ESCAPE_R
int main(int argc, char* argv[])
{
    MPI::JobRequest request;
    request.token = argc >= 3 ? abs(atoll(argv[2])) : 9999;
    request.height = argc >= 4 ? abs(atoi(argv[3])) : 2000;
    request.width = argc >= 5 ? abs(atoi(argv[4])) : 2000;
    request.zoom = argc >= 6 ? abs(atof(argv[5])) : 2.0;
    request.offset_x = argc >= 7 ? atof(argv[6]) : -1.0;
    request.offset_y = argc >= 8 ? atof(argv[7]) : 0.0;
    request.color_offset = argc >= 9 ? abs(atoi(argv[8])) : 5;
    request.power = argc >= 10 ? atoi(argv[9]) : 2;
    request.iterations = argc >= 11 ? abs(atoi(argv[10])) : 550;
    request.channels.r = argc >= 12? static_cast<float>(atof(argv[11])) : 9.0f;
    request.channels.g = argc >= 13? static_cast<float>(atof(argv[12])) : 15.0f;
    request.channels.b = argc >= 14? static_cast<float>(atof(argv[13])) : 8.5f;
    request.escape_r = argc >= 15 ? abs(atof(argv[14])) : 2.0;
    Net::Server serv("fractal_server.soc");
    serv.Listen(1);
    Net::Client soc("fractal_cluster.soc");
    sleep(5);
    soc.Connect();
    if (argc >= 2)
    {
        if (abs(atoi(argv[1])))
            soc.Write(MPI::MessageID::RequestIterative);
        else
            soc.Write(MPI::MessageID::RequestClassic);
    }
    else
        soc.Write(MPI::MessageID::RequestClassic);
    soc.Write(request);
    soc.Close();
    sleep(5);
    do
    {
        soc = serv.Accept();
    } while (!soc.IsConnected());
    soc.Read(request.token);
    soc.Close();
    serv.Close();
    Net::Client soc1("fractal_cluster.soc");
    soc1.Connect();
    soc1.Write(MPI::MessageID::Shutdown);
    soc1.Close();
    return 0;
}