#ifndef VECTOR3D_H_INCLUDED
#define VECTOR3D_H_INCLUDED

#include <math.h>

// class Vector3D		---> An object to represent a 3D vector or a 3D point in space
class Vector3D
{
public:
	float x;									// the x value of this Vector3D
	float y;									// the y value of this Vector3D
	float z;									// the z value of this Vector3D

	Vector3D()									// Constructor to set x = y = z = 0
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector3D(float x, float y, float z)			// Constructor that initializes this Vector3D to the intended values of x, y and z
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3D& operator= (Vector3D v)			// operator= sets values of v to this Vector3D. example: v1 = v2 means that values of v2 are set onto v1
	{
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	Vector3D operator+ (Vector3D v)				// operator+ is used to add two Vector3D's. operator+ returns a new Vector3D
	{
		return Vector3D(x + v.x, y + v.y, z + v.z);
	}

	Vector3D operator- (Vector3D v)				// operator- is used to take difference of two Vector3D's. operator- returns a new Vector3D
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	Vector3D operator* (float value)			// operator* is used to scale a Vector3D by a value. This value multiplies the Vector3D's x, y and z.
	{
		return Vector3D(x * value, y * value, z * value);
	}

	Vector3D operator/ (float value)			// operator/ is used to scale a Vector3D by a value. This value divides the Vector3D's x, y and z.
	{
		return Vector3D(x / value, y / value, z / value);
	}

	Vector3D& operator+= (Vector3D v)			// operator+= is used to add another Vector3D to this Vector3D.
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Vector3D& operator-= (Vector3D v)			// operator-= is used to subtract another Vector3D from this Vector3D.
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	Vector3D& operator*= (float value)			// operator*= is used to scale this Vector3D by a value.
	{
		x *= value;
		y *= value;
		z *= value;
		return *this;
	}

	Vector3D& operator/= (float value)			// operator/= is used to scale this Vector3D by a value.
	{
		x /= value;
		y /= value;
		z /= value;
		return *this;
	}


	Vector3D operator- ()						// operator- is used to set this Vector3D's x, y, and z to the negative of them.
	{
		return Vector3D(-x, -y, -z);
	}

    Vector3D cross(Vector3D v)
    {

        return Vector3D(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    }

	float length()								// length() returns the length of this Vector3D
	{
		return sqrtf(x*x + y*y + z*z);
	};

	void unitize()								// unitize() normalizes this Vector3D that its direction remains the same but its length is 1.
	{
		float length = this->length();

		if (length == 0)
			return;

		x /= length;
		y /= length;
		z /= length;
	}

	Vector3D unit()								// unit() returns a new Vector3D. The returned value is a unitized version of this Vector3D.
	{
		float length = this->length();

		if (length == 0)
			return *this;

		return Vector3D(x / length, y / length, z / length);
	}

	//egna tillägg
	Vector3D getNormal(void){
        Vector3D v;
        float i=0,j=0,k=1;
        v.x = this->y*k - this->z*j;
        v.y = this->z*i - this->x*k;
        v.z = this->x*j - this->y*i;
        //v.unitize();
        return v;
	}

	float getAngle(Vector3D u){
	    float angle;
	    Vector3D* v = this;
	    //använder definitionen av dot-product
	    // u (dot) v = |u||v|*cos(theta)
	    // u (dot) v = u.x*v.x + u.y*v.y + u.z*v.z
	    float dot = u.x*v->x + u.y*v->y + u.z*v->z;
	    float magnitude = v->length() * u.length();
	    angle = acos(dot/magnitude);

	    return angle;
	}

};

#endif // VECTOR3D_H_INCLUDED
