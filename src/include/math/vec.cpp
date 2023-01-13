#include "vec.hpp"



NAMESPACE_MATH_BEGIN




void MultiplyMat4Vec4(vec4f& a, mat4f& b, vec4f& out) {
	out.x = dot(a, b.column(0));
	out.y = dot(a, b.column(1));
	out.z = dot(a, b.column(2));
	out.w = dot(a, b.column(3));
}


void MultiplyMat4Mat4(mat4f& a, mat4f& b, mat4f& out) {
	mat4f fetch_btransposed = {
		b.column(0),
		b.column(1),
		b.column(2),
		b.column(3)
	};
	vec4f tmp0{};
	for(size_t i = 0; i < 4; ++i) {
		tmp0.x = dot(a[i], fetch_btransposed[0]);
		tmp0.y = dot(a[i], fetch_btransposed[1]);
		tmp0.z = dot(a[i], fetch_btransposed[2]);
		tmp0.w = dot(a[i], fetch_btransposed[3]);
		out[i] = tmp0;
	}
	return;
}


void identity(mat4f& out) {
	out.mem.zero();
	out[0 ] = 1.0f;
	out[5 ] = 1.0f;
	out[10] = 1.0f;
	out[15] = 1.0f;
	return;
}


void translate(vec3f const& translate, mat4f& out) {
	out.mem.zero();
	out[3] = translate.homogenised;
	out[3].w = 1.0f;
	return;
}


void scale(vec3f const& scale, mat4f& out) {
	out.mem.zero();
	out[0 ] = scale.x;
	out[5 ] = scale.y;
	out[10] = scale.z;
	out[15] = 1.0f;
	return;
}


void transposed(mat4f const& in, mat4f& out) {
	for(size_t i = 0; i < 4; ++i) {
		out(i, 0) = in(0, i);
		out(i, 1) = in(1, i);
		out(i, 2) = in(2, i);
		out(i, 3) = in(3, i);
	}
}


void transpose(mat4f& inout) {
	mat4f tmp = inout;
	for(size_t i = 0; i < 4; ++i) {
		inout(i, 0) = tmp(0, i);
		inout(i, 1) = tmp(1, i);
		inout(i, 2) = tmp(2, i);
		inout(i, 3) = tmp(3, i);
	}
	return;
}


void perspective(float fovy, float aspectRatio, float near, float far, mat4f& out) {
	float t, r, nmf;
	aspectRatio = 1.0f / aspectRatio;


	t    = 1.0f / std::tan(fovy * 0.5f);
	r    = t * aspectRatio;
	nmf  = 1.0f / (near - far);
	fovy = 2.0f * far * near;

	out.mem.zero();
	out.mem[ 0] = r;
	out.mem[ 5] = t;
	out.mem[10] = (far + near) * nmf;
	out.mem[11] = -1.0f;
	out.mem[14] = fovy * nmf;
	return;
}


void inv_perspective(mat4f const& in, mat4f& out)
{
	out.mem.zero();
				/* m00   m11    m32    m23  */
	vec4f inv = { in.x0, in.y1, in.w2, in.z3 };
	vec4f tmp = {1.0f};

	inv = tmp / inv;
	tmp.x  = -in.z2;
	tmp.x *= (inv.z * inv.w); // m33 = -m22 / (m32 * m23)

	out.x0 = inv.x; /* m00' = 1 / m00 */
	out.y1 = inv.y; /* m11' = 1 / m11 */
	out.w2 = inv.z; /* m32' = 1 / m23 */
	out.z3 = inv.w; /* m23' = 1 / m32 */
	out.w3 = tmp.x; /* m33' = tmp.x   */
	return;
}


void lookAt(
	vec3f const& eyePos, 
	vec3f const& at, 
	vec3f const& up,
	mat4f& 		 out
) {
	vec3f forward, right, newup, trans;

	/* (at - eye).div(|(eye-at)|) */
	forward  = eyePos; 
	forward -= at; 
	forward.normalize();

	/* camera_view_dir x up_basis_vector = right_basis_vector */
	right = cross(up, forward); 
	right.normalize();

	/* right_basis_vector(x_axis) x forward_basis_vector(z_axis) = up_basis_vector(y_axis) */
	newup = cross(forward, right);


	/* Set View Matrix Basis Vectors */
	out[0] = right.homogenised;
	out[1] = newup.homogenised;
	out[2] = forward.homogenised;
	/* Set The Translation Vector to (-eyePos, 1.0f)*/
	trans  = -1.0f * eyePos;

	out[3].x = dot(right,   trans);
	out[3].y = dot(newup,   trans);
	out[3].z = dot(forward, trans); 
	out[3].w = 1.0f;

	debug({
		printf("LookAt() Debug Report:\n");
		printf("+X    "); right.homogenised.print();
		printf("+Y    "); newup.homogenised.print();
		printf("+Z    "); forward.homogenised.print();
		printf("-P    "); trans.homogenised.print();
		printf("-P_mat"); out[3].print();
		printf("mat^-1"); out.print();
	});

	transpose(out);
	return;
}


void inv_lookAt(
	vec3f const& eyePos, 
	vec3f const& at, 
	vec3f const& up,
	mat4f& 		 out
) {
	vec3f forward, right, newup;

	/* (at - eye).div(|(eye-at)|) */
	forward  = eyePos; 
	forward -= at; 
	forward.normalize();

	/* camera_view_dir x up_basis_vector = right_basis_vector */
	right = cross(up, forward); 
	right.normalize();

	/* right_basis_vector(x_axis) x forward_basis_vector(z_axis) = up_basis_vector(y_axis) */
	newup = cross(forward, right);


	/* Set View Matrix Basis Vectors */
	out[0] = right.homogenised;
	out[1] = newup.homogenised;
	out[2] = forward.homogenised;
	out[3] = eyePos.homogenised;
	return;
}



void inverse(mat4f const& m, mat4f& out) 
{
	float A2323, A1323, A1223, A0323,
		  A0223, A0123, A2313, A1313, 
		  A1213, A2312, A1312, A1212, 
		  A0313, A0213, A0312, A0212, 
		  A0113, A0112, det;
	

	A2323 = m.z2 * m.w3 - m.w2 * m.z3;
	A1323 = m.y2 * m.w3 - m.w2 * m.y3;
	A1223 = m.y2 * m.z3 - m.z2 * m.y3;
	A0323 = m.x2 * m.w3 - m.w2 * m.x3;
	A0223 = m.x2 * m.z3 - m.z2 * m.x3;
	A0123 = m.x2 * m.y3 - m.y2 * m.x3;
	A2313 = m.z1 * m.w3 - m.w1 * m.z3;
	A1313 = m.y1 * m.w3 - m.w1 * m.y3;
	A1213 = m.y1 * m.z3 - m.z1 * m.y3;
	A2312 = m.z1 * m.w2 - m.w1 * m.z2;
	A1312 = m.y1 * m.w2 - m.w1 * m.y2;
	A1212 = m.y1 * m.z2 - m.z1 * m.y2;
	A0313 = m.x1 * m.w3 - m.w1 * m.x3;
	A0213 = m.x1 * m.z3 - m.z1 * m.x3;
	A0312 = m.x1 * m.w2 - m.w1 * m.x2;
	A0212 = m.x1 * m.z2 - m.z1 * m.x2;
	A0113 = m.x1 * m.y3 - m.y1 * m.x3;
	A0112 = m.x1 * m.y2 - m.y1 * m.x2;

	det =
	  m.x0 * ( m.y1 * A2323 - m.z1 * A1323 + m.w1 * A1223 )
	- m.y0 * ( m.x1 * A2323 - m.z1 * A0323 + m.w1 * A0223 )
	+ m.z0 * ( m.x1 * A1323 - m.y1 * A0323 + m.w1 * A0123 )
	- m.w0 * ( m.x1 * A1223 - m.y1 * A0223 + m.z1 * A0123 );
	det = 1 / det;

	out.x0 = det *   ( m.y1 * A2323 - m.z1 * A1323 + m.w1 * A1223 );
	out.y0 = det * - ( m.y0 * A2323 - m.z0 * A1323 + m.w0 * A1223 );
	out.z0 = det *   ( m.y0 * A2313 - m.z0 * A1313 + m.w0 * A1213 );
	out.w0 = det * - ( m.y0 * A2312 - m.z0 * A1312 + m.w0 * A1212 );
	out.x1 = det * - ( m.x1 * A2323 - m.z1 * A0323 + m.w1 * A0223 );
	out.y1 = det *   ( m.x0 * A2323 - m.z0 * A0323 + m.w0 * A0223 );
	out.z1 = det * - ( m.x0 * A2313 - m.z0 * A0313 + m.w0 * A0213 );
	out.w1 = det *   ( m.x0 * A2312 - m.z0 * A0312 + m.w0 * A0212 );
	out.x2 = det *   ( m.x1 * A1323 - m.y1 * A0323 + m.w1 * A0123 );
	out.y2 = det * - ( m.x0 * A1323 - m.y0 * A0323 + m.w0 * A0123 );
	out.z2 = det *   ( m.x0 * A1313 - m.y0 * A0313 + m.w0 * A0113 );
	out.w2 = det * - ( m.x0 * A1312 - m.y0 * A0312 + m.w0 * A0112 );
	out.x3 = det * - ( m.x1 * A1223 - m.y1 * A0223 + m.z1 * A0123 );
	out.y3 = det *   ( m.x0 * A1223 - m.y0 * A0223 + m.z0 * A0123 );
	out.z3 = det * - ( m.x0 * A1213 - m.y0 * A0213 + m.z0 * A0113 );
	out.w3 = det *   ( m.x0 * A1212 - m.y0 * A0212 + m.z0 * A0112 );
	return;
}




NAMESPACE_MATH_END