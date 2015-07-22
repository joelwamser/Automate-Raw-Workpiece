/* $RCSfile: make_box.cxx,v $
 * $Revision: 1.5 $ $Date: 2015/04/30 18:41:24 $
 * Auth: Martin Hardwick (hardwick@steptools.com)
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

// #include <stp_schema.h>

// for color_face
// #include "geometry_utils.h"
#include "make_box.h"


// CREATE BOX GEOMETRY - Function to create brep geometry and topology
// for any rectangular box aligned along the XYZ axes.  The top face
// is centered at the (x, y, z) and is x_width units by y_width.  This
// code is same as for fixed size box with key values replaced by
// parameters.
// 
// The function also takes a string color name, and if non-null,
// applies presentation information to the box faces to mark them as
// that color.
// 

void create_any_box (
    stp_shape_representation *shape, 
    double x, 
    double y, 
    double z, 
    double x_width, 
    double y_width, 
    double z_width, 
    const char *color 	// red green yellow cyan magenta black white 
    )
{
    // faces made in the order top, bottom, front, back, left, right

    // make top face of box - code for this face sets the pattern for the rest
    stp_vertex_point *tv1 = pnew stp_vertex_point;
    stp_cartesian_point *tpnt1 = pnew stp_cartesian_point;

    tpnt1->coordinates()->add (x - x_width / 2);	
    tpnt1->coordinates()->add (y - y_width / 2);	
    tpnt1->coordinates()->add (z);
    tv1->vertex_geometry (tpnt1);

    stp_vertex_point *tv2 = pnew stp_vertex_point;
    stp_cartesian_point *tpnt2 = pnew stp_cartesian_point;

    tpnt2->coordinates()->add (x + x_width / 2);	
    tpnt2->coordinates()->add (y - y_width / 2);	
    tpnt2->coordinates()->add (z);
    tv2->vertex_geometry (tpnt2);

    stp_vertex_point *tv3 = pnew stp_vertex_point;
    stp_cartesian_point *tpnt3 = pnew stp_cartesian_point;

    tpnt3->coordinates()->add (x + x_width / 2);	
    tpnt3->coordinates()->add (y + y_width / 2);	
    tpnt3->coordinates()->add (z);
    tv3->vertex_geometry (tpnt3);

    stp_vertex_point *tv4 = pnew stp_vertex_point;
    stp_cartesian_point *tpnt4 = pnew stp_cartesian_point;

    tpnt4->coordinates()->add (x - x_width / 2);	
    tpnt4->coordinates()->add (y + y_width / 2);	
    tpnt4->coordinates()->add (z);
    tv4->vertex_geometry (tpnt4);

    stp_line *tln1 = pnew stp_line;
    stp_vector *tve1 = pnew stp_vector;
    stp_direction *td1 = pnew stp_direction;
    stp_edge_curve *tec1 = pnew stp_edge_curve;
    stp_oriented_edge *tedge1 = pnew stp_oriented_edge;

    td1->direction_ratios()->add (1)->add(0)->add(0);
    tve1->orientation (td1);
    tve1->magnitude (x_width);
    tln1->pnt (tpnt1);
    tln1->dir (tve1);
    tec1->edge_geometry (tln1);
    tec1->same_sense (1);
    tec1->edge_start (tv1);
    tec1->edge_end (tv2);
    tedge1->edge_element (tec1);
    tedge1->orientation (1);

    stp_line *tln2 = pnew stp_line;
    stp_vector *tve2 = pnew stp_vector;
    stp_direction *td2 = pnew stp_direction;
    stp_edge_curve *tec2 = pnew stp_edge_curve;
    stp_oriented_edge *tedge2 = pnew stp_oriented_edge;

    td2->direction_ratios()->add (0)->add(1)->add(0);
    tve2->orientation (td2);
    tve2->magnitude (y_width);
    tln2->pnt (tpnt2);
    tln2->dir (tve2);
    tec2->edge_geometry (tln2);
    tec2->same_sense (1);
    tec2->edge_start (tv2);
    tec2->edge_end (tv3);
    tedge2->edge_element (tec2);
    tedge2->orientation (1);

    stp_line *tln3 = pnew stp_line;
    stp_vector *tve3 = pnew stp_vector;
    stp_direction *td3 = pnew stp_direction;
    stp_edge_curve *tec3 = pnew stp_edge_curve;
    stp_oriented_edge *tedge3 = pnew stp_oriented_edge;

    td3->direction_ratios()->add (-1)->add(0)->add(0);
    tve3->orientation (td3);
    tve3->magnitude (x_width);
    tln3->pnt (tpnt3);
    tln3->dir (tve3);
    tec3->edge_geometry (tln3);
    tec3->same_sense (1);
    tec3->edge_start (tv3);
    tec3->edge_end (tv4);
    tedge3->edge_element (tec3);
    tedge3->orientation (1);

    stp_line *tln4 = pnew stp_line;
    stp_vector *tve4 = pnew stp_vector;
    stp_direction *td4 = pnew stp_direction;
    stp_edge_curve *tec4 = pnew stp_edge_curve;
    stp_oriented_edge *tedge4 = pnew stp_oriented_edge;

    td4->direction_ratios()->add (0)->add(-1)->add(0);
    tve4->orientation (td4);
    tve4->magnitude (y_width);
    tln4->pnt (tpnt4);
    tln4->dir (tve4);
    tec4->edge_geometry (tln4);
    tec4->same_sense (1);
    tec4->edge_start (tv4);
    tec4->edge_end (tv1);
    tedge4->edge_element (tec4);
    tedge4->orientation (1);

    stp_advanced_face *tface1 = pnew stp_advanced_face;
    stp_plane *tplane1 = pnew stp_plane;

    stp_axis2_placement_3d *tax1 = pnew stp_axis2_placement_3d;
    stp_cartesian_point *torigin = pnew stp_cartesian_point;
    torigin->coordinates ()->add(x)->add(y)->add(z);
    stp_direction *tz_axis = pnew stp_direction;
    tz_axis->direction_ratios()->add(0)->add(0)->add(1);
    stp_direction *tx_axis = pnew stp_direction;
    tx_axis->direction_ratios()->add (1)->add(0)->add(0);

    tax1->location (torigin);
    tax1->axis (tz_axis);
    tax1->ref_direction (tx_axis);

    tplane1->position (tax1);
    tface1->face_geometry (tplane1);
    tface1->same_sense (1);

    stp_edge_loop *tloop1 = pnew stp_edge_loop;
    tloop1->edge_list()->add (tedge1);
    tloop1->edge_list()->add (tedge2);
    tloop1->edge_list()->add (tedge3);
    tloop1->edge_list()->add (tedge4);

    stp_face_outer_bound *bnd1 = pnew stp_face_outer_bound;
    bnd1->bound (tloop1);
    bnd1->orientation (1);

    tface1->bounds()->add (bnd1);

    stp_manifold_solid_brep *brep = pnew stp_manifold_solid_brep;
    stp_closed_shell *shell = pnew stp_closed_shell;
    shell->cfs_faces()->add (tface1);
    brep->outer (shell);

    shape->items()->add (brep);

    // Can display model with single face in the ST-Viewer if you exit here
    // return;

    // make bottom face.  Same code as top face but with rotation direction reversed
    // replace "t" with "b" in each variable name
    stp_vertex_point *bv1 = pnew stp_vertex_point;
    stp_cartesian_point *bpnt1 = pnew stp_cartesian_point;

    bpnt1->coordinates()->add (x - x_width/2);	
    bpnt1->coordinates()->add (y - y_width/2);	
    bpnt1->coordinates()->add (z - z_width);
    bv1->vertex_geometry (bpnt1);

    stp_vertex_point *bv2 = pnew stp_vertex_point;
    stp_cartesian_point *bpnt2 = pnew stp_cartesian_point;

    bpnt2->coordinates()->add (x + x_width/2);	
    bpnt2->coordinates()->add (y - y_width/2);	
    bpnt2->coordinates()->add (z - z_width);
    bv2->vertex_geometry (bpnt2);

    stp_vertex_point *bv3 = pnew stp_vertex_point;
    stp_cartesian_point *bpnt3 = pnew stp_cartesian_point;

    bpnt3->coordinates()->add (x + x_width/2);	
    bpnt3->coordinates()->add (y + y_width/2);	
    bpnt3->coordinates()->add (z - z_width);
    bv3->vertex_geometry (bpnt3);

    stp_vertex_point *bv4 = pnew stp_vertex_point;
    stp_cartesian_point *bpnt4 = pnew stp_cartesian_point;

    bpnt4->coordinates()->add (x - x_width/2);	
    bpnt4->coordinates()->add (y + y_width/2);	
    bpnt4->coordinates()->add (z - z_width);
    bv4->vertex_geometry (bpnt4);

    stp_line * bln1 = pnew stp_line;
    stp_vector *bve1 = pnew stp_vector;
    stp_direction *bd1 = pnew stp_direction;
    stp_edge_curve * bec1 = pnew stp_edge_curve;
    stp_oriented_edge *bedge1 = pnew stp_oriented_edge;

    bd1->direction_ratios()->add (1)->add(0)->add(0);
    bve1->orientation (bd1);
    bve1->magnitude (x_width);
    bln1->pnt (bpnt1);
    bln1->dir (bve1);
    bec1->edge_geometry (bln1);
    bec1->same_sense (1);
    bec1->edge_start (bv1);
    bec1->edge_end (bv2);
    bedge1->edge_element (bec1);
    bedge1->orientation (0);			// every edge must be used in positive and negative directions

    stp_line * bln2 = pnew stp_line;
    stp_vector *bve2 = pnew stp_vector;
    stp_direction *bd2 = pnew stp_direction;
    stp_edge_curve * bec2 = pnew stp_edge_curve;
    stp_oriented_edge *bedge2 = pnew stp_oriented_edge;

    bd2->direction_ratios()->add (0)->add(1)->add(0);
    bve2->orientation (bd2);
    bve2->magnitude (y_width);
    bln2->pnt (bpnt2);
    bln2->dir (bve2);
    bec2->edge_geometry (bln2);
    bec2->same_sense (1);
    bec2->edge_start (bv2);
    bec2->edge_end (bv3);
    bedge2->edge_element (bec2);
    bedge2->orientation (0);

    stp_line * bln3 = pnew stp_line;
    stp_vector *bve3 = pnew stp_vector;
    stp_direction *bd3 = pnew stp_direction;
    stp_edge_curve * bec3 = pnew stp_edge_curve;
    stp_oriented_edge *bedge3 = pnew stp_oriented_edge;

    bd3->direction_ratios()->add (-1)->add(0)->add(0);
    bve3->orientation (bd3);
    bve3->magnitude (x_width);
    bln3->pnt (bpnt3);
    bln3->dir (bve3);
    bec3->edge_geometry (bln3);
    bec3->same_sense (1);
    bec3->edge_start (bv3);
    bec3->edge_end (bv4);
    bedge3->edge_element (bec3);
    bedge3->orientation (0);

    stp_line * bln4 = pnew stp_line;
    stp_vector *bve4 = pnew stp_vector;
    stp_direction *bd4 = pnew stp_direction;
    stp_edge_curve * bec4 = pnew stp_edge_curve;
    stp_oriented_edge *bedge4 = pnew stp_oriented_edge;

    bd4->direction_ratios()->add (0)->add(-1)->add(0);
    bve4->orientation (bd4);
    bve4->magnitude (y_width);
    bln4->pnt (bpnt4);
    bln4->dir (bve4);
    bec4->edge_geometry (bln4);
    bec4->same_sense (1);
    bec4->edge_start (bv4);
    bec4->edge_end (bv1);
    bedge4->edge_element (bec4);
    bedge4->orientation (0);

    stp_advanced_face *bface1 = pnew stp_advanced_face;
    stp_plane *bplane1 = pnew stp_plane;

    stp_axis2_placement_3d *bax1 = pnew stp_axis2_placement_3d;
    stp_cartesian_point *borigin = pnew stp_cartesian_point;
    borigin->coordinates ()->add(x)->add(y)->add(z - z_width);	
    stp_direction *bz_axis = pnew stp_direction;
    bz_axis->direction_ratios()->add(0)->add(0)->add(1);
    stp_direction *bx_axis = pnew stp_direction;
    bx_axis->direction_ratios()->add (1)->add(0)->add(0);

    bax1->location (borigin);
    bax1->axis (bz_axis);
    bax1->ref_direction (bx_axis);

    bplane1->position (bax1);
    bface1->face_geometry (bplane1);
    bface1->same_sense (0); // opposite orientation to first face

    stp_edge_loop *bloop1 = pnew stp_edge_loop;
    bloop1->edge_list()->add (bedge4);		// all edges reverse of top face
    bloop1->edge_list()->add (bedge3);
    bloop1->edge_list()->add (bedge2);
    bloop1->edge_list()->add (bedge1);

    stp_face_outer_bound *bbnd1 = pnew stp_face_outer_bound;
    bbnd1->bound (bloop1);
    bbnd1->orientation (1); 

    bface1->bounds()->add (bbnd1);
    shell->cfs_faces()->add (bface1);
	// return;

    // Now fill in the four middle faces
    // No new points needed and some edges reused but in opposite orientation

    // make front face

    // fln1 already exists as bln1/bedge1
    stp_oriented_edge *fedge1 = pnew stp_oriented_edge;
    fedge1->edge_element (bec1);
    fedge1->orientation (1);		// reverse of orientation on bottom face

    stp_line * fln2 = pnew stp_line;
    stp_vector *fve2 = pnew stp_vector;
    stp_direction *fd2 = pnew stp_direction;
    stp_edge_curve * fec2 = pnew stp_edge_curve;
    stp_oriented_edge *fedge2 = pnew stp_oriented_edge;

    fd2->direction_ratios()->add (0)->add(0)->add(1);
    fve2->orientation (fd2);
    fve2->magnitude (z_width);
    fln2->pnt (bpnt2);
    fln2->dir (fve2);
    fec2->edge_geometry (fln2);
    fec2->same_sense (1);
    fec2->edge_start (bv2);
    fec2->edge_end (tv2);
    fedge2->edge_element (fec2);
    fedge2->orientation (1);

    // fln3 already exists as tln1/tedge1 (reversed)
    stp_oriented_edge *fedge3 = pnew stp_oriented_edge;
    fedge3->edge_element (tec1);
    fedge3->orientation (0);	// reverse of top

    // New edge from (-5, -2.5, -3) to  (-5, -2.5, 0)
    stp_line * fln4 = pnew stp_line;
    stp_vector *fve4 = pnew stp_vector;
    stp_direction *fd4 = pnew stp_direction;
    stp_edge_curve * fec4 = pnew stp_edge_curve;
    stp_oriented_edge *fedge4 = pnew stp_oriented_edge;

    fd4->direction_ratios()->add (0)->add(0)->add(-1);
    fve4->orientation (fd4);
    fve4->magnitude (z_width);
    fln4->pnt (tpnt1);
    fln4->dir (fve4);
    fec4->edge_geometry (fln4);
    fec4->same_sense (1);
    fec4->edge_start (tv1);
    fec4->edge_end (bv1);
    fedge4->edge_element (fec4);
    fedge4->orientation (1);

    stp_advanced_face *fface1 = pnew stp_advanced_face;
    stp_plane *fplane1 = pnew stp_plane;

    stp_axis2_placement_3d *fax1 = pnew stp_axis2_placement_3d;
    stp_cartesian_point *forigin = pnew stp_cartesian_point;
    forigin->coordinates ()->add(x)->add(y - y_width/2)->add(z - z_width/2);
    stp_direction *fz_axis = pnew stp_direction;
    fz_axis->direction_ratios()->add(0)->add(-1)->add(0);
    stp_direction *fx_axis = pnew stp_direction;
    fx_axis->direction_ratios()->add (1)->add(0)->add(0);

    fax1->location (forigin);
    fax1->axis (fz_axis);
    fax1->ref_direction (fx_axis);

    fplane1->position (fax1);
    fface1->face_geometry (fplane1);
    fface1->same_sense (1);

    stp_edge_loop *floop1 = pnew stp_edge_loop;
    floop1->edge_list()->add (fedge1);
    floop1->edge_list()->add (fedge2);
    floop1->edge_list()->add (fedge3);
    floop1->edge_list()->add (fedge4);

    stp_face_outer_bound *fbnd1 = pnew stp_face_outer_bound;
    fbnd1->bound (floop1);
    fbnd1->orientation (1);

    fface1->bounds()->add (fbnd1);
    shell->cfs_faces()->add (fface1);
	// return;

    // make back face (coded as k because b is bottom)
	// same as front face but with all orientations reversed

    // kln1 already exists as bln3/bedge3 (reversed)
    stp_oriented_edge *kedge1 = pnew stp_oriented_edge;
    kedge1->edge_element (bec3);
    kedge1->orientation (1);

    stp_line * kln2 = pnew stp_line;
    stp_vector *kve2 = pnew stp_vector;
    stp_direction *kd2 = pnew stp_direction;
    stp_edge_curve * kec2 = pnew stp_edge_curve;
    stp_oriented_edge *kedge2 = pnew stp_oriented_edge;

    kd2->direction_ratios()->add (0)->add (0)->add (1);
    kve2->orientation (kd2);
    kve2->magnitude (z_width);
    kln2->pnt (bpnt3);
    kln2->dir (kve2);
    kec2->edge_geometry (kln2);
    kec2->same_sense (1);
    kec2->edge_start (bv3);
    kec2->edge_end (tv3);
    kedge2->edge_element (kec2);
    kedge2->orientation (0);

    // kln3 already exists as tln3/tedge3 (reversed)
    stp_oriented_edge *kedge3 = pnew stp_oriented_edge;
    kedge3->edge_element (tec3);
    kedge3->orientation (0);

    stp_line * kln4 = pnew stp_line;
    stp_vector *kve4 = pnew stp_vector;
    stp_direction *kd4 = pnew stp_direction;
    stp_edge_curve * kec4 = pnew stp_edge_curve;
    stp_oriented_edge *kedge4 = pnew stp_oriented_edge;

    kd4->direction_ratios()->add (0)->add (0)->add (-1);
    kve4->orientation (kd4);
    kve4->magnitude (z_width);
    kln4->pnt (tpnt4);
    kln4->dir (kve4);
    kec4->edge_geometry (kln4);
    kec4->same_sense (1);
    kec4->edge_start (tv4);
    kec4->edge_end (bv4);
    kedge4->edge_element (kec4);
    kedge4->orientation (0);

    stp_advanced_face *kface1 = pnew stp_advanced_face;
    stp_plane *kplane1 = pnew stp_plane;

    stp_axis2_placement_3d *kax1 = pnew stp_axis2_placement_3d;
    stp_cartesian_point *korigin = pnew stp_cartesian_point;
    korigin->coordinates ()->add(x)->add(y + y_width/2)->add(z - z_width/2);
    stp_direction *kz_axis = pnew stp_direction;
    kz_axis->direction_ratios()->add(0)->add(-1)->add(0);
    stp_direction *kx_axis = pnew stp_direction;
    kx_axis->direction_ratios()->add (-1)->add(0)->add(0);

    kax1->location (korigin);
    kax1->axis (kz_axis);
    kax1->ref_direction (kx_axis);

    kplane1->position (kax1);
    kface1->face_geometry (kplane1);
    kface1->same_sense (0);

    stp_edge_loop *kloop1 = pnew stp_edge_loop;
    kloop1->edge_list()->add (kedge4);
    kloop1->edge_list()->add (kedge3);
    kloop1->edge_list()->add (kedge2);
    kloop1->edge_list()->add (kedge1);

    stp_face_outer_bound *kbnd1 = pnew stp_face_outer_bound;
    kbnd1->bound (kloop1);
    kbnd1->orientation (1);

    kface1->bounds()->add (kbnd1);
    shell->cfs_faces()->add (kface1);

    // Left and right faces - all points, lines and edges have been made
    // Make two new planes and add topology with existing edges reversed

    // Left face
    // lln1 already exists as bln4/bec4 (reversed)
    stp_oriented_edge *ledge1 = pnew stp_oriented_edge;
    ledge1->edge_element (bec4);
    ledge1->orientation (1);		// reversed

    // lln2 already exists as fln4/fec4 (reversed)
    stp_oriented_edge *ledge2 = pnew stp_oriented_edge;
    ledge2->edge_element (fec4);
    ledge2->orientation (0);

    // lln3 already exists as tln4/tec4 (reversed)
    stp_oriented_edge *ledge3 = pnew stp_oriented_edge;
    ledge3->edge_element (tec4);
    ledge3->orientation (0);	
	
    // lln4 already exists as kln4/kec4 (reversed)
    stp_oriented_edge *ledge4 = pnew stp_oriented_edge;
    ledge4->edge_element (kec4);
    ledge4->orientation (1);

    stp_advanced_face *lface1 = pnew stp_advanced_face;
    stp_plane *lplane1 = pnew stp_plane;

    stp_axis2_placement_3d *lax1 = pnew stp_axis2_placement_3d;
    stp_cartesian_point *lorigin = pnew stp_cartesian_point;
    lorigin->coordinates ()->add(x - x_width / 2)->add(y)->add(z - z_width/2);
    stp_direction *lz_axis = pnew stp_direction;
    lz_axis->direction_ratios()->add(-1)->add(0)->add(0);
    stp_direction *lx_axis = pnew stp_direction;
    lx_axis->direction_ratios()->add (0)->add(1)->add(0);

    lax1->location (lorigin);
    lax1->axis (lz_axis);
    lax1->ref_direction (lx_axis);

    lplane1->position (lax1);
    lface1->face_geometry (lplane1);
    lface1->same_sense (1);

    stp_edge_loop *lloop1 = pnew stp_edge_loop;
    lloop1->edge_list()->add (ledge1)->add(ledge2)->add(ledge3)->add (ledge4);

    stp_face_outer_bound *lbnd1 = pnew stp_face_outer_bound;
    lbnd1->bound (lloop1);
    lbnd1->orientation (1);

    lface1->bounds()->add (lbnd1);
    shell->cfs_faces()->add (lface1);
	
	// Right face (done as opposite of left)
    // rln1 already exists as bln2/ec2 (reversed)
    stp_oriented_edge *redge1 = pnew stp_oriented_edge;
    redge1->edge_element (bec2);
    redge1->orientation (1);

    // rln2 already exists as fln2/fec2 (reversed)
    stp_oriented_edge *redge2 = pnew stp_oriented_edge;
    redge2->edge_element (fec2);
    redge2->orientation (0);

    // rln3 already exists as tln2/bec2 (reversed)
    stp_oriented_edge *redge3 = pnew stp_oriented_edge;
    redge3->edge_element (tec2);
    redge3->orientation (0);	
	
    // rln4 already exists as kln2/kec2 (reversed)
    stp_oriented_edge *redge4 = pnew stp_oriented_edge;
    redge4->edge_element (kec2);
    redge4->orientation (1);

    stp_advanced_face *rface1 = pnew stp_advanced_face;
    stp_plane *rplane1 = pnew stp_plane;

    stp_axis2_placement_3d *rax1 = pnew stp_axis2_placement_3d;
    stp_cartesian_point *rorigin = pnew stp_cartesian_point;
    rorigin->coordinates ()->add(x + x_width/2)->add(y)->add(z - z_width/2);
    stp_direction *rz_axis = pnew stp_direction;
    rz_axis->direction_ratios()->add(-1)->add(0)->add(0);
    stp_direction *rx_axis = pnew stp_direction;
    rx_axis->direction_ratios()->add (0)->add(-1)->add(0);

    rax1->location (rorigin);
    rax1->axis (rz_axis);
    rax1->ref_direction (rx_axis);

    rplane1->position (rax1);
    rface1->face_geometry (rplane1);
    rface1->same_sense (0);

    stp_edge_loop *rloop1 = pnew stp_edge_loop;
    rloop1->edge_list()->add (redge4);
    rloop1->edge_list()->add (redge3);
    rloop1->edge_list()->add (redge2);
    rloop1->edge_list()->add (redge1);

    stp_face_outer_bound *rbnd1 = pnew stp_face_outer_bound;
    rbnd1->bound (rloop1);
    rbnd1->orientation (1);

    rface1->bounds()->add (rbnd1);
    shell->cfs_faces()->add (rface1);

    if (color) {
		color_face (tface1, color); 
		color_face (bface1, color);
		color_face (fface1, color);
		color_face (kface1, color);
		color_face (lface1, color);
		color_face (rface1, color);
    }

    return;
}
