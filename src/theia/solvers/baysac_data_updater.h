#ifndef THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_
#define THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_

#include <cmath>
#include <iostream>
#include <algorithm>
#include <limits>
#include <type_traits>

#include "theia/solvers/data_updater.h"

namespace theia {
template <class Datum>
class BaySacDataUpdater : public DataUpdater<Datum> {
   public:
    typedef typename std::vector<Datum> Data;

    virtual ~BaySacDataUpdater(){};

    virtual void UpdateData(
        Data& data, std::vector<std::reference_wrapper<Datum> >& modelSampleset,
        int num_iterations, double inlier_ratio, int min_num_samples) override {
        float sums = std::accumulate(
            std::begin(data), std::end(data), 0.0f,
            [](float a, const Datum& b) { return a + b.probability; });

        // Normalize probabilities
        for (auto& match : data) {
            match.probability /= sums;
        }

        sums = std::accumulate(
            data.begin(), data.end(), 0.0f,
            [](float a, const Datum& b) { return a + b.probability; });

        /*
        double modelProb =
            1.0 - pow(1.0 - pow(inlier_ratio, min_num_samples), num_iterations);
            */

        // Likelihood ≈ k/D
        // k = number inliers, D = Number of samples
        // k/D = inlier_ratio

        inlier_ratio = std::max(
            inlier_ratio, inlier_ratio + std::numeric_limits<float>::epsilon());

        for (auto& inlier : modelSampleset) {
            // An optimized BaySac Algorithm
            // P(i) = P(i)_{t-1} * k/D
            float newProb = inlier.get().probability * inlier_ratio;
            inlier.get().probability = newProb;
        };

        std::sort(data.begin(), data.end(), [](Datum& lhs, Datum& rhs) -> bool {
            return lhs.probability > rhs.probability;
        });

        // Normalize

        VLOG(3) << "Data[0] probability: " << data[0].probability;
        VLOG(3) << "Data[0] x, y: " << data[0].feature1.x() << " : "
                << data[0].feature1.y();
    };
};
}  // namespace theia

#endif  // THEIA_SOLVERS_BAYSAC_DATA_UPDATER_H_
