#pragma once
#include <vector>
#include "vectors.hpp"
#include <initializer_list>

namespace My {

	namespace Points {

#pragma pack(push, 1)

		template<typename T, std::size_t PS = 2, std::size_t NS = 0, std::size_t CS = 0, std::size_t TS = 0, std::size_t TAS = 0, std::size_t BS = 0>
		struct BasePoint {

			typedef T value_type;
			typedef Vectors::BaseVector<T, PS> point_vector;
			typedef Vectors::BaseVector<T, NS> normal_vector;
			typedef Vectors::BaseVector<T, CS> color_vector;
			typedef Vectors::BaseVector<T, TS> texture_vector;
			typedef Vectors::BaseVector<T, TAS> tangent_vector;
			typedef Vectors::BaseVector<T, BS> bitangent_vector;

			static const std::size_t Size = PS + NS + CS + TS + TAS + BS;

			point_vector Point;
			normal_vector Normal;
			color_vector Color;
			texture_vector Texture;
			tangent_vector Tangent;
			bitangent_vector Bitangent;

			BasePoint() {};
			BasePoint(const std::initializer_list<T> list) {
				InitFromIterator(list.begin());
			}
			BasePoint(const std::vector<T> values) {
				InitFromIterator(values.begin());
			}

			template<typename TI>
			void InitFromIterator(TI it) {
				Copy(it, it + Point.Size, (T*)&Point);
				Copy(it, it + Normal.Size, (T*)&Normal);
				Copy(it, it + Color.Size, (T*)&Color);
				Copy(it, it + Texture.Size, (T*)&Texture);
				Copy(it, it + Tangent.Size, (T*)&Tangent);
				Copy(it, it + Bitangent.Size, (T*)&Bitangent);
			}

			template<typename TI>
			void Copy(TI& start, TI end, T* to) {
				while (start != end)
					*(to++) = *(start++);
			}

		};

		template<typename T>
		struct Point4RGB : public BasePoint<T, 4, 0, 3> {

			Point4RGB() {};
			Point4RGB(Vectors::Vector4<T> Point, Vectors::Vector4<T> Color) : BasePoint(Vectors::CombineVectors<T>(Point, Color)) {};
			Point4RGB(T x, T y, T z, T w, T r, T g, T b) : BasePoint({ x, y, z, w, r, g, b }) {};

		};

		template<typename T>
		struct Point3RGB : public BasePoint<T, 3, 0, 3> {

			Point3RGB() {};
			Point3RGB(Vectors::Vector3<T> Point, Vectors::Vector3<T> Color) : BasePoint(Vectors::CombineVectors<T>(Point, Color)) {};
			Point3RGB(T x, T y, T z, T r, T g, T b) : BasePoint({ x, y, z, r, g, b }) {};

		};

		template<typename T>
		struct Point3NormTexTanBitan : public BasePoint<T, 3, 3, 0, 2, 3, 3> {

			Point3NormTexTanBitan() {};
			Point3NormTexTanBitan(Vectors::Vector3<T> Point, Vectors::Vector3<T> Normal, Vectors::Vector2<T> Texture, Vectors::Vector3<T> Tangent, Vectors::Vector3<T> Bitanget)
				: BasePoint(Vectors::CombineVectors<T>(Point, Normal, Texture, Tangent, Bitangent)) {
			};
			Point3NormTexTanBitan(T x, T y, T z, T nx, T ny, T nz, T tx, T ty, T tax, T tay, T taz, T btax, T btay, T btaz)
				: BasePoint({ x, y, z, nx, ny, nz, tx, ty, tax, tay, taz, btax, btay, btaz }) {
			};

		};

		template<typename T>
		struct Point3Norm : public BasePoint <T, 3, 3> {

			Point3Norm() {};
			Point3Norm(Vectors::Vector3<T> Point, Vectors::Vector3<T> Normal) : BasePoint(Vectors::CombineVectors<T>(Point, Normal)) {};
			Point3Norm(T x, T y, T z, T nx, T ny, T nz) : BasePoint({ x, y, z, nx, ny, nz }) {};

		};

		template<typename T>
		struct Point3 : public BasePoint<T, 3> {

			Point3() {};
			Point3(Vectors::Vector3<T> Point) : BasePoint(Vectors::CombineVectors(Point)) {};
			Point3(T x, T y, T z) : BasePoint({ x, y, z }) {};

		};

		template<typename T>
		struct Point2Tex : public BasePoint<T, 2, 0, 0, 2> {

			Point2Tex() {};
			Point2Tex(Vectors::Vector2<T> Point, Vectors::Vector2<T> Texture) : BasePoint(Vectors::CombineVectors<T>(Point, Texture)) {};
			Point2Tex(T x, T y, T tx, T tz) : BasePoint({ x, y, tx, tz }) {};

		};

#pragma pack(pop)

	}// namespace Points

}// namespace My
