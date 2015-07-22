/* $RCSfile: unit_utils.h,v $
 * $Revision: 1.3 $ $Date: 2015/04/30 18:41:24 $
 * Auth: Dave Loffredo (loffredo@steptools.com)
 * 
 * Copyright (c) 1991-2015 by STEP Tools Inc. 
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and
 * its documentation is hereby granted, provided that this copyright
 * notice and license appear on all copies of the software.
 * 
 * STEP TOOLS MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE
 * SUITABILITY OF THE SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT. STEP TOOLS
 * SHALL NOT BE LIABLE FOR ANY DAMAGES SUFFERED BY LICENSEE AS A
 * RESULT OF USING, MODIFYING OR DISTRIBUTING THIS SOFTWARE OR ITS
 * DERIVATIVES.
 */


/* Utilities for printing out STEP units and measure values */

extern void print_unit (stp_unit * u);
extern void print_measure_value (stp_measure_value * m);


// Construct the various common SI and English units.
//
stp_named_unit * make_mm_unit(
    RoseDesign * d = 0
    );

stp_named_unit * make_inch_unit(
    RoseDesign * d = 0
    );

stp_named_unit * make_radian_unit(
    RoseDesign * d = 0
    );

stp_named_unit * make_degree_unit(
    RoseDesign * d = 0
    );

stp_named_unit * make_steradian_unit(
    RoseDesign * d = 0
    );
