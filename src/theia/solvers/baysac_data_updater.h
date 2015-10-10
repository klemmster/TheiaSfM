#ifndef THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_
#define THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_

#include <cmath>
#include <algorithm>
#include <limits>
#include <type_traits>

#include "theia/solvers/data_updater.h"

namespace theia {
template <class Datum> class BaySacDataUpdater: public DataUpdater<Datum> {
    public:
        typedef typename std::vector<Datum> Data;

        virtual ~BaySacDataUpdater() {};

        virtual void UpdateData(Data &data,
                std::vector<std::reference_wrapper<Datum> >& modelSampleset,
                int num_iterations, double inlier_ratio,
                int min_num_samples) override {

            float max = data[0].probability;

            for(auto& match: data) {
                match.probability /= max;
            }

            double modelProb =
              1.0 - pow(1.0 - pow(inlier_ratio, min_num_samples),
                        num_iterations);

            VLOG(3) << "\n\n\n\n";
            VLOG(3) << "ModelProb: " << modelProb;
            VLOG(3) << "Data[0] probability: " << data[0].probability;
            VLOG(3) << "NUM Inliers: " << modelSampleset.size();

            /*
            inlier_ratio = std::max(inlier_ratio,
                    inlier_ratio + std::numeric_limits<float>::epsilon());
            */
            float probabilityMul = 1.0;
            // float probabilityMul = 1.0f;
            for(auto& inlier : modelSampleset) {
                probabilityMul *= inlier.get().probability;
            }
            probabilityMul *= std::max(modelProb, modelProb + std::numeric_limits<float>::epsilon());

            for(auto& inlier : modelSampleset) {
                float newProb = (inlier.get().probability - probabilityMul) /
                                      1.0f - probabilityMul;
                inlier.get().probability = newProb;
                VLOG(3) << "NewProb: " << inlier.get().probability;
            };

            std::sort(data.begin(), data.end(), [] (Datum& lhs, Datum& rhs) -> bool {
                return lhs.probability > rhs.probability;
            });

            // Normalize

            VLOG(3) << "Data[0] probability: " << data[0].probability;
            VLOG(3) << "Data[0] x, y: " << data[0].feature1.x() << " : " << data[0].feature1.y();
        };
    };
}  // namespace theia

#endif  // THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_
