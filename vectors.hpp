#pragma once
#include <vector>
#include <type_traits>

namespace My {

	namespace Vectors {

#pragma pack(push, 1)

		template<typename T, size_t S>
		struct BaseVector {

			typedef T value_type;
			static const size_t Size = S;

			T data[S];

		};

		namespace {

			template<typename T>
			struct BaseVector<T, 0> {

				static const size_t Size = 0;

			};

			template<typename T>
			struct BaseVector<T, 2> {

				static const size_t Size = 2;

				union {

					struct { T x, y; };
					T data[2];

				};

				BaseVector() : x(0), y(0) {};
				BaseVector(T x, T y) : x(x), y(y) {};

			};

			template<typename T>
			struct BaseVector<T, 3> {

				static const size_t Size = 3;

				union {

					struct { T x, y, z; };
					struct { T r, g, b; };
					T data[3];
				};

				BaseVector() : x(0), y(0), z(0) {};
				BaseVector(T x, T y, T z) : x(x), y(y), z(z) {};

			};

			template<typename T>
			struct BaseVector<T, 4> {

				static const size_t Size = 4;

				union {

					struct { T x, y, z, w; };
					struct { T r, g, b, a; };
					T data[4];

				};

				BaseVector() : x(0), y(0), z(0), w(0) {};
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

		template<typename VT, typename CV>
		void Combine(VT& result, CV current_vector) {
			for (size_t i = 0; i < current_vector.Size; ++i)
				result.push_back(current_vector.data[i]);
		}

		template<typename VT, typename CV, typename... V>
		void Combine(VT& result, CV current_vector, V... remaining_vectors) {
			Combine(result, current_vector);
			Combine(result, remaining_vectors...);
		}

		template<typename T, typename... A>
		std::vector<T> CombineVectors(A... vectors) {
			std::vector<T> result;
			Combine(result, vectors...);
			return result;
		}


		template<typename T, typename V, typename F>
		T ApplyFunc(T first, V second, F func) {
			for (size_t i = 0; i < T::Size; ++i)
				func(first, second, i);
			return first;
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type>
		T operator-(const T& a) {
			return ApplyFunc(a, a, [](T& a, T&b, size_t i) { *(&a.x + i) = -*(&a.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type>
		T operator+(const T& to, const T& from) {
			return ApplyFunc(to, from, [](T& a, T& b, size_t i) { *(&a.x + i) += *(&b.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type>
		T operator-(const T& to, const T& from) {
			return ApplyFunc(to, from, [](T& a, T& b, size_t i) { *(&a.x + i) -= *(&b.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type,
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
		T operator+(const T& to, const V& from) {
			return ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) += b; });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type,
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
		T operator-(const T& to, const V& from) {
			return ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) += b; });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type>
		T& operator+=(T& to, const T& from) {
			return to = ApplyFunc(to, from, [](T& a, T& b, size_t i) { *(&a.x + i) += *(&b.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type>
		T& operator-=(T& to, const T& from) {
			return to = ApplyFunc(to, from, [](T& a, T& b, size_t i) { *(&a.x + i) += *(&b.x + i); });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type,
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
		T& operator+=(T& to, const V& from) {
			return to = ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) += b; });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type,
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
		T& operator-=(T& to, const V& from) {
			return to = ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) += b; });
		}

		template <typename T = std::enable_if<std::is_base_of<BaseVector<typename T::value_type, T::Size>, T>::value>::type,
			typename V = std::enable_if<std::negation<std::is_same<T, V>::value>>::type>
			T& operator*=(T& to, const V& from) {
			return to = ApplyFunc(to, from, [](T& a, V& b, size_t i) { *(&a.x + i) *= b; });
		}

	}// namespace Vectors

}// namespace My