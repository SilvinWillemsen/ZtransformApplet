//=======================================================================
// Copyright (C) 2003-2013 William Hallahan
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//=======================================================================

//**********************************************************************
//  File: Polynomial.cpp
//  Author: Bill Hallahan
//  Date: January 30, 2003
//
//  Abstract:
//
//    This file contains the implementation for class Polynomial.
//
//**********************************************************************

#include <memory>
#include <float.h>
#include <math.h>
#include <assert.h>
#include <exception>
#include "Polynomial.h"
#include "PolynomialRootFinder.h"

//======================================================================
//  Constructor: Polynomial::Polynomial
//======================================================================

Polynomial::Polynomial()
  : m_degree(-1)
  , m_coefficient_vector_ptr(NULL)
{
    SetToScalar(0.0);
}

//======================================================================
//  Constructor: Polynomial::Polynomial
//
//  Input:
//
//    scalar    A scalar value.
//
//======================================================================

Polynomial::Polynomial(double scalar)
  : m_degree(-1)
  , m_coefficient_vector_ptr(NULL)
{
    SetToScalar(scalar);
}

//======================================================================
//  Constructor: Polynomial::Polynomial
//
//  Input:
//
//    x_coefficient            The coefficient for x term of the
//                             polynomial.
//
//    scalar                   The scalar value of the polynomial.
//
//    Where the resulting polynomial will be in the form of:
//
//        x_coefficient * x + scalar = 0
//
//    degree                    The degree of the polynomial.
//
//======================================================================

Polynomial::Polynomial(double x_coefficient, double scalar)
  : m_degree(-1)
  , m_coefficient_vector_ptr(NULL)
{
    SetToFirstOrderPolynomial(x_coefficient, scalar);
}

//======================================================================
//  Constructor: Polynomial::Polynomial
//
//  Input:
//
//    x_squared_coefficient    The coefficient for x * x term of
//                             the quadratic polynomial.
//
//    x_coefficient            The coefficient for x term of the
//                             quadratic polynomial.
//
//    scalar                   The scalar value of the quadratic
//                             polynomial.
//
//    Where the resulting polynomial will be in the form of:
//
//        x_squared_coefficient * x^2 + x_coefficient * x + scalar = 0
//
//    degree                    The degree of the polynomial.
//
//======================================================================

Polynomial::Polynomial(double x_squared_coefficient, double x_coefficient, double scalar)
  : m_degree(-1)
  , m_coefficient_vector_ptr(NULL)
{
    SetToQuadraticPolynomial(x_squared_coefficient, x_coefficient, scalar);
}

//======================================================================
//  Constructor: Polynomial::Polynomial
//
//  Input:
//
//    coefficient_vector_ptr    The vector of coefficients in order
//                              of increasing powers.
//
//    degree                    The degree of the polynomial.
//
//======================================================================

Polynomial::Polynomial(double * coefficient_vector_ptr, int degree)
  : m_degree(-1)
  , m_coefficient_vector_ptr(NULL)
{
    SetCoefficients(coefficient_vector_ptr, degree);
}

//======================================================================
//  Copy Constructor: Polynomial::Polynomial
//======================================================================

Polynomial::Polynomial(const Polynomial & polynomial)
  : m_degree(-1)
  , m_coefficient_vector_ptr(NULL)
{
    Copy(polynomial);
}

//======================================================================
//  Destructor: Polynomial::~Polynomial
//======================================================================

Polynomial::~Polynomial()
{
}

//======================================================================
//  Member Function: Polynomial::SetCoefficients
//
//  Abstract:
//
//    This method sets the polynomial coefficients.
//
//
//  Input:
//
//    coefficient_vector_ptr    The vector of coefficients in order
//                              of increasing powers.
//
//    degree                    The degree of the polynomial.
//
//
//  Return Value:
//
//    The function has no return value. 
//
//======================================================================

void Polynomial::SetCoefficients(double * coefficient_vector_ptr,
                                 int degree)
{
    assert(degree >= 0);

    m_degree = degree;

    SetLength(m_degree + 1, false);

    int ii = 0;

    for (ii = 0; ii <= m_degree; ++ii)
    {
        m_coefficient_vector_ptr[ii] = coefficient_vector_ptr[ii];
    }

    AdjustPolynomialDegree();
}

//======================================================================
//  Member Function: Polynomial::SetToScalar
//
//  Abstract:
//
//    This method sets the polynomial to be a scalar.
//    The polynomial degree is set to 0 in this method.
//
//
//  Input:
//
//    scalar                    A scalar value
//
//  Return Value:
//
//    The function has no return value. 
//
//======================================================================

void Polynomial::SetToScalar(double scalar)
{
    SetCoefficients(&scalar, 0);
}

//======================================================================
//  Member Function: Polynomial::SetToFirstOrderPolynomial
//
//  Input:
//
//    x_coefficient            The coefficient for x term of the
//                             polynomial.
//
//    scalar                   The scalar value of the polynomial.
//
//    Where the resulting polynomial will be in the form of:
//
//        x_coefficient * x + scalar = 0
//
//    degree                    The degree of the polynomial.
//
//======================================================================

void Polynomial::SetToFirstOrderPolynomial(double x_coefficient, double scalar)
{
    double coefficient_array[2];
    coefficient_array[0] = scalar;
    coefficient_array[1] = x_coefficient;
    SetCoefficients(&coefficient_array[0], 1);
}

//======================================================================
//  Member Function: Polynomial::SetToQuadraticPolynomial
//
//  Input:
//
//    x_squared_coefficient    The coefficient for x * x term of
//                             the quadratic polynomial.
//
//    x_coefficient            The coefficient for x term of the
//                             quadratic polynomial.
//
//    scalar                   The scalar value of the quadratic
//                             polynomial.
//
//    Where the resulting polynomial will be in the form of:
//
//        x_squared_coefficient * x^2 + x_coefficient * x + scalar = 0
//
//    degree                    The degree of the polynomial.
//
//======================================================================

void Polynomial::SetToQuadraticPolynomial(double x_squared_coefficient,
                                          double x_coefficient,
                                          double scalar)
{
    double coefficient_array[3];
    coefficient_array[0] = scalar;
    coefficient_array[1] = x_coefficient;
    coefficient_array[2] = x_squared_coefficient;
    SetCoefficients(&coefficient_array[0], 2);
}

//======================================================================
//  Member Function: Polynomial::EvaluateReal
//
//  Abstract:
//
//    This method evaluates the polynomial using the passed real
//    value x. The algorithm used is Horner's method.
//
//
//  Input:
//
//    xr    A real value.
//
//
//  Return Value:
//
//    This function returns a value of type 'double' that is equal
//    to the polynomial evaluated at the passed value x. 
//
//======================================================================

double Polynomial::EvaluateReal(double xr) const
{
    assert(m_degree >= 0);
    
    double pr = m_coefficient_vector_ptr[m_degree];
    int i = 0;
    
    for (i = m_degree - 1; i >= 0; --i)
    {
        pr = pr * xr + m_coefficient_vector_ptr[i];
    }
    
    return pr;
}

//======================================================================
//  Member Function: Polynomial::EvaluateReal
//
//  Abstract:
//
//    This method evaluates the polynomial using the passed real
//    value x. The algorithm used is Horner's method.
//
//
//  Input:
//
//    xr    A real value.
//
//    dr    A reference to a double which contains the real term
//          that the polynomial derivative evaluates to.
//
//  Return Value:
//
//    This function returns a value of type 'double' that is equal
//    to the polynomial evaluated at the passed value x. 
//
//======================================================================

double Polynomial::EvaluateReal(double xr, double & dr) const
{
    assert(m_degree >= 0);

    double pr = m_coefficient_vector_ptr[m_degree];
    dr = pr;

    int i = 0;

    for (i = m_degree - 1; i > 0; --i)
    {
        pr = pr * xr + m_coefficient_vector_ptr[i];
        dr = dr * xr + pr;
    }

    pr = pr * xr + m_coefficient_vector_ptr[0];

    return pr;
}

//======================================================================
//  Member Function: Polynomial::EvaluateImaginary
//
//  Abstract:
//
//    This function evaluates the value of a polynomial for a
//    specified pure imaginary value xi. The polynomial value
//    is evaluated by Horner's method.
//
//
//  Input:
//
//    xi        A double which equals the imaginary term used to
//              evaluate the polynomial.
//
//  Outputs:
//
//    pr        A reference to a double which contains the real term
//              that the polynomial evaluates to.
//
//    pi        A reference to a double which contains the imaginary
//              term that the polynomial evaluates to.
//
//  Return Value:
//
//    This function has no return value. 
//
//======================================================================

void Polynomial::EvaluateImaginary(double xi,
                                   double & pr,
                                   double & pi) const
{
    assert(m_degree >= 0);

    pr = m_coefficient_vector_ptr[m_degree];
    pi = 0;

    int i = 0;

    for (i = m_degree - 1; i >= 0; --i)
    {
        double temp = -pi * xi + m_coefficient_vector_ptr[i];
        pi = pr * xi;
        pr = temp;
    }

    return;
}

//======================================================================
//  Member Function: Polynomial::EvaluateComplex
//
//  Abstract:
//
//    This function evaluates the value of a polynomial for a
//    specified complex value xr + i xi. The polynomial value
//    is evaluated by Horner's method.
//
//
//  Input:
//
//    xr        A double which equals the real term used to evaluate
//              the polynomial.
//
//    xi        A double which equals the imaginary term used to
//              evaluate the polynomial.
//
//  Outputs:
//
//    pr        A reference to a double which contains the real term
//              that the polynomial evaluates to.
//
//    pi        A reference to a double which contains the imaginary
//              term that the polynomial evaluates to.
//
//  Return Value:
//
//    This function has no return value. 
//
//======================================================================

void Polynomial::EvaluateComplex(double xr,
                                 double xi,
                                 double & pr,
                                 double & pi) const
{
    assert(m_degree >= 0);

    pr = m_coefficient_vector_ptr[m_degree];
    pi = 0;

    int i = 0;

    for (i = m_degree - 1; i >= 0; --i)
    {
        double temp = pr * xr - pi * xi + m_coefficient_vector_ptr[i];
        pi = pr * xi + pi * xr;
        pr = temp;
    }

    return;
}

//======================================================================
//  Member Function: Polynomial::EvaluateComplex
//
//  Abstract:
//
//    This function evaluates the value of a polynomial and the
//    value of the polynomials derivative for a specified complex
//    value xr + i xi. The polynomial value is evaluated by
//    Horner's method. The combination of the polynomial evaluation
//    and the derivative evaluation is known as the Birge-Vieta method.
//
//
//  Input:
//
//    xr        A double which equals the real term used to evaluate
//              the polynomial.
//
//    xi        A double which equals the imaginary term used to
//              evaluate the polynomial.
//
//  Outputs:
//
//    pr        A reference to a double which contains the real term
//              that the polynomial evaluates to.
//
//    pi        A reference to a double which contains the imaginary
//              term that the polynomial evaluates to.
//
//    dr        A reference to a double which contains the real term
//              that the polynomial derivative evaluates to.
//
//    di        A reference to a double which contains the imaginary
//              term that the polynomial derivative evaluates to.
//
//  Return Value:
//
//    This function has no return value. 
//
//======================================================================

void Polynomial::EvaluateComplex(double xr,
                                 double xi,
                                 double & pr,
                                 double & pi,
                                 double & dr,
                                 double & di) const
{
    assert(m_degree >= 0);

    pr = m_coefficient_vector_ptr[m_degree];
    pi = 0;
    dr = pr;
    di = 0;

    double temp = 0.0;
    int i = 0;

    for (i = m_degree - 1; i > 0; --i)
    {
        temp = pr * xr - pi * xi + m_coefficient_vector_ptr[i];
        pi = pr * xi + pi * xr;
        pr = temp;

        temp = dr * xr - di * xi + pr;
        di = dr * xi + di * xr + pi;
        dr = temp;
    }

    temp = pr * xr - pi * xi + m_coefficient_vector_ptr[0];
    pi = pr * xi + pi * xr;
    pr = temp;

    return;
}

//======================================================================
//  Member Function: Polynomial::Derivative
//
//  Abstract:
//
//    This method calculates the derivative polynomial.
//
//
//  Input:
//
//    None
//
//  Return Value:
//
//    This function returns a polynomial that is the derivative
//    of this polynomial.
//
//======================================================================

Polynomial Polynomial::Derivative() const
{
    Polynomial derivative_polynomial;

    //------------------------------------------------------------------
    //  If this polynomial is just a scalar, i.e. it is of degree
    //  zero then the derivative is zero.
    //------------------------------------------------------------------

    assert(m_degree >= 0);

    if (m_degree > 0)
    {
        //--------------------------------------------------------------
        //  Set the size of the buffer for the derivative polynomial.
        //--------------------------------------------------------------

        derivative_polynomial.SetLength(m_degree);

        //--------------------------------------------------------------
        //  Set the degree of the derivative polynomial.
        //--------------------------------------------------------------

        derivative_polynomial.m_degree = m_degree - 1;

        //--------------------------------------------------------------
        //  Calculate the derivative polynomial.
        //--------------------------------------------------------------

        double * temp_ptr = derivative_polynomial.m_coefficient_vector_ptr;

        for (int i = m_degree; i > 0; --i)
        {
            temp_ptr[i - 1] = (double)(i) * m_coefficient_vector_ptr[i];
        }
    }
    else
    {
        derivative_polynomial = 0.0;
    }

    return derivative_polynomial;
}

//======================================================================
//  Member Function: Polynomial::Integral
//
//  Abstract:
//
//    This method calculates the integral polynomial.
//
//
//  Input:
//
//    None
//
//  Return Value:
//
//    This function returns a polynomial that is the integral
//    of this polynomial.
//
//======================================================================

Polynomial Polynomial::Integral() const
{
    Polynomial integral_polynomial;

    //------------------------------------------------------------------
    //  Set the size of the buffer for the integral polynomial.
    //------------------------------------------------------------------

    assert(m_degree >= 0);

    integral_polynomial.SetLength(m_degree + 2);

    //------------------------------------------------------------------
    //  Set the degree of the integral polynomial.
    //------------------------------------------------------------------

    integral_polynomial.m_degree = m_degree + 1;

    //------------------------------------------------------------------
    //  Calculate the integral polynomial.
    //------------------------------------------------------------------

    double * temp_ptr = integral_polynomial.m_coefficient_vector_ptr;
    int i = 0;

    for (i = m_degree; i > 0; --i)
    {
        temp_ptr[i + 1] = m_coefficient_vector_ptr[i] / (double)(i + 1);
    }

    return integral_polynomial;
}

//======================================================================
//  Member Function: Polynomial::Degree
//
//  Abstract:
//
//    This method gets the polynomial degree.
//
//
//  Input:
//
//    None.
//
//
//  Return Value:
//
//    This function returns a value of type 'int' that is the
//    degree of the polynomial.
//
//======================================================================

int Polynomial::Degree() const
{
    return m_degree;
}

//======================================================================
//  Member Function: Polynomial::FindRoots
//
//  Abstract:
//
//    This method determines the roots of a polynomial which has
//    real coefficients.
//
//
//  Input:
//
//
//    real_zero_vector_ptr       A double precision vector that will
//                               contain the real parts of the roots
//                               of the polynomial when this method
//                               returns.
//
//    imaginary_zero_vector_ptr  A double precision vector that will
//                               contain the real parts of the roots
//                               of the polynomial when this method
//                               returns.
//
//    roots_found_ptr           A pointer to an integer that will
//                              equal the number of roots found when
//                              this method returns. If the method
//                              returns SUCCESS then this value will
//                              always equal the degree of the
//                              polynomial.
//
//  Return Value:
//
//    This function returns an enum value of type
//    'PolynomialRootFinder::RootStatus_T'.
//
//======================================================================

PolynomialRootFinder::RootStatus_T Polynomial::FindRoots(double * real_zero_vector_ptr,
                                                         double * imaginary_zero_vector_ptr,
                                                         int * roots_found_ptr) const
{
    assert(m_degree >= 0);

    PolynomialRootFinder * polynomial_root_finder_ptr = new PolynomialRootFinder;

    if (polynomial_root_finder_ptr == NULL)
    {
        throw std::bad_alloc();
    }

    std::auto_ptr<PolynomialRootFinder> root_finder_ptr(polynomial_root_finder_ptr);

    PolynomialRootFinder::RootStatus_T status = root_finder_ptr->FindRoots(m_coefficient_vector_ptr,
                                                                           m_degree,
                                                                           real_zero_vector_ptr,
                                                                           imaginary_zero_vector_ptr,
                                                                           roots_found_ptr);
    return status;
}

//======================================================================
//  Member Function: Polynomial::IncludeRealRoot
//
//  Abstract:
//
//    This method multiplies this polynomial by a first order term
//    that incorporates the real root.
//
//
//  Input:
//
//    real_value    A real root value.
//
//
//  Return Value:
//
//    The function has no return value.
//
//======================================================================

void Polynomial::IncludeRealRoot(double real_value)
{
    if ((m_degree == 0) && (m_coefficient_vector_ptr[0] == 0.0))
    {
        SetToScalar(1.0);
    }

    double coefficient_array[2];
    coefficient_array[0] = -real_value;
    coefficient_array[1] = 1.0;
    Polynomial temp_polynomial(coefficient_array, 1);
    operator *=(temp_polynomial);
    return;
}

//======================================================================
//  Member Function: Polynomial::IncludeComplexConjugateRootPair
//
//  Abstract:
//
//    This method multiplies this polynomial by a second order
//    polynomial that incorporates a complex root pair.
//
//
//  Input:
//
//    real_value    A real root value.
//
//    imag_value    An imaginary root value.
//
//
//  Return Value:
//
//    The function has no return value.
//
//======================================================================

void Polynomial::IncludeComplexConjugateRootPair(double real_value, double imag_value)
{
    if ((m_degree == 0) && (m_coefficient_vector_ptr[0] == 0.0))
    {
        SetToScalar(1.0);
    }

    double coefficient_array[3];
    coefficient_array[0] = real_value * real_value + imag_value * imag_value;
    coefficient_array[1] = -(real_value + real_value);
    coefficient_array[2] = 1.0;
    Polynomial temp_polynomial(coefficient_array, 2);
    operator *=(temp_polynomial);
}

//======================================================================
//  Member Function: Polynomial::Divide
//
//  Abstract:
//
//    This method divides this polynomial by a passed divisor
//    polynomial. The remainder polynomial is stored in the passed
//    instance remainder_polynomial.
//
//
//  Input:
//
//    divisor_polynomial      The divisor polynomial
//
//    quotient_polynomial     A reference to an instance of class
//                            Polynomial that will contain the quotient
//                            polynomial when this method returns.
//
//    remainder_polynomial    A reference to an instance of class
//                            Polynomial that will contain the remainder
//                            polynomial when this method returns.
//
//  Return Value:
//
//    This function returns a value of type 'bool' that false if this
//    method fails. This can only occur if the divisor polynomial is
//    equal to the scalar value zero. Otherwise the return value is
//    true.
//
//======================================================================

bool Polynomial::Divide(const Polynomial & divisor_polynomial,
                        Polynomial & quotient_polynomial,
                        Polynomial & remainder_polynomial) const
{
    //------------------------------------------------------------------
    //  If the divisor is zero then fail.
    //------------------------------------------------------------------

    int divisor_degree = divisor_polynomial.Degree();

    bool non_zero_divisor_flag = ((divisor_polynomial.Degree() != 0)
                                     || (divisor_polynomial[0] != 0.0));

    if (non_zero_divisor_flag)
    {
        //--------------------------------------------------------------
        //  If this dividend polynomial's degree is not greater than
        //  or equal to the divisor polynomial's degree then do the division.
        //--------------------------------------------------------------

        remainder_polynomial = *this;
        int dividend_degree = Degree();
        quotient_polynomial = 0.0;
        int quotient_maximum_degree = dividend_degree - divisor_degree + 1;
        quotient_polynomial.SetLength(quotient_maximum_degree);
        quotient_polynomial.m_degree = -1;
        double * quotient_coefficient_ptr =
            quotient_polynomial.m_coefficient_vector_ptr;
        double * dividend_coefficient_ptr =
            remainder_polynomial.m_coefficient_vector_ptr;
        double leading_divisor_coefficient = divisor_polynomial[divisor_degree];

        //--------------------------------------------------------------
        //  Loop and subtract each scaled divisor polynomial
        //  to perform the division.
        //--------------------------------------------------------------

        int dividend_index = dividend_degree;

        for (dividend_index = dividend_degree;
             dividend_index >= divisor_degree;
             --dividend_index)
        {
            //----------------------------------------------------------
            //  Subtract the scaled divisor from the dividend.
            //----------------------------------------------------------

            double scale_value = remainder_polynomial[dividend_index] / leading_divisor_coefficient;

            //----------------------------------------------------------
            //  Increase the order of the quotient polynomial.
            //----------------------------------------------------------

            quotient_polynomial.m_degree += 1;
            int j = 0;

            for (j = quotient_polynomial.m_degree; j >= 1; --j)
            {
                quotient_coefficient_ptr[j] = quotient_coefficient_ptr[j - 1];
            }

            quotient_coefficient_ptr[0] = scale_value;

            //----------------------------------------------------------
            //  Subtract the scaled divisor from the dividend.
            //----------------------------------------------------------

            int dividend_degree_index = dividend_index;

            for (j = divisor_degree; j >=0; --j)
            {
                dividend_coefficient_ptr[dividend_degree_index] -= divisor_polynomial[j] * scale_value;
                --dividend_degree_index;
            }
        }

        //--------------------------------------------------------------
        //  Adjust the order of the current dividend polynomial.
        //  This is the remainder polynomial.
        //--------------------------------------------------------------

        remainder_polynomial.AdjustPolynomialDegree();
        quotient_polynomial.AdjustPolynomialDegree();
    }
    else
    {
        quotient_polynomial = DBL_MAX;
        remainder_polynomial = 0.0;
    }

    return non_zero_divisor_flag;
}

//======================================================================
//  Member Function: Polynomial::operator []
//
//  Abstract:
//
//    This method returns the specified polynomial coefficient.
//
//
//  Input:
//
//    power_index      The coefficient index.
//
//
//  Return Value:
//
//    This function returns a value of type 'double' that is the
//    coefficient value corresponding to the specified power.
//
//======================================================================

double Polynomial::operator [](int power_index) const
{
    //------------------------------------------------------------------
    //  Ensure that the index is within range.
    //------------------------------------------------------------------

    assert(m_degree >= 0);

    if ((power_index < 0) || (power_index > m_degree))
    {
//        throw std::exception("Polynomial index out of range");
    }

    return m_coefficient_vector_ptr[power_index];
}

//======================================================================
//  Member Function: Polynomial::operator []
//
//  Abstract:
//
//    This method returns the specified polynomial coefficient.
//
//
//  Input:
//
//    power_index      The coefficient index.
//
//
//  Return Value:
//
//    This function returns a value of type 'double' that is the
//    coefficient value corresponding to the specified power.
//
//======================================================================

double & Polynomial::operator [](int power_index)
{
    //------------------------------------------------------------------
    //  Ensure that the index is within range.
    //------------------------------------------------------------------

    assert(m_degree >= 0);

    if ((power_index < 0) || (power_index > m_degree))
    {
//        throw std::exception("Polynomial index out of range");
    }

    return m_coefficient_vector_ptr[power_index];
}

//======================================================================
//  Member Function: Polynomial::operator +=
//
//  Abstract:
//
//    This method adds a polynomial to this polynomial.
//
//
//  Input:
//
//    polynomial    An instance of class Polynomial
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator +=(const Polynomial & polynomial)
{
    assert(m_degree >= 0);

    int i = 0;

    if (m_degree >= polynomial.m_degree)
    {
        for (i = 0; i <= polynomial.m_degree; ++i)
        {
            m_coefficient_vector_ptr[i] += polynomial.m_coefficient_vector_ptr[i];
        }
    }
    else
    {
        SetLength(polynomial.m_degree + 1, true);

        for (i = 0; i <= m_degree; ++i)
        {
            m_coefficient_vector_ptr[i] += polynomial.m_coefficient_vector_ptr[i];
        }

        for (i = m_degree + 1; i <= polynomial.m_degree; ++i)
        {
            m_coefficient_vector_ptr[i] = polynomial.m_coefficient_vector_ptr[i];
        }

        m_degree = polynomial.m_degree;
    }

    //------------------------------------------------------------------
    //  If the leading coefficient(s) are zero, then decrease the
    //  polynomial degree.
    //------------------------------------------------------------------

    AdjustPolynomialDegree();

    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator +=
//
//  Abstract:
//
//    This method adds a scalar to this polynomial.
//
//
//  Input:
//
//    scalar    A scalar value.
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator +=(double scalar)
{
    assert(m_degree >= 0);

    m_coefficient_vector_ptr[0] += scalar;

    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator -=
//
//  Abstract:
//
//    This method subtracts a polynomial from this polynomial.
//
//
//  Input:
//
//    polynomial    An instance of class Polynomial
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator -=(const Polynomial & polynomial)
{
    assert(m_degree >= 0);

    int i = 0;

    if (m_degree >= polynomial.m_degree)
    {
        for (i = 0; i <= polynomial.m_degree; ++i)
        {
            m_coefficient_vector_ptr[i] -= polynomial.m_coefficient_vector_ptr[i];
        }
    }
    else
    {
        SetLength(polynomial.m_degree + 1, true);

        for (i = 0; i <= m_degree; ++i)
        {
            m_coefficient_vector_ptr[i] -= polynomial.m_coefficient_vector_ptr[i];
        }

        for (i = m_degree + 1; i <= polynomial.m_degree; ++i)
        {
            m_coefficient_vector_ptr[i] = -polynomial.m_coefficient_vector_ptr[i];
        }

        m_degree = polynomial.m_degree;
    }

    //------------------------------------------------------------------
    //  If the leading coefficient(s) are zero, then decrease the
    //  polynomial degree.
    //------------------------------------------------------------------

    AdjustPolynomialDegree();

    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator -=
//
//  Abstract:
//
//    This method subtracts a scalar from this polynomial.
//
//
//  Input:
//
//    scalar    A scalar value.
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator -=(double scalar)
{
    assert(m_degree >= 0);

    m_coefficient_vector_ptr[0] -= scalar;

    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator *=
//
//  Abstract:
//
//    This method multiplies a polynomial times this polynomial.
//
//
//  Input:
//
//    polynomial    An instance of class Polynomial
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator *=(const Polynomial & polynomial)
{
    //------------------------------------------------------------------
    //  Create a temporary buffer to hold the product of the two
    //  polynomials.
    //------------------------------------------------------------------

    assert(m_degree >= 0);

    int convolution_length = m_degree + polynomial.m_degree + 1;

    std::vector<double> temp_vector;
    temp_vector.resize(convolution_length + 1);
    double * temp_vector_ptr = &temp_vector[0];

    //------------------------------------------------------------------
    //  Zero the temporary buffer.
    //------------------------------------------------------------------

    int i = 0;

    for (i = 0; i < convolution_length; ++i)
    {
        temp_vector_ptr[i] = 0.0;
    }

    //------------------------------------------------------------------
    //  Calculate the convolution in the temporary buffer.
    //------------------------------------------------------------------

    for (i = 0; i <= m_degree; ++i)
    {
        for (int j = 0; j <= polynomial.m_degree; ++j)
        {
            temp_vector_ptr[i + j] += m_coefficient_vector_ptr[i] * polynomial.m_coefficient_vector_ptr[j];
        }
    }

    //------------------------------------------------------------------
    //  Make sure this buffer is large enough for the product.
    //------------------------------------------------------------------

    SetLength((unsigned int)(convolution_length), false);

    //------------------------------------------------------------------
    //  Store the result in this instance.
    //------------------------------------------------------------------

    m_degree = convolution_length - 1;

    for (i = 0; i <= m_degree; ++i)
    {
        m_coefficient_vector_ptr[i] = temp_vector_ptr[i];
    }

    //------------------------------------------------------------------
    //  If the leading coefficient(s) are zero, then decrease the
    //  polynomial degree.
    //------------------------------------------------------------------

    AdjustPolynomialDegree();

    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator *=
//
//  Abstract:
//
//    This method multiplies a scalar time this polynomial.
//
//
//  Input:
//
//    scalar    A scalar value.
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator *=(double scalar)
{
    assert(m_degree >= 0);

    int i = 0;

    for (i = 0; i <= m_degree; ++i)
    {
        m_coefficient_vector_ptr[i] *= scalar;
    }

    //------------------------------------------------------------------
    //  If the leading coefficient(s) are zero, then decrease the
    //  polynomial degree.
    //------------------------------------------------------------------

    AdjustPolynomialDegree();

    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator /=
//
//  Abstract:
//
//    This method divides this polynomial by a scalar.
//
//
//  Input:
//
//    scalar    A scalar value.
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator /=(double scalar)
{
    assert(m_degree >= 0);

    int i = 0;

    for (i = 0; i <= m_degree; ++i)
    {
        m_coefficient_vector_ptr[i] /= scalar;
    }

    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator +
//
//  Abstract:
//
//    This method implements unary operator +()
//
//
//  Input:
//
//    None.
//
//
//  Return Value:
//
//    This function returns a polynomial which is equal to this instance.
//
//======================================================================

Polynomial Polynomial::operator +()
{
    assert(m_degree >= 0);
    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator -
//
//  Abstract:
//
//    This method implements unary operator -().
//    This polynomials coefficients became the negative of
//    their present value and then this polynomial is returned.
//
//
//  Input:
//
//    None.
//
//
//  Return Value:
//
//    This function returns a polynomial which is the negative of
//    this instance.
//
//======================================================================

Polynomial Polynomial::operator -()
{
    assert(m_degree >= 0);

    for (int i = 0; i <= m_degree; ++i)
    {
        m_coefficient_vector_ptr[i] = -m_coefficient_vector_ptr[i];
    }

    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator =
//
//  Abstract:
//
//    This method sets this polynomial to a scalar value.
//
//
//  Input:
//
//    scalar    A scalar value.
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator =(double scalar)
{
    SetCoefficients(&scalar, 0);
    return *this;
}

//======================================================================
//  Member Function: Polynomial::operator =
//
//  Abstract:
//
//    This method copies this polynomial.
//
//
//  Input:
//
//    polynomial    An instance of class Polynomial
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

Polynomial Polynomial::operator =(const Polynomial & polynomial)
{
    if (this != &polynomial)
    {
        Copy(polynomial);
    }

    return *this;
}

//======================================================================
//  Member Function: Polynomial::AdjustPolynomialDegree
//
//  Abstract:
//
//    This method will decrease the polynomial degree until leading
//    coefficient is non-zero or until the polynomial degree is zero.
//
//
//  Input:
//
//    None.
//
//
//  Return Value:
//
//    This method has no return value.
//
//======================================================================

void Polynomial::AdjustPolynomialDegree()
{
    //------------------------------------------------------------------
    //  Any leading coefficient with a magnitude less than DBL_EPSILON
    //  is treated as if it was zero.
    //------------------------------------------------------------------

    while ((m_degree > 0)
        && (fabs(m_coefficient_vector_ptr[m_degree]) < DBL_EPSILON))
    {
        m_coefficient_vector_ptr[m_degree] = 0.0;
        m_degree--;
    }

    return;
}

//======================================================================
//  Member Function: Polynomial::Copy
//
//  Abstract:
//
//    This method copies a passed polynomial into this instance.
//
//
//  Input:
//
//    polynomial    An instance of class Polynomial.
//
//
//  Return Value:
//
//    This function returns this polynomial.
//
//======================================================================

void Polynomial::Copy(const Polynomial & polynomial)
{
    SetLength(polynomial.m_degree + 1);

    m_degree = polynomial.m_degree;

    for (int i = 0; i <= m_degree; ++i)
    {
        m_coefficient_vector_ptr[i] = polynomial.m_coefficient_vector_ptr[i];
    }

    return;
}

//======================================================================
//  Member Function: Polynomial::SetLength
//
//  Abstract:
//
//    This function is called to set the buffer lengths for the
//    coefficient vector. If the new buffer length is less than
//    or equal to the current buffer lengths then then the buffer
//    is not modified and the data length is set to the new buffer
//    length. If the new data length is greater than the current
//    buffer lengths then the buffer is reallocated to the new
//    buffer size. In this case, if argument copy_data_flag
//    is set to the value true then the data in the old buffer
//    is copied to the new buffer.
//
//
//  Input:
//
//    udata_length             The new length of the data.
//
//    copy_data_flag           If this is true then existing data
//                             is copied to any newly allocated buffer.
//                             This parameter defaults to the value
//                             'true'.
//
//  Output:
//
//    This function has no return value.
//
//======================================================================

void Polynomial::SetLength(unsigned int number_of_coefficients,
                           bool copy_data_flag)
{

    // If m_degree is equal to -1, then this is a new polynomial and the
    // caller will set m_degree.
    if ((!copy_data_flag) || (m_degree == -1))
    {
        // Clear and resize the coefficient vector.
        m_coefficient_vector.clear();
        m_coefficient_vector.resize(number_of_coefficients);
        m_coefficient_vector_ptr = &m_coefficient_vector[0];
    }
    else
    {
        // Save the polynomial values in a temporary vector.
        std::vector<double> temp_vector;
        temp_vector.resize(m_degree + 1);

        int i = 0;

        for (i = 0; i <= m_degree; ++i)
        {
            temp_vector[i] = m_coefficient_vector_ptr[i];
        }

        // Clear and resize the coefficient vector.
        m_coefficient_vector.clear();
        m_coefficient_vector.resize(number_of_coefficients);
        m_coefficient_vector_ptr = &m_coefficient_vector[0];

        // Restore the coefficients for the new vector size.
        // Was the polynomial size increased?
        if (number_of_coefficients > (unsigned int)(m_degree + 1))
        {
            // The polynomial size was increased.
            for (i = 0; i <= m_degree; ++i)
            {
                m_coefficient_vector_ptr[i] = temp_vector[i];
            }

            for (i = m_degree + 1; i < (int)(number_of_coefficients); ++i)
            {
                m_coefficient_vector_ptr[i] = 0.0;
            }
        }
        else
        {
            for (int i = 0; i < (int)(number_of_coefficients); ++i)
            {
                m_coefficient_vector_ptr[i] = temp_vector[i];
            }
        }
    }

    return;
}

//======================================================================
//  Global operators
//======================================================================

//======================================================================
//  Addition of two instances of this class.
//======================================================================

Polynomial operator +(const Polynomial & polynomial_0,
                      const Polynomial & polynomial_1)
{
    return Polynomial(polynomial_0) += polynomial_1;
}

//======================================================================
//  Addition of an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator +(const Polynomial & polynomial,
                      double scalar)
{
    return Polynomial(polynomial) += scalar;
}

Polynomial operator +(double scalar,
                      const Polynomial & polynomial)
{
    return Polynomial(polynomial) += scalar;
}

//======================================================================
//  Subtraction of two instances of this class.
//======================================================================

Polynomial operator -(const Polynomial & minuend_polynomial,
                      const Polynomial & subtrahend_polynomial)
{
    return Polynomial(minuend_polynomial) -= subtrahend_polynomial;
}

//======================================================================
//  Subtraction with an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator -(const Polynomial & minuend_polynomial,
                      double scalar)
{
    return Polynomial(minuend_polynomial) -= scalar;
}

Polynomial operator -(double scalar,
                      const Polynomial & polynomial)
{
    return (-Polynomial(polynomial)) + scalar;
}

//======================================================================
//  Multiplication of two instances of this class.
//======================================================================

Polynomial operator *(const Polynomial & polynomial_0,
                      const Polynomial & polynomial_1)
{
    return Polynomial(polynomial_0) *= polynomial_1;
}

//======================================================================
//  Multiplication of an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator *(const Polynomial & polynomial,
                      double scalar)
{
    return Polynomial(polynomial) *= scalar;
}

Polynomial operator *(double scalar,
                      const Polynomial & polynomial)
{
    return Polynomial(polynomial) *= scalar;
}

//======================================================================
//  Division with an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator /(const Polynomial & polynomial,
                      double scalar)
{
    return Polynomial(polynomial) /= scalar;
}
