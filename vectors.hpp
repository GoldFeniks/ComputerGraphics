#pragma once
#include <type_traits>

namespace My {

	namespace Vectors {

		#pragma pack(push, 1)

		template<typename T>
		struct BaseVector {

			typedef T value_type;

		};

		template<typename T>
		struct Vector2 : public BaseVector<T> {

			Vector2() {};
			Vector2(T x, T y) : x(x), y(y) {};

			T x = T(), y = T();

		};

		template<typename T>
		struct Vector3 : public BaseVector<T> {

			Vector3() {};
			Vector3(T x, T y, T z) : x(x), y(y), z(z) {};

			T x = T(), y = T(), z = T();

		};

		template<typename T>
		struct Vector4 : public BaseVector<T> {

			Vector4() {};
			Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {};

			T x = T(), y = T(), z = T(), w = T();
		};	

		#pragma pack(pop)

		namespace Sizes {

			template<typename T>
			struct Size {};

			template<typename T>
			struct Size<Vector2<T>> {

				static const size_t DimSize = 2;

			};

			template<typename T>
			struct Size<Vector3<T>> {

				static const size_t DimSize = 3;

			};

			template<typename T>
			struct Size<Vector4<T>> {

				static const size_t DimSize = 4;

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