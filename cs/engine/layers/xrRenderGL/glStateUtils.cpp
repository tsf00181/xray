#include "stdafx.h"
#include "glStateUtils.h"

namespace glStateUtils
{

GLenum ConvertCullMode(u32 Mode)
{
	switch (Mode)
	{
	//case D3DCULL_NONE:
	//	return ;
	case D3DCULL_CW:
		return GL_FRONT;
	case D3DCULL_CCW:
		return GL_BACK;
	default:
		VERIFY(!"Unexpected cull mode!");
		return GL_FRONT_AND_BACK;
	}
}

GLenum ConvertCmpFunction(u32 Func)
{
	switch (Func)
	{
	case D3DCMP_NEVER:
		return GL_NEVER;
	case D3DCMP_LESS:
		return GL_LESS;
	case D3DCMP_EQUAL:
		return GL_EQUAL;
	case D3DCMP_LESSEQUAL:
		return GL_LEQUAL;
	case D3DCMP_GREATER:
		return GL_GREATER;
	case D3DCMP_NOTEQUAL:
		return GL_NOTEQUAL;
	case D3DCMP_GREATEREQUAL:
		return GL_GEQUAL;
	case D3DCMP_ALWAYS:
		return GL_ALWAYS;
	default:
		VERIFY(!"ConvertCmpFunction can't convert argument!");
		return GL_ALWAYS;
	}
}

GLenum ConvertStencilOp(u32 Op)
{
	switch (Op)
	{
	case D3DSTENCILOP_KEEP:
		return GL_KEEP;
	case D3DSTENCILOP_ZERO:
		return GL_ZERO;
	case D3DSTENCILOP_REPLACE:
		return GL_REPLACE;
	case D3DSTENCILOP_INCRSAT:
		return GL_INCR;
	case D3DSTENCILOP_DECRSAT:
		return GL_DECR;
	case D3DSTENCILOP_INVERT:
		return GL_INVERT;
	case D3DSTENCILOP_INCR:
		return GL_INCR_WRAP;
	case D3DSTENCILOP_DECR:
		return GL_DECR_WRAP;
	default:
		VERIFY(!"ConvertStencilOp can't convert argument!");
		return GL_KEEP;
	}
}

GLenum ConvertBlendArg(u32 Arg)
{
	switch (Arg)
	{
	case D3DBLEND_ZERO:
		return GL_ZERO;
	case D3DBLEND_ONE:
		return GL_ONE;
	case D3DBLEND_SRCCOLOR:
		return GL_SRC_COLOR;
	case D3DBLEND_INVSRCCOLOR:
		return GL_ONE_MINUS_SRC_COLOR;
	case D3DBLEND_SRCALPHA:
		return GL_SRC_ALPHA;
	case D3DBLEND_INVSRCALPHA:
		return GL_ONE_MINUS_SRC_ALPHA;
	case D3DBLEND_DESTALPHA:
		return GL_DST_ALPHA;
	case D3DBLEND_INVDESTALPHA:
		return GL_ONE_MINUS_DST_ALPHA;
	case D3DBLEND_DESTCOLOR:
		return GL_DST_COLOR;
	case D3DBLEND_INVDESTCOLOR:
		return GL_ONE_MINUS_DST_COLOR;
	case D3DBLEND_SRCALPHASAT:
		return GL_SRC_ALPHA_SATURATE;
	//case D3DBLEND_BOTHSRCALPHA:
	//	return ;
	//case D3DBLEND_BOTHINVSRCALPHA:
	//	return ;
	//case D3DBLEND_BLENDFACTOR:
	//	return ;
	//case D3DBLEND_INVBLENDFACTOR:
	//	return ;
	//case D3DBLEND_SRCCOLOR2:
	//	return ;
	//case D3DBLEND_INVSRCCOLOR2:
	//	return ;
	default:
		VERIFY(!"ConvertBlendArg can't convert argument!");
		return GL_ONE;
	}
}

GLenum ConvertBlendOp(u32 Op)
{
	switch (Op)
	{
	case D3DBLENDOP_ADD:
		return GL_FUNC_ADD;
	case D3DBLENDOP_SUBTRACT:
		return GL_FUNC_SUBTRACT;
	case D3DBLENDOP_REVSUBTRACT:
		return GL_FUNC_REVERSE_SUBTRACT;
	case D3DBLENDOP_MIN:
		return GL_MIN;
	case D3DBLENDOP_MAX:
		return GL_MAX;
	default:
		VERIFY(!"ConvertBlendOp can't convert argument!");
		return GL_FUNC_ADD;
	}
}

GLenum	ConvertTextureAddressMode(u32 Mode)
{
	switch(Mode)
	{
	case D3DTADDRESS_WRAP:
		return GL_REPEAT;
	case D3DTADDRESS_MIRROR:
		return GL_MIRRORED_REPEAT;
	case D3DTADDRESS_CLAMP:
		return GL_CLAMP_TO_EDGE;
	case D3DTADDRESS_BORDER:
		return GL_CLAMP_TO_BORDER;
	//case D3DTADDRESS_MIRRORONCE:
	//	return ;
	default:
		VERIFY(!"ConvertTextureAddressMode can't convert argument!");
		return GL_CLAMP_TO_EDGE;
	}
}

};