/**
 * @file
 * @author	Juan I Carrano
 * @author	Andrés Calcabrini
 * @copyright	Copyright (c) 2019 Juan I Carrano
 * @copyright	Copyright (c) 2013 Juan I Carrano, Andrés Calcabrini,
 *                                 Juan I. Ubeira and Nicolás Venturo.
 * @copyright	All rights reserved.
 * ```
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of copyright holders nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ```
 *
 * Quaternion inline definitions
 */


#include "../quaternion.h"
#include "../vector.h"
#include "../fixed_point.h"

/**
 * @addtogroup fxp_quat
 * @{
 */

/**
 * Truncate double precision quaternion to single precision.
 */
FXP_DECLARATION(quat dq_to_q(dquat q))
{
	quat s;
#define _QTR(e) s.e = df_to_f(q.e)

	_QTR(r);
	_QTR(v.x);
	_QTR(v.y);
	_QTR(v.z);

#undef _QTR

	return s;
}

/**
 * Pseudo-error of the quaternion norm.
 *
 * Calculates an approximation to (1 - norm(q))
 * The approximation used is sqrt(x) = 0.5(x + 1) for x near 1
 */
FXP_DECLARATION(frac q_xnormerror(quat q))
{
	frac _0_5 = {FRAC_0_5_V};
	return f_sub(_0_5, df_to_f(
			df_idiv( df_add(df_add(df_add(
			f_mul_df(q.r, q.r), f_mul_df(q.v.x, q.v.x)),
			f_mul_df(q.v.y, q.v.y)),f_mul_df(q.v.z, q.v.z)),2)));
}

/**
 * Scale a quaternion by a fractional, return double precision quaternion.
 */
FXP_DECLARATION(dquat q_scale_dq(quat q, frac f))
{
	dquat s;

	s.r = f_mul_df(q.r, f);
	s.v.x = f_mul_df(q.v.x, f);
	s.v.y = f_mul_df(q.v.y, f);
	s.v.z = f_mul_df(q.v.z, f);

	return s;
}

/**
 * Scale a quaternion by a fractional, return simple precision quaternion.
 */
FXP_DECLARATION(quat q_scale(quat q, frac f))
{
	quat s;

	s.r = f_mul(q.r, f);
	s.v.x = f_mul(q.v.x, f);
	s.v.y = f_mul(q.v.y, f);
	s.v.z = f_mul(q.v.z, f);

	return s;
}

/**
 * Conjugate quaternion.
 *
 * Negates the vector component.
 */
FXP_DECLARATION(quat q_conj(quat q))
{
	quat p;

	p.r = q.r;
	p.v.x = f_neg(q.v.x);
	p.v.y = f_neg(q.v.y);
	p.v.z = f_neg(q.v.z);

	return p;
}

/**
 * Quaternion multiplication, return value is simple precision.
 */
FXP_DECLARATION(quat q_mul(quat q, quat p))
{
	quat s;

	s.r = f_sub( f_sub( f_sub(
		f_mul(q.r,p.r), f_mul(q.v.x,p.v.x))
		,f_mul(q.v.y,p.v.y)), f_mul(q.v.z,p.v.z));

	s.v.x = f_add(f_sub(f_add(
		f_mul(p.r,q.v.x), f_mul(p.v.x,q.r)),
		f_mul(p.v.y,q.v.z)), f_mul(p.v.z,q.v.y));

	s.v.y = f_sub(f_add(f_add(
		f_mul(p.r,q.v.y), f_mul(p.v.x,q.v.z)),
		f_mul(p.v.y,q.r)), f_mul(p.v.z,q.v.x));

	s.v.z = f_add(f_add(f_sub(
		f_mul(p.r,q.v.z), f_mul(p.v.x,q.v.y)),
		f_mul(p.v.y,q.v.x)),f_mul(p.v.z,q.r));

	return s;
}

/**
 * Scaled Quaternion multiplication, return value is double precision.
 *
 * @return	(Q x P)/f
 */
FXP_DECLARATION(dquat q_mul_s_dq(quat q, quat p, int f))
{
	dquat s;

	s.r = df_idiv(df_sub(df_sub(df_sub(
		f_mul_df(q.r,p.r),f_mul_df(q.v.x,p.v.x)),
		f_mul_df(q.v.y,p.v.y)),f_mul_df(q.v.z,p.v.z)) , f);

	s.v.x = df_idiv( df_add(df_sub(df_add(
		f_mul_df(p.r,q.v.x),f_mul_df(p.v.x,q.r)),
		f_mul_df(p.v.y,q.v.z)),f_mul_df(p.v.z,q.v.y)), f);

	s.v.y = df_idiv( df_sub(df_add(df_add(
		f_mul_df(p.r,q.v.y),f_mul_df(p.v.x,q.v.z)),
		f_mul_df(p.v.y,q.r)),f_mul_df(p.v.z,q.v.x)), f);

	s.v.z = df_idiv( df_add(df_add(df_sub(
		f_mul_df(p.r,q.v.z),f_mul_df(p.v.x,q.v.y)),
		f_mul_df(p.v.y,q.v.x)),f_mul_df(p.v.z,q.r)), f);

	return s;
}

/**
 * Quaternion multiplication, return value is double precision.
 *
 * @return	(Q x P)/f
 */
FXP_DECLARATION(dquat q_mul_dq(quat q, quat p))
{
	dquat s;

	s.r = df_sub(df_sub(df_sub(
		f_mul_df(q.r,p.r),f_mul_df(q.v.x,p.v.x)),
		f_mul_df(q.v.y,p.v.y)),f_mul_df(q.v.z,p.v.z));

	s.v.x = df_add(df_sub(df_add(
		f_mul_df(p.r,q.v.x),f_mul_df(p.v.x,q.r)),
		f_mul_df(p.v.y,q.v.z)),f_mul_df(p.v.z,q.v.y));

	s.v.y = df_sub(df_add(df_add(
		f_mul_df(p.r,q.v.y),f_mul_df(p.v.x,q.v.z)),
		f_mul_df(p.v.y,q.r)),f_mul_df(p.v.z,q.v.x));

	s.v.z = df_add(df_add(df_sub(
		f_mul_df(p.r,q.v.z),f_mul_df(p.v.x,q.v.y)),
		f_mul_df(p.v.y,q.v.x)),f_mul_df(p.v.z,q.r));

	return s;
}

/**
 * Quaternion addition (double precision).
 */
FXP_DECLARATION(dquat dq_add(dquat q, dquat p))
{
	dquat s;
#define _QSUM(e) s.e = df_add(q.e, p.e)

	_QSUM(r);
	_QSUM(v.x);
	_QSUM(v.y);
	_QSUM(v.z);

#undef _QSUM
	return s;
}

/**
 * Quaternion addition (single precision)
 */
FXP_DECLARATION(quat q_add(quat q, quat p))
{
	quat s;
#define _QSUM(e) s.e = f_add(q.e, p.e)

	_QSUM(r);
	_QSUM(v.x);
	_QSUM(v.y);
	_QSUM(v.z);

#undef _QSUM
	return s;
}

/**
 * Rotate 3D vector by quaternion.
 *
 * Creates a purely imaginary quaternion V with vector part equal to v and
 * returns the vector part of q*(V*q')'.
 */
FXP_DECLARATION(vec3 q_rot(quat q, vec3 v))
{
	quat _v;
	frac _0 = {0};

	_v.r = _0;
	_v.v = v;

	return q_mul(q, q_mul(_v, q_conj(q))).v;
}

/**
 * Renormalize quaternion (single precision)
 *
 * This function attempts to bring the norm of the quaternion closer to 1.
 * It only works for quaternions whose norm is already close to 1. It uses and
 * approximation for the norm (q_xnormerror).
 */
FXP_DECLARATION(quat q_xrenorm(quat q))
{
	frac err;
	quat correction;

	err = q_xnormerror(q);
	correction = q_scale(q, err);
	return q_add(q, correction);
}

/**
 * Renormalize quaternion.
 *
 * See @ref q_xrenorm.
 */
FXP_DECLARATION(dquat dq_xrenorm(dquat q))
{
	frac err;
	dquat correction;
	quat qh = dq_to_q(q);

	err = q_xnormerror(qh);
	correction = q_scale_dq(qh, err);
	return dq_add(q, correction);
}

/**
 * Extract a component from a unit quaternion.
 *
 * @param	q	A unit quaternion.
 * @param	axis 	Axis to return.
 *
 * @return		A quaternion whose vector part is null except on one axis.
 */
FXP_DECLARATION(quat q_udecompose(quat q, vec_axis axis))
{
	frac norm_err;
	frac the_axis;
	frac _0_5 = {FRAC_0_5_V};
	quat ret = {{0}, VEC0};

	switch (axis) {
	case AXIS_X:
		the_axis = q.v.x;
		break;
	case AXIS_Y:
		the_axis = q.v.y;
		break;
	case AXIS_Z:
		the_axis = q.v.z;
		break;
	}

	norm_err = f_sub(_0_5, f_idiv(df_to_f(df_add(f_mul_df(q.r, q.r),
					f_mul_df(the_axis,the_axis))),2));

	ret.r = f_add(q.r, f_mul(norm_err, q.r));
	the_axis = f_add(the_axis, f_mul(norm_err, the_axis));

	switch (axis) {
	case AXIS_X:
		ret.v.x = the_axis;
		break;
	case AXIS_Y:
		ret.v.y = the_axis;
		break;
	case AXIS_Z:
		ret.v.z = the_axis;
		break;
	}

	return ret;
}

/**
 * @defgroup fxp_q_errors Quaternion Errors
 *
 * This functions calculate the rotation needed to go from 'pos'
 * (current position) to 'setp' (setpoint), in the same reference frame as 'pos'.
 *
 * @{
 */

/**
 * The magnitude of this error reaches its maximum at 90 degrees and goes to
 * zero at 180 degrees.
 */
FXP_DECLARATION(vec3 q_error(quat setp, quat pos))
{
	quat c = q_mul(q_conj(pos), setp);

	return v_fmul(c.v, c.r);
}

/**
 * This error has a discontinuity at 180 degrees.
 *
 * When the deviation is less than 90 degrees, it has a gain of approximately 2
 * over @ref q_error.
 */
FXP_DECLARATION(vec3 q_error2(quat setp, quat pos))
{
	quat c = q_mul(q_conj(pos), setp);

	return v_imul(c.v, F_SIGN(c.r));
}

/** @}
 */

/** @}
 */
