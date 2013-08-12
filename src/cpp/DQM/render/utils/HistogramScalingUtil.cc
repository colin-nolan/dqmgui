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

#include "../models/display-data/HistogramData.h"
#include "../models/display-data/WeightedHistogramData.h"

namespace render {
	void HistogramScalingUtil::scaleHistogram(TH1D *histogram, Double_t targetArea) {
		Double_t integral = histogram->Integral();

		if(integral > 0) {
			Double_t inverseIntegral = targetArea / integral;
			histogram->Scale(inverseIntegral);

			// Note: We cannot assume that integrating the histogram will give exactly the
			//		 <code>targetArea</code> as we don't know the internals of <code>
			//		 TH1D::Scale(Double_t)</code> and <code>TH1D::Integral()</code>.
			//		 Therefore we'll just assume a tolerance of 0.01% is acceptable.
			assert(std::abs(histogram->Integral() - targetArea) < (targetArea * 0.0001));
		}
		else {
			// Histogram does not contain any samples - no scaling required
		}
	}

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

		assert(std::abs(std::abs(combinedArea - targetCombinedArea) < (targetCombinedArea * 0.0001)));
	}

	void HistogramScalingUtil::scaleWeightedHistogram(
			WeightedHistogramData weightedHistogramData,
			Double_t combinedTargetArea) {
		TH1D *histogram = weightedHistogramData.getHistogram();		// TODO: Change to reference
		Double_t weight = weightedHistogramData.getWeight();

		Double_t targetHistogramArea = weight * combinedTargetArea;

		HistogramScalingUtil::scaleHistogram(histogram, targetHistogramArea);
	}
}
