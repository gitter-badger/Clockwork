#pragma once

template<typename T, unsigned int D>
class Matrix
{
public:
	inline Matrix<T, D> InitIdentity()
	{
		for (unsigned int i = 0; i < D; i++)
		{
			for (unsigned int j = 0; j < D; j++)
			{
				if (i == j)
					m[i][j] = T(1);
				else
					m[i][j] = T(0);
			}
		}

		return *this;
	}

	inline Matrix<T, D> InitScale(const Vector<T, D - 1>& r)
	{
		for (unsigned int i = 0; i < D; i++)
		{
			for (unsigned int j = 0; j < D; j++)
			{
				if (i == j && i != D - 1)
					m[i][j] = r[i];
				else
					m[i][j] = T(0);
			}
		}

		m[D - 1][D - 1] = T(1);

		return *this;
	}

	inline Matrix<T, D> InitTranslation(const Vector<T, D - 1>& r)
	{
		for (unsigned int i = 0; i < D; i++)
		{
			for (unsigned int j = 0; j < D; j++)
			{
				if (i == D - 1 && j != D - 1)
					m[i][j] = r[j];
				else if (i == j)
					m[i][j] = T(1);
				else
					m[i][j] = T(0);
			}
		}

		m[D - 1][D - 1] = T(1);

		return *this;
	}

	inline Matrix<T, D> Transpose() const
	{
		Matrix<T, D> t;
		for (int j = 0; j < D; j++) {
			for (int i = 0; i < D; i++) {
				t[i][j] = m[j][i];
			}
		}
		return t;
	}

	// This function doesn't appear to work!
	inline Matrix<T, D> Inverse() const
	{
		int i, j, k;
		Matrix<T, D> s;
		Matrix<T, D> t(*this);

		// Forward elimination
		for (i = 0; i < D - 1; i++) {
			int pivot = i;

			T pivotsize = t[i][i];

			if (pivotsize < 0)
				pivotsize = -pivotsize;

			for (j = i + 1; j < D; j++) {
				T tmp = t[j][i];

				if (tmp < 0)
					tmp = -tmp;

				if (tmp > pivotsize) {
					pivot = j;
					pivotsize = tmp;
				}
			}

			if (pivotsize == 0) {
				//if (singExc)
				//	throw ::Imath::SingMatrixExc ("Cannot invert singular matrix.");

				return Matrix<T, D>();
			}

			if (pivot != i) {
				for (j = 0; j < D; j++) {
					T tmp;

					tmp = t[i][j];
					t[i][j] = t[pivot][j];
					t[pivot][j] = tmp;

					tmp = s[i][j];
					s[i][j] = s[pivot][j];
					s[pivot][j] = tmp;
				}
			}

			for (j = i + 1; j < D; j++) {
				T f = t[j][i] / t[i][i];

				for (k = 0; k < D; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		// Backward substitution
		for (i = D - 1; i >= 0; --i) {
			T f;

			if ((f = t[i][i]) == 0) {
				//if (singExc)
				//	throw ::Imath::SingMatrixExc ("Cannot invert singular matrix.");

				return Matrix<T, D>();
			}

			for (j = 0; j < D; j++) {
				t[i][j] /= f;
				s[i][j] /= f;
			}

			for (j = 0; j < i; j++) {
				f = t[j][i];

				for (k = 0; k < D; k++) {
					t[j][k] -= f * t[i][k];
					s[j][k] -= f * s[i][k];
				}
			}
		}

		return s;
	}

	inline Matrix<T, D> operator*(const Matrix<T, D>& r) const
	{
		Matrix<T, D> ret;
		for (unsigned int i = 0; i < D; i++)
		{
			for (unsigned int j = 0; j < D; j++)
			{
				ret.m[i][j] = T(0);
				for (unsigned int k = 0; k < D; k++)
					ret.m[i][j] += m[k][j] * r.m[i][k];
			}
		}
		return ret;
	}

	inline Vector<T, D> Transform(const Vector<T, D>& r) const
	{
		Vector<T, D> ret;

		for (unsigned int i = 0; i < D; i++)
		{
			ret[i] = 0;
			for (unsigned int j = 0; j < D; j++)
				ret[i] += m[j][i] * r[j];
		}

		return ret;
	}

	inline Vector<T, D - 1> Transform(const Vector<T, D - 1>& r) const
	{
		Vector<T, D> r2;

		for (int i = 0; i < D - 1; i++)
			r2[i] = r[i];

		r2[D - 1] = T(1);

		Vector<T, D> ret2 = Transform(r2);
		Vector<T, D - 1> ret;

		for (int i = 0; i < D - 1; i++)
			ret[i] = ret2[i];

		return ret;
	}

	inline void Set(unsigned int x, unsigned int y, T val) { m[x][y] = val; }

	inline const T* operator[](int index) const { return m[index]; }
	inline T* operator[](int index) { return m[index]; }
protected:
private:
	T m[D][D];
};

template<typename T>
class Matrix4 : public Matrix<T, 4>
{
public:
	Matrix4() { }

	template<unsigned int D>
	Matrix4(const Matrix<T, D>& r)
	{
		if (D < 4)
		{
			this->InitIdentity();

			for (unsigned int i = 0; i < D; i++)
				for (unsigned int j = 0; j < D; j++)
					(*this)[i][j] = r[i][j];
		}
		else
		{
			for (unsigned int i = 0; i < 4; i++)
				for (unsigned int j = 0; j < 4; j++)
					(*this)[i][j] = r[i][j];
		}
	}

	inline Matrix4<T> InitRotationEuler(T rotateX, T rotateY, T rotateZ)
	{
		Matrix4<T> rx, ry, rz;

		const T x = rotateX;
		const T y = rotateY;
		const T z = rotateZ;

		rx[0][0] = T(1);   rx[1][0] = T(0);  rx[2][0] = T(0); rx[3][0] = T(0);
		rx[0][1] = T(0);   rx[1][1] = cos(x);  rx[2][1] = -sin(x); rx[3][1] = T(0);
		rx[0][2] = T(0);   rx[1][2] = sin(x);  rx[2][2] = cos(x); rx[3][2] = T(0);
		rx[0][3] = T(0);   rx[1][3] = T(0);  rx[2][3] = T(0); rx[3][3] = T(1);

		ry[0][0] = cos(y); ry[1][0] = T(0);    ry[2][0] = -sin(y); ry[3][0] = T(0);
		ry[0][1] = T(0); ry[1][1] = T(1);    ry[2][1] = T(0); ry[3][1] = T(0);
		ry[0][2] = sin(y); ry[1][2] = T(0);    ry[2][2] = cos(y); ry[3][2] = T(0);
		ry[0][3] = T(0); ry[1][3] = T(0);    ry[2][3] = T(0); ry[3][3] = T(1);

		rz[0][0] = cos(z); rz[1][0] = -sin(z); rz[2][0] = T(0);    rz[3][0] = T(0);
		rz[0][1] = sin(z); rz[1][1] = cos(z); rz[2][1] = T(0);    rz[3][1] = T(0);
		rz[0][2] = T(0); rz[1][2] = T(0); rz[2][2] = T(1);    rz[3][2] = T(0);
		rz[0][3] = T(0); rz[1][3] = T(0); rz[2][3] = T(0);    rz[3][3] = T(1);

		*this = rz * ry * rx;

		return *this;
	}

	inline Matrix4<T> InitRotationFromVectors(const Vector3<T>& n, const Vector3<T>& v, const Vector3<T>& u)
	{
		(*this)[0][0] = u.GetX();   (*this)[1][0] = u.GetY();   (*this)[2][0] = u.GetZ();   (*this)[3][0] = T(0);
		(*this)[0][1] = v.GetX();   (*this)[1][1] = v.GetY();   (*this)[2][1] = v.GetZ();   (*this)[3][1] = T(0);
		(*this)[0][2] = n.GetX();   (*this)[1][2] = n.GetY();   (*this)[2][2] = n.GetZ();   (*this)[3][2] = T(0);
		(*this)[0][3] = T(0);       (*this)[1][3] = T(0);       (*this)[2][3] = T(0);       (*this)[3][3] = T(1);

		return *this;
	}

	inline Matrix4<T> InitRotationFromDirection(const Vector3<T>& forward, const Vector3<T>& up)
	{
		Vector3<T> n = forward.Normalized();
		Vector3<T> u = Vector3<T>(up.Normalized()).Cross(n);
		Vector3<T> v = n.Cross(u);

		return InitRotationFromVectors(n, v, u);
	}

	inline Matrix4<T> InitPerspective(T fov, T aspectRatio, T zNear, T zFar)
	{
		const T zRange = zNear - zFar;
		const T tanHalfFOV = tanf(fov / T(2));

		(*this)[0][0] = T(1) / (tanHalfFOV * aspectRatio); (*this)[1][0] = T(0);   (*this)[2][0] = T(0);            (*this)[3][0] = T(0);
		(*this)[0][1] = T(0);                   (*this)[1][1] = T(1) / tanHalfFOV; (*this)[2][1] = T(0);            (*this)[3][1] = T(0);
		(*this)[0][2] = T(0);                   (*this)[1][2] = T(0);            (*this)[2][2] = (-zNear - zFar) / zRange; (*this)[3][2] = T(2)*zFar*zNear / zRange;
		(*this)[0][3] = T(0);                   (*this)[1][3] = T(0);            (*this)[2][3] = T(1);            (*this)[3][3] = T(0);

		return *this;
	}

	inline Matrix4<T> InitOrthographic(T left, T right, T bottom, T top, T near, T far)
	{
		const T width = (right - left);
		const T height = (top - bottom);
		const T depth = (far - near);

		(*this)[0][0] = T(2) / width; (*this)[1][0] = T(0);        (*this)[2][0] = T(0);        (*this)[3][0] = -(right + left) / width;
		(*this)[0][1] = T(0);       (*this)[1][1] = T(2) / height; (*this)[2][1] = T(0);        (*this)[3][1] = -(top + bottom) / height;
		(*this)[0][2] = T(0);       (*this)[1][2] = T(0);        (*this)[2][2] = T(-2) / depth; (*this)[3][2] = -(far + near) / depth;
		(*this)[0][3] = T(0);       (*this)[1][3] = T(0);        (*this)[2][3] = T(0);        (*this)[3][3] = T(1);

		return *this;
	}
protected:
private:
};

template<typename T>
class Matrix3 : public Matrix<T, 3>
{
public:
	Matrix3() { }

	template<unsigned int D>
	Matrix3(const Matrix<T, D>& r)
	{
		if (D < 3)
		{
			this->InitIdentity();

			for (unsigned int i = 0; i < D; i++)
				for (unsigned int j = 0; j < D; j++)
					(*this)[i][j] = r[i][j];
		}
		else
		{
			for (unsigned int i = 0; i < 3; i++)
				for (unsigned int j = 0; j < 3; j++)
					(*this)[i][j] = r[i][j];
		}
	}
};