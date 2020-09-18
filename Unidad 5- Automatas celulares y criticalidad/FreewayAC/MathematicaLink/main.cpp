#include <sstream>
#include <string>
#include <chrono>
#include "../FreewayAC/CellularAutomata.h"
#include "mathlink.h"
using namespace std;


CellularAutomata* ca = nullptr;
CircularCA* circularca = nullptr;
OpenCA* openca = nullptr;
AutonomousCircularCA* smartcircularca = nullptr;    
AutonomousOpenCA* smartopenca = nullptr;

void clear()
{
    if (circularca)
        delete circularca;
    if (openca)
        delete openca;
    if (smartcircularca)
        delete smartcircularca;
    if (smartopenca)
        delete smartopenca;
    
    circularca = nullptr;
    openca = nullptr;
    smartcircularca = nullptr;
    smartopenca = nullptr;
    ca = nullptr;
}

void create_circular_ca(int size, int vmax, double density, double rand_prob, int init_vel)
{
    clear();
    ca = circularca = new CircularCA(size, density, vmax, rand_prob, init_vel);
    MLPutSymbol(stdlink, "Null");
}
void create_open_ca(int size, int vmax, double density, double rand_prob, int init_vel, double new_car_prob, int new_car_speed)
{
    clear();
    ca = openca = new OpenCA(size, density, vmax, rand_prob, init_vel, new_car_prob, new_car_speed);
    MLPutSymbol(stdlink, "Null");
}
void create_autonomous_circular_ca(int size, int vmax, double density, double rand_prob, int init_vel, double aut_density)
{
    clear();
    ca = smartcircularca = new AutonomousCircularCA(size, density, vmax, rand_prob, init_vel, aut_density);
    MLPutSymbol(stdlink, "Null");
}
void create_autonomous_open_ca(int size, int vmax, double density, double rand_prob, int init_vel, double aut_density, double new_car_prob, int new_car_speed)
{
    clear();
    ca = smartopenca = new AutonomousOpenCA(size, density, vmax, rand_prob, init_vel, aut_density, new_car_prob, new_car_speed);
    MLPutSymbol(stdlink, "Null");
}

void delete_ca()
{
    clear();
    MLPutSymbol(stdlink, "Null");
}
void ca_step()
{
    ca->Step();
    MLPutSymbol(stdlink, "Null");
}
void ca_evolve(int iterations)
{
    ca->Evolve(iterations);
    MLPutSymbol(stdlink, "Null");
}
int ca_count_cars()
{
    return ca->CountCars();
}
void ca_calculate_ocupancy()
{
    vector<double> ocupancy = ca->CalculateOcupancy();
    MLPutReal64List(stdlink, &ocupancy[0], ocupancy.size());
}
void ca_calculate_flow()
{
    vector<double> flow = ca->CalculateFlow();
    MLPutReal64List(stdlink, &flow[0], flow.size());
}
void ca_get_current()
{
    std::vector<int> ca_list = ca->GetCa();
    MLPutInteger32List(stdlink, &ca_list[0], ca_list.size());
}
void ca_get_history()
{
    vector< std::vector<int> > history = ca->GetCaHistory();
    unsigned hsize = ca->GetHistorySize();
    unsigned ca_size = ca->GetSize();
    
    const int dimensions[2] = {(int)history.size(), (int)ca->GetSize()};
    const char* heads[2] = {"List", "List"};
    
    int* out = new int[hsize*ca_size];
    for (unsigned i = 0; i < hsize; ++i)
    {
        for (unsigned j = 0; j < ca_size; ++j)
        {
            out[ca_size*i+j] = history[i][j];
        }
    }
    MLPutInteger32Array(stdlink, out, dimensions, heads, 2);
}
    

#if defined(WIN32)

int __stdcall WinMain(HINSTANCE hinstCurrent, HINSTANCE hinstPrevious, LPSTR lpszCmdLine, int nCmdShow)
{
    char  buff[512];
    char FAR * buff_start = buff;
    char FAR * argv[32];
    char FAR * FAR * argv_end = argv + 32;

    if (!MLInitializeIcon(hinstCurrent, nCmdShow)) return 1;
    MLScanString(argv, &argv_end, &lpszCmdLine, &buff_start);
    return MLMain(argv_end - argv, argv);
}

#else
int main(int argc, char* argv[])
{
    return MLMain(argc, argv);
}

#endif
