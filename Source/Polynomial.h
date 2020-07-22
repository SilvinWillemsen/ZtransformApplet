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
//  File: Polynomial.h
//  Author: Bill Hallahan
//  Date: January 30, 2003
//
//  Abstract:
//
//    This file contains the definition for class Polynomial.
//
//**********************************************************************

#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <vector>
#include "PolynomialRootFinder.h"

//======================================================================
//  Class definition.
//======================================================================

class Polynomial
{
protected:

    std::vector<double> m_coefficient_vector;
    int m_degree;
    double * m_coefficient_vector_ptr;

public:

    Polynomial();

    Polynomial(double scalar);

    Polynomial(double x_coefficient, double scalar);

    Polynomial(double x_squared_coefficient,
               double x_coefficient,
               double scalar);

    Polynomial(double * coefficient_vector, int degree);

    Polynomial(const Polynomial & polynomial);

    virtual ~Polynomial();

    void SetCoefficients(double * coefficient_vector_ptr,
                         int degree);

    void SetToScalar(double scalar);

    void SetToFirstOrderPolynomial(double x_coefficient, double scalar);

    void SetToQuadraticPolynomial(double x_squared_coefficient,
                                  double x_coefficient,
                                  double scalar);

    double EvaluateReal(double xr) const;

    double EvaluateReal(double xr, double & dr) const;

    void EvaluateImaginary(double xi,
                           double & pr,
                           double & pi) const;

    void EvaluateComplex(double xr,
                         double xi,
                         double & pr,
                         double & pi) const;

    void EvaluateComplex(double xr,
                         double xi,
                         double & pr,
                         double & pi,
                         double & dr,
                         double & di) const;

    Polynomial Derivative() const;

    Polynomial Integral() const;

    int Degree() const;

    PolynomialRootFinder::RootStatus_T FindRoots(double * real_zero_vector_ptr,
                                                 double * imaginary_zero_vector_ptr,
                                                 int * roots_found_ptr = 0) const;

    void IncludeRealRoot(double real_value);

    void IncludeComplexConjugateRootPair(double real_value, double imag_value);

    bool Divide(const Polynomial & divisor_polynomial,
                Polynomial & quotient_polynomial,
                Polynomial & remainder_polynomial) const;

    double operator [](int power_index) const;

    double & operator [](int power_index);

    Polynomial operator +=(const Polynomial & polynomial);

    Polynomial operator +=(double scalar);

    Polynomial operator -=(const Polynomial & polynomial);

    Polynomial operator -=(double scalar);

    Polynomial operator *=(const Polynomial & polynomial);

    Polynomial operator *=(double scalar);

    Polynomial operator /=(double scalar);

    Polynomial operator +();

    Polynomial operator -();

    Polynomial operator =(double scalar);

    Polynomial operator =(const Polynomial & polynomial);

private:

    void AdjustPolynomialDegree();

    void Copy(const Polynomial & polynomial);

    void SetLength(unsigned int number_of_coefficients,
                   bool copy_data_flag = true);
};

//======================================================================
//  Global operators.
//======================================================================

//======================================================================
//  Addition of two instances of this class.
//======================================================================

Polynomial operator +(const Polynomial & polynomial_0,
                      const Polynomial & polynomial_1);

//======================================================================
//  Addition of an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator +(const Polynomial & polynomial,
                      double scalar);

Polynomial operator +(double scalar,
                      const Polynomial & polynomial);

//======================================================================
//  Subtraction of two instances of this class.
//======================================================================

Polynomial operator -(const Polynomial & minuend_polynomial,
                      const Polynomial & subtrahend_polynomial);

//======================================================================
//  Subtraction with an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator -(const Polynomial & minuend_polynomial,
                      double scalar);

Polynomial operator -(double scalar,
                      const Polynomial & polynomial);

//======================================================================
//  Multiplication of two instances of this class.
//======================================================================

Polynomial operator *(const Polynomial & polynomial_0,
                      const Polynomial & polynomial_1);

//======================================================================
//  Multiplication of an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator *(const Polynomial & polynomial,
                      double scalar);

Polynomial operator *(double scalar,
                      const Polynomial & polynomial);

//======================================================================
//  Division with an instance of the Polynomial class and a scalar.
//======================================================================

Polynomial operator /(const Polynomial & polynomial,
                      double scalar);
#endif
