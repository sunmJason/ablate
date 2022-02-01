#ifndef ABLATELIBRARY_TCHEMREACTIONS_HPP
#define ABLATELIBRARY_TCHEMREACTIONS_HPP

#include <eos/tChem.hpp>
#include "process.hpp"

namespace ablate::finiteVolume::processes {

class TChemReactions : public Process {
   private:
    DM fieldDm;
    Vec sourceVec;

    // Petsc options specific to the chemTs. These may be null by default
    PetscOptions petscOptions;

    std::shared_ptr<eos::TChem> eos;
    const size_t numberSpecies;
    inline const static PetscReal dtInitDefault = 1E-5;
    PetscReal dtInit; /** this may be different than default if set with petsc options **/

    // Hold the single point TS
    TS ts;
    Vec pointData;
    Mat jacobian;
    /* Dense array workspace where Tchem computes the T, yi */
    double *tchemScratch;
    /* Dense array workspace where Tchem computes the Jacobian */
    double *jacobianScratch;
    /* Dense array for the local Jacobian rows */
    PetscInt *rows;

    // store any inert species
    std::vector<std::size_t> inertSpeciesIds;

    // Set a minimum/maximum value of mass fractions
    const std::vector<double> massFractionBounds;

    /**
     * Private function to integrate single point chemistry in time
     * @param ts
     * @param t
     * @param X
     * @param F
     * @param ptr
     * @return
     */
    static PetscErrorCode SinglePointChemistryRHS(TS ts, PetscReal t, Vec X, Vec F, void *ptr);

    /**
     * Private function to integrate single point chemistry in time by computing the jacobian
     * @param ts
     * @param t
     * @param X
     * @param F
     * @param ptr
     * @return
     */
    static PetscErrorCode SinglePointChemistryJacobian(TS ts, PetscReal t, Vec X, Mat aMat, Mat pMat, void *ptr);

    /**
     * private function to compute the energy and densityYi source terms over the next dt
     * @param flowTs
     * @param flow
     * @return
     */
    PetscErrorCode ChemistryFlowPreStage(TS flowTs, ablate::solver::Solver &flow, PetscReal stagetime);

    static PetscErrorCode AddChemistrySourceToFlow(const FiniteVolumeSolver &solver, DM dm, PetscReal time, Vec locX, Vec fVec, void *ctx);

   public:
    explicit TChemReactions(std::shared_ptr<eos::EOS> eos, std::shared_ptr<parameters::Parameters> options = {}, std::vector<std::string> inertSpecies = {},
                            std::vector<double> massFractionBounds = {});
    ~TChemReactions() override;
    /**
     * public function to link this process with the flow
     * @param flow
     */
    void Initialize(ablate::finiteVolume::FiniteVolumeSolver &flow) override;
};
}  // namespace ablate::finiteVolume::processes
#endif  // ABLATELIBRARY_TCHEMREACTIONS_HPP
