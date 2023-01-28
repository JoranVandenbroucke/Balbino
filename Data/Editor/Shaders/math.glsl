#include "mathUtil.glsl"
//Functions
//The sum of the two values.
float MathAdd(float a, float b)
{
    return a + b;
}
//The difference between the two values.
float MathSubtract(float a, float b)
{
    return a - b;
}
//The product of the two values.
float MathMultiply(float a, float b)
{
    return a * b;
}
//The division of the first value by the second value.
float MathDivide(float a, float b)
{
    return SafeDivide(a,b);
}
//The sum of the product of the two values with Addend.
float MathMultiplyAdd(float a, float b, float c)
{   
    return a * b + c;
}
//The Base raised to the power of Exponent.
float MathPower(float a, float b)
{
    if (a >= 0.0)
    {
        return CompatiblePow(a, b);
    }
    else
    {
        float fraction = mod(abs(b), 1.0);
        if (fraction > 0.999 || fraction < 0.001)
        {
            return CompatiblePow(a, floor(b + 0.5));
        }
        else
        {
            return 0.0;
        }
    }
}
//The log of the value with a Base as its base.
float MathLogarithm(float a, float b)
{
    return (a > 0.0 && b > 0.0) ? log2(a) / log2(b) : 0.0;
}
 //The square root of the value.
float MathSquareRoot(float a)
{
    return (a > 0.0) ? sqrt(a) : 0.0;
}
//One divided by the square root of the value.
float MathInverseSquareRoot(float a)
{
    return inversesqrt(a);
}
//The input value is read without regard to its sign.This turns negative values into positive values.
float MathAbsolute(float a)
{
    return abs(a);
}
//Raises Euler`s number to the power of the value.
float MathExponent(float a)
{
    return exp(a);
}

//Comparison
//Outputs the smallest of the input values.
float MathMinimum(float a, float b)
{
    return min(a, b);
}
//Outputs the largest of two input values.
float MathMaximum(float a, float b)
{
    return max(a, b);
}
//Outputs 1.0 if the first value is smaller than the second value.Otherwise the output is 0.0.
float MathLessThan(float a, float b)
{
    return (a < b) ? 1.0 : 0.0;
}
//Outputs 1.0 if the first value is larger than the second value.Otherwise the output is 0.0.
float MathGreaterThan(float a, float b)
{
    return (a > b) ? 1.0 : 0.0;
}
//Extracts the sign of the input value. All positive numbers will output 1.0. All negative numbers will output - 1.0. And 0.0 will output 0.0.
float MathSign(float a)
{
    return sign(a);
}
//Outputs 1.0 if the difference between the two input values is less than or equal to Epsilon.
float MathCompare(float a, float b, float c)
{
    return (abs(a - b) <= max(c, 1e-5)) ? 1.0 : 0.0;
}
//Smooth Minimum.
float MathSmoothMinimum(float a, float b, float c)
{
    if (c != 0.0) {
        float h = max(c - abs(a - b), 0.0) / c;
        return min(a, b) - h * h * h * c * (1.0 / 6.0);
    }
    else
    {
        return min(a, b);
    }
}
float MathSmoothMaximum(float a, float b, float c)
{
    float maximum = max(a, b);
	float minimum = min(a, b);
	return maximum + log( 1.0 + exp(minimum - maximum) ) * c;
}
//Smooth Maximum.

//Rounding
//Rounds the input value to the nearest integer.
float MathRound(float a)
{
    return floor(a + 0.5f);
}
//Rounds the input value down to the nearest integer.
float MathFloor(float a)
{
    return floor(a + 0.5f);
}
//Rounds the input value up to the nearest integer.
float MathCeil(float a)
{
    return ceil(a + 0.5f);
}
//Outputs the integer part of the value.
float MathTruncate(float a)
{
    return trunc(a);
}
//Fraction.
float MathFraction(float a)
{
    return a - floor(a);
}
//Outputs the remainder once the first value is divided by the second value.
float MathModulo(float a, float b)
{
    return CompatibleFmod( a, b );
}
//Outputs a value between Min and Max based on the absolute difference between the input value and the nearest integer multiple of Max less than the value.
float MathWrap(float a, float b, float c)
{
    return Wrap(a,b,c);
}
float MathSnap(float a, float b)
{
    return floor(SafeDivide(a, b)) * b;
}               //Rounds the input value down to the nearest integer multiple of Increment.
//The output value is moved between 0.0 and the Scale based on the input value.
float MathPingPong(float a, float b)
{
    return (b != 0.0) ? abs(fract((a - b) / (b * 2.0)) * b * 2.0 - b) : 0.0;
}

//Trigonometric
//The Sine of the input value.
float MathSine(float a)
{
    return sin(a);
}
//The Cosine of the input value.
float MathCosine(float a)
{
    return cos(a);
}
//The Tangent of the input value.
float MathTangent(float a)
{
    return tan(a);
}
//The Arcsine of the input value.
float MathArcsine(float a)
{
    return asin(a);    
}
//The Arccosine of the input value.
float MathArccosine(float a)
{
    return acos(a);    
}
//The Arctangent of the input value.
float MathArctangent(float a)
{
    return atan(a);    
}
//Outputs the Inverse Tangent of the first value divided by the second value measured in radians.
float MathArctan2(float a, float b)
{
    return atan(a, b);    
}
//The Hyperbolic Sine of the input value.
float MathHyperbolicSine(float a)
{
    return sinh(a);
}
//The Hyperbolic Cosine of the input value.
float MathHyperbolicCosine(float a)
{
    return cosh(a);
}
//The Hyperbolic Tangent of the input value.
float MathHyperbolicTangent(float a)
{
    return tanh(a);
}

//Conversion
//Converts the input from degrees to radians.
float MathToRadians(float a)
{
    return radians(a);
}
//Converts the input from radians to degrees.
float MathToDegrees(float a)
{
    return degrees(a);
}