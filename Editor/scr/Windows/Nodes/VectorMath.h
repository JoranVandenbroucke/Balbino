#pragma once
#include "INode.h"

#include <array>

class CVectorMath final : public INode
{
public:
	CVectorMath( const int id, int& attributeStartId );

	~CVectorMath() override;
	void Draw() override;
	void Attach( int endAttr ) override;
	void Detach( int endAttr ) override;
	void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType ) override;
	[[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
	[[nodiscard]] int GetId() const override;
	[[nodiscard]] std::vector<int> GetInputs() const override;
private:
	enum class EMode
	{
        //The sum of A and B.
        Add,

        //The difference between A and B.
        Subtract,

        //The entrywise product of A and B.
        Multiply,

        //The entrywise division of A by B.Division by zero results in zero.
        Divide,

        //The entrywise combination of the multiplyand addition operations.
        MultiplyAdd,

        //The cross product of A and B.
        CrossProduct,

        //The projection of A onto B.
        Project,

        //The reflection of A around the normal B.B need not be normalized.
        Reflect,

        //For a given incident vector A, surface normal Band ratio of indices of refraction( IOR ), refract outputs the refraction vector R.
        Refract,

        //Orients a vector A to point away from a surface B as defined by its normal C.Computes
        Faceforward,


        //The dot product of A and B.
        DotProduct,

        //The distance between A and B.
        Distance,

        //The length of A.
        Length,

        //The result of multiplying A by the scalar input Scale.
        Scale,

        //The result of normalizing A.The result vector points to the same direction as A and has a length of 1. If A is( 0, 0, 0 ), the result is( 0, 0, 0 ) as well.
        Normalize,

        //Wrap.
        Wrap,

        //The result of rounding A to the largest integer multiple of B less than or equal A.
        Snap,

        //The entrywise floor of A.
        Floor,

        //The entrywise ceiling of A.
        Ceil,

        //The entrywise modulo of A by B.
        Modulo,

        //The fractional part of A.
        Fraction,

        //The entrywise absolute value of A.
        Absolute,

        //The entrywise minimum from A and B.
        Minimum,

        //The entrywise maximum from A and B.
        Maximum,

        //The entrywise Sine of A.
        Sine,

        //The entrywise Cosine of A.
        Cosine,

        //The entrywise Tangent of A.
        Tangent,
        MaxIndex
	}m_mode;
    bool m_connected[3];
    std::array<float, 3> m_vectorA;
    std::array<float, 3> m_vectorB;
    std::array<float, 3> m_vectorC;
    float m_fac;

    static const char* ToString( EMode type );
};
