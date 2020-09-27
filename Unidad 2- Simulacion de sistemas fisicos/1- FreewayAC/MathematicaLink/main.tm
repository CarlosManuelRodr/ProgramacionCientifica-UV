:Begin:
:Function:       create_circular_ca
:Pattern:        CreateCircularCA[size_Integer, vmax_Integer, density_Real, randp_Real, initVel_Integer]
:Arguments:      { size, vmax, density, randp, initVel }
:ArgumentTypes:  { Integer, Integer, Real, Real, Integer }
:ReturnType:     Manual
:End:

:Begin:
:Function:       create_open_ca
:Pattern:        CreateOpenCA[size_Integer, vmax_Integer, density_Real, randp_Real, initVel_Integer, newCarProb_Real, newCarSpeed_Integer]
:Arguments:      { size, vmax, density, randp, initVel, newCarProb, newCarSpeed }
:ArgumentTypes:  { Integer, Integer, Real, Real, Integer, Real, Integer }
:ReturnType:     Manual
:End:

:Begin:
:Function:       create_autonomous_circular_ca
:Pattern:        CreateAutonomousCircularCA[size_Integer, vmax_Integer, density_Real, randp_Real, initVel_Integer, autDensity_Real]
:Arguments:      { size, vmax, density, randp, initVel, autDensity }
:ArgumentTypes:  { Integer, Integer, Real, Real, Integer, Real }
:ReturnType:     Manual
:End:

:Begin:
:Function:       create_autonomous_open_ca
:Pattern:        CreateAutonomousOpenCA[size_Integer, vmax_Integer, density_Real, randp_Real, initVel_Integer, autDensity_Real, newCarProb_Real, newCarSpeed_Integer]
:Arguments:      { size, vmax, density, randp, initVel, autDensity, newCarProb, newCarSpeed }
:ArgumentTypes:  { Integer, Integer, Real, Real, Integer, Real, Real, Integer }
:ReturnType:     Manual
:End:

:Begin:
:Function:       delete_ca
:Pattern:        DeleteCA[]
:Arguments:      Manual
:ArgumentTypes:  Manual
:ReturnType:     Manual
:End:

:Begin:
:Function:       ca_step
:Pattern:        CAStep[]
:Arguments:      Manual
:ArgumentTypes:  Manual
:ReturnType:     Manual
:End:

:Begin:
:Function:       ca_evolve
:Pattern:        CAEvolve[iterations_Integer]
:Arguments:      { iterations }
:ArgumentTypes:  { Integer }
:ReturnType:     Manual
:End:

:Begin:
:Function:       ca_count_cars
:Pattern:        CACountCars[]
:Arguments:      Manual
:ArgumentTypes:  Manual
:ReturnType:     Integer
:End:

:Begin:
:Function:       ca_calculate_ocupancy
:Pattern:        CACalculateOcupancy[]
:Arguments:      Manual
:ArgumentTypes:  Manual
:ReturnType:     Manual
:End:

:Begin:
:Function:       ca_calculate_flow
:Pattern:        CACalculateFlow[]
:Arguments:      Manual
:ArgumentTypes:  Manual
:ReturnType:     Manual
:End:


:Begin:
:Function:       ca_get_current
:Pattern:        CAGetCurrent[]
:Arguments:      Manual
:ArgumentTypes:  Manual
:ReturnType:     Manual
:End:

:Begin:
:Function:       ca_get_history
:Pattern:        CAGetHistory[]
:Arguments:      Manual
:ArgumentTypes:  Manual
:ReturnType:     Manual
:End:
