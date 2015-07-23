/* $RCSfile: facet_asm.cxx,v $
 * $Revision: 1.6 $ $Date: 2015/04/22 20:05:19 $
 * Auth: David Loffredo (loffredo@steptools.com)
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

// Sample program to facet  step assembly

#include <stp_schema.h>
#include <stix.h>
#include <stixmesh.h>
#include "context_utils.h"
#include "unit_utils.h"
#include <string>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>
#include "make_box.h"

using namespace System::Runtime::InteropServices;
#define VERTEX_NORMAL_RENAMED_IN_LATEST_VERSIONS 3.14
#define FOUNDMAX(x) (std::cout << "Found " << #x << ": " << x << std::endl << "Enter new " << #x << " that is greater than or equal to found " << #x << ": ")
#define FOUNDMIN(x) (std::cout << "Found " << #x << ": " << x << std::endl << "Enter new " << #x << " that is less than or equal to found " << #x << ": ")

// When this flag is 0, the print_mesh_details() function only prints
// some summary information about each mesh.  When set to 1, it will
// call print_triangle() to print the coordinates of each triangle,
// which produces a large amount of output!
//
int PRINT_ALL_TRIANGLES = 1;
bool initial_input_values = 0;

double xmin = 0.0;
double xmax = 0.0;
double ymin = 0.0;
double ymax = 0.0;
double zmin = 0.0;
double zmax = 0.0;
//this value must be > 0
double percentage = 0.0;

const char * prepend = "rawpiece_";

StixUnit angleUnit;
StixUnit lengthUnit;
StixUnit solidAngleUnit;

static void facet_product(stp_product_definition * pd);
static void facet_shape_tree(stp_representation * rep);

static void print_mesh_for_product(
    stp_product_definition * pd
    );

static void print_mesh_for_shape(
    stp_representation * rep,
    StixMtrx &xform,
    RoseObject * rep_rel_or_mapped_item,
    stp_product_definition * pd,   // null if not a new product
    unsigned nest_depth 	   // for indenting
    );

static void print_mesh_details(
    StixMeshStp * mesh,
    StixMtrx &xform,
    stp_representation_item * solid,
    const char * pfx
    );

static void print_triangle(
    const StixMeshFacetSet * fs,
    StixMtrx &xform,
    unsigned facet_num,
    const char * pfx
    );

//Joel Wamser
void calc_min_max_vertices(double &x, double &y, double &z);

double user_modify_max(double &input);

double user_modify_min(double &input);

void user_modify_all_minmax_values();

void output_raw_piece();

static void print_transform(
    StixMtrx &xform,
    const char * pfx
    );

stp_shape_representation * create_empty_product_with_geometry(
    const StixUnit &au, const StixUnit &lu, const StixUnit &sau);

std::string run(int argc, char ** argv)
{
    ROSE.quiet(1);
    stplib_init();	// initialize merged cad library
    stixmesh_init();

    char* outputFileName = (char*)malloc((strlen(argv[0]) + strlen(prepend) + 1)*sizeof(char));
    outputFileName[0] = '\0';
    strcat(outputFileName, prepend);
    strcat(outputFileName, argv[0]);
    outputFileName[strlen(prepend) + strlen(argv[0])] = '\0';

    RoseDesign * d = ROSE.findDesign(argv[0]);
    RoseDesign* output = new RoseDesign(outputFileName);
    ROSE.useDesign(output);
    if (!d)
	{
		printf("Could not open STEP file %s\n", argv[0]);
		system("pause");
		exit(1);
    }

    // prepare for working with assemblies
    rose_compute_backptrs(d);
    stix_tag_asms(d);
    stix_tag_units(d);

    // Navigate the shape tree of the assembly and facet any geometry.
    // We could just loop through all representations, but following
    // the shape tree lets us focus on a specific product.
    //
    unsigned i, sz;
    StpAsmProductDefVec roots;
    stix_find_root_products(&roots, d);

    rose_mark_begin();
    for (i = 0, sz = roots.size(); i < sz; i++)
	facet_product(roots[i]);
    rose_mark_end();


    // Now print the mesh details along with placement info
    for (i = 0, sz = roots.size(); i < sz; i++)
    {
		print_mesh_for_product(roots[i]);
    }

    //CREATE RAW PIECE
    if (PRINT_ALL_TRIANGLES)
    {
		printf("\nxmin: %f\nxmax: %f\nymin: %f\nymax: %f\nzmin: %f\nzmax: %f\n",
			xmin, xmax, ymin, ymax, zmin, zmax);
		//NOW CREATE BLOCK OF THIS SIZE
		user_modify_all_minmax_values();
		output_raw_piece();
    }

	std::string returnName(outputFileName);
    system("pause");
    free(outputFileName);
    return returnName;
}




//------------------------------------------------------------
//------------------------------------------------------------
// FACET THE SHAPE INFORMATION -- This follows the tree of shape
// information attached to a product and attempts to build a faceted
// version of each shape rep.  In an assembly, some reps only contain
// placements for subcomponents, so they will not have a mesh.
//
// This code operates synchronously -- We wait for the mesher to
// complete each representation before moving on to the next one.
// Internally, the mesher may still use several threads to complete
// the representation more quickly.
// ------------------------------------------------------------
//------------------------------------------------------------


void facet_product(
    stp_product_definition * pd
    )
{
    if (!pd || rose_is_marked(pd)) return;
    rose_mark_set(pd);

    unsigned i, sz;
    StixMgrAsmProduct * pd_mgr = StixMgrAsmProduct::find(pd);
    if (!pd_mgr) return;  // not a proper part

    // facet all direct shapes and any related shapes.
    for (i = 0, sz = pd_mgr->shapes.size(); i < sz; i++)
    {
		stp_representation * rep = pd_mgr->shapes[i];
		facet_shape_tree(rep);
    }
}


void facet_shape_tree(
    stp_representation * rep
    )
{
    unsigned i, sz;

    if (!rep || rose_is_marked(rep)) return;
    rose_mark_set(rep);

    // Create the mesh and save by attaching to the STEP solid.  A
    // representation can have multiple solids, so search the items
    // list to get them all.
    //
    // Normally, we must delete the returned mesh when finished, but
    // the cache function below will take care of that and delete the
    // cached mesh when the STEP data is deleted.
    //
    SetOfstp_representation_item * items = rep->items();
    for (i = 0, sz = items->size(); i < sz; i++)
    {
		stp_representation_item  * it = items->get(i);

		if (!StixMeshStpBuilder::canMake(rep, it))
			continue;

		// Chance that it might have been previously faceted if it is
		// somehow reused by a different part.
		if (stixmesh_cache_find(rep))
			continue;


		StixMeshStp * mesh = stixmesh_create(rep, it);
		if (mesh) {
			stixmesh_cache_add(it, mesh);
		}
    }


    // Now look for attached shapes
    StixMgrAsmShapeRep * rep_mgr = StixMgrAsmShapeRep::find(rep);
    if (!rep_mgr) return;


    // All shapes attached by a representation_relationship
    for (i = 0, sz = rep_mgr->child_rels.size(); i < sz; i++)
    {
		stp_shape_representation_relationship * rel = rep_mgr->child_rels[i];
		stp_representation * child = stix_get_shape_usage_child_rep(rel);

		facet_shape_tree(child);
    }


    // All shapes attached by mapped_item
    for (i = 0, sz = rep_mgr->child_mapped_items.size(); i < sz; i++)
    {
		stp_mapped_item * rel = rep_mgr->child_mapped_items[i];
		stp_representation * child = stix_get_shape_usage_child_rep(rel);

		facet_shape_tree(child);
    }
}



//------------------------------------------------------------
//------------------------------------------------------------
// PRINT THE FACET INFORMATION -- This follows the shape information
// attached to a single product or assembly and prints it to stdout.
//
// Since the shapes are in a tree that parallels the product tree, we
// look for attached next_assembly_usage_occurrences (NAUO) that tell
// us when we are moving into the shape of another product.
//
// Since this code touches all of the facets and transforms them with
// any assembly placements, you can adapt this code as needed to feed
// a display system or some other application instead of printing.
//------------------------------------------------------------
//------------------------------------------------------------


void print_mesh_for_product(
    stp_product_definition * pd
    )
{
    // Print the shape tree for each shape associated with a product,
    // and then follow the shape tree downward.  At each level we
    // check the shape relationship for a link to product relations
    // because shape side because there can be relationships there
    // that are not linked to products.

    unsigned i, sz;

    if (!pd) return;

    StixMgrAsmProduct * pm = StixMgrAsmProduct::find(pd);
    stp_product_definition_formation * pdf = pd->formation();
    stp_product * p = pdf ? pdf->of_product() : 0;

    const char * pname = p ? p->name() : 0;
    if (!pname || !*pname) pname = "[no name]";

    printf("ROOT PRODUCT #%lu - %s\n", pd->entity_id(), pname);
    for (i = 0, sz = pm->shapes.size(); i < sz; i++)
    {
		if (i > 0) {
			printf("------------------------------\n");
			printf("Alternate shape tree #%u for ROOT PRODUCT #%lu - %s\n",
			i, pd->entity_id(), pname);
		}

		// The root placement is usually the identity matrix but some
		// systems put a standalone AP3D at the top to place the whole
		// thing in the global space.
		StixMtrx starting_placement;
		stp_shape_representation * rep = pm->shapes[i];

		StixMgrAsmShapeRep * rep_mgr = StixMgrAsmShapeRep::find(rep);
		if (rep_mgr && rep_mgr->root_placement) {
			starting_placement.put(rep_mgr->root_placement);

			printf("Using Custom Coordinate System defined by #%lu\n",
			rep_mgr->root_placement->entity_id());
		}

		print_mesh_for_shape(rep, starting_placement, 0, 0, 0);
    }
}




void print_mesh_for_shape(
    stp_representation * rep,
    StixMtrx &rep_xform,
    RoseObject * rep_rel_or_mapped_item,
    stp_product_definition * pd,   // null if not a new product
    unsigned nest_depth 	   // for indenting
    )
{
    angleUnit = stix_get_context_angle_unit(rep);
    lengthUnit = stix_get_context_length_unit(rep);
    solidAngleUnit = stix_get_context_solid_angle_unit(rep);

    unsigned i, sz;
    RoseStringObject nest_indent = "";

    if (!rep) return;

    // indent for a nice tree printout
    if (pd) nest_depth++;
    for (i = 0; i < nest_depth; i++)
		nest_indent += "\t";


    // print the usage of a particular shape, given the relation that
    // hooks them together geometrically.  This relation will give the
    // transform for the shape.

    // Display the mesh information for the shape

    if (pd) {
		stp_product_definition_formation * pdf = pd->formation();
		stp_product * p = pdf ? pdf->of_product() : 0;

		const char * pname = p ? p->name() : 0;
		if (!pname || !*pname) pname = "[no name]";

		fputs(nest_indent.as_const(), stdout);
		fputs("--------------------\n", stdout);
		fputs(nest_indent.as_const(), stdout);
		printf("PRODUCT #%lu - %s\n", pd->entity_id(), pname);
    }

    // Does the rep have any meshed items?  In an assembly, some reps
    // just contain placements for transforming components. If there
    // are solids, we should have previously generated meshes.
    //
    fputs("\n", stdout);
    fputs(nest_indent.as_const(), stdout);
    printf("Shape #%lu (%s)\n", rep->entity_id(), rep->domain()->name());

    fputs(nest_indent.as_const(), stdout);
    printf("placement -- \n");
    print_transform(rep_xform, nest_indent.as_const());


    SetOfstp_representation_item * items = rep->items();
    unsigned solids_printed = 0;
    for (i = 0, sz = items->size(); i < sz; i++)
    {
		stp_representation_item  * it = items->get(i);
		StixMeshStp * mesh = stixmesh_cache_find(it);
		if (mesh) {
			print_mesh_details(mesh, rep_xform, it, nest_indent.as_const());
			solids_printed++;
		}
    }

    if (!solids_printed) {
		fputs(nest_indent.as_const(), stdout);
		printf(" -- no meshes in representation --\n");
    }

    // Go through all of the child shapes which can be attached by a
    // shape_reprepresentation_relationship or a mapped_item.  If the
    // relation has a NAUO associated with it, then it is the start of
    // a different product, otherwise it is still part of the shape of
    // this one.
    //
    StixMgrAsmShapeRep * rep_mgr = StixMgrAsmShapeRep::find(rep);
    if (!rep_mgr) return;


    for (i = 0, sz = rep_mgr->child_rels.size(); i < sz; i++)
    {
		stp_shape_representation_relationship * rel = rep_mgr->child_rels[i];
		stp_representation * child = stix_get_shape_usage_child_rep(rel);
		stp_product_definition * cpd =
			stix_get_shape_usage_child_product(rel);

		// Move to location in enclosing asm
		StixMtrx child_xform = stix_get_shape_usage_xform(rel);
		child_xform = child_xform * rep_xform;

		print_mesh_for_shape(child, child_xform, rel, cpd, nest_depth);
    }



    for (i = 0, sz = rep_mgr->child_mapped_items.size(); i < sz; i++)
    {
		stp_mapped_item * rel = rep_mgr->child_mapped_items[i];
		stp_representation * child = stix_get_shape_usage_child_rep(rel);
		stp_product_definition * cpd =
			stix_get_shape_usage_child_product(rel);

		// Move to location in enclosing asm
		StixMtrx child_xform = stix_get_shape_usage_xform(rel);
		child_xform = child_xform * rep_xform;

		print_mesh_for_shape(child, child_xform, rel, cpd, nest_depth);
    }

}


void print_mesh_details(
    StixMeshStp * mesh,
    StixMtrx &mesh_xform,
    stp_representation_item * solid,
    const char * pfx
    )
{
    fputs("\n", stdout);
    fputs(pfx, stdout);
    printf("Solid #%lu (%s)\n",
		solid->entity_id(),
		solid->domain()->name()
		);

    if (!mesh) {
		fputs(pfx, stdout);
		printf(" -- no mesh --\n");
    }
    else
    {
		const StixMeshFacetSet * fs = mesh->getFacetSet();

		fputs(pfx, stdout);
		printf(
			"mesh has %u triangles\n",
			fs->getFacetCount()
			);

		fputs(pfx, stdout);
		printf(
			"step faces: %u , step edges: %u\n",
			mesh->getFaceCount(),
			mesh->getEdgeCount()
			);

		fputs(pfx, stdout);
		printf(
			"step global uncertainty: %g\n",
			mesh->getGlobalUncertainty()
			);

		// Print all of the triangles - Flip the flag below if you
		// want this.  It produces a large amount of output!
		//

		if (PRINT_ALL_TRIANGLES)
		{
			unsigned i, sz;

			for (i = 0, sz = fs->getFacetCount(); i < sz; i++)
			print_triangle(fs, mesh_xform, i, pfx);
		}
    }
}


void print_triangle(
    const StixMeshFacetSet * fs,
    StixMtrx &xform,
    unsigned facet_num,
    const char * pfx
    )
{
    double v[3];
    double n[3];
    const StixMeshFacet * f = fs->getFacet(facet_num);

    fputs(pfx, stdout);
    printf("facet %u\n", facet_num);

    if (!f) return;


    // The components of the triangle verticies and vertex normals are
    // given by an index into internal tables.  Apply the transform so
    // that the coordinates and directions are placed correctly.

    stixmesh_transform(v, xform, fs->getVertex(f->verts[0]));
#ifdef VERTEX_NORMAL_RENAMED_IN_LATEST_VERSIONS
    stixmesh_transform_dir(n, xform, fs->getNormal(f->normals[0]));
#else
    stixmesh_transform_dir (n, xform, fs-> getNormal(f-> vert_normals[0]));
#endif

    fputs(pfx, stdout);
    printf("v1: (%.6g %.6g %.6g)\t", v[0], v[1], v[2]);
    printf("n1: (%.3f %.3f %.3f)\n", n[0], n[1], n[2]);

    //get initial values for the min and max values on first run through
    if (!initial_input_values)
    {
		xmin = xmax = v[0];
		ymin = ymax = v[1];
		zmin = zmax = v[2];
		initial_input_values = 1;
    }
    else
    {
		calc_min_max_vertices(v[0], v[1], v[2]);
    }

    stixmesh_transform(v, xform, fs->getVertex(f->verts[1]));
#ifdef VERTEX_NORMAL_RENAMED_IN_LATEST_VERSIONS
    stixmesh_transform_dir(n, xform, fs->getNormal(f->normals[1]));
#else
    stixmesh_transform_dir (n, xform, fs-> getNormal(f-> vert_normals[1]));
#endif

    fputs(pfx, stdout);
    printf("v2: (%.6g %.6g %.6g)\t", v[0], v[1], v[2]);
    printf("n2: (%.3f %.3f %.3f)\n", n[0], n[1], n[2]);

    calc_min_max_vertices(v[0], v[1], v[2]);

    stixmesh_transform(v, xform, fs->getVertex(f->verts[2]));
#ifdef VERTEX_NORMAL_RENAMED_IN_LATEST_VERSIONS
    stixmesh_transform_dir(n, xform, fs->getNormal(f->normals[2]));
#else
    stixmesh_transform_dir (n, xform, fs-> getNormal(f-> vert_normals[2]));
#endif

    fputs(pfx, stdout);
    printf("v3: (%.6g %.6g %.6g)\t", v[0], v[1], v[2]);
    printf("n3: (%.3f %.3f %.3f)\n", n[0], n[1], n[2]);

    calc_min_max_vertices(v[0], v[1], v[2]);
}

void calc_min_max_vertices(double &x, double &y, double &z)
{
    xmin = min(x, xmin);
    xmax = max(x, xmax);
    ymin = min(y, ymin);
    ymax = max(y, ymax);
    zmin = min(z, zmin);
    zmax = max(z, zmax);
}

void output_raw_piece()
{
    RoseDesign * d = ROSE.design();
    stp_shape_representation* shape = create_empty_product_with_geometry(angleUnit, lengthUnit, solidAngleUnit);
    double x_width = (xmax - xmin);
    double y_width = (ymax - ymin);
    double z_width = (zmax - zmin);
    double x = (xmax + xmin) / 2;
    double y = (ymax + ymin) / 2;
    //this will eventually be zmax.
    //its zmin/2 right now to leave room for the percentage increase
    double z = zmax;
    std::cout << std::endl << "x_width = " << x_width << std::endl << "y_width = "
	<< y_width << std::endl << "z_width = " << z_width << std::endl;
    char* color = "red";
    create_any_box(shape, x, y, z, x_width, y_width, z_width, color);
    d->save();
}


void print_transform(
    StixMtrx &xf,
    const char * pfx
    )
{
    fputs(pfx, stdout);
    printf("Loc: (%.6g %.6g %.6g)\n", xf.get(0, 3), xf.get(1, 3), xf.get(2, 3));
    fputs(pfx, stdout);
    printf("zdir (%.3f %.3f %.3f)\n", xf.get(0, 0), xf.get(1, 0), xf.get(2, 0));
    fputs(pfx, stdout);
    printf("xdir (%.3f %.3f %.3f)\n", xf.get(0, 2), xf.get(1, 2), xf.get(2, 2));
}

stp_shape_representation * create_empty_product_with_geometry(const StixUnit &au, const StixUnit &lu, const StixUnit &sau)
{
    //----------------------------------------
    // CREATE THE PRODUCT -
    // Create a product object to represent our part. 
    stp_product* pr = pnew stp_product();

    // Fill in the part number, name, and description. 
    pr->id("1234-K789");
    pr->name("widget");
    pr->description("a fictional product");

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
    pdf->description("first version of our widget");


    // The PDF needs a product definition.  This describes a the
    // version from a particular point of view in the life cycle, so
    // most APs only have one per PDF, but some APs use several.
    //
    stp_product_definition* pd = pnew stp_product_definition();
    pd->formation(pdf);
    pd->id("design");
    pd->description("example product_definition");

    // The context information is boilerplate information as above in
    // the product.  See the context_utils.cxx file for the specifics.
    // 
    pd->frame_of_reference(make_pdef_context());


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

    stp_representation_context * rep_ctx = stix_make_geometry_context(
	ROSE.design(),	// location to create the context
	"ID1", 		// some name for the context
	3, 		// number of dimensions, ie 3D
	lu, 	// length unit for geometry
	au,	// angle unit for geometry
	sau,	// solid angle unit
	1e-6		// global uncertainty (optional)
	);

    rep->name("");
    rep->context_of_items(rep_ctx);


    // Now we would add the geometry and topology items that define
    // the product shape.  For an example, create an axis placement
    // with a location but no direction vectors.
    //
    stp_cartesian_point* p1 = pnew stp_cartesian_point;
    p1->name("");
    p1->coordinates()->add(0.0);
    p1->coordinates()->add(0.0);
    p1->coordinates()->add(0.0);
    stp_axis2_placement_3d* ap3d = pnew stp_axis2_placement_3d(
	"orientation", p1, 0, 0
	);


    // Add it to the representation
    rep->items()->add(ap3d);

    // Done
    return rep;
}

double user_modify_max(double &input)
{
	double temp;
	std::cin >> temp;
	if(temp >= input)
	{
		return temp;
	}
	else
	{
		std::cout << "Inputted Value is less than found value" << std::endl
			<< "No change will be made to found value" << std::endl;
		return input;
	}
}

double user_modify_min(double &input)
{
	double temp;
	std::cin >> temp;
	if(temp <= input)
	{
		return temp;
	}
	else
	{
		std::cout << "Inputted Value is greater than found value" << std::endl
			<< "No change will be made to found value" << std::endl;
		return input;
	}
}

void user_modify_all_minmax_values()
{
	FOUNDMAX(xmax);
	xmax = user_modify_max(xmax);
	FOUNDMIN(xmin);
	xmin = user_modify_min(xmin);
	FOUNDMAX(ymax);
	ymax = user_modify_max(ymax);
	FOUNDMIN(ymin);
	ymin = user_modify_min(ymin);
	FOUNDMAX(zmax);
	zmax = user_modify_max(zmax);
	FOUNDMIN(zmin);
	zmin = user_modify_min(zmin);
}

public ref class BBoxer
{
	public:
		static System::String^ RunMain(System::String^ wpname)
		{
			char *foo[1];
			foo[0] = (char*)Marshal::StringToHGlobalAnsi(wpname).ToPointer();
			std::string returnFileName = run(1,foo);
			System::String^ rfn = gcnew System::String(returnFileName.c_str());
			return rfn;
		}
};