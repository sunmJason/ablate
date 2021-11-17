#include "compressibleFlowSolver.hpp"
#include <finiteVolume/processes/evTransport.hpp>
#include "compressibleFlowFields.hpp"
#include "finiteVolume/processes/eulerTransport.hpp"
#include "finiteVolume/processes/speciesTransport.hpp"
#include "utilities/vectorUtilities.hpp"

ablate::finiteVolume::CompressibleFlowSolver::CompressibleFlowSolver(std::string solverId, std::shared_ptr<domain::Region> region, std::shared_ptr<parameters::Parameters> options,
                                                                     std::shared_ptr<eos::EOS> eosIn, std::shared_ptr<parameters::Parameters> parameters,
                                                                     std::shared_ptr<eos::transport::TransportModel> transport, std::shared_ptr<fluxCalculator::FluxCalculator> fluxCalculatorIn,
                                                                     std::vector<std::shared_ptr<processes::Process>> additionalProcesses,
                                                                     std::vector<std::shared_ptr<mathFunctions::FieldFunction>> initialization,
                                                                     std::vector<std::shared_ptr<boundaryConditions::BoundaryCondition>> boundaryConditions,
                                                                     std::vector<std::shared_ptr<mathFunctions::FieldFunction>> exactSolutions)
    : FiniteVolumeSolver(solverId, region, options,
                         utilities::VectorUtilities::Merge(
                             {
                                 // create assumed processes for compressible flow
                                 std::make_shared<ablate::finiteVolume::processes::EulerTransport>(parameters, eosIn, fluxCalculatorIn, transport),
                                 std::make_shared<ablate::finiteVolume::processes::SpeciesTransport>(eosIn, fluxCalculatorIn, transport),
                                 std::make_shared<ablate::finiteVolume::processes::EVTransport>(finiteVolume::CompressibleFlowFields::DENSITY_EV_FIELD, finiteVolume::CompressibleFlowFields::EV_FIELD,
                                                                                                eosIn, fluxCalculatorIn, transport),
                             },
                             additionalProcesses),
                         initialization, boundaryConditions, exactSolutions) {}

ablate::finiteVolume::CompressibleFlowSolver::CompressibleFlowSolver(std::string solverId, std::shared_ptr<domain::Region> region, std::shared_ptr<parameters::Parameters> options,
                                                                     std::shared_ptr<eos::EOS> eosIn, std::shared_ptr<parameters::Parameters> parameters,
                                                                     std::shared_ptr<eos::transport::TransportModel> transport, std::shared_ptr<fluxCalculator::FluxCalculator> fluxCalculatorIn,
                                                                     std::vector<std::shared_ptr<mathFunctions::FieldFunction>> initialization,
                                                                     std::vector<std::shared_ptr<boundaryConditions::BoundaryCondition>> boundaryConditions,
                                                                     std::vector<std::shared_ptr<mathFunctions::FieldFunction>> exactSolutions)
    : CompressibleFlowSolver(solverId, region, options, eosIn, parameters, transport, fluxCalculatorIn, {}, initialization, boundaryConditions, exactSolutions) {}

#include "parser/registrar.hpp"
REGISTER(ablate::solver::Solver, ablate::finiteVolume::CompressibleFlowSolver, "compressible finite volume flow", ARG(std::string, "id", "the name of the flow field"),
         OPT(domain::Region, "region", "the region to apply this solver.  Default is entire domain"), OPT(ablate::parameters::Parameters, "options", "the options passed to PETSc"),
         ARG(ablate::eos::EOS, "eos", "the equation of state used to describe the flow"), OPT(ablate::parameters::Parameters, "parameters", "the parameters used for field values"),
         OPT(ablate::eos::transport::TransportModel, "transport", "the diffusion transport model"),
         OPT(ablate::finiteVolume::fluxCalculator::FluxCalculator, "fluxCalculator", "the flux calculators (defaults to none)"),
         OPT(std::vector<ablate::finiteVolume::processes::Process>, "additionalProcesses", "any additional processes besides euler/yi/ev transport"),
         OPT(std::vector<mathFunctions::FieldFunction>, "initialization", "the flow field initialization"),
         OPT(std::vector<finiteVolume::boundaryConditions::BoundaryCondition>, "boundaryConditions", "the boundary conditions for the flow field"),
         OPT(std::vector<mathFunctions::FieldFunction>, "exactSolution", "optional exact solutions that can be used for error calculations"));