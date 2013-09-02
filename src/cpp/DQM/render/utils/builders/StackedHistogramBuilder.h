/*
 * StackedHistogramBuilder.h
 *
 *  Created on: 2 Sep 2013
 *      Author: Colin - CERN
 */
#ifndef STACKEDHISTOGRAMBUILDER_H_
#define STACKEDHISTOGRAMBUILDER_H_

#include <vector>

#include "../../controllers/ColourController.h"
#include "Builder.h"

class TH1;
class THStack;
namespace render { class HistogramData; }

namespace render {
	/// TODO: Document
	template <class T> class StackedHistogramBuilder : public Builder<THStack*> {
		private:
			/// The object responsible for controlling the colours this builder uses
			/// to create a stacked histogram.
			ColourController colourController;

		public:
			/// @see StackedHistogramBuilder::build()
			THStack* build();

			/// TODO: Comment
			HistogramData addHistogramData(HistogramData histogramData);

		private:
			/// Adds a given histogram to the given histograms stack.
			/// @param histogram pointer to the histogram to put on the histogram stack
			/// @param histogramStack the histogram stack that the histogram is to be added to
			static void addToTHStack(TH1 *histogram, THStack *histogramStack);

			/// Adds all of the histograms given to the given histogram stack.
			/// @param histograms the list of histograms to add to the stack
			/// @param histogramStack the histogram stack that the histogram is to be added to
			static void addAllToTHStack(std::vector<TH1*> histograms, THStack *histogramStack);
	};
}
#endif
