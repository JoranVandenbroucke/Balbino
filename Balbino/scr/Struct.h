#pragma once
#include <cstdint>
#include <limits>

namespace Balbino
{
	class Transform;
	class Rigidbody2D;
	class Collider2D;

	struct comp
	{
		template<typename T1, typename T2>
		bool operator()( const std::pair<T1, T2>& p ) const
		{
			return std::hash<T1>{}( p.first ) ^ std::hash<T2>{}( p.second );
		}
	};

	struct Color
	{
		Color( unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255 );
		Color( uint32_t color );

		unsigned char r, g, b, a;
	};

	struct Vector2
	{
		Vector2( float x = 0.f, float y = 0.f );
		float x, y;

		Vector2& operator+=( const Vector2& rhs );
		friend Vector2 operator+( Vector2 lhs, const Vector2& rhs );
	};

	//Unity3D (https://docs.unity3d.com/ScriptReference/Vector3.html)
	struct Vector3
	{
		Vector3( float x = 0.f, float y = 0.f, float z = 0.f );
		float x, y, z;

		float Magnitude();				//Returns the length of this vector.
		Vector3 Normalized();			//Returns this vector with a magnitude of 1.
		float SqrMagnitude();			//Returns the squared length of this vector( Read Only ).

		Vector3& operator+=( const Vector3& rhs );
		Vector3& operator-=( const Vector3& rhs );
		Vector3& operator/=( float rhs );
		Vector3& operator*=( float rhs );
		friend Vector3 operator+( Vector3 lhs, const Vector3& rhs );
		friend Vector3 operator-( Vector3 lhs, const Vector3& rhs );
		friend Vector3 operator/( Vector3 lhs, float rhs );
		friend Vector3 operator*( Vector3 lhs, float rhs );
		friend bool operator!=( Vector3 lhs, const Vector3& rhs );
		friend bool operator==( Vector3 lhs, const Vector3& rhs );

		static const Vector3 Back()
		{
			return Vector3{ 0.f,0.f,-1.f };
		}				//Shorthand for writing Vector3( 0, 0, -1 ).
		static const Vector3 Down()
		{
			return Vector3{ 0.f,-1.f,0.f };
		}				//Shorthand for writing Vector3( 0, -1, 0 ).
		static const Vector3 Forward()
		{
			return Vector3{ 0.f,0.f,1.f };
		}			//Shorthand for writing Vector3( 0, 0, 1 ).
		static const Vector3 Left()
		{
			return Vector3{ -1.f,0.f,1.f };
		}				//Shorthand for writing Vector3( -1, 0, 0 ).
		static const Vector3 NegativeInfinity()
		{
			return Vector3{ -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity() };
		}	//Shorthand for writing Vector3( float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity ).
		static const Vector3 One()
		{
			return Vector3{ 1.f,1.f,1.f };
		}				//Shorthand for writing Vector3( 1, 1, 1 ).
		static const Vector3 PositiveInfinity()
		{
			return Vector3{ std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
		}	//Shorthand for writing Vector3( float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity ).
		static const Vector3 Right()
		{
			return Vector3{ 1.f,0.f,0.f };
		}				//Shorthand for writing Vector3( 1, 0, 0 ).
		static const Vector3 Up()
		{
			return Vector3{ 0.f,1.f,0.f };
		}					//Shorthand for writing Vector3( 0, 1, 0 ).
		static const Vector3 Zero()
		{
			return Vector3{ 0.f,0.f,0.f };
		}				//Shorthand for writing Vector3( 0, 0, 0 ).
	};

	struct Vector4
	{
		Vector4( float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f );
		float x, y, z, w;

		Vector4& operator+=( const Vector4& rhs );
		friend Vector4 operator+( Vector4 lhs, const Vector4& rhs );
	};

	struct Rect
	{
		Rect( float xMin = 0.f, float xMax = 0.f, float yMin = 0.f, float yMax = 0.f );
		union
		{
			float xMin;
			float left;
		};
		union
		{
			float xMax;
			float right;
		};
		union
		{
			float yMin;
			float top;
		};
		union
		{
			float yMax;
			float bottom;
		};
		float width, height;

		Rect& operator+=( const Rect& rhs );
		friend Rect operator+( Rect lhs, const Rect& rhs );
	};

	//https://docs.unity3d.com/ScriptReference/Ray.html
	struct Ray
	{
		Ray( const Balbino::Vector3& origine, const Balbino::Vector3& direction );

		Balbino::Vector3 origine;
		Balbino::Vector3 direction;
	};

	//https://docs.unity3d.com/ScriptReference/RaycastHit.html
	struct RaycastHit
	{
		const Collider2D* collider;

		Vector3 distance;
		Vector3 point;
		Rigidbody2D* rigidbody;

		Transform* transform;
	};

	enum class ComponentList
	{
		Audio,
		LoggedAudio,
		Avatar,
		BoxCollider2D,
		Camera,
		FPSScript,
		LevelLoader,
		Rigidbody2D,
		Sprite,
		Text,
		Transform
	};
}