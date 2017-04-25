#pragma once
#include "vectors.hpp"

namespace My {

	namespace Points {

#pragma pack(push, 1)

		template<typename T>
		struct BasePoint {

			typedef T value_type;

		};

		template<typename T>
		struct Point4RGB : public BasePoint<T> {

			Point4RGB() {};
			Point4RGB(Vectors::Vector4<T> Point, Vectors::Vector4<T> Color) : Point(Point), Color(Color) {};
			Point4RGB(T x, T y, T z, T w, T r, T g, T b) : Point(Vectors::Vector4<T>(x, y, z, w)), Color(Vectors::Vector3<T>(r, g, b)) {};

			Vectors::Vector4<T> Point;
			Vectors::Vector3<T> Color;

		};

		template<typename T>
		struct Point3RGB : public BasePoint<T> {

			Point3RGB() {};
			Point3RGB(Vectors::Vector3<T> Point, Vectors::Vector3<T> Color) : Point(Point), Color(Color) {};
			Point3RGB(T x, T y, T z, T r, T g, T b) : Point(Vectors::Vector3<T>(x, y, z)), Color(Vectors::Vector3<T>(r, g, b)) {};

			Vectors::Vector3<T> Point, Color;

		};

		template<typename T>
		struct Point3TexNormTangents : public BasePoint<T> {

			Point3TexNormTangents() {};
			Point3TexNormTangents(Vectors::Vector3<T> Point, Vectors::Vector2<T> Texture, Vectors::Vector3<T> Normal, Vectors::Vector3<T> Tangent, Vectors::Vector3<T> Bitanget)
				: Point(Point), Texture(Texture), Normal(Normal), Tangent(Tanget), Bitangent(Bitanget) {};
			Point3TexNormTangents(T x, T y, T z, T tx, T ty, T nx, T ny, T nz, T tax, T tay, T taz, T btax, T btay, T btaz)
				: Point(Vectors::Vector3<T>(x, y, z)), 
				Texture(Vectors::Vector2<T>(tx, ty)), Normal(Vectors::Vector3<T>(nx, ny, nz)),
				Tangent(Vectors::Vector3<T>(tax, tay, taz)), Bitangent(Vectors::Vector3<T>(btax, btay, btaz)) {};
			
			Vectors::Vector3<T> Point, Normal;
			Vectors::Vector2<T> Texture;
			Vectors::Vector3<T> Tangent, Bitangent;

		};

		template<typename T>
		struct Point3Norm : public BasePoint<T> {

			Point3Norm() {};
			Point3Norm(Vectors::Vector3<T> Point, Vectors::Vector3<T> Normal) : Point(Point), Normal(Normal) {};
			Point3Norm(T x, T y, T z, T nx, T ny, T nz) : Point(Vectors::Vector3<T>(x, y, z)), Normal(Vectors::Vector3<T>(nx, ny, nz)) {};
			
			Vectors::Vector3<T> Point, Normal;

		};

		template<typename T>
		struct Point3 : public BasePoint<T> {

			Point3() {};
			Point3(Vectors::Vector3<T> Point) : Point(Point) {};
			Point3(T x, T y, T z) : Point(Vectors::Vector3<T>(x, y, z)) {};

			Vectors::Vector3<T> Point;

		};

#pragma pack(pop)

		template<typename T>
		struct Size {};

		template<typename T>
		struct Size<Point4RGB<T>> {

			static const size_t PointSize = 4;
			static const size_t TextureSize = 0;
			static const size_t ColorSize = 3;
			static const size_t NormalSize = 0;
			static const size_t TangentSize = 0;
			static const size_t BitangentSize = 0;

		};

		template<typename T>
		struct Size<Point3RGB<T>> {

			static const size_t PointSize = 3;
			static const size_t TextureSize = 0;
			static const size_t ColorSize = 3;
			static const size_t NormalSize = 0;
			static const size_t TangentSize = 0;
			static const size_t BitangentSize = 0;

		};

		template<typename T>
		struct Size<Point3TexNormTangents<T>> {

			static const size_t PointSize = 3;
			static const size_t TextureSize = 2;
			static const size_t ColorSize = 0;
			static const size_t NormalSize = 3;
			static const size_t TangentSize = 3;
			static const size_t BitangentSize = 3;

		};

		template<typename T>
		struct Size<Point3Norm<T>> {

			static const size_t PointSize = 3;
			static const size_t TextureSize = 0;
			static const size_t ColorSize = 0;
			static const size_t NormalSize = 3;
			static const size_t TangentSize = 0;
			static const size_t BitangentSize = 0;

		};

		template<typename T>
		struct Size<Point3<T>> {

			static const size_t PointSize = 3;
			static const size_t TextureSize = 0;
			static const size_t ColorSize = 0;
			static const size_t NormalSize = 0;
			static const size_t TangentSize = 0;
			static const size_t BitangentSize = 0;

		};

	}// namespace Points

}// namespace My
