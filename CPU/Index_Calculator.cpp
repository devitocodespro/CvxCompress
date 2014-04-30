#include <stdio.h>
#include <stdlib.h>
#include "Index_Calculator.hxx"

Index_Calculator::Index_Calculator(int Target_Platform, int num_fields, int nx, int ny, int nz, int sky_z, int xh, int yh, int zh)
{
	_Target_Platform = Target_Platform;
	_num_fields = num_fields;
	_nx = nx;
	_ny = ny;
	_nz = nz;
	_sky_z = sky_z;
	_xh = xh;
	_yh = yh;
	_zh = zh;
}

Index_Calculator::~Index_Calculator()
{
}

unsigned long Index_Calculator::Get_Field_Offset()
{
	if (_Target_Platform == 1)
	{
		unsigned long stride_z = 16;
		unsigned long stride_field = stride_z * _nz;
		return stride_field;
	}
	else
	{
		return 0;
	}
}

unsigned long Index_Calculator::Calculate_Index(int iX, int iY, int iZ)
{
	unsigned long max_idx = (_nx+2*_xh) * (_ny+2*_yh) * (_nz+2*_zh) - 1;
	if (_Target_Platform == 0)
	{
		unsigned long dimxh = _nx + 2 * _xh;
		unsigned long dimyh = _ny + 2 * _yh;
		unsigned long dimzh = _nz + 2 * _zh;
		unsigned long stride_y = dimxh;
		unsigned long stride_z = stride_y * dimyh;
		unsigned long X = (unsigned long)iX + _xh;
		unsigned long Y = (unsigned long)iY + _yh;
		unsigned long Z = (unsigned long)iZ + _zh + _sky_z;
		unsigned long idx = Z * stride_z + Y * stride_y + X;
		//printf("iX=%d, iY=%d, iZ=%d, idx=%ld\n",iX,iY,iZ,idx);
		if (idx > max_idx) 
		{
			printf("Error! idx=%ld, max_idx=%ld, iX=%d, iY=%d, iZ=%d\n",idx,max_idx,iX,iY,iZ);
			exit(-1);
		}
		return idx;
	}
	else if (_Target_Platform == 1)
	{
		max_idx = max_idx * _num_fields;
		unsigned long stride_z = 16;
		unsigned long stride_field = stride_z * _nz;
		unsigned long stride_y = _num_fields * stride_field;
		unsigned long stride_blk = stride_y * _ny;
		unsigned long blk_idx = (unsigned long)(iX >> 4);
		unsigned long X = (unsigned long)(iX & 15);
		unsigned long Y = (unsigned long)iY;
		unsigned long Z = (unsigned long)iZ + _sky_z;
		unsigned long idx = blk_idx * stride_blk + Y * stride_y + Z * stride_z + X;
		if (idx > max_idx) 
		{
			printf("Error! idx=%ld, max_idx=%ld\n",idx,max_idx);
			exit(-1);
		}
		return idx;
	}
	else
	{
		fprintf(stderr, "ERROR! Target_Platform %d not supported!\n",_Target_Platform);
		return 0;
	}
}
