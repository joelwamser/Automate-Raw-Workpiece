/* $RCSfile: geometry_utils.h,v $
 * $Revision: 1.5 $ $Date: 2015/04/30 18:41:24 $
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
// GEOMETRY UTILITIES  -- 
//

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
    double length_uncertainty = ROSE_NULL_REAL
    );

// Axis2_placements are used to place things.
// In assemblies they are used to define the transformation from the
// parent coordinate system to the child systems
stp_axis2_placement_3d * make_axis2_placement_3d (
    RoseDesign * design,
    const char * id,
    double x, double y, double z,
    double i, double j, double k,
    double a, double b, double c
    );

// Define the direction for an axis in a placement
stp_direction * make_direction (
    RoseDesign * design, 
    const char *id,
    double i, double j, double k);

// ======================================================================
// ADD PRESENTATION COLOR INFORMATION -- Build color presentation
// information and assign it to a particular face.  This creates a
// pre-defined color with a symbolic name.  Using a different type of
// presentation entity, you can make a RGB color.
//
// The STEP definitions define the following predefined color strings:
// 'red', 'green', 'blue', 'yellow', 'magenta', 'cyan', 'black', and
// 'white'
//
// This is used by the box creating functions
//
void color_face (
    stp_advanced_face * face,
    const char * color
    );
