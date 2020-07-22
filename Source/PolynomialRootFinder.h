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
//  File: PolynomialRootFinder.h
//  Author: Bill Hallahan
//  Date: January 30, 2003
//
//  Abstract:
//
//    This file contains the definition for class PolynomialRootFinder.
//
//**********************************************************************

#ifndef POLYNOMIALROOTFINDER_H
#define POLYNOMIALROOTFINDER_H

#include <vector>

//======================================================================
//  Class definition.
//======================================================================

class PolynomialRootFinder
{
protected:

    typedef double PRF_Float_T;

    std::vector<double> m_p_vector;
    std::vector<double> m_qp_vector;
    std::vector<double> m_k_vector;
    std::vector<double> m_qk_vector;
    std::vector<double> m_svk_vector;

    double * m_p_vector_ptr;
    double * m_qp_vector_ptr;
    double * m_k_vector_ptr;
    double * m_qk_vector_ptr;
    double * m_svk_vector_ptr;

    int m_degree;
    int m_n;
    int m_n_plus_one;
    double m_real_s;
    double m_imag_s;
    double m_u;
    double m_v;
    double m_a;
    double m_b;
    double m_c;
    double m_d;
    double m_a1;
    double m_a2;
    double m_a3;
    double m_a6;
    double m_a7;
    double m_e;
    double m_f;
    double m_g;
    double m_h;
    double m_real_sz;
    double m_imag_sz;
    double m_real_lz;
    double m_imag_lz;
    PRF_Float_T m_are;
    PRF_Float_T m_mre;

public:

    enum RootStatus_T
    {
        SUCCESS,
        LEADING_COEFFICIENT_IS_ZERO,
        SCALAR_VALUE_HAS_NO_ROOTS,
        FAILED_TO_CONVERGE
    };

    PolynomialRootFinder();

    virtual ~PolynomialRootFinder();

    PolynomialRootFinder::RootStatus_T FindRoots(double * coefficient_ptr,
                                                 int degree,
                                                 double * real_zero_vector_ptr,
                                                 double * imaginary_zero_vector_ptr,
                                                 int * number_of_roots_found_ptr = 0);

private:

    int Fxshfr(int l2var);

    int QuadraticIteration(double uu, double vv);

    int RealIteration(double & sss, int & flag);

    int CalcSc();

    void NextK(int itype);

    void Newest(int itype, double & uu, double & vv);

    void QuadraticSyntheticDivision(int n_plus_one,
                                    double u,
                                    double v,
                                    double * p_ptr,
                                    double * q_ptr,
                                    double & a,
                                    double & b);

    void SolveQuadraticEquation(double a,
                                double b,
                                double c,
                                double & sr,
                                double & si,
                                double & lr,
                                double & li);

    //==================================================================
    //  Declare the copy constructor and operator equals to be private
    //  and do not implement them to prevent copying instances of this
    //  class.
    //==================================================================

    PolynomialRootFinder(const PolynomialRootFinder & that);

    PolynomialRootFinder operator =(const PolynomialRootFinder & that);
};

#endif
