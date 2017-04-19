#pragma once
#include "figures.hpp"
#include "points.hpp"

namespace My {

	namespace Figures {

		template<typename T>
		class Line3RGB : public BaseLine<Points::Point3RGB<T>> {

		public:

			Line3RGB(Program* program) : BaseLine<Points::Point3RGB<T>>(program) {};

		};


	}// namespace Figures

}// namespacce My