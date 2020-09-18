#include <sstream>
#include <string>
#include <iostream>

#include "optionparser.h"
#include "../FreewayAC/Auxiliar.h"
#include "../FreewayAC/CellularAutomata.h"

#if defined(_WIN32)
#include <windows.h>
#endif

using namespace std;


/****************************
*                           *
*   Parser de argumentos    *
*                           *
****************************/

struct Arg: public option::Arg
{
    static void PrintError(const char* msg1, const option::Option& opt, const char* msg2)
    {
#if defined(_WIN32)
        fprintf(stderr, "%s", msg1);
        fwrite(opt.name, opt.namelen, 1, stderr);
        fprintf(stderr, "%s", msg2);
#endif
    }

    static option::ArgStatus Required(const option::Option& option, bool msg)
    {
        if (option.arg != 0)
            return option::ARG_OK;

        if (msg) PrintError("Opcion '", option, "' requiere un argumento.\n");
        return option::ARG_ILLEGAL;
    }
};

enum  OptionIndex { UNKNOWN, FWSIZE, ITERATIONS, VMAX, DENSITY, RAND_PROB, INIT_VEL,
                    PLOT_TRAFFIC, PLOT_FLOW,
                    CA_CIRCULAR, CA_OPEN, CA_AUTONOMOUS_CIRCULAR, CA_AUTONOMOUS_OPEN,
					NEW_CAR_PROB, NEW_CAR_SPEED, AUT_DENSITY,
					OUT_FILE_NAME, PATH, HELP };

const option::Descriptor usage[] =
{
    {UNKNOWN, 0, "", "",Arg::None, "INSTRUCCIONES: FreewayAC [opciones]\n"},
    {FWSIZE,  0,"s", "size", Arg::Required, "  -s <arg>, \t--size=<arg>  \tTamano del AC." },
    {ITERATIONS,  0,"i", "iter", Arg::Required, "  -i <arg>, \t--iter=<arg>  \tIteraciones del AC." },
    {VMAX,  0,"v", "vmax", Arg::Required, "  -v <arg>, \t--vmax=<arg>  \tVelocidad maxima de auto." },
    {DENSITY,  0,"d", "density", Arg::Required, "  -d <arg>, \t--density=<arg>  \tDensidad de autos." },
    {RAND_PROB,  0,"r", "rand_prob", Arg::Required, "  -r <arg>, \t--rand_prob=<arg>  \tProbabilidad de descenso de velocidad." },
	{INIT_VEL, 0, "i", "init_vel", Arg::Required, "  -i <arg>, \t--init_vel=<arg>  \tVelocidad inicial de los autos." },

    {PLOT_TRAFFIC,  0,"p","plot_traffic", Arg::None,
    "  -p , \t--plot_traffic  \tCrea mapa de posicion de autos vs tiempo." },
    {PLOT_FLOW, 0, "p", "plot_flow", Arg::None,
    "  -p , \t--plot_flow  \tCrea mapa de flujo de autos vs tiempo." },

    {CA_CIRCULAR,  0,"","ca_circular", Arg::None, "  \t--ca_circular  \tAutomata celular circular." },
    {CA_OPEN,  0,"","ca_open", Arg::None, "  \t--ca_open  \tAutomata celular con frontera abierta." },
    {CA_AUTONOMOUS_CIRCULAR,  0,"","ca_autonomous_circular", Arg::None, "  \t--ca_autonomous_circular  \tAutomata celular circular con vehiculos autonomos." },
    {CA_AUTONOMOUS_OPEN,  0,"","ca_autonomous_open", Arg::None, "  \t--ca_autonomous_open  \tAutomata celular abierto con vehiculos autonomos." },

	{NEW_CAR_PROB,  0,"","new_car_prob", Arg::Required, "  \t--new_car_prob  \tProbabilidad de que se aparezca nuevo auto en frontera abierta." },
	{NEW_CAR_SPEED, 0, "", "new_car_speed", Arg::Required, "  \t--new_car_speed  \tVelocidad que entre a AC abierto." },
	{AUT_DENSITY,  0,"","aut_density", Arg::Required, "  \t--aut_density  \tDensidad de autos autonomos." },

	{OUT_FILE_NAME,  0,"", "out_file_name", Arg::Required, "  \t--out_file_name=<arg>  \tCambia el nombre del archivo de salida al especificado." },
	{PATH,  0,"", "path", Arg::Required, "  \t--path=<arg>  \tRuta donde guardar archivos de salida." },
    {HELP, 0,"", "help", Arg::None,    "  \t--help  \tMuestra instrucciones detalladas de cada experimento." },
    {0,0,0,0,0,0}
};

void describe_experiments()
{
    const char *text = "A continuacion se enumera la lista de AC disponibles y sus parametros.\n"
        "Todas las opciones se representan en mayusculas.\n\n"
        "=== Tipos de automatas celulares ===\n"
        "CA_CIRCULAR            -> Descripcion: Automata celular con fronteras periodicas. Pista circular.\n"
        "                          Parametros relevantes: Ninguno.\n"
        "CA_OPEN                -> Descripcion: Automata celular con fronteras abiertas. Entran autos en\n"
        "                                       la primera pos del AC.\n"
        "                          Parametros relevantes: NEW_CAR_PROB, NEW_CAR_SPEED.\n"
        "CA_AUTONOMOUS_CIRCULAR -> Descripcion: Automata celular circular con vehiculos autonomos.\n"
        "                          Parametros relevantes: AUT_DENSITY.\n"
        "CA_AUTONOMOUS_OPEN     -> Descripcion: Automata celular abierto con vehiculos autonomos.\n"
        "                          Parametros relevantes: NEW_CAR_PROB, NEW_CAR_SPEED, AUT_DENSITY.\n"
        "\n=== Experimentos ===\n"
        "PLOT_TRAFFIC           -> Descripcion: Evoluciona automata celular y grafica su representacion.\n"
        "PLOT_FLOW              -> Descripcion: Evoluciona automata celular y grafica su flujo.\n";
    cout << text << endl;
}

int main(int argc, char* argv[])
{
    // Valores por defecto.
    unsigned size = 100, iterations = 100;
    int vmax = 5, init_vel = 1;
    double density = 0.2, rand_prob = 0.2;

    bool plot_traffic = false, plot_flow = false;

    CA_TYPE ca_type = CIRCULAR_CA;
    double new_car_prob = 0.1, aut_density = 0.1;
    int new_car_speed = 1;
    string out_file_name = "", path = "";

    // Ejecuta parser de argumentos.
    argc -= (argc > 0); argv += (argc > 0);
    option::Stats  stats(usage, argc, argv);
    option::Option *options = new option::Option[stats.options_max];
    option::Option *buffer = new option::Option[stats.buffer_max];
    option::Parser parse(usage, argc, argv, options, buffer);

    if (parse.error())
        return 1;

    if (options[HELP] || argc == 0)
    {
        if (options[HELP])
            describe_experiments();
        option::printUsage(cout, usage);
        return 0;
    }

    for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
        cout << "Opcion desconocida: " << opt->name << "\n";

    for (int i = 0; i < parse.optionsCount(); ++i)
    {
        option::Option& opt = buffer[i];
        switch (opt.index())
        {
            case FWSIZE:
            size = aux_string_to_num<unsigned>(opt.arg);
            break;

            case ITERATIONS:
            iterations = aux_string_to_num<unsigned>(opt.arg);
            break;

            case VMAX:
            vmax = aux_string_to_num<int>(opt.arg);
            break;

            case DENSITY:
            density = aux_string_to_num<double>(opt.arg);
            break;

            case RAND_PROB:
            rand_prob = aux_string_to_num<double>(opt.arg);
            break;

            case PLOT_TRAFFIC:
            plot_traffic = true;
            break;

            case PLOT_FLOW:
            plot_flow = true;
            break;

            case CA_CIRCULAR:
            ca_type = CIRCULAR_CA;
            break;

            case CA_OPEN:
            ca_type = OPEN_CA;
            break;

            case CA_AUTONOMOUS_CIRCULAR:
            ca_type = AUTONOMOUS_CIRCULAR_CA;
            break;

            case CA_AUTONOMOUS_OPEN:
            ca_type = AUTONOMOUS_OPEN_CA;
            break;

            case NEW_CAR_PROB:
            new_car_prob = aux_string_to_num<double>(opt.arg);
            break;

            case NEW_CAR_SPEED:
            new_car_speed = aux_string_to_num<int>(opt.arg);
            break;

            case AUT_DENSITY:
            aut_density = aux_string_to_num<double>(opt.arg);
            break;

            case OUT_FILE_NAME:
            out_file_name = opt.arg;
            break;

            case PATH:
            path = opt.arg;
            break;
        }
    }

    delete[] options;
    delete[] buffer;


    // Inicio de simulación
    RandomGen::SetAlgorithm(MT19937);
    RandomGen::Seed();
    CellularAutomata *cellularAutomata;

    // Carga el autómata celular
    switch (ca_type)
    {
        case CIRCULAR_CA:
            cout << "Creating circular CA" << endl;
            cellularAutomata = new CircularCA(size, density, vmax, rand_prob, init_vel);
            break;
        case OPEN_CA:
            cout << "Creating open CA" << endl;
            cellularAutomata = new OpenCA(size, density, vmax, rand_prob, init_vel, new_car_prob, new_car_speed);
            break;
        case AUTONOMOUS_CIRCULAR_CA:
            cout << "Creating autonomous circular CA" << endl;
            cellularAutomata = new AutonomousCircularCA(size, density, vmax, rand_prob, init_vel, aut_density);
            break;
        case AUTONOMOUS_OPEN_CA:
            cout << "Creating autonomous open CA" << endl;
            cellularAutomata = new AutonomousOpenCA(size, density, vmax, rand_prob, init_vel, aut_density, new_car_prob, new_car_speed);
            break;
        default:
            cout << "Creating circular CA" << endl;
            cellularAutomata = new CircularCA(size, density, vmax, rand_prob, init_vel);
            break;
    }

    // Itera
    cellularAutomata->Evolve(iterations);

    // Genera resultados
    if (!plot_traffic && !plot_flow)
        plot_traffic = true;

    if (plot_traffic)
    {
        cout << "Plotting traffic" << endl;
        cellularAutomata->DrawHistory(path, out_file_name);
    }
    if (plot_flow)
    {
        cout << "Plotting flow" << endl;
        cellularAutomata->DrawFlowHistory(path, out_file_name);
    }

    delete cellularAutomata;

    cout << "Done" << endl;

    return 0;
}
