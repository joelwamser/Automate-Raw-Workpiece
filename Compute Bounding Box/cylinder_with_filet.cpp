#include <rose.h>
#include <stp_schema.h>
#include "geometry_utils.h"
#include "context_utils.h"
#include "unit_utils.h"

stp_advanced_brep_shape_representation * create_empty_product_with_geometry();
void create_any_cylinder(stp_advanced_brep_shape_representation * advsol, double rad, double hei, double radius);

int main(int argc, char* argv[])
{
	/* Force optimizing linkers to bring in all C++ classes */
	stplib_init();

	/* Create a RoseDesign to hold the instance data */
	const char * output_name = "cylinder_with_filet";
	RoseDesign * design = new RoseDesign(output_name);

	/* Give the design some Part 21 header information */
	design->initialize_header();
	design->header_name()->originating_system("Cylinder Creation");
	design->header_description()->description()->
		add("Empty Shape Representation Sample File");

	ROSE.useDesign(design);

	stp_advanced_brep_shape_representation * cylin = create_empty_product_with_geometry();

	create_any_cylinder(cylin, 8, 10, 4);

	design->save();
	return 0;
}

stp_advanced_brep_shape_representation * create_empty_product_with_geometry()
{
	//----------------------------------------
	// CREATE THE PRODUCT -
	// Create a product object to represent our part. 
	stp_product* pr = pnew stp_product();

	// Fill in the part number, name, and description. 
	pr->id("1234-K789");
	pr->name("Cylinder");
	pr->description("the start of something new");

	// The context information is boilerplate information that is the
	// same for all files defined by a particular AP, although it does
	// change a bit from AP to AP.  See the context_utils.cxx file for
	// the specifics.
	//
	pr->frame_of_reference()->add(make_product_context());


	// The product needs a product definition formation to describe
	// the version with an id and description
	// 
	stp_product_definition_formation* pdf =
		pnew stp_product_definition_formation;

	pdf->of_product(pr);
	pdf->id("1.0");
	pdf->description("first version of cylinder creator");


	// The PDF needs a product definition.  This describes the
	// version from a particular point of view in the life cycle, so
	// most APs only have one per PDF, but some APs use several.

	stp_product_definition* pd = pnew stp_product_definition();
	pd->formation(pdf);
	pd->id("design");
	pd->description("Great Cylinder");

	// The context information is boilerplate information as above in
	// the product.  See the context_utils.cxx file for the specifics.
	// 
	pd->frame_of_reference(make_pdef_context());

	stp_advanced_brep_shape_representation * rep = pnew stp_advanced_brep_shape_representation;

	// Give the product a shape property.  Product_definition_shape is
	// a subtype of property_definition used for shape properties.  It 
	// refers to the product through the product_definition.
	//
	stp_product_definition_shape * pds =
		pnew stp_product_definition_shape;

	pds->name("");
	pds->definition(pnew stp_characterized_definition);
	pds->definition()->_characterized_product_definition
		(pnew stp_characterized_product_definition);
	pds->definition()->_characterized_product_definition()->
		_product_definition(pd);


	// Attach the shape representation to the property.  The
	// shape_definition_representation subtype is used for shape
	// properties and the property_definition_representation supertype
	// is used for other types of properties.
	//
	stp_shape_definition_representation * sdr =
		pnew stp_shape_definition_representation;

	sdr->definition(pnew stp_represented_definition);
	sdr->definition()->_property_definition(pds);
	sdr->used_representation(rep);

	stp_representation_context * rep_ctx = make_geometry_context(
		ROSE.design(),	// location to create the context
		"ID1", 		// some name for the context
		3, 		// number of dimensions, ie 3D
		make_mm_unit(), 	// length unit for geometry
		make_degree_unit(),	// angle unit for geometry
		make_steradian_unit(),	// solid angle unit
		1e-6
		);

	rep->name("");
	rep->context_of_items(rep_ctx);

	return rep;
}

void create_any_cylinder(stp_advanced_brep_shape_representation * advsol, double diam, double hei, double radius){

	//Variable to create radius entity for various entities used
	double temp = diam / 2.0;
	double maj_rad = temp - radius;

	if (radius > (temp)){ //Makes sure that the filet doesn't degenerate the solid
		printf("Filet radius too large");
		return;
	}

	if (temp == radius){ //Creates a sphere at bottom of cylinder instead of toroid
		
		//Cartesian_point declarations
		stp_cartesian_point* p1 = pnew stp_cartesian_point; //#23
		p1->name("");
		p1->coordinates()->add(temp);
		p1->coordinates()->add(0);
		p1->coordinates()->add(radius);

		stp_cartesian_point* p2 = pnew stp_cartesian_point; //#25
		p2->name("");
		p2->coordinates()->add(temp);
		p2->coordinates()->add(0);
		p2->coordinates()->add(hei);

		stp_cartesian_point* p3 = pnew stp_cartesian_point; //#28
		p3->name("");
		p3->coordinates()->add(temp);
		p3->coordinates()->add(0);
		p3->coordinates()->add(0);

		stp_cartesian_point* p4 = pnew stp_cartesian_point; //#34, #137, #150
		p4->name("");
		p4->coordinates()->add(0);
		p4->coordinates()->add(0);
		p4->coordinates()->add(0);

		stp_cartesian_point* p5 = pnew stp_cartesian_point; //#39
		p5->name("");
		p5->coordinates()->add(6.28318530718);
		p5->coordinates()->add(0);

		stp_cartesian_point* p6 = pnew stp_cartesian_point; //#46, #74, #101, #121, #156
		p6->name("");
		p6->coordinates()->add(0);
		p6->coordinates()->add(0);

		stp_cartesian_point* p7 = pnew stp_cartesian_point; //#55, #68
		p7->name("");
		p7->coordinates()->add(0);
		p7->coordinates()->add(0);
		p7->coordinates()->add(hei);

		stp_cartesian_point* p8 = pnew stp_cartesian_point; //#61
		p8->name("");
		p8->coordinates()->add(0);
		p8->coordinates()->add(hei);

		stp_cartesian_point* p9 = pnew stp_cartesian_point; //#83, #96
		p9->name("");
		p9->coordinates()->add(0);
		p9->coordinates()->add(0);
		p9->coordinates()->add(radius);

		stp_cartesian_point* p10 = pnew stp_cartesian_point; //#89
		p10->name("");
		p10->coordinates()->add(0);
		p10->coordinates()->add(radius);

		stp_cartesian_point* p11 = pnew stp_cartesian_point; //#111
		p11->name("");
		p11->coordinates()->add(maj_rad);
		p11->coordinates()->add(0);
		p11->coordinates()->add(0);

		stp_cartesian_point* p12 = pnew stp_cartesian_point; //#115
		p12->name("");
		p12->coordinates()->add(maj_rad);
		p12->coordinates()->add(0);
		p12->coordinates()->add(radius);

		stp_cartesian_point* p13 = pnew stp_cartesian_point; //#122, #143
		p13->name("");
		p13->coordinates()->add(0);
		p13->coordinates()->add(1.570796326795);

		stp_cartesian_point* p14 = pnew stp_cartesian_point; //#127
		p14->name("");
		p14->coordinates()->add(-6.28318530718);
		p14->coordinates()->add(0);

		stp_cartesian_point* p15 = pnew stp_cartesian_point; //#128
		p15->name("");
		p15->coordinates()->add(-6.28318530718);
		p15->coordinates()->add(1.570796326795);

		//Vertex Point Declarations
		stp_vertex_point * vp1 = pnew stp_vertex_point; //#22
		vp1->vertex_geometry(p1);

		stp_vertex_point * vp2 = pnew stp_vertex_point; //#24
		vp2->vertex_geometry(p2);

		stp_vertex_point * vp3 = pnew stp_vertex_point; //#110
		vp3->vertex_geometry(p11);

		//Direction Declarations
		stp_direction * dir1 = pnew stp_direction; //#30, #35, #56, #69, #84, #138, #151
		dir1->direction_ratios()->add(0);
		dir1->direction_ratios()->add(0);
		dir1->direction_ratios()->add(1);

		stp_direction * dir2 = pnew stp_direction; //#36, #57, #70, #85, #98, #117, #139, #152
		dir2->direction_ratios()->add(1);
		dir2->direction_ratios()->add(0);
		dir2->direction_ratios()->add(0);

		stp_direction * dir3 = pnew stp_direction; //#41, #48
		dir3->direction_ratios()->add(0);
		dir3->direction_ratios()->add(1);

		stp_direction * dir4 = pnew stp_direction; //#63, #75, #91, #157
		dir4->direction_ratios()->add(1);
		dir4->direction_ratios()->add(0);

		stp_direction * dir5 = pnew stp_direction; //#97
		dir5->direction_ratios()->add(0);
		dir5->direction_ratios()->add(0);
		dir5->direction_ratios()->add(-1);

		stp_direction * dir6 = pnew stp_direction; //#103, #145
		dir6->direction_ratios()->add(-1);
		dir6->direction_ratios()->add(0);

		stp_direction * dir7 = pnew stp_direction; //#116
		dir7->direction_ratios()->add(0);
		dir7->direction_ratios()->add(1);
		dir7->direction_ratios()->add(0);

		//Axis2_placement_2d and selects
		stp_axis2_placement_2d * a2p2d = pnew stp_axis2_placement_2d;
		a2p2d->location(p6);
		a2p2d->ref_direction(dir4);

		stp_axis2_placement * a2p1 = pnew stp_axis2_placement; //#73
		rose_put_nested_object(a2p1, a2p2d);

		//Axis2_placement_3d and selects
		stp_axis2_placement_3d * a2p3d = pnew stp_axis2_placement_3d("", p4, dir1, dir2); //#33

		stp_axis2_placement * a2p3 = pnew stp_axis2_placement;
		rose_put_nested_object(a2p3, a2p3d);

		stp_axis2_placement_3d * a2p3d2 = pnew stp_axis2_placement_3d("", p7, dir1, dir2); //#54, #67

		stp_axis2_placement * a2p4 = pnew stp_axis2_placement;
		rose_put_nested_object(a2p4, a2p3d2);

		stp_axis2_placement_3d * a2p3d3 = pnew stp_axis2_placement_3d("", p9, dir1, dir2); //#82

		stp_axis2_placement * a2p5 = pnew stp_axis2_placement;
		rose_put_nested_object(a2p5, a2p3d3);

		stp_axis2_placement_3d * a2p3d4 = pnew stp_axis2_placement_3d("", p9, dir5, dir2); //#95

		//Planes
		stp_plane * plane1 = pnew stp_plane; //#66
		plane1->position(a2p3d2);

		//Vectors
		stp_vector * vec1 = pnew stp_vector; //#29
		vec1->orientation(dir1);
		vec1->magnitude(1);

		stp_vector * vec2 = pnew stp_vector; //#40, #47
		vec2->orientation(dir3);
		vec2->magnitude(1);

		stp_vector * vec3 = pnew stp_vector; //#62, #90
		vec3->orientation(dir4);
		vec3->magnitude(1);

		stp_vector * vec4 = pnew stp_vector; //#102, #144
		vec4->orientation(dir6);
		vec4->magnitude(1);

		//Lines
		stp_line * l1 = pnew stp_line; //#27
		l1->pnt(p3);
		l1->dir(vec1);

		stp_line * l2 = pnew stp_line; //#38
		l2->pnt(p5);
		l2->dir(vec2);

		stp_line * l3 = pnew stp_line; //#45
		l3->pnt(p6);
		l3->dir(vec2);

		stp_line * l4 = pnew stp_line; //#60
		l4->pnt(p8);
		l4->dir(vec3);

		stp_line * l5 = pnew stp_line; //#88
		l5->pnt(p10);
		l5->dir(vec3);

		stp_line * l6 = pnew stp_line; //#100
		l6->pnt(p6);
		l6->dir(vec4);

		//Circles
		stp_circle * circ1 = pnew stp_circle; //#53
		circ1->position(a2p4);
		circ1->radius(temp);

		stp_circle * circ2 = pnew stp_circle; //#72
		circ2->position(a2p1);
		circ2->radius(temp);

		stp_circle * circ3 = pnew stp_circle; //#81
		circ3->position(a2p5);
		circ3->radius(temp);

		stp_circle * circ4 = pnew stp_circle; //#113
		circ4->position(a2p5);
		circ4->radius(temp);

		//Surface Geometry
		stp_cylindrical_surface * cs1 = pnew stp_cylindrical_surface; //#32
		cs1->position(a2p3d);
		cs1->radius(temp);

		stp_spherical_surface * ts1 = pnew stp_spherical_surface; //#94
		ts1->position(a2p3d4);
		ts1->radius(temp);

		//Geometric Representation Context
		stp_geometric_representation_context * rc1 = pnew stp_geometric_representation_context;
		rc1->context_identifier("REPRESENTATION_CONTEXT('2D SPACE','')");
		rc1->context_type("PARAMETRIC_REPRESENTATION_CONTEXT()");
		rc1->coordinate_space_dimension(2);

		//B_Spline Curve with Knots
		stp_b_spline_curve_with_knots * bscwk1 = pnew stp_b_spline_curve_with_knots;
		bscwk1->degree(1);
		bscwk1->control_points_list()->add(p6);
		bscwk1->control_points_list()->add(p13);
		bscwk1->curve_form(stp_b_spline_curve_form_unspecified);
		bscwk1->closed_curve(0);
		bscwk1->self_intersect(0);
		bscwk1->knot_multiplicities()->add(2);
		bscwk1->knot_multiplicities()->add(2);
		bscwk1->knots()->add(0);
		bscwk1->knots()->add(1.570796326795);
		bscwk1->knot_spec(stp_knot_type_piecewise_bezier_knots);

		stp_b_spline_curve_with_knots * bscwk2 = pnew stp_b_spline_curve_with_knots;
		bscwk2->degree(1);
		bscwk2->control_points_list()->add(p14);
		bscwk2->control_points_list()->add(p15);
		bscwk2->curve_form(stp_b_spline_curve_form_unspecified);
		bscwk2->closed_curve(0);
		bscwk2->self_intersect(0);
		bscwk2->knot_multiplicities()->add(2);
		bscwk2->knot_multiplicities()->add(2);
		bscwk2->knots()->add(0);
		bscwk2->knots()->add(1.570796326795);
		bscwk2->knot_spec(stp_knot_type_piecewise_bezier_knots);

		//Definitional Representation
		stp_definitional_representation * dr1 = pnew stp_definitional_representation; //#37
		dr1->items()->add(l2);
		dr1->context_of_items(rc1);

		stp_definitional_representation * dr2 = pnew stp_definitional_representation; //#44
		dr2->items()->add(l3);
		dr2->context_of_items(rc1);

		stp_definitional_representation * dr3 = pnew stp_definitional_representation; //#59
		dr3->items()->add(l4);
		dr3->context_of_items(rc1);

		stp_definitional_representation * dr4 = pnew stp_definitional_representation; //#71
		dr4->items()->add(circ2);
		dr4->context_of_items(rc1);

		stp_definitional_representation * dr5 = pnew stp_definitional_representation; //#87
		dr5->items()->add(l5);
		dr5->context_of_items(rc1);

		stp_definitional_representation * dr6 = pnew stp_definitional_representation; //#99
		dr6->items()->add(l6);
		dr6->context_of_items(rc1);

		stp_definitional_representation * dr7 = pnew stp_definitional_representation; //#119
		dr7->items()->add(bscwk1);
		dr7->context_of_items(rc1);

		stp_definitional_representation * dr8 = pnew stp_definitional_representation; //#125
		dr8->items()->add(bscwk2);
		dr8->context_of_items(rc1);

		//Pcurve definitions and selects
		stp_pcurve * pc1 = pnew stp_pcurve;
		pc1->basis_surface(cs1);
		pc1->reference_to_curve(dr1);

		stp_pcurve_or_surface * pcos1 = pnew stp_pcurve_or_surface; //#31
		rose_put_nested_object(pcos1, pc1);

		stp_pcurve * pc2 = pnew stp_pcurve;
		pc2->basis_surface(cs1);
		pc2->reference_to_curve(dr2);

		stp_pcurve_or_surface * pcos2 = pnew stp_pcurve_or_surface; //#43
		rose_put_nested_object(pcos2, pc2);

		stp_pcurve * pc3 = pnew stp_pcurve;
		pc3->basis_surface(cs1);
		pc3->reference_to_curve(dr3);

		stp_pcurve_or_surface * pcos3 = pnew stp_pcurve_or_surface; //#58
		rose_put_nested_object(pcos3, pc3);

		stp_pcurve * pc4 = pnew stp_pcurve;
		pc4->basis_surface(plane1);
		pc4->reference_to_curve(dr4);

		stp_pcurve_or_surface * pcos4 = pnew stp_pcurve_or_surface; //#65
		rose_put_nested_object(pcos4, pc4);

		stp_pcurve * pc5 = pnew stp_pcurve;
		pc5->basis_surface(cs1);
		pc5->reference_to_curve(dr5);

		stp_pcurve_or_surface * pcos5 = pnew stp_pcurve_or_surface; //#86
		rose_put_nested_object(pcos5, pc5);

		stp_pcurve * pc6 = pnew stp_pcurve;
		pc6->basis_surface(ts1);
		pc6->reference_to_curve(dr6);

		stp_pcurve_or_surface * pcos6 = pnew stp_pcurve_or_surface; //#93
		rose_put_nested_object(pcos6, pc6);

		stp_pcurve * pc7 = pnew stp_pcurve;
		pc7->basis_surface(ts1);
		pc7->reference_to_curve(dr7);

		stp_pcurve_or_surface * pcos7 = pnew stp_pcurve_or_surface; //#118
		rose_put_nested_object(pcos7, pc7);

		stp_pcurve * pc8 = pnew stp_pcurve;
		pc8->basis_surface(ts1);
		pc8->reference_to_curve(dr8);

		stp_pcurve_or_surface * pcos8 = pnew stp_pcurve_or_surface; //#124
		rose_put_nested_object(pcos8, pc8);

		//Surface Curves
		stp_surface_curve * sc1 = pnew stp_surface_curve; //#52
		sc1->curve_3d(circ1);
		sc1->associated_geometry()->add(pcos3);
		sc1->associated_geometry()->add(pcos4);
		sc1->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		stp_surface_curve * sc2 = pnew stp_surface_curve; //#80
		sc2->curve_3d(circ3);
		sc2->associated_geometry()->add(pcos5);
		sc2->associated_geometry()->add(pcos6);
		sc2->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		stp_seam_curve * seam1 = pnew stp_seam_curve; //#26
		seam1->curve_3d(l1);
		seam1->associated_geometry()->add(pcos1);
		seam1->associated_geometry()->add(pcos2);
		seam1->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		stp_seam_curve * seam2 = pnew stp_seam_curve; //#112
		seam2->curve_3d(circ4);
		seam2->associated_geometry()->add(pcos7);
		seam2->associated_geometry()->add(pcos8);
		seam2->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		//Edge Curve definition
		stp_edge_curve * ec1 = pnew stp_edge_curve; //#21
		ec1->edge_start(vp1);
		ec1->edge_end(vp2);
		ec1->edge_geometry(seam1);
		ec1->same_sense(1);

		stp_edge_curve * ec2 = pnew stp_edge_curve; //#51
		ec2->edge_start(vp2);
		ec2->edge_end(vp2);
		ec2->edge_geometry(sc1);
		ec2->same_sense(1);

		stp_edge_curve * ec3 = pnew stp_edge_curve; //#79
		ec3->edge_start(vp1);
		ec3->edge_end(vp1);
		ec3->edge_geometry(sc2);
		ec3->same_sense(1);

		stp_edge_curve * ec4 = pnew stp_edge_curve; //#109
		ec4->edge_start(vp1);
		ec4->edge_end(vp3);
		ec4->edge_geometry(seam2);
		ec4->same_sense(1);

		//Oriented Edge Definitions
		stp_oriented_edge * oe1 = pnew stp_oriented_edge; //#20
		oe1->edge_element(ec1);
		oe1->orientation(1);

		stp_oriented_edge * oe2 = pnew stp_oriented_edge; //#50
		oe2->edge_element(ec2);
		oe2->orientation(0);

		stp_oriented_edge * oe3 = pnew stp_oriented_edge; //#77
		oe3->edge_element(ec1);
		oe3->orientation(0);

		stp_oriented_edge * oe4 = pnew stp_oriented_edge; //#78
		oe4->edge_element(ec3);
		oe4->orientation(1);

		stp_oriented_edge * oe5 = pnew stp_oriented_edge; //#108
		oe5->edge_element(ec4);
		oe5->orientation(0);

		stp_oriented_edge * oe6 = pnew stp_oriented_edge; //#130
		oe6->edge_element(ec3);
		oe6->orientation(1);

		stp_oriented_edge * oe7 = pnew stp_oriented_edge; //#131
		oe7->edge_element(ec4);
		oe7->orientation(1);

		stp_oriented_edge * oe9 = pnew stp_oriented_edge; //#162
		oe9->edge_element(ec2);
		oe9->orientation(1);

		//Edge Loop definition and Axis2 Placements
		stp_edge_loop * el1 = pnew stp_edge_loop; //#19
		el1->edge_list()->add(oe1);
		el1->edge_list()->add(oe2);
		el1->edge_list()->add(oe3);
		el1->edge_list()->add(oe4);

		stp_edge_loop * el2 = pnew stp_edge_loop; //#107
		el2->edge_list()->add(oe5);
		el2->edge_list()->add(oe6);
		el2->edge_list()->add(oe7);

		stp_edge_loop * el3 = pnew stp_edge_loop; //#161
		el3->edge_list()->add(oe9);

		//Face bounds and surface declarations
		stp_face_bound * b1 = pnew stp_face_bound; //#18
		b1->bound(el1);
		b1->orientation(1);

		stp_face_bound * b2 = pnew stp_face_bound; //#106
		b2->bound(el2);
		b2->orientation(0);

		stp_face_bound * b3 = pnew stp_face_bound; //#160
		b3->bound(el3);
		b3->orientation(1);

		//Advanced Faces declarations
		stp_advanced_face * f1 = pnew stp_advanced_face; //#17
		f1->bounds()->add(b1);
		f1->face_geometry(cs1);
		f1->same_sense(1);

		stp_advanced_face * f2 = pnew stp_advanced_face; //#105
		f2->bounds()->add(b2);
		f2->face_geometry(ts1);
		f2->same_sense(1);

		stp_advanced_face * f3 = pnew stp_advanced_face; //#159
		f3->bounds()->add(b3);
		f3->face_geometry(plane1);
		f3->same_sense(1);


		//Closed Shell Declaration
		stp_closed_shell * sh = pnew stp_closed_shell;
		sh->cfs_faces()->add(f1);
		sh->cfs_faces()->add(f2);
		sh->cfs_faces()->add(f3);

		//Manifold solid brep and axis2_placement_3d 
		stp_manifold_solid_brep * brep = pnew stp_manifold_solid_brep;
		brep->outer(sh);

		advsol->items()->add(a2p3d); //Add everything to the shape rep
		advsol->items()->add(brep);

		return;
	}

	else{ //Creates any other type of cylinder with a filet on the bottom
		//Cartesian_point declarations
		stp_cartesian_point* p1 = pnew stp_cartesian_point; //#23
		p1->name("");
		p1->coordinates()->add(temp);
		p1->coordinates()->add(0);
		p1->coordinates()->add(radius);

		stp_cartesian_point* p2 = pnew stp_cartesian_point; //#25
		p2->name("");
		p2->coordinates()->add(temp);
		p2->coordinates()->add(0);
		p2->coordinates()->add(hei);

		stp_cartesian_point* p3 = pnew stp_cartesian_point; //#28
		p3->name("");
		p3->coordinates()->add(temp);
		p3->coordinates()->add(0);
		p3->coordinates()->add(0);

		stp_cartesian_point* p4 = pnew stp_cartesian_point; //#34, #137, #150
		p4->name("");
		p4->coordinates()->add(0);
		p4->coordinates()->add(0);
		p4->coordinates()->add(0);

		stp_cartesian_point* p5 = pnew stp_cartesian_point; //#39
		p5->name("");
		p5->coordinates()->add(6.28318530718);
		p5->coordinates()->add(0);

		stp_cartesian_point* p6 = pnew stp_cartesian_point; //#46, #74, #101, #121, #156
		p6->name("");
		p6->coordinates()->add(0);
		p6->coordinates()->add(0);

		stp_cartesian_point* p7 = pnew stp_cartesian_point; //#55, #68
		p7->name("");
		p7->coordinates()->add(0);
		p7->coordinates()->add(0);
		p7->coordinates()->add(hei);

		stp_cartesian_point* p8 = pnew stp_cartesian_point; //#61
		p8->name("");
		p8->coordinates()->add(0);
		p8->coordinates()->add(hei);

		stp_cartesian_point* p9 = pnew stp_cartesian_point; //#83, #96
		p9->name("");
		p9->coordinates()->add(0);
		p9->coordinates()->add(0);
		p9->coordinates()->add(radius);

		stp_cartesian_point* p10 = pnew stp_cartesian_point; //#89
		p10->name("");
		p10->coordinates()->add(0);
		p10->coordinates()->add(radius);

		stp_cartesian_point* p11 = pnew stp_cartesian_point; //#111
		p11->name("");
		p11->coordinates()->add(maj_rad);
		p11->coordinates()->add(0);
		p11->coordinates()->add(0);

		stp_cartesian_point* p12 = pnew stp_cartesian_point; //#115
		p12->name("");
		p12->coordinates()->add(maj_rad);
		p12->coordinates()->add(0);
		p12->coordinates()->add(radius);

		stp_cartesian_point* p13 = pnew stp_cartesian_point; //#122, #143
		p13->name("");
		p13->coordinates()->add(0);
		p13->coordinates()->add(1.570796326795);

		stp_cartesian_point* p14 = pnew stp_cartesian_point; //#127
		p14->name("");
		p14->coordinates()->add(-6.28318530718);
		p14->coordinates()->add(0);

		stp_cartesian_point* p15 = pnew stp_cartesian_point; //#128
		p15->name("");
		p15->coordinates()->add(-6.28318530718);
		p15->coordinates()->add(1.570796326795);

		//Vertex Point Declarations
		stp_vertex_point * vp1 = pnew stp_vertex_point; //#22
		vp1->vertex_geometry(p1);

		stp_vertex_point * vp2 = pnew stp_vertex_point; //#24
		vp2->vertex_geometry(p2);

		stp_vertex_point * vp3 = pnew stp_vertex_point; //#110
		vp3->vertex_geometry(p11);

		//Direction Declarations
		stp_direction * dir1 = pnew stp_direction; //#30, #35, #56, #69, #84, #138, #151
		dir1->direction_ratios()->add(0);
		dir1->direction_ratios()->add(0);
		dir1->direction_ratios()->add(1);

		stp_direction * dir2 = pnew stp_direction; //#36, #57, #70, #85, #98, #117, #139, #152
		dir2->direction_ratios()->add(1);
		dir2->direction_ratios()->add(0);
		dir2->direction_ratios()->add(0);

		stp_direction * dir3 = pnew stp_direction; //#41, #48
		dir3->direction_ratios()->add(0);
		dir3->direction_ratios()->add(1);

		stp_direction * dir4 = pnew stp_direction; //#63, #75, #91, #157
		dir4->direction_ratios()->add(1);
		dir4->direction_ratios()->add(0);

		stp_direction * dir5 = pnew stp_direction; //#97
		dir5->direction_ratios()->add(0);
		dir5->direction_ratios()->add(0);
		dir5->direction_ratios()->add(-1);

		stp_direction * dir6 = pnew stp_direction; //#103, #145
		dir6->direction_ratios()->add(-1);
		dir6->direction_ratios()->add(0);

		stp_direction * dir7 = pnew stp_direction; //#116
		dir7->direction_ratios()->add(0);
		dir7->direction_ratios()->add(1);
		dir7->direction_ratios()->add(0);

		//Axis2_placement_2d and selects
		stp_axis2_placement_2d * a2p2d = pnew stp_axis2_placement_2d;
		a2p2d->location(p6);
		a2p2d->ref_direction(dir4);

		stp_axis2_placement * a2p1 = pnew stp_axis2_placement; //#73
		rose_put_nested_object(a2p1, a2p2d);

		stp_axis2_placement_2d * a2p2d2 = pnew stp_axis2_placement_2d;
		a2p2d2->location(p6);
		a2p2d2->ref_direction(dir4);

		stp_axis2_placement * a2p2 = pnew stp_axis2_placement; //#155
		rose_put_nested_object(a2p2, a2p2d2);

		//Axis2_placement_3d and selects
		stp_axis2_placement_3d * a2p3d = pnew stp_axis2_placement_3d("", p4, dir1, dir2); //#33

		stp_axis2_placement * a2p3 = pnew stp_axis2_placement;
		rose_put_nested_object(a2p3, a2p3d);

		stp_axis2_placement_3d * a2p3d2 = pnew stp_axis2_placement_3d("", p7, dir1, dir2); //#54, #67

		stp_axis2_placement * a2p4 = pnew stp_axis2_placement;
		rose_put_nested_object(a2p4, a2p3d2);

		stp_axis2_placement_3d * a2p3d3 = pnew stp_axis2_placement_3d("", p9, dir1, dir2); //#82

		stp_axis2_placement * a2p5 = pnew stp_axis2_placement;
		rose_put_nested_object(a2p5, a2p3d3);

		stp_axis2_placement_3d * a2p3d4 = pnew stp_axis2_placement_3d("", p9, dir5, dir2); //#95

		stp_axis2_placement_3d * a2p3d5 = pnew stp_axis2_placement_3d("", p12, dir7, dir2); //#114

		stp_axis2_placement_3d * a2p3d6 = pnew stp_axis2_placement_3d("", p4, dir1, dir2); //#136, #149

		stp_axis2_placement * a2p6 = pnew stp_axis2_placement;
		rose_put_nested_object(a2p6, a2p3d6);

		//Planes
		stp_plane * plane1 = pnew stp_plane; //#66
		plane1->position(a2p3d2);

		stp_plane * plane2 = pnew stp_plane; //#148
		plane2->position(a2p3d6);

		//Vectors
		stp_vector * vec1 = pnew stp_vector; //#29
		vec1->orientation(dir1);
		vec1->magnitude(1);

		stp_vector * vec2 = pnew stp_vector; //#40, #47
		vec2->orientation(dir3);
		vec2->magnitude(1);

		stp_vector * vec3 = pnew stp_vector; //#62, #90
		vec3->orientation(dir4);
		vec3->magnitude(1);

		stp_vector * vec4 = pnew stp_vector; //#102, #144
		vec4->orientation(dir6);
		vec4->magnitude(1);

		//Lines
		stp_line * l1 = pnew stp_line; //#27
		l1->pnt(p3);
		l1->dir(vec1);

		stp_line * l2 = pnew stp_line; //#38
		l2->pnt(p5);
		l2->dir(vec2);

		stp_line * l3 = pnew stp_line; //#45
		l3->pnt(p6);
		l3->dir(vec2);

		stp_line * l4 = pnew stp_line; //#60
		l4->pnt(p8);
		l4->dir(vec3);

		stp_line * l5 = pnew stp_line; //#88
		l5->pnt(p10);
		l5->dir(vec3);

		stp_line * l6 = pnew stp_line; //#100
		l6->pnt(p6);
		l6->dir(vec4);

		stp_line * l7 = pnew stp_line; //#142
		l7->pnt(p13);
		l7->dir(vec4);

		//Circles
		stp_circle * circ1 = pnew stp_circle; //#53
		circ1->position(a2p4);
		circ1->radius(temp);

		stp_circle * circ2 = pnew stp_circle; //#72
		circ2->position(a2p1);
		circ2->radius(temp);

		stp_circle * circ3 = pnew stp_circle; //#81
		circ3->position(a2p5);
		circ3->radius(temp);

		stp_circle * circ4 = pnew stp_circle; //#113
		circ4->position(a2p3);
		circ4->radius(temp - radius);

		stp_circle * circ5 = pnew stp_circle; //#135
		circ5->position(a2p6);
		circ5->radius(temp - radius);

		stp_circle * circ6 = pnew stp_circle; //#154
		circ6->position(a2p2);
		circ6->radius(temp - radius);

		//Surface Geometry
		stp_cylindrical_surface * cs1 = pnew stp_cylindrical_surface; //#32
		cs1->position(a2p3d);
		cs1->radius(temp);

		stp_toroidal_surface * ts1 = pnew stp_toroidal_surface; //#94
		ts1->position(a2p3d4);
		ts1->major_radius(maj_rad);
		ts1->minor_radius(radius);

		//Geometric Representation Context
		stp_geometric_representation_context * rc1 = pnew stp_geometric_representation_context;
		rc1->context_identifier("REPRESENTATION_CONTEXT('2D SPACE','')");
		rc1->context_type("PARAMETRIC_REPRESENTATION_CONTEXT()");
		rc1->coordinate_space_dimension(2);

		//B_Spline Curve with Knots
		stp_b_spline_curve_with_knots * bscwk1 = pnew stp_b_spline_curve_with_knots;
		bscwk1->degree(1);
		bscwk1->control_points_list()->add(p6);
		bscwk1->control_points_list()->add(p13);
		bscwk1->curve_form(stp_b_spline_curve_form_unspecified);
		bscwk1->closed_curve(0);
		bscwk1->self_intersect(0);
		bscwk1->knot_multiplicities()->add(2);
		bscwk1->knot_multiplicities()->add(2);
		bscwk1->knots()->add(0);
		bscwk1->knots()->add(1.570796326795);
		bscwk1->knot_spec(stp_knot_type_piecewise_bezier_knots);

		stp_b_spline_curve_with_knots * bscwk2 = pnew stp_b_spline_curve_with_knots;
		bscwk2->degree(1);
		bscwk2->control_points_list()->add(p14);
		bscwk2->control_points_list()->add(p15);
		bscwk2->curve_form(stp_b_spline_curve_form_unspecified);
		bscwk2->closed_curve(0);
		bscwk2->self_intersect(0);
		bscwk2->knot_multiplicities()->add(2);
		bscwk2->knot_multiplicities()->add(2);
		bscwk2->knots()->add(0);
		bscwk2->knots()->add(1.570796326795);
		bscwk2->knot_spec(stp_knot_type_piecewise_bezier_knots);

		//Definitional Representation
		stp_definitional_representation * dr1 = pnew stp_definitional_representation; //#37
		dr1->items()->add(l2);
		dr1->context_of_items(rc1);

		stp_definitional_representation * dr2 = pnew stp_definitional_representation; //#44
		dr2->items()->add(l3);
		dr2->context_of_items(rc1);

		stp_definitional_representation * dr3 = pnew stp_definitional_representation; //#59
		dr3->items()->add(l4);
		dr3->context_of_items(rc1);

		stp_definitional_representation * dr4 = pnew stp_definitional_representation; //#71
		dr4->items()->add(circ2);
		dr4->context_of_items(rc1);

		stp_definitional_representation * dr5 = pnew stp_definitional_representation; //#87
		dr5->items()->add(l5);
		dr5->context_of_items(rc1);

		stp_definitional_representation * dr6 = pnew stp_definitional_representation; //#99
		dr6->items()->add(l6);
		dr6->context_of_items(rc1);

		stp_definitional_representation * dr7 = pnew stp_definitional_representation; //#119
		dr7->items()->add(bscwk1);
		dr7->context_of_items(rc1);

		stp_definitional_representation * dr8 = pnew stp_definitional_representation; //#125
		dr8->items()->add(bscwk2);
		dr8->context_of_items(rc1);

		stp_definitional_representation * dr9 = pnew stp_definitional_representation; //#141
		dr9->items()->add(l7);
		dr9->context_of_items(rc1);

		stp_definitional_representation * dr10 = pnew stp_definitional_representation; //#153
		dr10->items()->add(circ6);
		dr10->context_of_items(rc1);

		//Pcurve definitions and selects
		stp_pcurve * pc1 = pnew stp_pcurve;
		pc1->basis_surface(cs1);
		pc1->reference_to_curve(dr1);

		stp_pcurve_or_surface * pcos1 = pnew stp_pcurve_or_surface; //#31
		rose_put_nested_object(pcos1, pc1);

		stp_pcurve * pc2 = pnew stp_pcurve;
		pc2->basis_surface(plane1);
		pc2->reference_to_curve(dr2);

		stp_pcurve_or_surface * pcos2 = pnew stp_pcurve_or_surface; //#43
		rose_put_nested_object(pcos2, pc2);

		stp_pcurve * pc3 = pnew stp_pcurve;
		pc3->basis_surface(cs1);
		pc3->reference_to_curve(dr3);

		stp_pcurve_or_surface * pcos3 = pnew stp_pcurve_or_surface; //#58
		rose_put_nested_object(pcos3, pc3);

		stp_pcurve * pc4 = pnew stp_pcurve;
		pc4->basis_surface(plane1);
		pc4->reference_to_curve(dr4);

		stp_pcurve_or_surface * pcos4 = pnew stp_pcurve_or_surface; //#65
		rose_put_nested_object(pcos4, pc4);

		stp_pcurve * pc5 = pnew stp_pcurve;
		pc5->basis_surface(cs1);
		pc5->reference_to_curve(dr5);

		stp_pcurve_or_surface * pcos5 = pnew stp_pcurve_or_surface; //#86
		rose_put_nested_object(pcos5, pc5);

		stp_pcurve * pc6 = pnew stp_pcurve;
		pc6->basis_surface(ts1);
		pc6->reference_to_curve(dr6);

		stp_pcurve_or_surface * pcos6 = pnew stp_pcurve_or_surface; //#93
		rose_put_nested_object(pcos6, pc6);

		stp_pcurve * pc7 = pnew stp_pcurve;
		pc7->basis_surface(ts1);
		pc7->reference_to_curve(dr7);

		stp_pcurve_or_surface * pcos7 = pnew stp_pcurve_or_surface; //#118
		rose_put_nested_object(pcos7, pc7);

		stp_pcurve * pc8 = pnew stp_pcurve;
		pc8->basis_surface(ts1);
		pc8->reference_to_curve(dr8);

		stp_pcurve_or_surface * pcos8 = pnew stp_pcurve_or_surface; //#124
		rose_put_nested_object(pcos8, pc8);

		stp_pcurve * pc9 = pnew stp_pcurve;
		pc9->basis_surface(ts1);
		pc9->reference_to_curve(dr9);

		stp_pcurve_or_surface * pcos9 = pnew stp_pcurve_or_surface; //#140
		rose_put_nested_object(pcos9, pc9);

		stp_pcurve * pc10 = pnew stp_pcurve;
		pc10->basis_surface(plane2);
		pc10->reference_to_curve(dr10);

		stp_pcurve_or_surface * pcos10 = pnew stp_pcurve_or_surface; //#147
		rose_put_nested_object(pcos10, pc10);

		//Surface Curves
		stp_surface_curve * sc1 = pnew stp_surface_curve; //#52
		sc1->curve_3d(circ1);
		sc1->associated_geometry()->add(pcos3);
		sc1->associated_geometry()->add(pcos4);
		sc1->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		stp_surface_curve * sc2 = pnew stp_surface_curve; //#80
		sc2->curve_3d(circ3);
		sc2->associated_geometry()->add(pcos5);
		sc2->associated_geometry()->add(pcos6);
		sc2->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		stp_surface_curve * sc3 = pnew stp_surface_curve; //#134
		sc3->curve_3d(circ5);
		sc3->associated_geometry()->add(pcos9);
		sc3->associated_geometry()->add(pcos10);
		sc3->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		stp_seam_curve * seam1 = pnew stp_seam_curve; //#26
		seam1->curve_3d(l1);
		seam1->associated_geometry()->add(pcos1);
		seam1->associated_geometry()->add(pcos2);
		seam1->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		stp_seam_curve * seam2 = pnew stp_seam_curve; //#112
		seam2->curve_3d(circ4);
		seam2->associated_geometry()->add(pcos7);
		seam2->associated_geometry()->add(pcos8);
		seam2->master_representation(stp_preferred_surface_curve_representation_pcurve_s1);

		//Edge Curve definition
		stp_edge_curve * ec1 = pnew stp_edge_curve; //#21
		ec1->edge_start(vp1);
		ec1->edge_end(vp2);
		ec1->edge_geometry(seam1);
		ec1->same_sense(1);

		stp_edge_curve * ec2 = pnew stp_edge_curve; //#51
		ec2->edge_start(vp2);
		ec2->edge_end(vp2);
		ec2->edge_geometry(sc1);
		ec2->same_sense(1);

		stp_edge_curve * ec3 = pnew stp_edge_curve; //#79
		ec3->edge_start(vp1);
		ec3->edge_end(vp1);
		ec3->edge_geometry(sc2);
		ec3->same_sense(1);

		stp_edge_curve * ec4 = pnew stp_edge_curve; //#109
		ec4->edge_start(vp1);
		ec4->edge_end(vp3);
		ec4->edge_geometry(seam2);
		ec4->same_sense(1);

		stp_edge_curve * ec5 = pnew stp_edge_curve; //#133
		ec5->edge_start(vp3);
		ec5->edge_end(vp3);
		ec5->edge_geometry(sc3);
		ec5->same_sense(1);

		//Oriented Edge Definitions
		stp_oriented_edge * oe1 = pnew stp_oriented_edge; //#20
		oe1->edge_element(ec1);
		oe1->orientation(1);

		stp_oriented_edge * oe2 = pnew stp_oriented_edge; //#50
		oe2->edge_element(ec2);
		oe2->orientation(0);

		stp_oriented_edge * oe3 = pnew stp_oriented_edge; //#77
		oe3->edge_element(ec1);
		oe3->orientation(0);

		stp_oriented_edge * oe4 = pnew stp_oriented_edge; //#78
		oe4->edge_element(ec3);
		oe4->orientation(1);

		stp_oriented_edge * oe5 = pnew stp_oriented_edge; //#108
		oe5->edge_element(ec4);
		oe5->orientation(0);

		stp_oriented_edge * oe6 = pnew stp_oriented_edge; //#130
		oe6->edge_element(ec3);
		oe6->orientation(1);

		stp_oriented_edge * oe7 = pnew stp_oriented_edge; //#131
		oe7->edge_element(ec4);
		oe7->orientation(1);

		stp_oriented_edge * oe8 = pnew stp_oriented_edge; //#132
		oe8->edge_element(ec5);
		oe8->orientation(0);

		stp_oriented_edge * oe9 = pnew stp_oriented_edge; //#162
		oe9->edge_element(ec2);
		oe9->orientation(1);

		stp_oriented_edge * oe10 = pnew stp_oriented_edge; //#166
		oe10->edge_element(ec5);
		oe10->orientation(1);

		//Edge Loop definition and Axis2 Placements
		stp_edge_loop * el1 = pnew stp_edge_loop; //#19
		el1->edge_list()->add(oe1);
		el1->edge_list()->add(oe2);
		el1->edge_list()->add(oe3);
		el1->edge_list()->add(oe4);

		stp_edge_loop * el2 = pnew stp_edge_loop; //#107
		el2->edge_list()->add(oe5);
		el2->edge_list()->add(oe6);
		el2->edge_list()->add(oe7);
		el2->edge_list()->add(oe8);

		stp_edge_loop * el3 = pnew stp_edge_loop; //#161
		el3->edge_list()->add(oe9);

		stp_edge_loop * el4 = pnew stp_edge_loop; //#165
		el4->edge_list()->add(oe10);

		//Face bounds and surface declarations
		stp_face_bound * b1 = pnew stp_face_bound; //#18
		b1->bound(el1);
		b1->orientation(1);

		stp_face_bound * b2 = pnew stp_face_bound; //#106
		b2->bound(el2);
		b2->orientation(0);

		stp_face_bound * b3 = pnew stp_face_bound; //#160
		b3->bound(el3);
		b3->orientation(1);

		stp_face_bound * b4 = pnew stp_face_bound; //#164
		b4->bound(el4);
		b4->orientation(0);

		//Advanced Faces declarations
		stp_advanced_face * f1 = pnew stp_advanced_face; //#17
		f1->bounds()->add(b1);
		f1->face_geometry(cs1);
		f1->same_sense(1);

		stp_advanced_face * f2 = pnew stp_advanced_face; //#105
		f2->bounds()->add(b2);
		f2->face_geometry(ts1);
		f2->same_sense(1);

		stp_advanced_face * f3 = pnew stp_advanced_face; //#159
		f3->bounds()->add(b3);
		f3->face_geometry(plane1);
		f3->same_sense(1);

		stp_advanced_face * f4 = pnew stp_advanced_face; //#163
		f4->bounds()->add(b4);
		f4->face_geometry(plane2);
		f4->same_sense(0);

		//Closed Shell Declaration
		stp_closed_shell * sh = pnew stp_closed_shell;
		sh->cfs_faces()->add(f1);
		sh->cfs_faces()->add(f2);
		sh->cfs_faces()->add(f3);
		sh->cfs_faces()->add(f4);

		//Manifold solid brep and axis2_placement_3d 
		stp_manifold_solid_brep * brep = pnew stp_manifold_solid_brep;
		brep->outer(sh);

		advsol->items()->add(a2p3d); //Add everything to the shape rep
		advsol->items()->add(brep);

		return;
	}
}