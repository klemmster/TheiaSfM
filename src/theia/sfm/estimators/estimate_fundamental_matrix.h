// Copyright (C) 2014 The Regents of the University of California (Regents).
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//
//     * Neither the name of The Regents or University of California nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// Please contact the author of this library if you have any questions.
// Author: Chris Sweeney (cmsweeney@cs.ucsb.edu)

#ifndef THEIA_SFM_ESTIMATORS_ESTIMATE_FUNDAMENTAL_MATRIX_H_
#define THEIA_SFM_ESTIMATORS_ESTIMATE_FUNDAMENTAL_MATRIX_H_

#include <Eigen/Core>
#include <vector>

#include "theia/matching/feature_correspondence.h"
#include "theia/sfm/create_and_initialize_ransac_variant.h"
#include "theia/sfm/pose/seven_point_fundamental_matrix.h"
#include "theia/sfm/pose/util.h"
#include "theia/solvers/estimator.h"
#include "theia/util/util.h"

namespace theia {

// An estimator for computing the fundamental matrix from 6 feature
// correspondences. The feature correspondences should be in pixel coordinates.
class FundamentalMatrixEstimator
    : public Estimator<FeatureCorrespondence, Eigen::Matrix3d> {
public:
    FundamentalMatrixEstimator() {}

    // 7 correspondences are needed to determine an fundamental matrix.
    double SampleSize() const { return 7; }

    // Estimates candidate fundamental matrices from correspondences.
    bool EstimateModel(std::vector<std::reference_wrapper<FeatureCorrespondence> >& correspondences,
        std::vector<Eigen::Matrix3d>* fundamental_matrices)
    {
        std::vector<Eigen::Vector2d> image1_points, image2_points;
        for (int i = 0; i < 7; i++) {
            image1_points.emplace_back(correspondences[i].get().feature1);
            image2_points.emplace_back(correspondences[i].get().feature2);
        }

        if (!SevenPointFundamentalMatrix(
                image1_points, image2_points, fundamental_matrices)) {
            return false;
        }
        return true;
    }

    // The error for a correspondences given a model. This is the squared sampson
    // error.
    double Error(const FeatureCorrespondence& correspondence,
        const Eigen::Matrix3d& fundamental_matrix) const
    {
        return SquaredSampsonDistance(
            fundamental_matrix, correspondence.feature1, correspondence.feature2);
    }

private:
    DISALLOW_COPY_AND_ASSIGN(FundamentalMatrixEstimator);
};

// Estimates the essential matrix from feature correspondences using the 5-pt
// algorithm. The feature correspondences must be normalized such that the
// principal point and focal length has been removed. Returns true if a pose
// could be successfully estimated and false otherwise.
bool EstimateFundamentalMatrix(const RansacParameters& ransac_params,
    const RansacType& ransac_type,
    std::vector<FeatureCorrespondence>& normalized_correspondences,
    Eigen::Matrix3d* essential_matrix, RansacSummary* ransac_summary);

} // namespace theia

#endif // THEIA_SFM_ESTIMATORS_ESTIMATE_FUNDAMENTAL_MATRIX_H_
