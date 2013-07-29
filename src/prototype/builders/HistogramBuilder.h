/*
 * HistogramBuilder.h
 *
 *  Created on: 26 Jul 2013
 *      Author: Colin - CERN
 */
#ifndef HISTOGRAMBUILDER_H_
#define HISTOGRAMBUILDER_H_

namespace prototype {
	/// Abstract superclass for all builder classes.
	/// @param <T> the type of the object that is built
	template <class T> class HistogramBuilder {
		public:
			/// Builds an object of type <code>T</code>, considering
			/// all settings that may have been set beforehand.
			virtual T build() = 0;
	};
}
#endif
