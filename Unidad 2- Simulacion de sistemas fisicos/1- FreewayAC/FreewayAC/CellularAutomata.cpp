#include "CellularAutomata.h"
#include "BmpWriter.h"

#include <algorithm>
#include <vector>
using namespace std;


/****************************
*                           *
*         AC Básico         *
*                           *
****************************/

CellularAutomata::CellularAutomata(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob, const CaVelocity init_vel)
{
    // Inicializa variables.
    m_test = false;
    m_size = size;
    m_vmax = vmax;
    m_rand_prob = rand_prob;
    m_init_vel = init_vel;
    m_ca.assign(size, CA_EMPTY);
    m_ca_temp.assign(size, CA_EMPTY);
    m_ca_flow_temp.assign(size, NO_FLOW);
    m_ca_history.clear();
    m_ca_flow_history.clear();
 
    unsigned vehicles = (unsigned)(((double)size)*density);

    // Coloca autos al azar.
    vector<unsigned> car_positions;
    for (unsigned i = 0; i < m_size; ++i)
        car_positions.push_back(i);

    random_shuffle(car_positions.begin(), car_positions.end(), RandomGen::GetInt);
    for (unsigned i = 0; i < vehicles; ++i)
        m_ca[car_positions[i]] = m_init_vel;
}
CellularAutomata::CellularAutomata(const vector<int> &ca, const vector<bool> &rand_values, const CaVelocity vmax)
{
    // Inicializa variables.
    m_test = true;
    m_ca = ca;
    m_rand_values = rand_values;
    m_size = m_ca.size();
    m_vmax = vmax;
    m_rand_prob = 0;
    m_ca_temp.assign(m_size, CA_EMPTY);
    m_ca_history.clear();
    m_ca_flow_temp.assign(m_size, NO_FLOW);
    m_ca_history.push_back(m_ca);
    m_init_vel = 1;
}
CellularAutomata::~CellularAutomata() {}
void CellularAutomata::DrawHistory(string path, string out_file_name) const
{
    if (out_file_name == "")
        out_file_name = path + "ca.bmp";
    else
        out_file_name = path + out_file_name;

    unsigned height = m_ca_history.size();
    unsigned width = m_size;
    BMPWriter writer(out_file_name.c_str(), width, height);
    if (writer.IsOpen())
    {
        BMPPixel* bmpData = new BMPPixel[width];
        for (int i = height-1; i >= 0; --i)     // Los archivos BMP se escriben de abajo a arriba.
        {
            for (unsigned j = 0; j < width; ++j)
            {
                BMPPixel color;
                if (m_ca_history[i][j] == CA_EMPTY)
                    color = BMPPixel((char)255, (char)255, (char)255);
                else
                    color = BMPPixel(0, 0, (char)(255.0*(double)m_ca_history[i][j]/(double)m_vmax));
                bmpData[j] = color;
            }
            writer.WriteLine(bmpData);
        }
        writer.CloseBMP();
        delete[] bmpData;
    }
}
void CellularAutomata::DrawFlowHistory(string path, string out_file_name) const
{
    if (out_file_name == "")
        out_file_name = path + "ca_flow.bmp";
    else
        out_file_name = path + out_file_name;

    unsigned height = m_ca_flow_history.size();
    unsigned width = m_size;
    BMPWriter writer(out_file_name.c_str(), width, height);
    if (writer.IsOpen())
    {
        BMPPixel* bmpData = new BMPPixel[width];
        for (int i = height-1; i >= 0; --i)
        {
            for (unsigned j=0; j<width; ++j)
            {
                BMPPixel color;
                if (m_ca_flow_history[i][j] == 0)
                    color = BMPPixel((char)255, (char)255, (char)255);
                else
                    color = BMPPixel(0, 0, (char)(255.0*(double)m_ca_flow_history[i][j]/(double)m_vmax));
                bmpData[j] = color;
            }
            writer.WriteLine(bmpData);
        }
        writer.CloseBMP();
        delete[] bmpData;
    }
}
inline void CellularAutomata::Step() noexcept
{
    // Iterar sobre AC hasta encotrar vehiculo.
    for (unsigned i = 0; i < m_ca.size(); ++i)
    {
        if (m_ca[i] != CA_EMPTY)
        {
            // Aceleracion.
            if ((m_ca[i] < m_vmax) && (NextCarDist(i) > (CaSize)(m_ca[i] + 1)))
                m_ca[i]++;
            else
            {
                // Frenado.
                if (m_ca[i] > 0)
                {
                    int nd = NextCarDist(i);
                    if (nd <= m_ca[i])
                        m_ca[i] = nd - 1;
                }
            }

            // Aleatoriedad.
            bool rnd = Randomization();
            if ((m_ca[i] > 0) && rnd)
                m_ca[i]--;
        }
    }

    // Aplicar cambios.
    m_ca_history.push_back(m_ca);
    Move();
}
inline void CellularAutomata::Move() noexcept
{
    for (unsigned i = 0; i < m_ca.size(); ++i)
    {
        // Cambia las posiciones de los autos en AC.
        if (m_ca[i] != CA_EMPTY)
        {
            AtTemp(i + m_ca[i]) = m_ca[i];

            // Marca las casillas donde hay flujo de autos.
            for (unsigned j = i; j < i + m_ca[i]; ++j)
                AtFlowTemp(j) = IS_FLOW;
        }
    }

    AssignChanges();
}
inline void CellularAutomata::AssignChanges() noexcept
{
    m_ca_flow_history.push_back(m_ca_flow_temp);
    m_ca.assign(m_ca_temp.begin(), m_ca_temp.end());
    m_ca_flow_temp.assign(m_size, NO_FLOW);
    m_ca_temp.assign(m_size, CA_EMPTY);
}
inline CaSize CellularAutomata::NextCarDist(const CaPosition pos) const noexcept
{
    CaSize dist = 1;
    while ((GetAt(pos + dist) == CA_EMPTY) && (dist < 2 * m_size))
        dist++;
    return dist;
}
std::vector<CaVelocity> CellularAutomata::GetCa()
{
    return m_ca;
}
std::vector< std::vector<CaVelocity> > CellularAutomata::GetCaHistory()
{
    return m_ca_history;
}
void CellularAutomata::Evolve(const unsigned iter) noexcept
{
    for (unsigned i = 0; i < iter; ++i)
        Step();
}
CaSize CellularAutomata::GetSize() const noexcept
{
    return m_size;
}
CaSize CellularAutomata::GetHistorySize() const noexcept
{
    return m_ca_history.size();
}
unsigned CellularAutomata::CountCars() const noexcept
{
    return count_if(m_ca.begin(), m_ca.end(), [](CaVelocity i) {return i != CA_EMPTY; });
}
bool CellularAutomata::Randomization(const double prob) noexcept
{
    double l_prob;
    if (prob < 0)
        l_prob = m_rand_prob;
    else
        l_prob = prob;

    // Si está en modo de prueba toma los valores aleatorios de la lista.
    if (m_test)
    {
        if (m_rand_values.size() != 0)
        {
            bool ret = m_rand_values[0];
            m_rand_values.erase(m_rand_values.begin());
            return ret;
        }
        else
            return false;
    }
    else
    {
        if (RandomGen::GetDouble() <= l_prob)
            return true;
        else
            return false;
    }
}
vector<double> CellularAutomata::CalculateOcupancy() const noexcept
{
    vector<double> ocupancy;
    ocupancy.assign(this->GetSize(), 0.0);
    unsigned height = this->GetHistorySize();
    unsigned width = this->GetSize();

    for (unsigned i = 0; i < width; ++i)
    {
        int sum = 0;
        for (unsigned j = 1; j < height; ++j)
        {
            if (m_ca_history[j][i] != CA_EMPTY)
                sum++;
        }
        ocupancy[i] = (double)sum/(double)height;
    }
    return ocupancy;
}
vector<double> CellularAutomata::CalculateFlow() const noexcept
{
    vector<double> flow;
    flow.assign(this->GetSize(), 0.0);
    unsigned height = this->GetHistorySize();
    unsigned width = this->GetSize();

    for (unsigned i = 0; i < width - 1; ++i)
    {
        int sum = 0;
        for (unsigned j = 1; j < height; ++j)
        {
            if ((m_ca_flow_history[j][i] != NO_FLOW) && (m_ca_flow_history[j][i + 1] != NO_FLOW))
                sum++;
        }
        flow[i] = (double)sum/(double)height;
    }
    return flow;
}
double CellularAutomata::CalculateMeanFlow() const noexcept
{
    return aux_mean(this->CalculateFlow());
}


/****************************
*                           *
*        AC Circular        *
*                           *
****************************/

CircularCA::CircularCA(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob, const CaVelocity init_vel)
    : CellularAutomata(size, density, vmax, rand_prob, init_vel) {}
CircularCA::CircularCA(const vector<int> &ca, const vector<bool> &rand_values, const CaVelocity vmax)
    : CellularAutomata(ca, rand_values, vmax) {}
inline CaVelocity &CircularCA::At(const CaPosition i) noexcept
{
    return m_ca[i % m_ca.size()];
}
inline CaVelocity &CircularCA::AtTemp(const CaPosition i) noexcept
{
    return m_ca_temp[i % m_ca.size()];
}
inline CaFlow &CircularCA::AtFlowTemp(const CaPosition i) noexcept
{
    return m_ca_flow_temp[i % m_ca.size()];
}
inline CaVelocity CircularCA::GetAt(const CaPosition i) const noexcept
{
    return m_ca[i % m_ca.size()];
}
void CircularCA::Evolve(const unsigned iter) noexcept
{
    unsigned cars = CountCars();
    for (unsigned i = 0; i < iter; ++i)
        Step();

    if (cars != CountCars())
        cout << "Error: La cantidad de autos no se conserva." << endl;
}


/****************************
*                           *
*        AC Abierto         *
*                           *
****************************/

OpenCA::OpenCA(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob, const CaVelocity init_vel,
               const double new_car_prob, const CaVelocity new_car_speed)
    : CellularAutomata(size, density, vmax, rand_prob, init_vel)
{
    m_new_car_prob = new_car_prob;
    m_new_car_speed = new_car_speed;
    m_ca_empty = CA_EMPTY;
    m_ca_flow_empty = NO_FLOW;
}
OpenCA::OpenCA(const vector<int> &ca, const vector<bool> &rand_values, const CaVelocity vmax,
               const CaVelocity new_car_speed)
    : CellularAutomata(ca, rand_values, vmax)
{
    m_new_car_prob = -1.0;
    m_new_car_speed = new_car_speed;
    m_ca_empty = CA_EMPTY;
    m_ca_flow_empty = NO_FLOW;
}
inline CaVelocity &OpenCA::At(const CaPosition i) noexcept
{
    return ((unsigned)i >= m_ca.size()) ? m_ca_empty : m_ca[i];
}
inline CaVelocity &OpenCA::AtTemp(const CaPosition i) noexcept
{
    return ((unsigned)i >= m_ca.size()) ? m_ca_empty : m_ca_temp[i];
}
inline CaFlow &OpenCA::AtFlowTemp(const CaPosition i) noexcept
{
    return ((unsigned)i >= m_ca.size()) ? m_ca_flow_empty : m_ca_flow_temp[i];
}
inline CaVelocity OpenCA::GetAt(const CaPosition i) const noexcept
{
    return ((unsigned)i >= m_ca.size()) ? CA_EMPTY : m_ca[i];
}
void OpenCA::Step() noexcept
{
    // Iterar sobre AC hasta encotrar vehiculo.
    for (unsigned i = 0; i < m_ca.size(); ++i)
    {
        if (m_ca[i] != CA_EMPTY)
        {
            // Aceleracion.
            if ((m_ca[i] < m_vmax) && (NextCarDist(i) > (CaSize)(m_ca[i] + 1)))
                m_ca[i]++;
            else
            {
                // Frenado.
                if (m_ca[i] > 0)
                {
                    int nd = NextCarDist(i);
                    if (nd <= m_ca[i])
                        m_ca[i] = nd - 1;
                }
            }

            // Aleatoriedad.
            bool rnd = Randomization();
            if ((m_ca[i] > 0) && rnd)
                m_ca[i]--;
        }
    }

    m_ca_history.push_back(m_ca);

    // Aplicar cambios.
    Move();

    // Añade coche con probabilidad aleatoria.
    if (m_ca[0] == CA_EMPTY && Randomization(m_new_car_prob))
        m_ca[0] = m_new_car_speed;
}


/****************************
*                           *
*  AC Coches inteligentes   *
*                           *
****************************/

// Frontera circular
AutonomousCircularCA::AutonomousCircularCA(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob,
                           const CaVelocity init_vel, const double aut_density)
    : CircularCA(size, density, vmax, rand_prob, init_vel)
{
    // Selecciona autos inteligentes.
    unsigned aut_car_number = (unsigned)(((double)size*density)*aut_density);
    vector<int> aut_car_positions;
    for (unsigned i = 0; i < m_size; ++i)
    {
        if (m_ca[i] != CA_EMPTY)
            aut_car_positions.push_back(i);
    }

    random_shuffle(aut_car_positions.begin(), aut_car_positions.end(), RandomGen::GetInt);
    for (unsigned i = 0; i < aut_car_positions.size() && i < aut_car_number; ++i)
        m_aut_cars.push_back(aut_car_positions[i]);
}
void AutonomousCircularCA::Move() noexcept
{
    for (unsigned i = 0; i < m_ca.size(); ++i)
    {
        if (m_ca[i] != CA_EMPTY)
        {
            int pos = aux_find_pos<int>(m_aut_cars, i);
            if (pos != -1)
                m_aut_cars[pos] = (i + m_ca[i]) % m_size;

            // Cambia las posiciones de los autos en AC.
            AtTemp(i + m_ca[i]) = m_ca[i];

            // Marca las casillas donde hay flujo de autos.
            for (unsigned j = i; j < i + m_ca[i]; ++j)
                AtFlowTemp(j) = IS_FLOW;
        }
    }
    AssignChanges();
}
void AutonomousCircularCA::Step() noexcept
{
    // Iterar sobre AC hasta encotrar vehiculo.
    for (unsigned i = 0; i < m_ca.size(); ++i)
    {
        if (m_ca[i] != CA_EMPTY)
        {
            bool smart = aux_is_in<int>(m_aut_cars, i);
            if (smart)
            {
                // Auto inteligente.
                int nc = i + NextCarDist(i);
                nc %= m_size;
                if ((m_ca[nc] < m_vmax) && (NextCarDist(nc) > (CaSize)(m_ca[nc] + 1)) && (NextCarDist(i) <= (CaSize)m_ca[i]))
                {
                    if ((m_ca[i] < m_vmax) && (NextCarDist(i) > (CaSize)(m_ca[i] + 1)))
                        m_ca[i]++;
                    else
                    {
                        // Frenado.
                        if (m_ca[i] > 0)
                        {
                            int nd = NextCarDist(i);
                            if (m_ca[nc] >= m_ca[i])
                            {
                                if (nd <= m_ca[i]-1)
                                    m_ca[i] = nd - 1;
                            }
                            else
                            {
                                if (nd <= m_ca[i])
                                    m_ca[i] = nd - 1;
                            }
                        }
                    }
                }
                else
                {
                    // Aceleracion.
                    if ((m_ca[i] < m_vmax) && (NextCarDist(i) > (CaSize)(m_ca[i] + 1)))
                        m_ca[i]++;
                    else
                    {
                        // Frenado.
                        if (m_ca[i] > 0)
                        {
                            int nd = NextCarDist(i);
                            if (nd <= m_ca[i])
                                m_ca[i] = nd - 1;
                        }
                    }
                }
            }
            else
            {
                // Aceleracion.
                if ((m_ca[i] < m_vmax) && (NextCarDist(i) > (CaSize)(m_ca[i] + 1)))
                    m_ca[i]++;
                else
                {
                    // Frenado.
                    if (m_ca[i] > 0)
                    {
                        int nd = NextCarDist(i);
                        if (nd <= m_ca[i])
                            m_ca[i] = nd - 1;
                    }
                }
            }

            // Aleatoriedad.
            if (!smart)
            {
                bool rnd = Randomization();
                if ((m_ca[i] > 0) && rnd)
                    m_ca[i]--;
            }
        }
    }

    // Aplicar cambios.
    m_ca_history.push_back(m_ca);
    Move();
}

// Frontera abierta
AutonomousOpenCA::AutonomousOpenCA(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob,
                           const CaVelocity init_vel, const double aut_density, const double new_car_prob, const CaVelocity new_car_speed)
    : OpenCA(size, density, vmax, rand_prob, init_vel, new_car_prob, new_car_speed)
{
    // Selecciona autos inteligentes.
    unsigned aut_car_number = (unsigned)(((double)size*density)*aut_density);
    vector<int> aut_car_positions;
    for (unsigned i = 0; i < m_size; ++i)
    {
        if (m_ca[i] != CA_EMPTY)
            aut_car_positions.push_back(i);
    }

    random_shuffle(aut_car_positions.begin(), aut_car_positions.end(), RandomGen::GetInt);
    for (unsigned i = 0; i < aut_car_positions.size() && i < aut_car_number; ++i)
        m_aut_cars.push_back(aut_car_positions[i]);
}
void AutonomousOpenCA::Move() noexcept
{
    for (unsigned i = 0; i < m_ca.size(); ++i)
    {
        if (m_ca[i] != CA_EMPTY)
        {
            int pos = aux_find_pos<int>(m_aut_cars, i);
            if (pos != -1)
                m_aut_cars[pos] = (i + m_ca[i]) % m_size;

            // Cambia las posiciones de los autos en AC.
            AtTemp(i + m_ca[i]) = m_ca[i];

            // Marca las casillas donde hay flujo de autos.
            for (unsigned j = i; j < i + m_ca[i]; ++j)
                AtFlowTemp(j) = IS_FLOW;
        }
    }
    AssignChanges();
}
void AutonomousOpenCA::Step() noexcept
{
    // Iterar sobre AC hasta encontrar vehiculo.
    for (unsigned i = 0; i < m_ca.size(); ++i)
    {
        if (m_ca[i] != CA_EMPTY)
        {
            bool smart = aux_is_in<int>(m_aut_cars, i);
            if (smart)
            {
                // Auto inteligente.
                int nc = i + NextCarDist(i);
                nc %= m_size;
                if ((m_ca[nc] < m_vmax) && (NextCarDist(nc) > (CaSize)(m_ca[nc] + 1)) && (NextCarDist(i) <= (CaSize)m_ca[i]))
                {
                    if ((m_ca[i] < m_vmax) && (NextCarDist(i) > (CaSize)(m_ca[i] + 1)))
                        m_ca[i]++;
                    else
                    {
                        // Frenado.
                        if (m_ca[i] > 0)
                        {
                            int nd = NextCarDist(i);
                            if (m_ca[nc] >= m_ca[i])
                            {
                                if (nd <= m_ca[i]-1)
                                    m_ca[i] = nd - 1;
                            }
                            else
                            {
                                if (nd <= m_ca[i])
                                    m_ca[i] = nd - 1;
                            }
                        }
                    }
                }
                else
                {
                    // Aceleracion.
                    if ((m_ca[i] < m_vmax) && (NextCarDist(i) > (CaSize)(m_ca[i] + 1)))
                        m_ca[i]++;
                    else
                    {
                        // Frenado.
                        if (m_ca[i] > 0)
                        {
                            int nd = NextCarDist(i);
                            if (nd <= m_ca[i])
                                m_ca[i] = nd - 1;
                        }
                    }
                }
            }
            else
            {
                // Aceleracion.
                if ((m_ca[i] < m_vmax) && (NextCarDist(i) > (CaSize)(m_ca[i] + 1)))
                    m_ca[i]++;
                else
                {
                    // Frenado.
                    if (m_ca[i] > 0)
                    {
                        int nd = NextCarDist(i);
                        if (nd <= m_ca[i])
                            m_ca[i] = nd - 1;
                    }
                }
            }

            // Aleatoriedad.
            if (!smart)
            {
                bool rnd = Randomization();
                if ((m_ca[i] > 0) && rnd)
                    m_ca[i]--;
            }
        }
    }

    // Añade coche con probabilidad aleatoria.
    if (m_ca[0] == CA_EMPTY && Randomization(m_new_car_prob))
        m_ca[0] = m_new_car_speed;

    // Aplicar cambios.
    m_ca_history.push_back(m_ca);
    Move();
}