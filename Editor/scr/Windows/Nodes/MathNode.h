#pragma once
#include "INode.h"

class CMathNode final : public INode
{
public:
    explicit CMathNode(int id, int& attributeStartId );
    ~CMathNode() override = default;
    CMathNode( const CMathNode& ) = default;
    CMathNode( CMathNode&& ) = delete;
    CMathNode& operator=( const CMathNode& ) = delete;
    CMathNode& operator=( CMathNode&& ) = delete;

    void Draw() override;
    void Attach( int endAttr ) override;
    void Detach( int endAttr ) override;
    void Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType = EAttributeType::None ) override;
    [[nodiscard]] bool HasFreeAttachment( int endAttr ) const override;
    [[nodiscard]] int GetId() const override;
    [[nodiscard]] std::vector<int> GetInputs() const override;
private:
    enum class EMode
    {
        //Functions
        Add,                //The sum of the two values.
        Subtract,           //The difference between the two values.
        Multiply,           //The product of the two values.
    	Divide,             //The division of the first value by the second value.
        MultiplyAdd,        //The sum of the product of the two values with Addend.
    	Power,              //The Base raised to the power of Exponent.
        Logarithm,          //The log of the value with a Base as its base.
        SquareRoot,         //The square root of the value.
        InverseSquareRoot,  //One divided by the square root of the value.
        Absolute,           //The input value is read without regard to its sign.This turns negative values into positive values.
        Exponent,           //Raises Euler’s number to the power of the value.

        //Comparison
        Minimum,            //Outputs the smallest of the input values.
        Maximum,            //Outputs the largest of two input values.
        LessThan,           //Outputs 1.0 if the first value is smaller than the second value.Otherwise the output is 0.0.
        GreaterThan,        //Outputs 1.0 if the first value is larger than the second value.Otherwise the output is 0.0.
        Sign,               //Extracts the sign of the input value. All positive numbers will output 1.0. All negative numbers will output - 1.0. And 0.0 will output 0.0.
        Compare,            //Outputs 1.0 if the difference between the two input values is less than or equal to Epsilon.
        SmoothMinimum,      //Smooth Minimum.
        SmoothMaximum,      //Smooth Maximum.

        //Rounding
        Round,              //Rounds the input value to the nearest integer.
        Floor,              //Rounds the input value down to the nearest integer.
        Ceil,               //Rounds the input value up to the nearest integer.
        Truncate,           //Outputs the integer part of the value.
        Fraction,           //Fraction.
        Modulo,             //Outputs the remainder once the first value is divided by the second value.
        Wrap,               //Outputs a value between Min and Max based on the absolute difference between the input value and the nearest integer multiple of Max less than the value.
        Snap,               //Rounds the input value down to the nearest integer multiple of Increment.
        PingPong,           //The output value is moved between 0.0 and the Scale based on the input value.

        //Trigonometric
        Sine,               //The Sine of the input value.
        Cosine,             //The Cosine of the input value.
        Tangent,            //The Tangent of the input value.
        Arcsine,            //The Arcsine of the input value.
        Arccosine,          //The Arccosine of the input value.
        Arctangent,         //The Arctangent of the input value.
        Arctan2,            //Outputs the Inverse Tangent of the first value divided by the second value measured in radians.
        HyperbolicSine,     //The Hyperbolic Sine of the input value.
        HyperbolicCosine,   //The Hyperbolic Cosine of the input value.
        HyperbolicTangent,  //The Hyperbolic Tangent of the input value.

        //Conversion
        ToRadians,          //Converts the input from degrees to radians.
        ToDegrees,          //Converts the input from radians to degrees.

        MaxIndex
    }m_currentMode;
    static const char* ToString( EMode e );
    bool m_clamp;
    float m_a{ 1.0f }, m_b{ 1.0f }, m_c{ 1.0f };
    bool m_connectedA{}, m_connectedB{}, m_connectedC{};
};
