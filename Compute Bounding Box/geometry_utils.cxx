/* $RCSfile: geometry_utils.cxx,v $
 * $Revision: 1.6 $ $Date: 2015/04/30 18:41:24 $
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

#include <stp_schema.h>
#include "geometry_utils.h"
#include "unit_utils.h"


// The geometric context information gives the number of dimensions
// (usually 3), length, angle, and solid angle units, and potentially
// a global uncertainty measure for comparing brep point locations.
//
// This information is held by a single representation context object
// that is an instance of the following subtypes:
//
//    - geometric_representation_context
//    - global_uncertainty_assigned_context
//    - global_unit_assigned_context (if an uncertainty is given)
//
stp_representation_context * make_geometry_context (
    RoseDesign * design,
    const char * id,
    unsigned dims,
    stp_named_unit * len_unit,
    stp_named_unit * ang_unit,
    stp_named_unit * solid_ang_unit,
    double length_uncertainty
    )
{
    stp_geometric_representation_context * 	as_geo;
    stp_global_unit_assigned_context * 		as_units;
    stp_global_uncertainty_assigned_context * 	as_uncert;

    if (length_uncertainty == ROSE_NULL_REAL) {
	stp_geometric_representation_context_and_global_unit_assigned_context * ctx = 
	    pnewIn (design) stp_geometric_representation_context_and_global_unit_assigned_context;
	as_geo = ctx;
	as_units = ctx;
	as_uncert = 0;	// no uncertainty part
    }
    else {
	stp_geometric_representation_context_and_global_uncertainty_assigned_context_and_global_unit_assigned_context * ctx = 
	    pnewIn (design) stp_geometric_representation_context_and_global_uncertainty_assigned_context_and_global_unit_assigned_context;
	as_geo = ctx;
	as_units = ctx;
	as_uncert = ctx;
    }

    // Set the general geometric part of the context
    char buf[50]; 
    sprintf (buf, "%dD", dims);
    as_geo-> context_identifier ("ID1");
    as_geo-> context_type (buf);
    as_geo-> coordinate_space_dimension (dims);

    // Set the global units part of the context.  Each of the units is
    // wrapped in an instance of the stp_unit select type.
    //
    stp_unit * u;

    u = pnewIn(design) stp_unit;
    u-> _named_unit (len_unit);
    as_units-> units()-> add (u);

    u = pnewIn(design) stp_unit;
    u-> _named_unit (ang_unit);
    as_units-> units()-> add (u);

    u = pnewIn(design) stp_unit;
    u-> _named_unit (solid_ang_unit);
    as_units-> units()-> add (u);

    // Set the uncertainty value if given
    if (as_uncert) {
	stp_uncertainty_measure_with_unit * umwu = 
	    pnewIn(design) stp_uncertainty_measure_with_unit;

	umwu-> value_component (pnewIn(design) stp_measure_value);
	umwu-> value_component ()-> _length_measure (length_uncertainty);

	umwu-> unit_component (pnewIn(design) stp_unit);
	umwu-> unit_component ()-> _named_unit (len_unit);

	umwu-> name ("DISTANCE_ACCURACY_VALUE");
	umwu-> description ("Maximum model space distance between geometric "
			    "entities at asserted connectivities");

	as_uncert-> uncertainty()-> add (umwu);
    }
    return as_geo;
}

stp_axis2_placement_3d * make_axis2_placement_3d (
    RoseDesign * design,
    const char * id,
    double x, double y, double z,
    double i, double j, double k,
    double a, double b, double c
    )
{
    stp_axis2_placement_3d * the_axis = pnewIn(design) stp_axis2_placement_3d;
    the_axis->name (id);

    stp_cartesian_point *the_point = pnewIn(design) stp_cartesian_point;
    the_point->name (id);

    the_point->coordinates()->add(x);
    the_point->coordinates()->add(y);
    the_point->coordinates()->add(z);

    stp_direction * the_axis_direction = make_direction (design, id, i, j, k);
    stp_direction * the_ref_direction = make_direction (design, id, a, b, c);	

    the_axis->location (the_point);
    the_axis->axis (the_axis_direction);
    the_axis->ref_direction (the_ref_direction);

    return the_axis;
}

stp_direction * make_direction (
    RoseDesign * design, 
    const char *id,
    double i, double j, double k)
{
    stp_direction * the_d;

    the_d = pnewIn(design) stp_direction;
    the_d->name (id);
    the_d->direction_ratios()->add (i);
    the_d->direction_ratios()->add (j);
    the_d->direction_ratios()->add (k);

    return the_d;
}


// ======================================================================
// ADD PRESENTATION COLOR INFORMATION -- Build color presentation
// information and assign it to a particular face.  This creates a
// pre-defined color with a symbolic name.  Using a different type of
// presentation entity, you can make a RGB color.
//
// This is used by the box creating functions

// This is in stix as  stix_put_styled_repitem()

void put_styled_repitem ( 
    stp_styled_item * si,
    stp_representation_item * ri
    )
{
    if (!si) return;
    stp_styled_item_target * sel = si-> item();
    
    if (!sel) {
	sel = pnewIn(si->design()) stp_styled_item_target;
	si-> item(sel);
    }

    rose_put_nested_object (sel, ri);
}



void color_face (
    stp_advanced_face *face, 
    const char * color
    ) 
{
    RoseCursor objs;
    RoseObject * obj;

    RoseDesign * design = face-> design();

    // This part adds color to the top face
    stp_styled_item *style = pnewIn(design) stp_styled_item;
    put_styled_repitem (style, face);
	
    stp_presentation_style_assignment *psa = 
	pnewIn(design) stp_presentation_style_assignment;
    style->styles ()->add (psa);
	
    // There is a long way and a short way. This is the short way The
    // long way appears to give you more options on how a face and its
    // boundary and rendered
    psa->styles ()->add (pnewIn(design) stp_presentation_style_select);

    stp_fill_area_style *fas = pnewIn(design) stp_fill_area_style;
    fas->fill_styles ()->add (pnewIn(design) stp_fill_style_select);

    stp_fill_area_style_colour *fasc = pnewIn(design) stp_fill_area_style_colour;
    fas->fill_styles ()->get(0)->_fill_area_style_colour (fasc);

    // The color instances can be shared by looking for an existing
    // predefined color instance with the required name.
    //
    stp_draughting_pre_defined_colour *colour = 0;

    objs.traverse (design);
    objs.domain (ROSE_DOMAIN(stp_draughting_pre_defined_colour));
    while ((obj = objs.next()) != 0) {
	stp_draughting_pre_defined_colour * pdc = 
	    ROSE_CAST(stp_draughting_pre_defined_colour, obj);
	if (pdc && pdc-> name() && !strcmp(pdc-> name(), color))
	    colour = pdc;
    }
    if (!colour) {
	colour = pnewIn(design) stp_draughting_pre_defined_colour;
	colour->name (color); // English vs USA spelling finally useful!
    }

    fasc->fill_colour (colour);
    psa->styles()->get(0)->_fill_area_style (fas);

    // All presentation information is owned by a mechanical design
    // geometric presentation representation instance.  This rep does
    // not need to be attached to anything.  Find one in the file or
    // create a new one if needed.
    //
    stp_mechanical_design_geometric_presentation_representation *color_bag = 0;
	
    objs.traverse(design);
    objs.domain(
	ROSE_DOMAIN(stp_mechanical_design_geometric_presentation_representation)
	);

    if ((obj = objs.next()) != 0) {
	color_bag = ROSE_CAST(stp_mechanical_design_geometric_presentation_representation,obj);
    }
    else {
	color_bag = pnewIn(design) stp_mechanical_design_geometric_presentation_representation;
    }
    color_bag->items()->add (style);
}
