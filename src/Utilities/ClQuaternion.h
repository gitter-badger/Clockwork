#pragma once 

class Quaternion : public Vector4<float>
{
public:
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		(*this)[3] = w;
	}

	Quaternion(const Vector4<float>& r)
	{
		(*this)[0] = r[0];
		(*this)[1] = r[1];
		(*this)[2] = r[2];
		(*this)[3] = r[3];
	}

	Quaternion(const Vector3f& axis, float angle)
	{
		float sinHalfAngle = sinf(angle / 2);
		float cosHalfAngle = cosf(angle / 2);

		(*this)[0] = axis.GetX() * sinHalfAngle;
		(*this)[1] = axis.GetY() * sinHalfAngle;
		(*this)[2] = axis.GetZ() * sinHalfAngle;
		(*this)[3] = cosHalfAngle;
	}

	Quaternion(const Matrix4f& m)
	{
		float trace = m[0][0] + m[1][1] + m[2][2];

		if (trace > 0)
		{
			float s = 0.5f / sqrtf(trace + 1.0f);
			(*this)[3] = 0.25f / s;
			(*this)[0] = (m[1][2] - m[2][1]) * s;
			(*this)[1] = (m[2][0] - m[0][2]) * s;
			(*this)[2] = (m[0][1] - m[1][0]) * s;
		}
		else if (m[0][0] > m[1][1] && m[0][0] > m[2][2])
		{
			float s = 2.0f * sqrtf(1.0f + m[0][0] - m[1][1] - m[2][2]);
			(*this)[3] = (m[1][2] - m[2][1]) / s;
			(*this)[0] = 0.25f * s;
			(*this)[1] = (m[1][0] + m[0][1]) / s;
			(*this)[2] = (m[2][0] + m[0][2]) / s;
		}
		else if (m[1][1] > m[2][2])
		{
			float s = 2.0f * sqrtf(1.0f + m[1][1] - m[0][0] - m[2][2]);
			(*this)[3] = (m[2][0] - m[0][2]) / s;
			(*this)[0] = (m[1][0] + m[0][1]) / s;
			(*this)[1] = 0.25f * s;
			(*this)[2] = (m[2][1] + m[1][2]) / s;
		}
		else
		{
			float s = 2.0f * sqrtf(1.0f + m[2][2] - m[1][1] - m[0][0]);
			(*this)[3] = (m[0][1] - m[1][0]) / s;
			(*this)[0] = (m[2][0] + m[0][2]) / s;
			(*this)[1] = (m[1][2] + m[2][1]) / s;
			(*this)[2] = 0.25f * s;
		}

		float length = Length();
		(*this)[3] = (*this)[3] / length;
		(*this)[0] = (*this)[0] / length;
		(*this)[1] = (*this)[1] / length;
		(*this)[2] = (*this)[2] / length;
	}

	inline Quaternion NLerp(const Quaternion& r, float lerpFactor, bool shortestPath) const
	{
		Quaternion correctedDest;

		if (shortestPath && this->Dot(r) < 0)
			correctedDest = r * -1;
		else
			correctedDest = r;

		return Quaternion(Lerp(correctedDest, lerpFactor).Normalized());
	}

	inline Quaternion SLerp(const Quaternion& r, float lerpFactor, bool shortestPath) const
	{
		static const float EPSILON = 1e3;

		float cos = this->Dot(r);
		Quaternion correctedDest;

		if (shortestPath && cos < 0)
		{
			cos *= -1;
			correctedDest = r * -1;
		}
		else
			correctedDest = r;

		if (fabs(cos) >(1 - EPSILON))
			return NLerp(correctedDest, lerpFactor, false);

		float sin = (float)sqrtf(1.0f - cos * cos);
		float angle = atan2(sin, cos);
		float invSin = 1.0f / sin;

		float srcFactor = sinf((1.0f - lerpFactor) * angle) * invSin;
		float destFactor = sinf((lerpFactor)* angle) * invSin;

		return Quaternion((*this) * srcFactor + correctedDest * destFactor);
	}

	inline Matrix4f ToRotationMatrix() const
	{
		Vector3f forward = Vector3f(2.0f * (GetX() * GetZ() - GetW() * GetY()), 2.0f * (GetY() * GetZ() + GetW() * GetX()), 1.0f - 2.0f * (GetX() * GetX() + GetY() * GetY()));
		Vector3f up = Vector3f(2.0f * (GetX()*GetY() + GetW()*GetZ()), 1.0f - 2.0f * (GetX()*GetX() + GetZ()*GetZ()), 2.0f * (GetY()*GetZ() - GetW()*GetX()));
		Vector3f right = Vector3f(1.0f - 2.0f * (GetY()*GetY() + GetZ()*GetZ()), 2.0f * (GetX()*GetY() - GetW()*GetZ()), 2.0f * (GetX()*GetZ() + GetW()*GetY()));

		return Matrix4f().InitRotationFromVectors(forward, up, right);
	}

	inline Vector3f GetForward() const
	{
		return Vector3f(0, 0, 1).Rotate(*this);
	}

	inline Vector3f GetBack() const
	{
		return Vector3f(0, 0, -1).Rotate(*this);
	}

	inline Vector3f GetUp() const
	{
		return Vector3f(0, 1, 0).Rotate(*this);
	}

	inline Vector3f GetDown() const
	{
		return Vector3f(0, -1, 0).Rotate(*this);
	}

	inline Vector3f GetRight() const
	{
		return Vector3f(1, 0, 0).Rotate(*this);
	}

	inline Vector3f GetLeft() const
	{
		return Vector3f(-1, 0, 0).Rotate(*this);
	}

	inline Quaternion Conjugate() const { return Quaternion(-GetX(), -GetY(), -GetZ(), GetW()); }

	inline Quaternion operator*(const Quaternion& r) const
	{
		const float _w = (GetW() * r.GetW()) - (GetX() * r.GetX()) - (GetY() * r.GetY()) - (GetZ() * r.GetZ());
		const float _x = (GetX() * r.GetW()) + (GetW() * r.GetX()) + (GetY() * r.GetZ()) - (GetZ() * r.GetY());
		const float _y = (GetY() * r.GetW()) + (GetW() * r.GetY()) + (GetZ() * r.GetX()) - (GetX() * r.GetZ());
		const float _z = (GetZ() * r.GetW()) + (GetW() * r.GetZ()) + (GetX() * r.GetY()) - (GetY() * r.GetX());

		return Quaternion(_x, _y, _z, _w);
	}

	inline Quaternion operator*(const Vector3<float>& v) const
	{
		const float _w = -(GetX() * v.GetX()) - (GetY() * v.GetY()) - (GetZ() * v.GetZ());
		const float _x = (GetW() * v.GetX()) + (GetY() * v.GetZ()) - (GetZ() * v.GetY());
		const float _y = (GetW() * v.GetY()) + (GetZ() * v.GetX()) - (GetX() * v.GetZ());
		const float _z = (GetW() * v.GetZ()) + (GetX() * v.GetY()) - (GetY() * v.GetX());

		return Quaternion(_x, _y, _z, _w);
	}
};