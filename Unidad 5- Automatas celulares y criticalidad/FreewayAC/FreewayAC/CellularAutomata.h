/**
* @file CellularAutomata.h
* @brief Clases de autómatas celulares.
* @author Carlos Manuel Rodríguez Martínez
* @date 18/05/2015
*/

#ifndef _CELLULARAUTOMATA
#define _CELLULARAUTOMATA

#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>

#include "Auxiliar.h"

enum CA_TYPE
{
    CIRCULAR_CA, OPEN_CA, AUTONOMOUS_CIRCULAR_CA, AUTONOMOUS_OPEN_CA
};


/****************************
*                           *
*         AC Básico         *
*                           *
****************************/

/**
* @enum CAS
* @brief Enumera los tipos de listas de AC.
*
* Se usan diferentes tipos de listas para guardar información del autómata celular.
*/
enum CAS
{
    CA, CA_TEMP, CA_FLOW_TEMP, CA_HISTORY, CA_FLOW_HISTORY
};


using CaSize = unsigned;
using CaLane = unsigned;
using CaPosition = int;
using CaVelocity = int;
using CaFlow = char;

const CaVelocity CA_EMPTY = -1;
const CaPosition CA_NULL_POS = -1;
const CaFlow NO_FLOW = 0;
const CaFlow IS_FLOW = 1;

/**
 * @class CellularAutomata
 * @brief Clase base para autómata celular.
 * Esta clase implementa los métodos básicos que todos los autómatas celulares de tráfico poseen.
 * Las condiciones de frontera del autómata se reflejan en los métodos At que se necesitan
 * definir en las clases hijas.
 */
class CellularAutomata
{
protected:
    bool m_test;                 ///< Modo de prueba.
    double m_rand_prob;          ///< Valor de la probabilidad de descenso de velocidad.
    CaVelocity m_vmax;           ///< Valor máximo de la velocidad.
    CaVelocity m_init_vel;       ///< Velocidad inicial de los autos.
    CaSize m_size;               ///< Tamaño del autómata celular
    std::vector<CaVelocity> m_ca;       ///< Automata celular. -1 para casillas sin auto, y valores >= 0 indican velocidad del auto en esa casilla.
    std::vector<CaVelocity> m_ca_temp;
    std::vector<CaFlow> m_ca_flow_temp;                         ///< Variable temporal para operaciones con AC.
    std::vector< std::vector<CaVelocity> > m_ca_history;
    std::vector< std::vector<CaFlow> > m_ca_flow_history;       ///< Lista con valores históricos de AC.
    std::vector<bool> m_rand_values;                            ///< Lista con valores aleatorios para usar en modo de prueba.

public:
    ///@brief Constructor.
    ///@param size Tamaño del AC.
    ///@param density Densidad de autos.
    ///@param vmax Velocidad máxima de los autos.
    ///@param rand_prob Probabilidad de descenso de velocidad.
    CellularAutomata(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob, const CaVelocity init_vel);

    ///@brief Constructor.
    ///@param ca Lista con valores de AC.
    ///@param rand_values Valores aleatorios en cada paso.
    ///@param vmax Velocidad máxima de los autos.
    CellularAutomata(const std::vector<int> &ca, const std::vector<bool> &rand_values, const CaVelocity vmax);

    virtual ~CellularAutomata();

    ///@brief Dibuja mapa histórico del AC en formato BMP.
	///@param path Ruta del archivo.
	///@param out_file_name Nombre del archivo de salida.
	virtual void DrawHistory(std::string path = "", std::string out_file_name = "") const;

	///@brief Dibuja mapa histórico del flujo de AC en formato BMP.
	///@param path Ruta del archivo.
	///@param out_file_name Nombre del archivo de salida.
	virtual void DrawFlowHistory(std::string path = "", std::string out_file_name = "") const;

    ///@brief Evoluciona (itera) el AC.
    ///@param iter Número de iteraciones.
    virtual void Evolve(const unsigned iter) noexcept;

    ///@brief Devuelve la distancia al auto más próximo desde la posición pos.
    ///@param pos Posición desde dónde iniciar la búsqueda.
    CaSize NextCarDist(const CaPosition pos) const noexcept;

    ///@brief Devuelve valores verdaderos con probabilidad prob. Si se usa en prueba usa valores de lista.
    ///@param prob Probabilidad de obtener valor verdadero. Por defecto se utiliza m_rand_prob.
    bool Randomization(const double prob = -1.0) noexcept;

    ///@brief Devuelve referencia a elemento del AC considerando las condiciones de frontera.
    ///@param i Posición dentro del AC.
    virtual CaVelocity &At(const CaPosition i) noexcept = 0;
    virtual CaVelocity &AtTemp(const CaPosition i) noexcept = 0;
    virtual CaFlow &AtFlowTemp(const CaPosition i) noexcept = 0;
    virtual CaVelocity GetAt(const CaPosition i) const noexcept = 0;

    std::vector<CaVelocity> GetCa();
    std::vector< std::vector<CaVelocity> > GetCaHistory();
    
    virtual std::vector<double> CalculateOcupancy() const noexcept;
    virtual std::vector<double> CalculateFlow() const noexcept;
    virtual double CalculateMeanFlow() const noexcept;

    CaSize GetSize() const noexcept;             ///< Devuelve tamaño del AC.
    CaSize GetHistorySize() const noexcept;      ///< Devuelve tamaño de la lista histórica de evolución del AC.
    unsigned CountCars() const noexcept;           ///< Cuenta la cantidad de autos en AC.
    virtual void Step() noexcept;            ///< Aplica reglas de evolución temporal del AC.
    virtual void Move() noexcept;            ///< Mueve los autos según las condiciones de frontera especificadas en clase hija.
    void AssignChanges() noexcept;           ///< Asigna cambios de los arrays teporales al array m_ca e historico.
};


/****************************
*                           *
*        AC Circular        *
*                           *
****************************/

/**
 * @class CircularCA
 * @brief AC con condiciones de frontera periódicas.
 */
class CircularCA : public CellularAutomata
{
public:
    ///@brief Constructor.
    ///@param size Tamaño del AC.
    ///@param density Densidad de autos.
    ///@param vmax Velocidad máxima de los autos.
    ///@param rand_prob Probabilidad de descenso de velocidad.
    CircularCA(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob, const CaVelocity init_vel);

    ///@brief Constructor.
    ///@param ca Lista con valores de AC.
    ///@param rand_values Valores aleatorios en cada paso.
    ///@param density Densidad de autos.
    ///@param vmax Velocidad máxima de los autos.
    CircularCA(const std::vector<int> &ca, const std::vector<bool> &rand_values, const CaVelocity vmax);

    ///@brief Devuelve elemento de valores del autómata celular considerando las condiciones de frontera.
    ///@param i Posición dentro del AC.
    CaVelocity &At(const CaPosition i) noexcept;
    CaVelocity &AtTemp(const CaPosition i) noexcept;
    CaFlow &AtFlowTemp(const CaPosition i) noexcept;
    CaVelocity GetAt(const CaPosition i) const noexcept;

    ///@brief Evoluciona (itera) el AC. Verifica si se conserva la cantidad de autos.
    ///@param iter Número de iteraciones.
    void Evolve(const unsigned iter) noexcept;
};


/****************************
*                           *
*        AC Abierto         *
*                           *
****************************/

/**
 * @class OpenCA
 * @brief AC con condiciones de frontera abiertas.
 */
class OpenCA : public CellularAutomata
{
protected:
    CaVelocity m_ca_empty;         ///< Se usa para devolver referencia de lugar vacío.
    CaFlow m_ca_flow_empty;
    double m_new_car_prob;         ///< Probabilidad de que aparezca un nuevo auto en la posición 0 del AC en la siguiente iteración.
    CaVelocity m_new_car_speed;    ///< Velocidad de nuevo auto cuando ingresa a la pista.
public:
    ///@brief Constructor.
    ///@param size Tamaño del AC.
    ///@param density Densidad de autos.
    ///@param vmax Velocidad máxima de los autos.
    ///@param rand_prob Probabilidad de descenso de velocidad.
    ///@param new_car_prob Probabilidad de que aparezca un nuevo auto en la posición 0 del AC en la siguiente iteración.
    ///@param new_car_speed Velocidad de nuevo auto cuando ingresa a la pista.
    OpenCA(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob, const CaVelocity init_vel,
           const double new_car_prob, const CaVelocity new_car_speed);

    ///@brief Constructor.
    ///@param ca Lista con valores de AC.
    ///@param rand_values Valores aleatorios en cada paso.
    ///@param density Densidad de autos.
    ///@param vmax Velocidad máxima de los autos.
    ///@param new_car_speed Velocidad de nuevo auto cuando ingresa a la pista.
    OpenCA(const std::vector<int> &ca, const std::vector<bool> &rand_values, const CaVelocity vmax, const CaVelocity new_car_speed);

    ///@brief Devuelve elemento de valores del autómata celular considerando las condiciones de frontera.
    ///@param i Posición dentro del AC.
    CaVelocity &At(const CaPosition i) noexcept;
    CaVelocity &AtTemp(const CaPosition i) noexcept;
    CaFlow &AtFlowTemp(const CaPosition i) noexcept;
    CaVelocity GetAt(const CaPosition i) const noexcept;

    void Step() noexcept;    ///< Aplica reglas de evolución temporal del AC.
};


/****************************
*                           *
*    AC Coches autónomos    *
*                           *
****************************/

/**
 * @class AutonomousCircularCA
 * @brief AC con autos autónomos y condiciones de frontera periódicas.
 */
class AutonomousCircularCA : public CircularCA
{
protected:
    std::vector<int> m_aut_cars;  ///< Lista con posiciones de autos autónomos.
public:
    ///@brief Constructor.
    ///@param size Tamaño del AC.
    ///@param density Densidad de autos.
    ///@param vmax Velocidad máxima de los autos.
    ///@param rand_prob Probabilidad de descenso de velocidad.
    ///@param aut_density Densidad de autos autónomos respecto a número total de autos.
    AutonomousCircularCA(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob,
                 const CaVelocity init_vel, const double aut_density);

    void Move() noexcept;    ///< Mueve los autos con condiciones de frontera periódicas.
    virtual void Step() noexcept;    ///< Aplica reglas de evolución temporal del AC para autos normales e inteligentes.
};


/**
 * @class AutonomousOpenCA
 * @brief AC con autos autónomos y condiciones de frontera abiertas.
 */
class AutonomousOpenCA : public OpenCA
{
protected:
    std::vector<int> m_aut_cars;  ///< Lista con posiciones de autos autónomos.
public:
    ///@brief Constructor.
    ///@param size Tamaño del AC.
    ///@param density Densidad de autos.
    ///@param vmax Velocidad máxima de los autos.
    ///@param rand_prob Probabilidad de descenso de velocidad.
    ///@param aut_density Densidad de autos autónomos respecto a número total de autos.
    ///@param new_car_prob Probabilidad de que aparezca un nuevo auto en la posición 0 del AC en la siguiente iteración.
    ///@param new_car_speed Velocidad de nuevo auto cuando ingresa a la pista.
    AutonomousOpenCA(const CaSize size, const double density, const CaVelocity vmax, const double rand_prob,
                 const CaVelocity init_vel, const double aut_density, const double new_car_prob, const CaVelocity new_car_speed);


    void Move() noexcept;    ///< Mueve los autos con condiciones de frontera periódicas.
    virtual void Step() noexcept;    ///< Aplica reglas de evolución temporal del AC para autos normales e inteligentes.
};

#endif
