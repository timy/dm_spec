#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <cmath>

namespace std
{
	// Forward declarations.
	template<typename _Tp> class complex;
    template<> class complex<float>;
    template<> class complex<double>;
    template<> class complex<long double>;

    ///  Return magnitude of @a z.
    template<typename _Tp> _Tp abs(const complex<_Tp>&);
    ///  Return phase angle of @a z.
    template<typename _Tp> _Tp arg(const complex<_Tp>&);
    ///  Return @a z magnitude squared.
    template<typename _Tp> _Tp norm(const complex<_Tp>&);

    ///  Return complex conjugate of @a z.
    template<typename _Tp> complex<_Tp> conj(const complex<_Tp>&);
    ///  Return complex with magnitude @a rho and angle @a theta.
    template<typename _Tp> complex<_Tp> polar(const _Tp&, const _Tp& = 0);

    // Transcendentals:
    /// Return complex cosine of @a z.
    template<typename _Tp> complex<_Tp> cos(const complex<_Tp>&);
    /// Return complex hyperbolic cosine of @a z.
    template<typename _Tp> complex<_Tp> cosh(const complex<_Tp>&);
    /// Return complex base e exponential of @a z.
    template<typename _Tp> complex<_Tp> exp(const complex<_Tp>&);
    /// Return complex natural logarithm of @a z.
    template<typename _Tp> complex<_Tp> log(const complex<_Tp>&);
    /// Return complex base 10 logarithm of @a z.
    template<typename _Tp> complex<_Tp> log10(const complex<_Tp>&);
    // DR 844.
    /// Return @a x to the @a y'th power.
    template<typename _Tp> complex<_Tp> pow(const complex<_Tp>&, int);

    /// Return @a x to the @a y'th power.
    template<typename _Tp> complex<_Tp> pow(const complex<_Tp>&, const _Tp&);
    /// Return @a x to the @a y'th power.
    template<typename _Tp> complex<_Tp> pow(const complex<_Tp>&, 
                                            const complex<_Tp>&);
    /// Return @a x to the @a y'th power.
    template<typename _Tp> complex<_Tp> pow(const _Tp&, const complex<_Tp>&);
    /// Return complex sine of @a z.
    template<typename _Tp> complex<_Tp> sin(const complex<_Tp>&);
    /// Return complex hyperbolic sine of @a z.
    template<typename _Tp> complex<_Tp> sinh(const complex<_Tp>&);
    /// Return complex square root of @a z.
    template<typename _Tp> complex<_Tp> sqrt(const complex<_Tp>&);
    /// Return complex tangent of @a z.
    template<typename _Tp> complex<_Tp> tan(const complex<_Tp>&);
    /// Return complex hyperbolic tangent of @a z.
    template<typename _Tp> complex<_Tp> tanh(const complex<_Tp>&);
    
    
    // 26.2.2  Primary template class complex
    /**
     *  Template to represent complex numbers.
     *
     *  Specializations for float, double, and long double are part of the
     *  library.  Results with any other type are not guaranteed.
     *
     *  @param  Tp  Type of real and imaginary values.
	 */
    template<typename _Tp>
	struct complex
	{
		/// Value typedef.
		typedef _Tp value_type;
      
		///  Default constructor.  First parameter is x, second parameter is y.
		///  Unspecified parameters default to 0.
		complex(const _Tp& __r = _Tp(), const _Tp& __i = _Tp())
            : _M_real(__r), _M_imag(__i) { }

		// Lets the compiler synthesize the copy constructor   
		// complex (const complex<_Tp>&);
		///  Copy constructor.
		template<typename _Up>
		complex(const complex<_Up>& __z)
			: _M_real(__z.real()), _M_imag(__z.imag()) { }

		///  Return real part of complex number.
		_Tp& 
		real() { return _M_real; }

		///  Return real part of complex number.
		const _Tp& 
		real() const { return _M_real; }

		///  Return imaginary part of complex number.
		_Tp& 
		imag() { return _M_imag; }

		///  Return imaginary part of complex number.
		const _Tp& 
		imag() const { return _M_imag; }

		// _GLIBCXX_RESOLVE_LIB_DEFECTS
		// DR 387. std::complex over-encapsulated.
		void 
		real(_Tp __val) { _M_real = __val; }

		void 
		imag(_Tp __val) { _M_imag = __val; }

		/// Assign this complex number to scalar @a t.
		complex<_Tp>& operator=(const _Tp&);
      
		/// Add @a t to this complex number.
		// 26.2.5/1
		complex<_Tp>&
		operator+=(const _Tp& __t)
		{
			_M_real += __t;
			return *this;
		}

		/// Subtract @a t from this complex number.
		// 26.2.5/3
		complex<_Tp>&
		operator-=(const _Tp& __t)
		{
			_M_real -= __t;
			return *this;
		}

		/// Multiply this complex number by @a t.
		complex<_Tp>& operator*=(const _Tp&);
		/// Divide this complex number by @a t.
		complex<_Tp>& operator/=(const _Tp&);

		// Lets the compiler synthesize the
		// copy and assignment operator
		// complex<_Tp>& operator= (const complex<_Tp>&);
		/// Assign this complex number to complex @a z.
		template<typename _Up>
		complex<_Tp>& operator=(const complex<_Up>&);
		/// Add @a z to this complex number.
		template<typename _Up>
		complex<_Tp>& operator+=(const complex<_Up>&);
		/// Subtract @a z from this complex number.
		template<typename _Up>
		complex<_Tp>& operator-=(const complex<_Up>&);
		/// Multiply this complex number by @a z.
		template<typename _Up>
		complex<_Tp>& operator*=(const complex<_Up>&);
		/// Divide this complex number by @a z.
		template<typename _Up>
		complex<_Tp>& operator/=(const complex<_Up>&);

		complex __rep() const
		{ return *this; }

	private:
		_Tp _M_real;
		_Tp _M_imag;
	};

    template<typename _Tp>
	complex<_Tp>&
	complex<_Tp>::operator=(const _Tp& __t)
	{
		_M_real = __t;
		_M_imag = _Tp();
		return *this;
	} 

    // 26.2.5/5
    template<typename _Tp>
	complex<_Tp>&
	complex<_Tp>::operator*=(const _Tp& __t)
	{
		_M_real *= __t;
		_M_imag *= __t;
		return *this;
	}

    // 26.2.5/7
    template<typename _Tp>
	complex<_Tp>&
	complex<_Tp>::operator/=(const _Tp& __t)
	{
		_M_real /= __t;
		_M_imag /= __t;
		return *this;
	}

    template<typename _Tp>
	template<typename _Up>
	complex<_Tp>&
	complex<_Tp>::operator=(const complex<_Up>& __z)
	{
		_M_real = __z.real();
		_M_imag = __z.imag();
		return *this;
	}

    // 26.2.5/9
    template<typename _Tp>
	template<typename _Up>
	complex<_Tp>&
	complex<_Tp>::operator+=(const complex<_Up>& __z)
	{
		_M_real += __z.real();
		_M_imag += __z.imag();
		return *this;
	}

    // 26.2.5/11
    template<typename _Tp>
	template<typename _Up>
	complex<_Tp>&
	complex<_Tp>::operator-=(const complex<_Up>& __z)
	{
		_M_real -= __z.real();
		_M_imag -= __z.imag();
		return *this;
	}

    // 26.2.5/13
    // XXX: This is a grammar school implementation.
    template<typename _Tp>
	template<typename _Up>
	complex<_Tp>&
	complex<_Tp>::operator*=(const complex<_Up>& __z)
	{
		const _Tp __r = _M_real * __z.real() - _M_imag * __z.imag();
		_M_imag = _M_real * __z.imag() + _M_imag * __z.real();
		_M_real = __r;
		return *this;
	}

    // 26.2.5/15
    // XXX: This is a grammar school implementation.
    template<typename _Tp>
	template<typename _Up>
	complex<_Tp>&
	complex<_Tp>::operator/=(const complex<_Up>& __z)
	{
		const _Tp __r =  _M_real * __z.real() + _M_imag * __z.imag();
		const _Tp __n = std::norm(__z);
		_M_imag = (_M_imag * __z.real() - _M_real * __z.imag()) / __n;
		_M_real = __r / __n;
		return *this;
	}
    
    // Operators:
    //@{
    ///  Return new complex value @a x plus @a y.
    template<typename _Tp>
	inline complex<_Tp>
	operator+(const complex<_Tp>& __x, const complex<_Tp>& __y)
	{
		complex<_Tp> __r = __x;
		__r += __y;
		return __r;
	}

    template<typename _Tp>
	inline complex<_Tp>
	operator+(const complex<_Tp>& __x, const _Tp& __y)
	{
		complex<_Tp> __r = __x;
		__r += __y;
		return __r;
	}

    template<typename _Tp>
	inline complex<_Tp>
	operator+(const _Tp& __x, const complex<_Tp>& __y)
	{
		complex<_Tp> __r = __y;
		__r += __x;
		return __r;
	}
    //@}

    //@{
    ///  Return new complex value @a x minus @a y.
    template<typename _Tp>
	inline complex<_Tp>
	operator-(const complex<_Tp>& __x, const complex<_Tp>& __y)
	{
		complex<_Tp> __r = __x;
		__r -= __y;
		return __r;
	}
    
    template<typename _Tp>
	inline complex<_Tp>
	operator-(const complex<_Tp>& __x, const _Tp& __y)
	{
		complex<_Tp> __r = __x;
		__r -= __y;
		return __r;
	}

    template<typename _Tp>
	inline complex<_Tp>
	operator-(const _Tp& __x, const complex<_Tp>& __y)
	{
		complex<_Tp> __r(__x, -__y.imag());
		__r -= __y.real();
		return __r;
	}
    //@}

    //@{
    ///  Return new complex value @a x times @a y.
    template<typename _Tp>
	inline complex<_Tp>
	operator*(const complex<_Tp>& __x, const complex<_Tp>& __y)
	{
		complex<_Tp> __r = __x;
		__r *= __y;
		return __r;
	}

    template<typename _Tp>
	inline complex<_Tp>
	operator*(const complex<_Tp>& __x, const _Tp& __y)
	{
		complex<_Tp> __r = __x;
		__r *= __y;
		return __r;
	}

    template<typename _Tp>
	inline complex<_Tp>
	operator*(const _Tp& __x, const complex<_Tp>& __y)
	{
		complex<_Tp> __r = __y;
		__r *= __x;
		return __r;
	}
    //@}

    //@{
    ///  Return new complex value @a x divided by @a y.
    template<typename _Tp>
	inline complex<_Tp>
	operator/(const complex<_Tp>& __x, const complex<_Tp>& __y)
	{
		complex<_Tp> __r = __x;
		__r /= __y;
		return __r;
	}
    
    template<typename _Tp>
	inline complex<_Tp>
	operator/(const complex<_Tp>& __x, const _Tp& __y)
	{
		complex<_Tp> __r = __x;
		__r /= __y;
		return __r;
	}

    template<typename _Tp>
	inline complex<_Tp>
	operator/(const _Tp& __x, const complex<_Tp>& __y)
	{
		complex<_Tp> __r = __x;
		__r /= __y;
		return __r;
	}
    //@}

    ///  Return @a x.
    template<typename _Tp>
	inline complex<_Tp>
	operator+(const complex<_Tp>& __x)
	{ return __x; }

    ///  Return complex negation of @a x.
    template<typename _Tp>
	inline complex<_Tp>
	operator-(const complex<_Tp>& __x)
	{  return complex<_Tp>(-__x.real(), -__x.imag()); }

    //@{
    ///  Return true if @a x is equal to @a y.
    template<typename _Tp>
	inline bool
	operator==(const complex<_Tp>& __x, const complex<_Tp>& __y)
	{ return __x.real() == __y.real() && __x.imag() == __y.imag(); }

    template<typename _Tp>
	inline bool
	operator==(const complex<_Tp>& __x, const _Tp& __y)
	{ return __x.real() == __y && __x.imag() == _Tp(); }

    template<typename _Tp>
	inline bool
	operator==(const _Tp& __x, const complex<_Tp>& __y)
	{ return __x == __y.real() && _Tp() == __y.imag(); }
    //@}

    //@{
    ///  Return false if @a x is equal to @a y.
    template<typename _Tp>
	inline bool
	operator!=(const complex<_Tp>& __x, const complex<_Tp>& __y)
	{ return __x.real() != __y.real() || __x.imag() != __y.imag(); }

    template<typename _Tp>
	inline bool
	operator!=(const complex<_Tp>& __x, const _Tp& __y)
	{ return __x.real() != __y || __x.imag() != _Tp(); }

    template<typename _Tp>
	inline bool
	operator!=(const _Tp& __x, const complex<_Tp>& __y)
	{ return __x != __y.real() || _Tp() != __y.imag(); }
    //@}

    // Values
    template<typename _Tp>
	inline _Tp&
	real(complex<_Tp>& __z)
	{ return __z.real(); }
    
    template<typename _Tp>
	inline const _Tp&
	real(const complex<_Tp>& __z)
	{ return __z.real(); }
    
    template<typename _Tp>
	inline _Tp&
	imag(complex<_Tp>& __z)
	{ return __z.imag(); }
    
    template<typename _Tp>
	inline const _Tp&
	imag(const complex<_Tp>& __z)
	{ return __z.imag(); }

    // 26.2.7/3 abs(__z):  Returns the magnitude of __z.
    template<typename _Tp>
	inline _Tp
	__complex_abs(const complex<_Tp>& __z)
	{
		_Tp __x = __z.real();
		_Tp __y = __z.imag();
		const _Tp __s = abs(__x) > abs(__y) ? abs(__x) : abs(__y);
		if (__s == _Tp())  // well ...
			return __s;
		__x /= __s; 
		__y /= __s;
		return __s * sqrt(__x * __x + __y * __y);
	}

    template<typename _Tp>
	inline _Tp
	abs(const complex<_Tp>& __z) { return __complex_abs(__z); }

    // 26.2.7/4: arg(__z): Returns the phase angle of __z.
    template<typename _Tp>
	inline _Tp
	__complex_arg(const complex<_Tp>& __z)
	{ return  atan2(__z.imag(), __z.real()); }

    template<typename _Tp>
	inline _Tp
	arg(const complex<_Tp>& __z) { return __complex_arg(__z); }

    // 26.2.7/5: norm(__z) returns the squared magnitude of __z.
    //     As defined, norm() is -not- a norm is the common mathematical
    //     sens used in numerics.  The helper class _Norm_helper<> tries to
    //     distinguish between builtin floating point and the rest, so as
    //     to deliver an answer as close as possible to the real value.
    template<bool>
	struct _Norm_helper
	{
		template<typename _Tp>
		static inline _Tp _S_do_it(const complex<_Tp>& __z)
		{
			const _Tp __x = __z.real();
			const _Tp __y = __z.imag();
			return __x * __x + __y * __y;
		}
	};

    template<>
	struct _Norm_helper<true>
	{
		template<typename _Tp>
		static inline _Tp _S_do_it(const complex<_Tp>& __z)
		{
			_Tp __res = std::abs(__z);
			return __res * __res;
		}
	};
  
    template<typename _Tp>
	inline _Tp
	norm(const complex<_Tp>& __z)
	{
		return _Norm_helper<false>::_S_do_it(__z);
	}

    template<typename _Tp>
	inline complex<_Tp>
	polar(const _Tp& __rho, const _Tp& __theta)
	{ return complex<_Tp>(__rho * cos(__theta), __rho * sin(__theta)); }

    template<typename _Tp>
	inline complex<_Tp>
	conj(const complex<_Tp>& __z)
	{ return complex<_Tp>(__z.real(), -__z.imag()); }
  
    // Transcendentals

    // 26.2.8/1 cos(__z):  Returns the cosine of __z.
    template<typename _Tp>
	inline complex<_Tp>
	__complex_cos(const complex<_Tp>& __z)
	{
		const _Tp __x = __z.real();
		const _Tp __y = __z.imag();
		return complex<_Tp>(cos(__x) * cosh(__y), -sin(__x) * sinh(__y));
	}

    template<typename _Tp>
	inline complex<_Tp>
	cos(const complex<_Tp>& __z) { return __complex_cos(__z); }

    // 26.2.8/2 cosh(__z): Returns the hyperbolic cosine of __z.
    template<typename _Tp>
	inline complex<_Tp>
	__complex_cosh(const complex<_Tp>& __z)
	{
		const _Tp __x = __z.real();
		const _Tp __y = __z.imag();
		return complex<_Tp>(cosh(__x) * cos(__y), sinh(__x) * sin(__y));
	}

    template<typename _Tp>
	inline complex<_Tp>
	cosh(const complex<_Tp>& __z) { return __complex_cosh(__z); }

    // 26.2.8/3 exp(__z): Returns the complex base e exponential of x
    template<typename _Tp>
	inline complex<_Tp>
	__complex_exp(const complex<_Tp>& __z)
	{ return std::polar(exp(__z.real()), __z.imag()); }

    template<typename _Tp>
	inline complex<_Tp>
	exp(const complex<_Tp>& __z) { return __complex_exp(__z); }

    // 26.2.8/5 log(__z): Returns the natural complex logarithm of __z.
    //                    The branch cut is along the negative axis.
    template<typename _Tp>
	inline complex<_Tp>
	__complex_log(const complex<_Tp>& __z)
	{ return complex<_Tp>(log(std::abs(__z)), std::arg(__z)); }

    template<typename _Tp>
	inline complex<_Tp>
	log(const complex<_Tp>& __z) { return __complex_log(__z); }

    template<typename _Tp>
	inline complex<_Tp>
	log10(const complex<_Tp>& __z)
	{ return std::log(__z) / log(_Tp(10.0)); }

    // 26.2.8/10 sin(__z): Returns the sine of __z.
    template<typename _Tp>
	inline complex<_Tp>
	__complex_sin(const complex<_Tp>& __z)
	{
		const _Tp __x = __z.real();
		const _Tp __y = __z.imag();
		return complex<_Tp>(sin(__x) * cosh(__y), cos(__x) * sinh(__y)); 
	}

    template<typename _Tp>
	inline complex<_Tp>
	sin(const complex<_Tp>& __z) { return __complex_sin(__z); }

    // 26.2.8/11 sinh(__z): Returns the hyperbolic sine of __z.
    template<typename _Tp>
	inline complex<_Tp>
	__complex_sinh(const complex<_Tp>& __z)
	{
		const _Tp __x = __z.real();
		const _Tp  __y = __z.imag();
		return complex<_Tp>(sinh(__x) * cos(__y), cosh(__x) * sin(__y));
	}

    template<typename _Tp>
	inline complex<_Tp>
	sinh(const complex<_Tp>& __z) { return __complex_sinh(__z); }

    // 26.2.8/13 sqrt(__z): Returns the complex square root of __z.
    //                     The branch cut is on the negative axis.
    template<typename _Tp>
	complex<_Tp>
	__complex_sqrt(const complex<_Tp>& __z)
	{
		_Tp __x = __z.real();
		_Tp __y = __z.imag();

		if (__x == _Tp())
			{
				_Tp __t = sqrt(abs(__y) / 2);
				return complex<_Tp>(__t, __y < _Tp() ? -__t : __t);
			}
		else
			{
				_Tp __t = sqrt(2 * (std::abs(__z) + abs(__x)));
				_Tp __u = __t / 2;
				return __x > _Tp()
					? complex<_Tp>(__u, __y / __t)
					: complex<_Tp>(abs(__y) / __t, __y < _Tp() ? -__u : __u);
			}
	}

    template<typename _Tp>
	inline complex<_Tp>
	sqrt(const complex<_Tp>& __z) { return __complex_sqrt(__z); }

    // 26.2.8/14 tan(__z):  Return the complex tangent of __z.
  
    template<typename _Tp>
	inline complex<_Tp>
	__complex_tan(const complex<_Tp>& __z)
	{ return std::sin(__z) / std::cos(__z); }

    template<typename _Tp>
	inline complex<_Tp>
	tan(const complex<_Tp>& __z) { return __complex_tan(__z); }

    // 26.2.8/15 tanh(__z):  Returns the hyperbolic tangent of __z.
  
    template<typename _Tp>
	inline complex<_Tp>
	__complex_tanh(const complex<_Tp>& __z)
	{ return std::sinh(__z) / std::cosh(__z); }

    template<typename _Tp>
	inline complex<_Tp>
	tanh(const complex<_Tp>& __z) { return __complex_tanh(__z); }

    // 26.2.8/9  pow(__x, __y): Returns the complex power base of __x
    //                          raised to the __y-th power.  The branch
    //                          cut is on the negative axis.
    template<typename _Tp>
	complex<_Tp>
	__complex_pow_unsigned(complex<_Tp> __x, unsigned __n)
	{
		complex<_Tp> __y = __n % 2 ? __x : complex<_Tp>(1);

		while (__n >>= 1)
			{
				__x *= __x;
				if (__n % 2)
					__y *= __x;
			}

		return __y;
	}

    // _GLIBCXX_RESOLVE_LIB_DEFECTS
    // DR 844. complex pow return type is ambiguous.
    template<typename _Tp>
	inline complex<_Tp>
	pow(const complex<_Tp>& __z, int __n)
	{
		return __n < 0
                     ? complex<_Tp>(1) / std::__complex_pow_unsigned(__z, -(unsigned)__n)
                     : std::__complex_pow_unsigned(__z, __n);
	}

    template<typename _Tp>
	complex<_Tp>
	pow(const complex<_Tp>& __x, const _Tp& __y)
	{
        if (__x == _Tp())
            return _Tp();
        if (__x.imag() == _Tp() && __x.real() > _Tp())
            return pow(__x.real(), __y);

        complex<_Tp> __t = std::log(__x);
        return std::polar(exp(__y * __t.real()), __y * __t.imag());
    }

    template<typename _Tp>
	inline complex<_Tp>
	__complex_pow(const complex<_Tp>& __x, const complex<_Tp>& __y)
	{ return __x == _Tp() ? _Tp() : std::exp(__y * std::log(__x)); }

    template<typename _Tp>
	inline complex<_Tp>
	pow(const complex<_Tp>& __x, const complex<_Tp>& __y)
	{ return __complex_pow(__x, __y); }

    template<typename _Tp>
	inline complex<_Tp>
	pow(const _Tp& __x, const complex<_Tp>& __y)
	{
        return __x > _Tp() ? std::polar(pow(__x, __y.real()),
                                        __y.imag() * log(__x))
            : std::pow(complex<_Tp>(__x), __y);
    }

    /// 26.2.3  complex specializations
    /// complex<float> specialization
    template<>
	struct complex<float>
	{
        typedef float value_type;
        typedef __complex__ float _ComplexT;

        complex(_ComplexT __z) : _M_value(__z) { }

        complex(float __r = 0.0f, float __i = 0.0f)

        {
            __real__ _M_value = __r;
            __imag__ _M_value = __i;
        }

        explicit complex(const complex<double>&);
        explicit complex(const complex<long double>&);	

        float& 
        real() { return __real__ _M_value; }

        const float& 
        real() const { return __real__ _M_value; }      

        float& 
        imag() { return __imag__ _M_value; }

        const float& 
        imag() const { return __imag__ _M_value; }

        // _GLIBCXX_RESOLVE_LIB_DEFECTS
        // DR 387. std::complex over-encapsulated.
        void 
        real(float __val) { __real__ _M_value = __val; }

        void 
        imag(float __val) { __imag__ _M_value = __val; }

        complex&
        operator=(float __f)
        {
            _M_value = __f;
            return *this;
        }

        complex&
        operator+=(float __f)
        {
            _M_value += __f;
            return *this;
        }

        complex&
        operator-=(float __f)
        {
            _M_value -= __f;
            return *this;
        }

        complex&
        operator*=(float __f)
        {
            _M_value *= __f;
            return *this;
        }

        complex&
        operator/=(float __f)
        {
            _M_value /= __f;
            return *this;
        }

        // Let the compiler synthesize the copy and assignment
        // operator.  It always does a pretty good job.
        // complex& operator=(const complex&);

        template<typename _Tp>
        complex&
        operator=(const complex<_Tp>&  __z)
        {
            __real__ _M_value = __z.real();
            __imag__ _M_value = __z.imag();
            return *this;
        }

        template<typename _Tp>
        complex&
        operator+=(const complex<_Tp>& __z)
        {
            __real__ _M_value += __z.real();
            __imag__ _M_value += __z.imag();
            return *this;
        }

        template<class _Tp>
        complex&
        operator-=(const complex<_Tp>& __z)
        {
            __real__ _M_value -= __z.real();
            __imag__ _M_value -= __z.imag();
            return *this;
        }

        template<class _Tp>
        complex&
        operator*=(const complex<_Tp>& __z)
        {
            _ComplexT __t;
            __real__ __t = __z.real();
            __imag__ __t = __z.imag();
            _M_value *= __t;
            return *this;
        }

        template<class _Tp>
        complex&
        operator/=(const complex<_Tp>& __z)
        {
            _ComplexT __t;
            __real__ __t = __z.real();
            __imag__ __t = __z.imag();
            _M_value /= __t;
            return *this;
        }

        _ComplexT __rep() const { return _M_value; }

    private:
        _ComplexT _M_value;
    };

    /// 26.2.3  complex specializations
    /// complex<double> specialization
    template<>
	struct complex<double>
	{
        typedef double value_type;
        typedef __complex__ double _ComplexT;

        complex(_ComplexT __z) : _M_value(__z) { }

        complex(double __r = 0.0, double __i = 0.0)
        {
            __real__ _M_value = __r;
            __imag__ _M_value = __i;
        }

        complex(const complex<float>& __z)
            : _M_value(__z.__rep()) { }

        explicit complex(const complex<long double>&);	

        double& 
        real() { return __real__ _M_value; }

        const double& 
        real() const { return __real__ _M_value; }

        double& 
        imag() { return __imag__ _M_value; }

        const double& 
        imag() const { return __imag__ _M_value; }

        // _GLIBCXX_RESOLVE_LIB_DEFECTS
        // DR 387. std::complex over-encapsulated.
        void 
        real(double __val) { __real__ _M_value = __val; }

        void 
        imag(double __val) { __imag__ _M_value = __val; }

        complex&
        operator=(double __d)
        {
            _M_value = __d;
            return *this;
        }

        complex&
        operator+=(double __d)
        {
            _M_value += __d;
            return *this;
        }
	
        complex&
        operator-=(double __d)
        {
            _M_value -= __d;
            return *this;
        }

        complex&
        operator*=(double __d)
        {
            _M_value *= __d;
            return *this;
        }

        complex&
        operator/=(double __d)
        {
            _M_value /= __d;
            return *this;
        }

        // The compiler will synthesize this, efficiently.
        // complex& operator=(const complex&);

        template<typename _Tp>
        complex&
        operator=(const complex<_Tp>& __z)
        {
            __real__ _M_value = __z.real();
            __imag__ _M_value = __z.imag();
            return *this;
        }

        template<typename _Tp>
        complex&
        operator+=(const complex<_Tp>& __z)
        {
            __real__ _M_value += __z.real();
            __imag__ _M_value += __z.imag();
            return *this;
        }

        template<typename _Tp>
        complex&
        operator-=(const complex<_Tp>& __z)
        {
            __real__ _M_value -= __z.real();
            __imag__ _M_value -= __z.imag();
            return *this;
        }

        template<typename _Tp>
        complex&
        operator*=(const complex<_Tp>& __z)
        {
            _ComplexT __t;
            __real__ __t = __z.real();
            __imag__ __t = __z.imag();
            _M_value *= __t;
            return *this;
        }

        template<typename _Tp>
        complex&
        operator/=(const complex<_Tp>& __z)
        {
            _ComplexT __t;
            __real__ __t = __z.real();
            __imag__ __t = __z.imag();
            _M_value /= __t;
            return *this;
        }

        _ComplexT __rep() const { return _M_value; }

    private:
        _ComplexT _M_value;
    };

    /// 26.2.3  complex specializations
    /// complex<long double> specialization
    template<>
	struct complex<long double>
	{
		typedef long double value_type;
		typedef __complex__ long double _ComplexT;

		complex(_ComplexT __z) : _M_value(__z) { }

		complex(long double __r = 0.0L, 
                long double __i = 0.0L)

		{
			__real__ _M_value = __r;
			__imag__ _M_value = __i;
		}


		complex(const complex<float>& __z)
            : _M_value(__z.__rep()) { }

		complex(const complex<double>& __z)
            : _M_value(__z.__rep()) { }

		long double& 
		real() { return __real__ _M_value; }

		const long double& 
		real() const { return __real__ _M_value; }

		long double& 
		imag() { return __imag__ _M_value; }

		const long double& 
		imag() const { return __imag__ _M_value; }


		// _GLIBCXX_RESOLVE_LIB_DEFECTS
		// DR 387. std::complex over-encapsulated.
		void 
		real(long double __val) { __real__ _M_value = __val; }

		void 
		imag(long double __val) { __imag__ _M_value = __val; }

		complex&
		operator=(long double __r)
		{
			_M_value = __r;
			return *this;
		}

		complex&
		operator+=(long double __r)
		{
			_M_value += __r;
			return *this;
		}

		complex&
		operator-=(long double __r)
		{
			_M_value -= __r;
			return *this;
		}

		complex&
		operator*=(long double __r)
		{
			_M_value *= __r;
			return *this;
		}

		complex&
		operator/=(long double __r)
		{
			_M_value /= __r;
			return *this;
		}

		// The compiler knows how to do this efficiently
		// complex& operator=(const complex&);

		template<typename _Tp>
		complex&
		operator=(const complex<_Tp>& __z)
		{
			__real__ _M_value = __z.real();
			__imag__ _M_value = __z.imag();
			return *this;
		}

		template<typename _Tp>
		complex&
		operator+=(const complex<_Tp>& __z)
		{
			__real__ _M_value += __z.real();
			__imag__ _M_value += __z.imag();
			return *this;
		}

		template<typename _Tp>
		complex&
		operator-=(const complex<_Tp>& __z)
		{
			__real__ _M_value -= __z.real();
			__imag__ _M_value -= __z.imag();
			return *this;
		}

		template<typename _Tp>
		complex&
		operator*=(const complex<_Tp>& __z)
		{
			_ComplexT __t;
			__real__ __t = __z.real();
			__imag__ __t = __z.imag();
			_M_value *= __t;
			return *this;
		}

		template<typename _Tp>
		complex&
		operator/=(const complex<_Tp>& __z)
		{
			_ComplexT __t;
			__real__ __t = __z.real();
			__imag__ __t = __z.imag();
			_M_value /= __t;
			return *this;
		}

		_ComplexT __rep() const { return _M_value; }

	private:
		_ComplexT _M_value;
	};

    // These bits have to be at the end of this file, so that the
    // specializations have all been defined.
    inline
	complex<float>::complex(const complex<double>& __z)
		: _M_value(__z.__rep()) { }

    inline
	complex<float>::complex(const complex<long double>& __z)
		: _M_value(__z.__rep()) { }

    inline
	complex<double>::complex(const complex<long double>& __z)
		: _M_value(__z.__rep()) { }

    // @} group complex_numbers

} // namespace

#endif  /* COMPLEX_H_ */
