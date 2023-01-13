#include "texture.hpp"
#include "base.hpp"
#include "glad/glad.h"
#include <immintrin.h>



/* 
	Translation of the function: 
	glBindImageTexture(
		GLuint    unit,    ==> Which Image Unit do we want? (We provide the ID of it.)
		GLuint    texture, ==> Which Texture    do we want? (We provide the ID of it.)
		GLint     level,   ==> Which Mipmap Level ('Downscaled texture') do we want? (We provide an index to the appropriate level. as level goes up, the image shrinks by 2)
		GLboolean layered, ==> A Mipmap Level is Layered if it contains multiple images in it. We specify if we want to bind to ALL images in the selected layer
		GLint     layer,   ==> If we don't bind to ALL images in the layer, we need to know which image do we actually bind to. (We provide an image(layer) index.)
		GLenum    access,  ==> What type of access patterns will occur with this Image Texture 
		GLenum    format   ==> The internal format of the data inside the image. (RGB24B, RGB32F, ...)
	);
*/


i32 Texture2D::textureDimensionsMaxLen() const
{
	i32 retval;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &retval);
	return retval;
}



void Texture2D::recreateImage(std::array<i32, 2> const& newDims) 
{
	static const i32 maxLen = textureDimensionsMaxLen();
	ifcrashdbg(newDims[0] > maxLen || newDims[1] > maxLen);
	debug(
		printf("Texture2D::recreateImage() ==> Resized Texture to <%u, %u>\n", newDims[0], newDims[1]);
	)

	#ifdef _DEBUG
	if(id != DEFAULT32 && id != 0) {
		printf("Texture2D()::recreateImage() => Deleting Texture ID %u...\n", id);
		glDeleteTextures(1, &id);
	}
	#endif
	#ifndef _DEBUG
		glDeleteTextures(1, &id);
	#endif

	mark();
	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	mark();
	for(auto& param : texParams) 
	{
		glTextureParameteri(id, param.paramID, param.paramValue);
	}
	mark();
	glTextureStorage2D(id, 1, texPixelFmt.internal, newDims[0], newDims[1]);
	mark();

	// if(oncpu_dataHandle != nullptr) {
	// 	glTextureSubImage2D(id, 
	// 		0, 
	// 		0, 
	// 		0, 
	// 		newDims[0], 
	// 		newDims[1], 
	// 		texPixelFmt.format, 
	// 		texPixelFmt.dtype, 
	// 		oncpu_dataHandle
	// 	);
	// }


	currDims = newDims;
	return;
}




void Texture2D::create(
	texDataFmtDescriptor      fmtDesc,
	std::array<i32, 2> const& dims,
	manvec<texConfig>  const& params,
	std::optional<void*>      imgptr
) {
	ifcrash(!params.len());


	texPixelFmt = fmtDesc;
	texParams.copy(params);
	if (imgptr.has_value()) 
		oncpu_dataHandle = imgptr.value();
	

	recreateImage(dims);
	return;
}


void Texture2D::destroy()
{
	glDeleteTextures(1, &id);
	return;
}


void Texture2D::bindToTexUnit(u32 unit) {
	boundTexUnit = unit;
	glBindTextureUnit(boundTexUnit, id);
}


void Texture2D::bindToImageUnit(u32 unit){
	boundImageUnit = unit;
	glBindImageTexture(boundImageUnit, id, 0, GL_FALSE, 0, GL_READ_ONLY, texPixelFmt.internal);
}


void Texture2D::unbindTexUnit() {
	glBindTextureUnit(boundTexUnit, 0);
	boundTexUnit = 0;
}


void Texture2D::unbindImageUnit() {
	glBindImageTexture(boundImageUnit, 0, 0, GL_FALSE, 0, GL_READ_ONLY, texPixelFmt.internal);
	boundImageUnit = 0;
}


void Texture2D::activeTexture(u32 id) {
	ifcrashdbg(id > 31); glActiveTexture(GL_TEXTURE0 + id); return;
}


void Texture2D::print()
{
	printf("\
Texture2D (ID %u):\n\
    bound to Texture Unit %u\n\
    bound to Image   Unit %u\n\
    Width, Height: [ %d, %d ]\n\
    Pixel Format: \n\
        internal_format %u\n\
        format          %u\n\
        data_type       %u\n",
		id,
		boundTexUnit,
		boundImageUnit,
		currDims[0],
		currDims[1],
		texPixelFmt.internal,
		texPixelFmt.format,
		texPixelFmt.dtype
	);


#ifdef _DEBUG
	printf("    Texture Parameters: \n");
	for(auto& desc: texParams) {
		printf("        ID: %u Equals %u\n", desc.paramID, desc.paramValue);
	}
#endif

	return;
}



/* 
Implementation Details:
	iterate over y axis:
		iterate over x axis with increment=8:
			currPixel = MatrixToArrayIdx(x, y)
			load 16 bytes into A
			sign-extend-to-u32 First 8 Bytes of A, store into B
			convert u32's to f32 in B, store in C
			multiply C by 1/255 (constf32), store in D
			store D in resultBuffer[currPixel]
	return
*/


union U256f {
    __m256 v;
    float a[8];
	u32   b[8];
};

void print(const __m256 v)
{
    const U256f u = { v };

	printf("{ ");
    for (int i = 0; i < 8; ++i) {
        printf("%.6f ", u.a[i]);
	}
	printf("}\n");
}


void TextureConvertRGBAUI32ToRGBA32F(u8* inData, float* outData, i32 dimx, i32 dimy)
{
    size_t  currPixel = 0;
	__m256  constf32;
	__m128i A; 
	__m256i B;
	__m256  C;
	__m256  D;

	constf32 = _mm256_broadcastss_ps(_mm_set_ss(0.00392156862745098f)); /* load constant 1/255 to each element */
	for(i32 c = 0; c < 4; ++c) 
	{
		for(i32 y = 0; y < dimy; ++y) 
		{
			for(i32 x = 0; x < dimx; x += 8) 
			{
				A = _mm_lddqu_si128((const __m128i*) &inData[currPixel] ); /* Load 16 bytes 				          */
				B = _mm256_cvtepu8_epi32 (A); 	     	                  /* Convert first 8 bytes to u32's       */
				C = _mm256_cvtepi32_ps(B);   	     	                  /* Convert u32's to f32's 		      */
				D = _mm256_mul_ps(C, constf32);                         /* Multiply f32's by (1/255). 		  */
			    _mm256_store_ps(&outData[currPixel], D);                /* Store result in appropriate location */
				currPixel += 8;
			}
		}  
	}

	return;
}