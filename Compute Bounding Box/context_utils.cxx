/* $RCSfile: context_utils.cxx,v $
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

#include <rose.h>
#include <stp_schema.h>
#include "context_utils.h"

//----------------------------------------
// CONTEXT OBJECTS -- Context information is boilerplate information
// that is the same for all files defined by a particular AP, although
// it does change a bit from AP to AP.  
//
// Context Description parameters for AP214
//
#define AP_SCHEMA_NAME		"automotive_design"
#define AP_STATUS_DESC		"international standard"
#define AP_CONTENTS_DESC \
   "Core Data for Automotive Mechanical Design Process"

#define AP_PUBLICATION_YEAR	2003 /* Edition 2 */

#define AP_CONTEXT_NAME		"3D Mechanical Parts"
#define AP_DISCIPLINE		"mechanical"
#define AP_LIFECYCLE_STAGE	"design"

// The application context identifies the application protocol and
// area of use.  Normally only one is needed in a given file.  Here we
// search for one in a file and if none are present, we create one.
// 
// Once we find/create it, we keep a shortcut to the context using the
// design nametable.
//

stp_application_context * make_ap_context (
    RoseDesign * d
    )
{
    const char * shortcut = "__CONSTANT APPLICATION CONTEXT";

    /* use the default design if none is provided */
    if (!d) d = ROSE.design();

    /* Have we already found or created one and saved a shortcut to it 
     * in the design nametable?  If so, just return that one.
     */
    RoseObject * obj = d-> findObject (shortcut);
    if (obj) return ROSE_CAST (stp_application_context, obj);

    /* This must be a new design or one newly read in, search it to
     * see if there is already an application context.  There may be
     * more than one in the file, but we just look for the first one.
     */
    RoseCursor objs;
    objs.traverse (d);
    objs.domain (ROSE_DOMAIN(stp_application_context));
    obj = objs.next();
    if (obj) {
	d-> addName (shortcut, obj);
	return ROSE_CAST (stp_application_context, obj);
    }

    /* No application context in the file.  Create a new one and
     * return it.  The application string describes the application
     * protocol.
     */
    // AP214 - Core Data for Automotive Mechanical Design Process
    // AP203 - configuration controlled 3D designs of mechanical
    //		parts and assemblies
    stp_application_context * ctx = 
	pnewIn(d) stp_application_context;

    ctx-> application (AP_CONTENTS_DESC);

    /* The application_protocol_definition further specifies the AP.
     * Status is the status of the AP within the standardization
     * process.  The schema name is the name specified in the EXPRESS
     * file for the AP.  The year is the year that the standard was
     * released at the specified status.  
     */
    stp_application_protocol_definition* apdef = 
	pnewIn(d) stp_application_protocol_definition;

    apdef->status(AP_STATUS_DESC);
    apdef->application_interpreted_model_schema_name (AP_SCHEMA_NAME);
    apdef->application_protocol_year (AP_PUBLICATION_YEAR);
    apdef->application (ctx);

    d-> addName (shortcut, ctx);
    return ctx;
}



// A product is required to have a product_context which identifies
// engineering discipline's point of view the data is being presented
// by the AP properties (mechanical, electrical, etc.)  Normally only
// one is needed in a given file.  Here we search for one in a file
// and if none are present, we create one.
// 
stp_product_context * make_product_context (
    RoseDesign * d
    )
{
    const char * shortcut = "__CONSTANT PRODUCT CONTEXT";

    /* use the default design if none is provided */
    if (!d) d = ROSE.design();

    /* Have we already found or created one and saved a shortcut to it 
     * in the design nametable?  If so, just return that one.
     */
    RoseObject * obj = d-> findObject (shortcut);
    if (obj) return ROSE_CAST (stp_product_context, obj);

    /* This must be a new design or one newly read in, search it to
     * see if there is already an application context.  There may be
     * more than one in the file, but we just look for the first one.
     */
    RoseCursor objs;
    objs.traverse (d);
    objs.domain (ROSE_DOMAIN(stp_product_context));
    obj = objs.next();
    if (obj) {
	d-> addName (shortcut, obj);
	return ROSE_CAST (stp_product_context, obj);
    }

    stp_product_context* ctx = pnewIn(d) stp_product_context;
    ctx-> name (AP_CONTEXT_NAME);
    ctx-> discipline_type (AP_DISCIPLINE);
    ctx-> frame_of_reference (make_ap_context(d));

    d-> addName (shortcut, ctx);
    return ctx;
}


// The product definition context which identifies the the life cycle
// stage or maturity of the data being presented.  Normally only one
// is needed in a given file.  Here we search for one in a file and if
// none are present, we create one.
// 
stp_product_definition_context * make_pdef_context (
    RoseDesign * d
    )
{
    const char * shortcut = "__CONSTANT PRODUCT DEFINITION CONTEXT";

    /* use the default design if none is provided */
    if (!d) d = ROSE.design();

    /* Have we already found or created one and saved a shortcut to it 
     * in the design nametable?  If so, just return that one.
     */
    RoseObject * obj = d-> findObject (shortcut);
    if (obj) return ROSE_CAST (stp_product_definition_context, obj);

    /* This must be a new design or one newly read in, search it to
     * see if there is already an application context.  There may be
     * more than one in the file, but we just look for the first one.
     */
    RoseCursor objs;
    objs.traverse (d);
    objs.domain (ROSE_DOMAIN(stp_product_definition_context));
    obj = objs.next();
    if (obj) {
	d-> addName (shortcut, obj);
	return ROSE_CAST (stp_product_definition_context, obj);
    }

    stp_product_definition_context* ctx = 
	pnewIn(d) stp_product_definition_context;

    ctx-> name (AP_CONTEXT_NAME);
    ctx-> life_cycle_stage (AP_LIFECYCLE_STAGE);
    ctx-> frame_of_reference (make_ap_context(d));

    d-> addName (shortcut, ctx);
    return ctx;
}
