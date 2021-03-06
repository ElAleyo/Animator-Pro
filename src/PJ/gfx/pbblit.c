#define PROCBLIT_INTERNALS
#include "rastlib.h"
#include "gfx.h"
#include "memory.h"
#include "errcodes.h"
#include "ptrmacro.h"

Errcode procblit(Raster *src,  /* doesn't really need to be a ramrast. */
			Coor src_x, Coor src_y,
			Raster *dest, Coor dest_x, Coor dest_y, Ucoor width, Ucoor height,
			Procline pline, void *data )
{
Pixel *src_buf, *dest_buf;
Pixel stack_buf[512];

	if((width = pj_lclip2rects(&src_x,&dest_x,width,
											src->width,dest->width)) <= 0)
		return(Err_clipped);
	if((height = pj_lclip2rects(&src_y,&dest_y,height,
											src->height,dest->height)) <= 0)
		return(Err_clipped);

	if (src->type == RT_BYTEMAP)
	{
	#define Src ((Bytemap *)src)

		if(width <= Array_els(stack_buf))
			dest_buf = stack_buf;
		else if ((dest_buf = pj_malloc(sizeof(Pixel)*width)) == NULL)
			return(Err_no_memory);

		src_buf = Src->bm.bp[0] + src_y*Src->bm.bpr + src_x;
		while (height--)
		{
			GET_HSEG(dest,dest_buf,dest_x,dest_y,width);
			(*pline)(src_buf, dest_buf, width, data);
			src_buf += Src->bm.bpr;
			PUT_HSEG(dest,dest_buf,dest_x,dest_y++,width);
		}
	#undef Src
	}
	else
	{
		if(width <= Array_els(stack_buf)/2)
			dest_buf = stack_buf;
		else if ((dest_buf = pj_malloc(2*sizeof(Pixel)*width)) == NULL)
			return(Err_no_memory);
		src_buf = dest_buf+width;

		while(height--)
		{
			GET_HSEG(src,src_buf,src_x,src_y++,width);
			GET_HSEG(dest,dest_buf,dest_x,dest_y,width);
			(*pline)(src_buf, dest_buf, width, data);
			PUT_HSEG(dest,dest_buf,dest_x,dest_y++,width);
		}
	}
	if(dest_buf != stack_buf)
		pj_free(dest_buf);
	return(Success);
}
