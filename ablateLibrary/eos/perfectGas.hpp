#ifndef ABLATELIBRARY_PERFECTGAS_HPP
#define ABLATELIBRARY_PERFECTGAS_HPP
#include <memory>
#include "eos.hpp"
#include "parameters/parameters.hpp"
namespace ablate::eos {

class PerfectGas : public EOS {
   private:
    // the perfect gas does not allow species
    const std::vector<std::string> species;
    struct Parameters {
        PetscReal gamma;
        PetscReal rGas;
        PetscInt numberSpecies;
    };
    Parameters parameters;

    static PetscErrorCode PerfectGasDecodeState(PetscInt dim, PetscReal density, PetscReal totalEnergy, const PetscReal* velocity, const PetscReal densityYi[], PetscReal* internalEnergy, PetscReal* a,
                                                PetscReal* p, void* ctx);
    static PetscErrorCode PerfectGasComputeTemperature(PetscInt dim, PetscReal density, PetscReal totalEnergy, const PetscReal* massFlux, const PetscReal densityYi[], PetscReal* T, void* ctx);

    static PetscErrorCode PerfectGasComputeSpeciesSensibleEnthalpy(PetscReal T, PetscReal* hi, void* ctx);

   public:
    explicit PerfectGas(std::shared_ptr<ablate::parameters::Parameters>, std::vector<std::string> species = {});
    void View(std::ostream& stream) const override;
    DecodeStateFunction GetDecodeStateFunction() override { return PerfectGasDecodeState; }
    void* GetDecodeStateContext() override { return &parameters; }
    ComputeTemperatureFunction GetComputeTemperatureFunction() override { return PerfectGasComputeTemperature; }
    void* GetComputeTemperatureContext() override { return &parameters; }
    ComputeSpeciesSensibleEnthalpyFunction GetComputeSpeciesSensibleEnthalpyFunction() override { return PerfectGasComputeSpeciesSensibleEnthalpy; }
    void* GetComputeSpeciesSensibleEnthalpyContext() override { return &parameters; }

    const std::vector<std::string>& GetSpecies() const override { return species; }
};

}  // namespace ablate::eos
#endif  // ABLATELIBRARY_PERFECTGAS_HPP
