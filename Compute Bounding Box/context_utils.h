/* $RCSfile: context_utils.h,v $
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

//----------------------------------------
// CONTEXT OBJECTS -- Context information is boilerplate information
// that is the same for all files defined by a particular AP, although
// it does change a bit from AP to AP.  
//


// The application context identifies the application protocol and
// area of use.  Normally only one is needed in a given file.  Here we
// search for one in a file and if none are present, we create one.
// 
stp_application_context * make_ap_context (
    RoseDesign * design = 0
    );


// A product is required to have a product_context which identifies
// engineering discipline's point of view the data is being presented
// by the AP properties (mechanical, electrical, etc.)  Normally only
// one is needed in a given file.  Here we search for one in a file
// and if none are present, we create one.
// 
stp_product_context * make_product_context (
    RoseDesign * design = 0
    );

// The product definition context which identifies the the life cycle
// stage or maturity of the data being presented.  Normally only one
// is needed in a given file.  Here we search for one in a file and if
// none are present, we create one.
// 
stp_product_definition_context * make_pdef_context (
    RoseDesign * design = 0
    );
