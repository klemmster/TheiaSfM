#ifndef THEIA_SOLVERS_DATA_UPDATER_H_
#define THEIA_SOLVERS_DATA_UPDATER_H_

#include "theia/solvers/estimator.h"

namespace theia {
    template <class Datum>
    class DataUpdater
    {
    public:

        typedef typename std::vector<Datum> Data;

        DataUpdater () {};
        virtual ~DataUpdater () {};

        virtual void UpdateData(Data &data, std::vector<size_t>& inlierSamplesetIndices,
                int num_iterations, double inlier_ratio,
                int min_num_samples) = 0;

    };
}  // namespace theia

#endif  // THEIA_SOLVERS_DATA_UPDATER_H_
