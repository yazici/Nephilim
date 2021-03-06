#include <Nephilim/Foundation/Matrix.h>
#include <Nephilim/Foundation/Math.h>
#include <Nephilim/Foundation/Logging.h>
#include <math.h>

NEPHILIM_NS_BEGIN

#include <stdio.h>
void debugPrint(mat4& m)
{
	printf("[%f][%f][%f][%f]\n", m[0], m[4], m[8], m[12]);
	printf("[%f][%f][%f][%f]\n", m[1], m[5], m[9], m[13]);
	printf("[%f][%f][%f][%f]\n", m[2], m[6], m[10], m[14]);
	printf("[%f][%f][%f][%f]\n", m[3], m[7], m[11], m[15]);
}

const mat4 mat4::identity = mat4();

float m3_det( mat3 mat )
{
	float det;

	det = mat[0] * ( mat[4]*mat[8] - mat[7]*mat[5] )
		- mat[1] * ( mat[3]*mat[8] - mat[6]*mat[5] )
		+ mat[2] * ( mat[3]*mat[7] - mat[6]*mat[4] );

	return( det );
}

void m4_submat( mat4 mr, mat3& mb, int i, int j )
{
	int ti, tj, idst, jdst;

	for ( ti = 0; ti < 4; ti++ )
	{
		if ( ti < i )
			idst = ti;
		else
			if ( ti > i )
				idst = ti-1;

		for ( tj = 0; tj < 4; tj++ )
		{
			if ( tj < j )
				jdst = tj;
			else
				if ( tj > j )
					jdst = tj-1;

			if ( ti != i && tj != j )
				mb[idst*3 + jdst] = mr[ti*4 + tj ];
		}
	}
}

float m4_det( mat4 mr )
{
	// test this det now
	/*float det2 = mr[0] * (mr[15] * mr[5] - mr[7] * mr[13]) -
		mr[1] * (mr[15] * mr[4] - mr[7] * mr[12]) +
		mr[3] * (mr[13] * mr[4] - mr[5] * mr[12]);
	return det2;*/

	float  det, result = 0, i = 1;
	mat3 msub3;
	int     n;

	for ( n = 0; n < 4; n++, i *= -1 )
	{
		m4_submat( mr, msub3, 0, n );

		det     = m3_det( msub3 );
		result += mr[n] * det * i;
	}

	return( result );
}

int m4_inverse( mat4& mr, mat4 ma )
{
	mr = mat4::identity; // start as identity then store the inversion

	float  mdet = ma.determinant();

	mat3 mtemp;
	int     i, j, sign;

	if ( fabs( mdet ) < 0.0005 )
		return( 0 );

	for ( i = 0; i < 4; i++ )
	{
		for ( j = 0; j < 4; j++ )
		{
			sign = 1 - ( (i +j) % 2 ) * 2;

			m4_submat( ma, mtemp, i, j );

			mr[i+j*4] = ( m3_det( mtemp ) * sign ) / mdet;
		}
	}  

	// We also get a row major inverse, lets transpose first
	//mr.transpose();

	return( 1 );
}

mat4::mat4()
{
	m_matrix[0] = 1.f; m_matrix[4] = 0.f; m_matrix[8]  = 0.f; m_matrix[12] = 0.f;
	m_matrix[1] = 0.f; m_matrix[5] = 1.f; m_matrix[9]  = 0.f; m_matrix[13] = 0.f;
	m_matrix[2] = 0.f; m_matrix[6] = 0.f; m_matrix[10] = 1.f; m_matrix[14] = 0.f;
	m_matrix[3] = 0.f; m_matrix[7] = 0.f; m_matrix[11] = 0.f; m_matrix[15] = 1.f;
}


////////////////////////////////////////////////////////////
mat4::mat4(	float a00, float a01, float a02, float a03,
			float a10, float a11, float a12, float a13,
			float a20, float a21, float a22, float a23,
			float a30, float a31, float a32, float a33)
{
	m_matrix[0] = a00; m_matrix[4] = a01; m_matrix[8]  = a02; m_matrix[12] = a03;
	m_matrix[1] = a10; m_matrix[5] = a11; m_matrix[9]  = a12; m_matrix[13] = a13;
	m_matrix[2] = a20; m_matrix[6] = a21; m_matrix[10] = a22; m_matrix[14] = a23;
	m_matrix[3] = a30; m_matrix[7] = a31; m_matrix[11] = a32; m_matrix[15] = a33;
}

mat4::mat4(const float* elements)
{
	for(int i = 0; i < 16; i++) m_matrix[i] = elements[i];
}

/// Construct a mat4x4 from a mat3x3 with the fourth line as identity
mat4::mat4(const mat3& m)
{
	// col 1
	m_matrix[0] = m[0];
	m_matrix[1] = m[1];
	m_matrix[2] = 0.f;
	m_matrix[3] = m[2];

	// col 2
	m_matrix[4] = m[3];
	m_matrix[5] = m[4];
	m_matrix[6] = 0.f;
	m_matrix[7] = m[5];

	// col 3
	m_matrix[8] = 0.f;
	m_matrix[9] = 0.f;
	m_matrix[10] = 1.f;
	m_matrix[11] = 0.f;

	// col 4
	m_matrix[12] = m[6];
	m_matrix[13] = m[7];
	m_matrix[14] = 0.f;
	m_matrix[15] = m[8];
}

float mat4::element(int row, int col)
{
	return m_matrix[row + col*4];
}

const float* mat4::get() const
{
	return &m_matrix[0];
}

/// Get the determinant of the 4x4 matrix
float mat4::determinant()
{
	return m4_det(*this);
}

/// Calculate the determinant of the matrix
/// Works only for transforms in 2D, as it ignores
/// there is a third dimension in the matrix
inline float mat4::determinant2d()
{
	return m_matrix[0] * (m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) -
		m_matrix[1] * (m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12]) +
		m_matrix[3] * (m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12]);
}

mat4 mat4::inverse2d()
{
	float det = determinant2d();

	// Compute the inverse if the determinant is not zero
	// (don't use an epsilon because the determinant may *really* be tiny)
	if (det != 0.f)
	{
		return mat4(
			mat3((m_matrix[15] * m_matrix[5] - m_matrix[7] * m_matrix[13]) / det,
			-(m_matrix[15] * m_matrix[4] - m_matrix[7] * m_matrix[12]) / det,
			(m_matrix[13] * m_matrix[4] - m_matrix[5] * m_matrix[12]) / det,
			-(m_matrix[15] * m_matrix[1] - m_matrix[3] * m_matrix[13]) / det,
			(m_matrix[15] * m_matrix[0] - m_matrix[3] * m_matrix[12]) / det,
			-(m_matrix[13] * m_matrix[0] - m_matrix[1] * m_matrix[12]) / det,
			(m_matrix[7] * m_matrix[1] - m_matrix[3] * m_matrix[5]) / det,
			-(m_matrix[7] * m_matrix[0] - m_matrix[3] * m_matrix[4]) / det,
			(m_matrix[5] * m_matrix[0] - m_matrix[1] * m_matrix[4]) / det)
			);
	}
	else
	{
		return mat4::identity;
	}
}


float& mat4::operator[](unsigned int index)
{
	return m_matrix[index];
}

float mat4::operator[](unsigned int index) const
{
	return m_matrix[index];
}

/// Transpose the matrix
void mat4::transpose()
{
	mat4 m = *this;

	m_matrix[0] = m[0]; m_matrix[4] = m[1]; m_matrix[8]  = m[2]; m_matrix[12] = m[3];
	m_matrix[1] = m[4]; m_matrix[5] = m[5]; m_matrix[9]  = m[6]; m_matrix[13] = m[7];
	m_matrix[2] = m[8]; m_matrix[6] = m[9]; m_matrix[10] = m[10]; m_matrix[14] = m[11];
	m_matrix[3] = m[12]; m_matrix[7] = m[13]; m_matrix[11] = m[14]; m_matrix[15] = m[15];
}

/// Get a transposed copy of this matrix
mat4 mat4::transposed()
{
	mat4 m = *this;
	m.transpose();
	return m;
}

/// Multiply by a scalar
mat4 mat4::operator*(float scalar)
{
	return mat4( m_matrix[0] * scalar, m_matrix[4] * scalar, m_matrix[8] * scalar, m_matrix[12] * scalar,
				 m_matrix[1] * scalar, m_matrix[5] * scalar, m_matrix[9] * scalar, m_matrix[13] * scalar,
				 m_matrix[2] * scalar, m_matrix[6] * scalar, m_matrix[10] * scalar, m_matrix[14] * scalar,
				 m_matrix[3] * scalar, m_matrix[7] * scalar, m_matrix[11] * scalar, m_matrix[15] * scalar);
}

/// Multiply the matrix by a vector
vec4 mat4::operator*(const vec4& v) const
{
	return vec4(	m_matrix[0]*v.x + m_matrix[4]*v.y + m_matrix[8]*v.z + m_matrix[12]*v.w,
					m_matrix[1]*v.x + m_matrix[5]*v.y + m_matrix[9]*v.z + m_matrix[13]*v.w,
					m_matrix[2]*v.x + m_matrix[6]*v.y + m_matrix[10]*v.z + m_matrix[14]*v.w,
					m_matrix[3]*v.x + m_matrix[7]*v.y + m_matrix[11]*v.z + m_matrix[15]*v.w);
}

/// Multiply the 4x4 matrix by a vec3. W component is assumed to be 1.0
/// Returns vec3 for convenience
vec3 mat4::operator*(const vec3& v) const
{
	return (*this * vec4(v, 1.0f)).xyz();
}

/// Multiply two 4x4 matrices
mat4 mat4::operator*(const mat4& m) const
{	
	return mat4 ( m_matrix[0]*m[0] + m_matrix[4]*m[1] + m_matrix[8]*m[2] + m_matrix[12]*m[3],
				  m_matrix[0]*m[4] + m_matrix[4]*m[5] + m_matrix[8]*m[6] + m_matrix[12]*m[7],
				  m_matrix[0]*m[8] + m_matrix[4]*m[9] + m_matrix[8]*m[10] + m_matrix[12]*m[11],
				  m_matrix[0]*m[12] + m_matrix[4]*m[13] + m_matrix[8]*m[14] + m_matrix[12]*m[15],

				  m_matrix[1]*m[0] + m_matrix[5]*m[1] + m_matrix[9]*m[2] + m_matrix[13]*m[3],
				  m_matrix[1]*m[4] + m_matrix[5]*m[5] + m_matrix[9]*m[6] + m_matrix[13]*m[7],
				  m_matrix[1]*m[8] + m_matrix[5]*m[9] + m_matrix[9]*m[10] + m_matrix[13]*m[11],
				  m_matrix[1]*m[12] + m_matrix[5]*m[13] + m_matrix[9]*m[14] + m_matrix[13]*m[15],

				  m_matrix[2]*m[0] + m_matrix[6]*m[1] + m_matrix[10]*m[2] + m_matrix[14]*m[3],
				  m_matrix[2]*m[4] + m_matrix[6]*m[5] + m_matrix[10]*m[6] + m_matrix[14]*m[7],
				  m_matrix[2]*m[8] + m_matrix[6]*m[9] + m_matrix[10]*m[10] + m_matrix[14]*m[11],
				  m_matrix[2]*m[12] + m_matrix[6]*m[13] + m_matrix[10]*m[14] + m_matrix[14]*m[15],

				  m_matrix[3]*m[0] + m_matrix[7]*m[1] + m_matrix[11]*m[2] + m_matrix[15]*m[3],
				  m_matrix[3]*m[4] + m_matrix[7]*m[5] + m_matrix[11]*m[6] + m_matrix[15]*m[7],
				  m_matrix[3]*m[8] + m_matrix[7]*m[9] + m_matrix[11]*m[10] + m_matrix[15]*m[11],
				  m_matrix[3]*m[12] + m_matrix[7]*m[13] + m_matrix[11]*m[14] + m_matrix[15]*m[15]  );
}

/// Invert the matrix
void mat4::invert()
{
	*this = inverse();
}

mat4 mat4::ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	mat4 m = mat4::identity;
	m[0] = 2.0f / (right - left);
	m[5] = 2.0f / (top - bottom);
	m[10] = 1.0f / (zNear - zFar);
	m[12] = (left + right) / (left - right);
	m[13] = (top + bottom) / (bottom - top);
	m[14] = zNear / (zNear - zFar);
	return m;
}


mat4 mat4::scale(float x, float y, float z)
{
	mat4 m;

	m[0] = x;
	m[5] = y;
	m[10] = z;

	return m;
}

mat4 mat4::rotatey(float angle)
{
	return mat4(cos(angle),  0.f, sin(angle), 0.f,
				0.f,         1.f, 0.f,        0.f,
				-sin(angle), 0.f, cos(angle), 0.f,
				0.f,         0.f, 0.f,        1.f);
}

mat4 mat4::rotatex(float angle)
{
	return mat4(1.f, 0.f,        0.f,         0.f,
		        0.f, cos(angle), -sin(angle), 0.f,
		        0.f, sin(angle), cos(angle),  0.f,
		        0.f, 0.f,        0.f,         1.f);
}

mat4 mat4::rotatez(float angle)
{
	return mat4(cos(angle),-sin(angle), 0.f,   0.f,
		        sin(angle), cos(angle), 0.f,   0.f,
		        0.f,        0.f,        1.f,   0.f,
		        0.f,        0.f,        0.f,   1.f);
}

mat4 mat4::rotate(float angle_x, float angle_y, float angle_z)
{
	float A       = cos(angle_x);
	float B       = sin(angle_x);
	float C       = cos(angle_y);
	float D       = sin(angle_y);
	float E       = cos(angle_z);
	float F       = sin(angle_z);

	float AD      =   A * D;
	float BD      =   B * D;

	mat4 mat;

	mat[0]  =   C * E;
	mat[4]  =  -C * F;
	mat[8]  =   D;
	mat[1]  =  BD * E + A * F;
	mat[5]  = -BD * F + A * E;
	mat[9]  =  -B * C;
	mat[2]  = -AD * E + B * F;
	mat[6]  =  AD * F + B * E;
	mat[10] =   A * C;

	return mat;
}

mat4 mat4::quaternion(float qx, float qy, float qz, float qw)
{
	const float n = 1.0f/sqrt(qx*qx+qy*qy+qz*qz+qw*qw);
	qx *= n;
	qy *= n;
	qz *= n;
	qw *= n;

	mat4 m(
		1.0f - 2.0f*qy*qy - 2.0f*qz*qz, 2.0f*qx*qy - 2.0f*qz*qw, 2.0f*qx*qz + 2.0f*qy*qw, 0.0f,
		2.0f*qx*qy + 2.0f*qz*qw, 1.0f - 2.0f*qx*qx - 2.0f*qz*qz, 2.0f*qy*qz - 2.0f*qx*qw, 0.0f,
		2.0f*qx*qz - 2.0f*qy*qw, 2.0f*qy*qz + 2.0f*qx*qw, 1.0f - 2.0f*qx*qx - 2.0f*qy*qy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return m;
}


/// Creates a texture matrix to transform texture coordinates
/// Multiplying texture coordinates by this matrix will allow effects such as showing the texture upside-down
/// Also, it allows to convert pixel coordinates to unit coordinates
/// textureTransform(false, 1.f, 1.f) is the default and leaves the texture coordinates unchanged
mat4 mat4::textureTransform(bool flipVertically, float scaleX, float scaleY)
{
	mat4 matrix;

	matrix[0] = 1.f / scaleX;
	matrix[5] = 1.f / scaleY;	

	// If pixels are flipped we must invert the Y axis
	if (flipVertically)
	{
		matrix[5] = -matrix[5];
		//matrix[13] = static_cast<float>(m_size.y / m_actualSize.y);
		matrix[13] = static_cast<float>(scaleY);
	}
	return matrix;
}

bool m44_inv(const float m[16], float *invOut)
{
	float inv[16], det;
	int i;

	inv[0] = m[5] * m[10] * m[15] -
		m[5] * m[11] * m[14] -
		m[9] * m[6] * m[15] +
		m[9] * m[7] * m[14] +
		m[13] * m[6] * m[11] -
		m[13] * m[7] * m[10];

	inv[4] = -m[4] * m[10] * m[15] +
		m[4] * m[11] * m[14] +
		m[8] * m[6] * m[15] -
		m[8] * m[7] * m[14] -
		m[12] * m[6] * m[11] +
		m[12] * m[7] * m[10];

	inv[8] = m[4] * m[9] * m[15] -
		m[4] * m[11] * m[13] -
		m[8] * m[5] * m[15] +
		m[8] * m[7] * m[13] +
		m[12] * m[5] * m[11] -
		m[12] * m[7] * m[9];

	inv[12] = -m[4] * m[9] * m[14] +
		m[4] * m[10] * m[13] +
		m[8] * m[5] * m[14] -
		m[8] * m[6] * m[13] -
		m[12] * m[5] * m[10] +
		m[12] * m[6] * m[9];

	inv[1] = -m[1] * m[10] * m[15] +
		m[1] * m[11] * m[14] +
		m[9] * m[2] * m[15] -
		m[9] * m[3] * m[14] -
		m[13] * m[2] * m[11] +
		m[13] * m[3] * m[10];

	inv[5] = m[0] * m[10] * m[15] -
		m[0] * m[11] * m[14] -
		m[8] * m[2] * m[15] +
		m[8] * m[3] * m[14] +
		m[12] * m[2] * m[11] -
		m[12] * m[3] * m[10];

	inv[9] = -m[0] * m[9] * m[15] +
		m[0] * m[11] * m[13] +
		m[8] * m[1] * m[15] -
		m[8] * m[3] * m[13] -
		m[12] * m[1] * m[11] +
		m[12] * m[3] * m[9];

	inv[13] = m[0] * m[9] * m[14] -
		m[0] * m[10] * m[13] -
		m[8] * m[1] * m[14] +
		m[8] * m[2] * m[13] +
		m[12] * m[1] * m[10] -
		m[12] * m[2] * m[9];

	inv[2] = m[1] * m[6] * m[15] -
		m[1] * m[7] * m[14] -
		m[5] * m[2] * m[15] +
		m[5] * m[3] * m[14] +
		m[13] * m[2] * m[7] -
		m[13] * m[3] * m[6];

	inv[6] = -m[0] * m[6] * m[15] +
		m[0] * m[7] * m[14] +
		m[4] * m[2] * m[15] -
		m[4] * m[3] * m[14] -
		m[12] * m[2] * m[7] +
		m[12] * m[3] * m[6];

	inv[10] = m[0] * m[5] * m[15] -
		m[0] * m[7] * m[13] -
		m[4] * m[1] * m[15] +
		m[4] * m[3] * m[13] +
		m[12] * m[1] * m[7] -
		m[12] * m[3] * m[5];

	inv[14] = -m[0] * m[5] * m[14] +
		m[0] * m[6] * m[13] +
		m[4] * m[1] * m[14] -
		m[4] * m[2] * m[13] -
		m[12] * m[1] * m[6] +
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
		m[1] * m[7] * m[10] +
		m[5] * m[2] * m[11] -
		m[5] * m[3] * m[10] -
		m[9] * m[2] * m[7] +
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
		m[0] * m[7] * m[10] -
		m[4] * m[2] * m[11] +
		m[4] * m[3] * m[10] +
		m[8] * m[2] * m[7] -
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
		m[0] * m[7] * m[9] +
		m[4] * m[1] * m[11] -
		m[4] * m[3] * m[9] -
		m[8] * m[1] * m[7] +
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
		m[0] * m[6] * m[9] -
		m[4] * m[1] * m[10] +
		m[4] * m[2] * m[9] +
		m[8] * m[1] * m[6] -
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}

// Get the inverse of the matrix
mat4 mat4::inverse() const
{
	/*mat4 invertedMatrix;
	m4_inverse(invertedMatrix, *this);
	return invertedMatrix;*/

	// attempt 2
	mat4 inv_result;
	m44_inv(m_matrix, reinterpret_cast<float*>(&inv_result));
	return inv_result;
}

mat4 mat4::perspective(float fov, float aspectRatio, float zNear, float zFar)
{
	float rFov = math::degreeToRadian<float>(fov);

	float cotan = 0.0f, DeltaZ = float(zNear) - float(zFar),
		angle = rFov / 2.0f, sine = sinf(angle);

	cotan = cosf(angle) / sine;

	mat4 m;

	m[0] = cotan / aspectRatio;
	m[5] = cotan;
	m[10] = zFar / DeltaZ;
	m[11] = -1.0f;
	m[14] = zNear * zFar / DeltaZ;
	m[15] = 0.0f;

	return m;
}

mat4 mat4::lookAt(Vec3f eye, Vec3f center, Vec3f up)
{
	Vec3f ZAxis = Vec3f(eye - center);
	ZAxis.normalize();

	Vec3f XAxis = up.cross(ZAxis);
	XAxis.normalize();

	Vec3f YAxis = ZAxis.cross(XAxis);

	mat4 m;

	m[0] = XAxis.x;
	m[1] = YAxis.x;
	m[2] = ZAxis.x;

	m[4] = XAxis.y;
	m[5] = YAxis.y;
	m[6] = ZAxis.y;

	m[8] = XAxis.z;
	m[9] = YAxis.z;
	m[10] = ZAxis.z;

	m[12] = -XAxis.dot(eye);
	m[13] = -YAxis.dot(eye);
	m[14] = -ZAxis.dot(eye);

	return m;
}

mat4 mat4::translate(vec3 v)
{
	return mat4::translate(v.x, v.y, v.z);
}

mat4 mat4::translate(float x, float y, float z)
{
	mat4 m;
	
	m[12] = x;
	m[13] = y;
	m[14] = z;

	return m;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////     3x3 Matrix

/// Default construction
mat3::mat3()
{

}

mat3::mat3(float a00, float a01, float a02,
	       float a10, float a11, float a12,
	       float a20, float a21, float a22)
{
	m_matrix[0] = a00;
	m_matrix[1] = a10;
	m_matrix[2] = a20;

	m_matrix[3] = a01;
	m_matrix[4] = a11;
	m_matrix[5] = a21;

	m_matrix[6] = a02;
	m_matrix[7] = a12;
	m_matrix[8] = a22;
}

/// Access operator for elements
float mat3::operator[](unsigned int index) const
{
	return m_matrix[index];
}

void mat3::transpose()
{
	// Temp matrix
	mat3 m = *this;

	m_matrix[0] = m[0];
	m_matrix[1] = m[3];
	m_matrix[2] = m[6];

	m_matrix[3] = m[1];
	m_matrix[4] = m[4];
	m_matrix[5] = m[7];

	m_matrix[6] = m[2];
	m_matrix[7] = m[5];
	m_matrix[8] = m[8];
}

/// Assign operator
float& mat3::operator[](unsigned int index)
{
	return m_matrix[index];
}

NEPHILIM_NS_END