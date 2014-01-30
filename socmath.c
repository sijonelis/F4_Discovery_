
/*! \file *********************************************************************
 *
 * \brief  socmath.c   Math functions for hardware FPU.
 *
 *      This file contains the function implementations the XMEGA Timer/Counter
 *      driver.
 *
 *      The driver is not intended for size and/or speed critical code, since
 *      most functions are just a few lines of code, and the function call
 *      overhead would decrease code performance. The driver is intended for
 *      rapid prototyping and documentation purposes for getting started with
 *      the XMEGA Timer/Counter module.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call.
 *
 *      Several functions use the following construct:
 *          "some_register = ... | (some_parameter ? SOME_BIT_bm : 0) | ..."
 *      Although the use of the ternary operator ( if ? then : else ) is discouraged,
 *      in some occasions the operator makes it possible to write pretty clean and
 *      neat code. In this driver, the construct is used to set or not set a
 *      configuration bit based on a boolean input parameter, such as
 *      the "some_parameter" in the example above.
 *
 *
 * \author
 *      
 *    
 *
 * $Revision: 1569 $
 * $Date: 2008-04-22 13:03:43 +0200 (ti, 22 apr 2008) $  \n
 *
 * Copyright (c) 2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

//#include "compiler.h"
#include "math.h"

// Taylor series expansion accurate from -pi to +pi
float sinfp(float x)
{
	float result3,result5,result7,result9,result11,sinxvalue,sinx2value;
	
	sinxvalue = x;
	sinx2value = x * x;
	result3 = sinxvalue * sinxvalue * sinxvalue;
	result5 = result3 * sinx2value;
	result7 = result5 * sinx2value;
	result9 = result7 * sinx2value;
	result11 = result9 * sinx2value;
	
	sinxvalue += (-(float)1.0/(float)6.0)*result3 + ((float)1.0/(float)120.0)*result5 
	              - ((float)1.0/(float)5040.0)*result7 + ((float)1.0/(float)362880.0)*result9 
				  - ((float)1.0/((float)3628800.0*(float)11.0))*result11;
	return sinxvalue;
}

// Taylor series expansion accurate from -pi to +pi
float cosfp(float x)
{
	float result2,result4,result6,result8,result10,cosxvalue;
	
	cosxvalue = 1;
	result2 = x * x;
	//result4 = result2 * x * x;
	//result6 = result4 * x * x;
	//result8 = result6 * x * x;
	//result10 = result8 *x * x;
	result4 = result2 * result2;
	result6 = result4 * result2;
	result8 = result6 * result2;
	result10 = result8 * result2;
	
	cosxvalue += (-(float)1.0/(float)2.0)*result2 + ((float)1.0/(float)24.0)*result4 
	              - ((float)1.0/(float)720.0)*result6 + ((float)1.0/(float)40320.0)*result8 
				  - ((float)1.0/(float)3628800.0)*result10;
	return cosxvalue;
}

// Taylor series expansion accurate from -pi to +pi
float tanfp(float x)
{
	float result3,result5,result7,result9,tanxvalue,tanx2value;
	
	tanxvalue = x;
	tanx2value = x * x;
	result3 = tanxvalue * tanx2value;
	result5 = result3 * tanx2value;
	result7 = result5 * tanx2value;
	result9 = result7 * tanx2value;
	
//	tanxvalue += ((float)1.0/(float)3.0)*result3 + ((float)2.0/(float)15.0)*result5 + ((float)17.0/(float)315.0)*result7;
	tanxvalue += ((float)1.0/(float)3.0)*result3 + ((float)2.0/(float)15.0)*result5 + ((float)17.0/(float)315.0)*result7 + ((float)62.0/(float)2835.0)*result9;
	return tanxvalue;
}

// Tan implementation using sinfp and cosfp - takes more than twice as long
float tanfp2(float x)
{
	return sinfp(x)/cosfp(x);
}

// Taylor series expansion accurate from -pi to +pi
float arctanfp(float x)
{
	float result3,result5,result7,result9,arctanxvalue,arctanx2value;
	
	arctanxvalue = x;
	arctanx2value = x * x;
	result3 = arctanxvalue * arctanx2value;
	result5 = result3 * arctanx2value;
	result7 = result5 * arctanx2value;
	result9 = result7 * arctanx2value;
	
//	arctanxvalue += (-(float)1.0/(float)3.0)*result3 + ((float)1.0/(float)5.0)*result5 - ((float)1.0/(float)7.0)*result7;
	arctanxvalue += (-(float)1.0/(float)3.0)*result3 + ((float)1.0/(float)5.0)*result5 - ((float)1.0/(float)7.0)*result7 + ((float)1.0/(float)9.0)*result9;
	return arctanxvalue;
}

// Taylor series expansion accurate from -pi to +pi
float lnfp(float x)
{
	float result3,result5,result7,result9,lnxvalue,lnx2value;
	
	lnxvalue = x;
	lnx2value = x * x;
	result3 = lnxvalue * lnx2value;
	result5 = result3 * lnx2value;
	result7 = result5 * lnx2value;
	result9 = result7 * lnx2value;
	
//	lnxvalue += ((float)1.0/(float)3.0)*result3 + ((float)1.0/(float)5.0)*result5 + ((float)1.0/(float)7.0)*result7;
	lnxvalue += ((float)1.0/(float)3.0)*result3 + ((float)1.0/(float)5.0)*result5 + ((float)1.0/(float)7.0)*result7 + ((float)1.0/(float)9.0)*result9;
	return 2*lnxvalue;
}

float sqrtp(float m)
{
	int j;
    float i=0;
	float x1,x2;
	
	while((i*i) <= m ) i += (float)0.1;
	x1 = i;
	for(j=0; j<10; j++) {
		x2 = m;
		x2 /= x1;
		x2 += x1;
		x2 /= 2;
		x1 = x2;
		}
   return x2;
}
