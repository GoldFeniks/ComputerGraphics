#pragma once
#include "figures.hpp"
#include "points.hpp"
#include "program.hpp"
#include "types.hpp"

namespace My {

	namespace Figures {

		template<typename T>
		class Triangle4RGB : public BaseTriangle<Points::Point4RGB<T>> {

		public:

			Triangle4RGB(Program* program) : BaseTriangle<Points::Point4RGB<T>>(program) {};

		};

		template<typename T>
		class Triangle3RGB : public BaseTriangle<Points::Point3RGB<T>> {

		public:

			Triangle3RGB(Program* program) : BaseTriangle<Points::Point3RGB<T>>(program) {};

		};

	}// namespace Figures

}// namespace My
