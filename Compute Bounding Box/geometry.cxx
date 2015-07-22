/* $RCSfile: geometry.cxx,v $
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

#include <rose.h>
#include <rose_p28.h>
#include <stp_schema.h>
#include "geometry_utils.h"
#include "context_utils.h"
#include "unit_utils.h"

// BUILD GEOMETRY - Build a product structure with an empty shape
// representation to hold our geometry.  This program builds all of
// the STEP backbone entities that go into creating a product with
// geometry.  
//
// Some of the details in the product structure differ from
// AP to AP, but most of this

// This program creates the data set and saves it as a STEP Part 21
// file called "output_file.stp".  This is a text file, so you can
// look at it with a text editor or the STEP Part 21 file browser to
// see how the information is represented.

stp_shape_representation * create_empty_product_with_geometry();

// Functions to create simple brep cubes.  One is written to make a
// specific size and position while the other is generalized so that
// you can play with different values.  These are in make_box.cxx and
// make_const_box.cxx
//
void create_one_10x5x3_box (stp_shape_representation *shape);
void create_any_box (
    stp_shape_representation *shape, 
    double x, double y, double z, 
    double x_width, double y_width, double z_width, 
    const char * color
    );



int main(int argc, char* argv[])
{
    stplib_init();	// initialize merged cad library
    rose_p28_init();	// support xml read/write

    /* Create a RoseDesign to hold the instance data */
    const char * output_name = "output_file";
    RoseDesign * design = new RoseDesign (output_name);

    // Declare it as an AP214 file
    stplib_put_schema (design, stplib_schema_ap214);

    // Give the design some Part 21 header information 
    design-> initialize_header();
    design-> header_name()-> originating_system ("Geometry Demo");
    design-> header_description()-> description ()-> 
	add ("Empty Shape Representation Sample File");

    // Set as the current design. Objects created using the "pnew"
    // macro go here.  We can create objects in a specific design
    // using "pnewIn(design)", but is less convenient to write.
    //
    ROSE.useDesign(design);

    // Call the code to create an empty shape
    stp_shape_representation * shape = create_empty_product_with_geometry();

    // Call the code to put a box into that shape
    create_one_10x5x3_box (shape);
    create_any_box (shape, 0, 0, 5, 4, 2, 1, "yellow");
    create_any_box (shape, 6, 6, 0, 4, 4, 4, "cyan");
    create_any_box (shape, 20, 20, 0, 10, 2, 0.25, "black");

    // Uncomment the following to write as zip compressed P28 XML
    // rather than Part 21 ASCII.  Change the string "p28-raw" to
    // write uncompressed XML.  The default is "p21".
    //
    // design-> format ("p28");

    design-> save();
    return 0;
}


stp_shape_representation * create_empty_product_with_geometry()
{
    //----------------------------------------
    // CREATE THE PRODUCT -
    // Create a product object to represent our part. 
    stp_product* pr = pnew stp_product();

    // Fill in the part number, name, and description. 
    pr-> id ("1234-K789");
    pr-> name ("widget");
    pr-> description ("a fictional product");

    // The context information is boilerplate information that is the
    // same for all files defined by a particular AP, although it does
    // change a bit from AP to AP.  See the context_utils.cxx file for
    // the specifics.
    //
    pr-> frame_of_reference()-> add (make_product_context());


    // The product needs a product definition formation to describe
    // the version with an id and description
    // 
    stp_product_definition_formation* pdf = 
	pnew stp_product_definition_formation;

    pdf-> of_product(pr);
    pdf-> id ("1.0");
    pdf-> description ("first version of our widget");


    // The PDF needs a product definition.  This describes a the
    // version from a particular point of view in the life cycle, so
    // most APs only have one per PDF, but some APs use several.
    //
    stp_product_definition* pd = pnew stp_product_definition();
    pd-> formation (pdf);
    pd-> id ("design");
    pd-> description ("example product_definition");

    // The context information is boilerplate information as above in
    // the product.  See the context_utils.cxx file for the specifics.
    // 
    pd-> frame_of_reference (make_pdef_context());


    //----------------------------------------
    // Depending on the application protocol, the product, PDF, and PD
    // may also need some configuration management information, like a
    // design owner, approvals, etc.  For simplicity, we do not create
    // any of that here, but a real application would.


    //----------------------------------------
    // CREATE AND ATTACH A SHAPE REPRESENTATION
    //
    // The geometric description of product shape is attached to the
    // product_definition as a property.  The way properties work in
    // STEP is with a series of objects:
    //
    //   (owner of the property) 
    //      <- property_definition
    // 		<- property_definition_representation -> 
    //		      representation ->
    //			 representation_items  
    //
    // The property_definition identifies the property type while the
    // representation + rep items describe the property value.  The
    // property_definition_representation relates the property with
    // its value description.  In this way STEP can describe the value
    // of a particular property in several different ways if needed
    // (such as a brep version of geometry and a facetted version)
    // 
    // The geometric shape of a product uses some special subtypes of
    // the things above to make the shape property more distinctive in
    // the data set.
    //----------------------------------------


    // Create the shape representation.  This is the container that
    // holds geometry items.  Normally we would create some subtype of
    // shape rep like advanced_brep_shape_representation to indicate a
    // particular style of geometry/topology description.
    //
    stp_shape_representation * rep = pnew stp_shape_representation;

    // Give the product a shape property.  Product_definition_shape is
    // a subtype of property_definition used for shape properties.  It 
    // refers to the product through the product_definition.
    //
    stp_product_definition_shape * pds = 
	pnew stp_product_definition_shape;

    pds-> name ("");
    pds-> definition (pnew stp_characterized_definition);
    pds-> definition ()-> _characterized_product_definition
	(pnew stp_characterized_product_definition);
    pds-> definition ()-> _characterized_product_definition()-> 
	_product_definition (pd);


    // Attach the shape representation to the property.  The
    // shape_definition_representation subtype is used for shape
    // properties and the property_definition_representation supertype
    // is used for other types of properties.
    //
    stp_shape_definition_representation * sdr = 
	pnew stp_shape_definition_representation;

    sdr-> definition (pnew stp_represented_definition);
    sdr-> definition ()-> _property_definition (pds);
    sdr-> used_representation (rep);



    
    //----------------------------------------
    // INITIALIZE THE SHAPE REPRESENTATION
    //
    // The STEP geometry definitions in ISO 10303-42 are unit-less, so
    // we must give the shape representation context information that
    // identifies the units, the number of dimensions (2 or 3), and
    // possibly some geometric uncertainty (how close two points can
    // be before they are considered the same).
    //
    // Refer to geometry_utils.cxx for specifics on the context.
    // Refer to unit_utils.cxx  for specifics on the units.
    //

    stp_representation_context * rep_ctx = make_geometry_context (
	ROSE.design(),	// location to create the context
	"ID1", 		// some name for the context
	3, 		// number of dimensions, ie 3D
	make_mm_unit(), 	// length unit for geometry
	make_degree_unit(),	// angle unit for geometry
	make_steradian_unit(),	// solid angle unit
	1e-6		// global uncertainty (optional)
    );

    rep-> name ("");
    rep-> context_of_items (rep_ctx);


    // Now we would add the geometry and topology items that define
    // the product shape.  For an example, create an axis placement
    // with a location but no direction vectors.
    //
    stp_cartesian_point* p1 = pnew stp_cartesian_point;
    p1-> name("");
    p1-> coordinates()->add(0.0);
    p1-> coordinates()->add(0.0);
    p1-> coordinates()->add(0.0);
    stp_axis2_placement_3d* ap3d = pnew stp_axis2_placement_3d (
	"orientation", p1, 0, 0
	);

    
    // Add it to the representation
    rep-> items()-> add (ap3d);

    // Done
    return rep;
}



