#ifndef THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_
#define THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_

#include <cmath>
#include <algorithm>

#include "theia/solvers/data_updater.h"

namespace theia {
template <class Datum> class BaySacDataUpdater: public DataUpdater<Datum> {
    public:
        typedef typename std::vector<Datum> Data;

        virtual ~BaySacDataUpdater() {};

        virtual void UpdateData(Data& data, std::vector<size_t>& inlierSamplesetIndices,
                int num_iterations, double inlier_ratio,
                int min_num_samples) override {

            /*
            double modelProb =
              1.0 - pow(1.0 - pow(inlier_ratio, min_num_samples),
                        num_iterations);

            VLOG(3) << "\n\n";
            VLOG(3) << "ModelProb" << modelProb;
            VLOG(3) << "Data[0] probability: " << data[0].probability;

            for(size_t i=0; i< inlierSampleset.size(); i++) {
                Datum& sample = inlierSampleset[i];
                sample.probability = (sample.probability - modelProb) /
                                       ( 1.0 - modelProb);
            };


            std::sort(data.begin(), data.end(), [] (Datum& lhs, Datum& rhs) -> bool {
                return lhs.probability > rhs.probability;
            });
            */

            VLOG(3) << "Data[0] probability: " << data[0].probability;
        };
    };
}  // namespace theia

#endif  // THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_
