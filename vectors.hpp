#pragma once
#include <type_traits>

namespace My {

	namespace Vectors {

#pragma pack(push, 1)

		template<typename T, std::size_t S>
		struct BaseVector {

			typedef T value_type;

			T data[S];

		};

		namespace {

			template<typename T>
			struct BaseVector<T, 2> {

				union {

					struct { T x, y; };
					T data[2];

				};

				BaseVector() {};
				BaseVector(T x, T y) : x(x), y(y) {};

			};

			template<typename T>
			struct BaseVector<T, 3> {

				union {

					struct { T x, y, z; };
					T data[3];
				};

				BaseVector() {};
				BaseVector(T x, T y, T z) : x(x), y(y), z(z) {};

			};

			template<typename T>
			struct BaseVector<T, 4> {

				union {

					struct { T x, y, z, w; };
					T data[4];

				};

				BaseVector() {};
				BaseVector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {};

			};

		}

#pragma pack(pop)

		template<typename T>
		using Vector2 = BaseVector<T, 2>;

		template<typename T>
		using Vector3 = BaseVector<T, 3>;

		template<typename T>
		using Vector4 = BaseVector<T, 4>;

		namespace Sizes {

			template<typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::S>, T>::value>::type>
			struct Size {

				static const std::size_t DimSize = T::S;
				
			};

		}// namespace Sizes

		template<typename T, typename V, typename F>
		T ApplyFunc(T first, V second, F func) {
			for (size_t i = 0; i < Sizes::Size<T>::DimSize; ++i)
				func(first, second, i);
			return first;
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type>
		T operator-(const T& a) {
			return ApplyFunc(a, a, [](T& a, T&b, size_t i) { *(&a.x + i) = -*(&a.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type>
		T operator+(const T& to, const T& from) {
			return ApplyFunc(to, from, [](T& a, T& b, size_t i) { *(&a.x + i) += *(&b.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type>
		T operator-(const T& to, const T& from) {
			return ApplyFunc(to, from, [](T& a, T& b, size_t i) { *(&a.x + i) -= *(&b.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type, 
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
		T operator+(const T& to, const V& from) {
			return ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) += b; });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type,
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
		T operator-(const T& to, const V& from) {
			return ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) += b; });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type>
		T& operator+=(T& to, const T& from) {
			return to = ApplyFunc(to, from, [](T& a, T& b, size_t i) { *(&a.x + i) += *(&b.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type>
		T& operator-=(T& to, const T& from) {
			return to = ApplyFunc(to, from, [](T& a, T& b, size_t i) { *(&a.x + i) += *(&b.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type,
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
		T& operator+=(T& to, const V& from) {
			return to = ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) += b; });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type>, T>::value>::type,
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
		T& operator-=(T& to, const V& from) {
			return to = ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) += b; });
		}

	}// namespace Vectors

}// namespace My