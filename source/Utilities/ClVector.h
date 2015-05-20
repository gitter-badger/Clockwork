#pragma once

template<typename T, unsigned int D>
class Vector
{
public:
	Vector() { }

	//Returns the dot product of 2 vectors
	inline T Dot(const Vector<T, D>& r) const
	{
		T result = T(0);
		for (unsigned int i = 0; i < D; i++)
			result += (*this)[i] * r[i];

		return result;
	}

	//Returns the vector consisting of tha largest value
	inline Vector<T, D> Max(const Vector<T, D>& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
		{
			result[i] = values[i] > r[i] ? values[i] : r[i];
		}

		return result;
	}

	//Returns the biggest value in a vector
	inline T Max() const
	{
		T maxVal = (*this)[0];

		for (int i = 0; i < D; i++)
			if ((*this)[i] > maxVal)
				maxVal = (*this)[i];

		return maxVal;
	}

	//Returns the lenght of the vector squared
	inline T LengthSq() const 
	{
		return this->Dot(*this);
	}

	//Returns the lenght of the vector
	inline T Length() const 
	{
		return sqrt(LengthSq());
	}
	
	//Normalizes a vector
	inline Vector<T, D> Normalized() const 
	{
		return *this / Length(); 
	}

	//Linier interpolates between 2 vectors
	inline Vector<T, D> Lerp(const Vector<T, D>& r, T lerpFactor) const 
	{
		return (r - *this) * lerpFactor + *this; 
	}

	//Reflects a vector over a normal line
	inline Vector<T, D> Reflect(const Vector<T, D>& normal) const
	{
		return *this - (normal * (this->Dot(normal) * 2));
	}

	inline Vector<T, D> operator+(const Vector<T, D>& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
			result[i] = values[i] + r[i];

		return result;
	}

	inline Vector<T, D> operator-(const Vector<T, D>& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
			result[i] = values[i] - r[i];

		return result;
	}

	inline Vector<T, D> operator*(const T& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
			result[i] = values[i] * r;

		return result;
	}

	inline Vector<T, D> operator/(const T& r) const
	{
		Vector<T, D> result;
		for (unsigned int i = 0; i < D; i++)
			result[i] = values[i] / r;

		return result;
	}

	inline Vector<T, D>& operator+=(const Vector<T, D>& r)
	{
		for (unsigned int i = 0; i < D; i++)
			(*this)[i] = values[i] + r[i];

		return *this;
	}

	inline Vector<T, D>& operator-=(const Vector<T, D>& r)
	{
		for (unsigned int i = 0; i < D; i++)
			(*this)[i] = values[i] - r[i];

		return *this;
	}

	inline Vector<T, D>& operator*=(const T& r)
	{
		for (unsigned int i = 0; i < D; i++)
			(*this)[i] = values[i] * r;

		return *this;
	}

	inline Vector<T, D>& operator/=(const T& r)
	{
		for (unsigned int i = 0; i < D; i++)
			(*this)[i] = values[i] / r;

		return *this;
	}

	T& operator [] (unsigned int i) 
	{
		return values[i]; 
	}

	T operator [] (unsigned int i) const 
	{ 
		return values[i]; 
	}

	inline bool operator==(const Vector<T, D>& r) const
	{
		for (unsigned int i = 0; i < D; i++)
			if ((*this)[i] != r[i])
				return false;
		return true;
	}

	inline bool operator!=(const Vector<T, D>& r) const 
	{ 
		return !operator==(r); 
	}
protected:
private:
	T values[D];
};

template<typename T>
class Vector2 : public Vector<T, 2>
{
public:
	Vector2() { }

	Vector2(const Vector<T, 2>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
	}

	Vector2(T x, T y)
	{
		(*this)[0] = x;
		(*this)[1] = y;
	}

	//Reutrn the cross product of 3 vectors
	T Cross(const Vector2<T>& r) const
	{
		return GetX() * r.GetY() - GetY() * r.GetX();
	}

	//Return the X value
	inline T GetX() const 
	{
		return (*this)[0]; 
	}
	
	//Return the Y value
	inline T GetY() const 
	{ 
		return (*this)[1]; 
	}

	//Set the X value
	inline void SetX(const T& x) 
	{
		(*this)[0] = x;
	}
	
	//Set the Y value
	inline void SetY(const T& y) 
	{
		(*this)[1] = y;
	}

	//Set the X and Y value
	inline void Set(const T& x, const T& y) 
	{
		SetX(x); SetY(y); 
	}
protected:
private:
};

template<typename T>
class Vector3 : public Vector<T, 3>
{
public:
	Vector3() { }

	Vector3(const Vector<T, 3>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
	}

	Vector3(T x, T y, T z)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
	}


	//Returns the cross product of 2 vectors
	inline Vector3<T> Cross(const Vector3<T>& r) const
	{
		T x = (*this)[1] * r[2] - (*this)[2] * r[1];
		T y = (*this)[2] * r[0] - (*this)[0] * r[2];
		T z = (*this)[0] * r[1] - (*this)[1] * r[0];

		return Vector3<T>(x, y, z);
	}

	//Rotates a vectors around an angle
	inline Vector3<T> Rotate(T angle, const Vector3<T>& axis) const
	{
		const T sinAngle = sin(-angle);
		const T cosAngle = cos(-angle);

		return this->Cross(axis * sinAngle) +        
			(*this * cosAngle) +                     
			axis * this->Dot(axis * (1 - cosAngle)); 
	}

	//Returns the X and Y value
	inline Vector2<T> GetXY() const 
	{
		return Vector2<T>(GetX(), GetY()); 
	}
	
	//Returns the Y and Z value
	inline Vector2<T> GetYZ() const 
	{
		return Vector2<T>(GetY(), GetZ()); 
	}

	//Returns the Z and X value
	inline Vector2<T> GetZX() const 
	{
		return Vector2<T>(GetZ(), GetX()); 
	}

	//Returns the Y and X value
	inline Vector2<T> GetYX() const 
	{ 
		return Vector2<T>(GetY(), GetX()); 
	}

	//Returns the Z and Y value
	inline Vector2<T> GetZY() const 
	{ 
		return Vector2<T>(GetZ(), GetY()); 
	}
	
	//Returns the X and Z value
	inline Vector2<T> GetXZ() const
	{ 
		return Vector2<T>(GetX(), GetZ()); 
	}

	//Returns the X value
	inline T GetX() const 
	{
		return (*this)[0]; 
	}
	
	//Returns the Y value
	inline T GetY() const 
	{
		return (*this)[1]; 
	}

	//Returns the Z value
	inline T GetZ() const 
	{
		return (*this)[2]; 
	}

	//Set the X value
	inline void SetX(const T& x) 
	{
		(*this)[0] = x; 
	}

	//Set the Y value
	inline void SetY(const T& y) 
	{ 
		(*this)[1] = y; 
	}

	//Set the Z value
	inline void SetZ(const T& z) 
	{
		(*this)[2] = z; 
	}

	//Set the X, Y and Z value
	inline void Set(const T& x, const T& y, const T& z) 
	{ 
		SetX(x); SetY(y); SetZ(z); 
	}
protected:
private:
};

template<typename T>
class Vector4 : public Vector<T, 4>
{
public:
	Vector4() { }

	Vector4(const Vector<T, 4>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
		(*this)[3] = r[3];
	}

	Vector4(T x, T y, T z, T w)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = w;
	}


	//Returns the X value
	inline T GetX() const 
	{
		return (*this)[0]; 
	}

	//Returns the Y value
	inline T GetY() const 
	{
		return (*this)[1]; 
	}

	//Returns the Z value
	inline T GetZ() const 
	{
		return (*this)[2];
	}
	
	//Return the W value
	inline T GetW() const 
	{
		return (*this)[3]; 
	}

	//Set the X value
	inline void SetX(const T& x) 
	{
		(*this)[0] = x; 
	}

	//Set the Y value
	inline void SetY(const T& y) 
	{
		(*this)[1] = y; 
	}

	//Set the Z value
	inline void SetZ(const T& z) 
	{
		(*this)[2] = z; 
	}
	
	//Set the W value
	inline void SetW(const T& w) 
	{
		(*this)[3] = w;
	}

	//Set the X, Y, Z and W value
	inline void Set(const T& x, const T& y, const T& z, const T& w) 
	{
		SetX(x); SetY(y); SetZ(z); SetW(w); 
	}
protected:
private:
};

class Vector3f : public Vector3<float>
{
public:
	Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
	}

	Vector3f(const Vector3<float>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
	}

	//Reutrn the lenght of the vector
	inline float Length() const 
	{
		return sqrtf(GetX() * GetX() + GetY() * GetY() + GetZ() * GetZ()); 
	}
	
	//return the Dot product
	inline float Dot(const Vector3f& v) const 
	{
		return GetX() * v.GetX() + GetY() * v.GetY() + GetZ() * v.GetZ(); 
	}

	//Return the cross product
	inline Vector3f Cross(const Vector3f& v) const
	{
		const float _x = GetY() * v.GetZ() - GetZ() * v.GetY();
		const float _y = GetZ() * v.GetX() - GetX() * v.GetZ();
		const float _z = GetX() * v.GetY() - GetY() * v.GetX();

		return Vector3f(_x, _y, _z);
	}

	//Rotate a vector around an axis using vectors
	inline Vector3f Rotate(float angle, const Vector3f& axis) const
	{
		const float sin = sinf(-angle);
		const float cos = cosf(-angle);

		return this->Cross(axis * sin) +        //Rotation on local X
			(*this * cos) +                     //Rotation on local Z
			axis * this->Dot(axis * (1 - cos)); //Rotation on local Y
	}

	//Rotate a vector around an axis using Quaternions
	Vector3f Rotate(const Quaternion& rotation) const;

	inline Vector3f Normalized() const
	{
		const float length = Length();

		return Vector3f(GetX() / length, GetY() / length, GetZ() / length);
	}

	inline Vector3f operator+(const Vector3f& r) const 
	{
		return Vector3f(GetX() + r.GetX(), GetY() + r.GetY(), GetZ() + r.GetZ()); 
	}
	
	inline Vector3f operator-(const Vector3f& r) const 
	{
		return Vector3f(GetX() - r.GetX(), GetY() - r.GetY(), GetZ() - r.GetZ()); 
	}
	
	inline Vector3f operator*(float f) const 
	{
		return Vector3f(GetX() * f, GetY() * f, GetZ() * f); 
	}
	
	inline Vector3f operator/(float f) const 
	{
		return Vector3f(GetX() / f, GetY() / f, GetZ() / f); 
	}

	inline bool operator==(const Vector3f& r) const 
	{ 
		return GetX() == r.GetX() && GetY() == r.GetY() && GetZ() == r.GetZ(); 
	}

	inline bool operator!=(const Vector3f& r) const 
	{ 
		return !operator==(r); 
	}

	inline Vector3f& operator+=(const Vector3f& r)
	{
		(*this)[0] += r.GetX();
		(*this)[1] += r.GetY();
		(*this)[2] += r.GetZ();

		return *this;
	}

	inline Vector3f& operator-=(const Vector3f& r)
	{
		(*this)[0] -= r.GetX();
		(*this)[1] -= r.GetY();
		(*this)[2] -= r.GetZ();

		return *this;
	}

	inline Vector3f& operator*=(float f)
	{
		(*this)[0] *= f;
		(*this)[1] *= f;
		(*this)[2] *= f;

		return *this;
	}

	inline Vector3f& operator/=(float f)
	{
		(*this)[0] /= f;
		(*this)[1] /= f;
		(*this)[2] /= f;

		return *this;
	}

	//Return the X value
	inline float GetX() const 
	{
		return (*this)[0]; 
	}

	//Return the Y value
	inline float GetY() const 
	{
		return (*this)[1]; 
	}

	//Return the Z value
	inline float GetZ() const 
	{ 
		return (*this)[2];
	}

	//Set the X value
	inline void SetX(float x) 
	{
		(*this)[0] = x;
	}
	
	//Set the Y value
	inline void SetY(float y)
	{ 
		(*this)[1] = y; 
	}
	
	//set the Z value
	inline void SetZ(float z) 
	{
		(*this)[2] = z;
	}

	//Set the X,Y and Z value
	inline void Set(float x, float y, float z) 
	{
		(*this)[0] = x; (*this)[1] = y; (*this)[2] = z; 
	}
private:
};
