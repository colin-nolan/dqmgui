/*
 * HistogramScalingUtil.cc
 *
 *  Created on: 1 Aug 2013
 *      Author: Colin - CERN
 */
#define DNDEBUG

#include "HistogramScalingUtil.h"

#include <Rtypes.h>
#include <TH1.h>
#include <cassert>
#include <list>
#include <stdlib.h>
#include <cmath>

#include "../models/HistogramData.h"
#include "../models/WeightedHistogramData.h"

namespace render {
	const Double_t HistogramScalingUtil::REQUIRED_ACCURACY = 0.0001;

	void HistogramScalingUtil::scaleHistogramToArea(TH1 *histogram, Double_t targetArea) {
		Double_t integral = histogram->Integral();

		if(integral > 0) {
			Double_t inverseIntegral = targetArea / integral;
			HistogramScalingUtil::scaleHistogram(histogram, inverseIntegral);

			// Note: We cannot assume that integrating the histogram will give exactly the
			//		 <code>targetArea</code> as we don't know the internals of <code>
			//		 TH1::Scale(Double_t)</code> and <code>TH1::Integral()</code>.
			//		 Therefore we'll just assume a tolerance of 0.01% is acceptable.
			assert(std::abs(histogram->Integral() - targetArea) <= (targetArea * REQUIRED_ACCURACY));
		}
		else {
			// Histogram does not contain any samples - no scaling required
		}
	}

	void HistogramScalingUtil::scaleHistogram(TH1 *histogram, Double_t scalingFactor) {
		#ifdef DNDEBUG
		Double_t originalArea = histogram->Integral();
		#endif

		histogram->Scale(scalingFactor);

		#ifdef DNDEBUG
		Double_t postOperationArea = histogram->Integral();
		Double_t targetArea = originalArea * scalingFactor;
		assert(std::abs(postOperationArea - targetArea) <= (targetArea * REQUIRED_ACCURACY));
		#endif
	}

	// XXX: Is this method still required?
	void HistogramScalingUtil::scaleWeightedHistograms(
			std::list<WeightedHistogramData> weightedHistogramData,
			Double_t targetCombinedArea) {
		#ifdef DNDEBUG
		Double_t combinedArea = 0;
		#endif

		std::list<WeightedHistogramData>::iterator it = weightedHistogramData.begin();

		while(it != weightedHistogramData.end()) {
			WeightedHistogramData weightedHistogramData = *it;

			// TODO: This is likely a reasonably computational expensive function call.
			//		 For efficiency gains, consider executing the below/loop in a new thread
			//		 (if thread safe) and then waiting for all threads to complete.
			HistogramScalingUtil::scaleWeightedHistogram(weightedHistogramData, targetCombinedArea);

			#ifdef DNDEBUG
			combinedArea += *weightedHistogramData.getHistogram()->GetIntegral();
			#endif
			it++;
		}

		#ifdef DNDEBUG
		assert(std::abs(std::abs(combinedArea - targetCombinedArea) <= (targetCombinedArea * REQUIRED_ACCURACY)));
		#endif
	}

	void HistogramScalingUtil::scaleWeightedHistogram(
			WeightedHistogramData weightedHistogramData,
			Double_t combinedTargetArea) {
		TH1 *histogram = weightedHistogramData.getHistogram();
		Double_t weight = weightedHistogramData.getWeight();

		Double_t targetHistogramArea = weight * combinedTargetArea;

		HistogramScalingUtil::scaleHistogramToArea(histogram, targetHistogramArea);
	}
}
