/* $RCSfile: unit_utils.cxx,v $
 * $Revision: 1.4 $ $Date: 2015/04/30 18:41:24 $
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
#include "unit_utils.h"


/* PRINT MEASURE VALUE -- STEP measures are stored as a select type of
 * many different types of value. This function examines a measure and
 * prints out the numeric or string value.  Measures are usually
 * associated with a unit.
 */
void print_measure_value (stp_measure_value * m)
{
    if (!m) return;
    if (m-> is_area_measure()) { 
	printf ("%.15g", m-> _area_measure()); 
	return;
    }
    if (m-> is_context_dependent_measure()) { 
	printf ("%.15g", m-> _context_dependent_measure());	
	return;
    }
    if (m-> is_descriptive_measure()) { 
	printf ("%s", m-> _descriptive_measure());	
	return;
    }
    if (m-> is_length_measure()) { 
	printf ("%.15g", m-> _length_measure());	
	return;
    }
    if (m-> is_mass_measure()) { 
	printf ("%.15g", m-> _mass_measure());	
	return;
    }
    if (m-> is_plane_angle_measure()) { 
	printf ("%.15g", m-> _plane_angle_measure());	
	return;
    }
    if (m-> is_parameter_value()) { 
	printf ("%.15g", m-> _parameter_value());	
	return;
    }
    if (m-> is_positive_length_measure()) { 
	printf ("%.15g", m-> _positive_length_measure());	
	return;
    }
    if (m-> is_positive_plane_angle_measure()) { 
	printf ("%.15g", m-> _positive_plane_angle_measure());	
	return;
    }
    if (m-> is_solid_angle_measure()) { 
	printf ("%.15g", m-> _solid_angle_measure());	
	return;
    }
    if (m-> is_volume_measure()) { 
	printf ("%.15g", m-> _volume_measure());	
	return;
    }
}

/* PRINT UNIT NAME -- STEP units are usually represented as a complex
 * instance composed of named unit types.  The usual set of units are
 * the SI units (meter, gram, second), although it is posible to define
 * conversion-based units such as inches, pounds, etc.  It is also
 * possible to define arbitrary contxt-dependant units such as scoops,
 * rolls, etc.
 */
void print_unit (stp_unit * u)
{
    if (!u) return;
    if (u-> is_named_unit()) {
	stp_named_unit * nu = u-> _named_unit();
	if (nu-> isa (ROSE_TYPE(stp_si_unit))) {
	    stp_si_unit * su = ROSE_CAST (stp_si_unit, nu);
	    switch (su->prefix()) {
	    case stp_si_prefix_exa:		printf ("exa");		break;
	    case stp_si_prefix_pico:	printf ("pico");	break;
	    case stp_si_prefix_mega:	printf ("mega");	break;
	    case stp_si_prefix_femto:	printf ("femto");	break;
	    case stp_si_prefix_atto:	printf ("atto");	break;
	    case stp_si_prefix_centi:	printf ("centi");	break;
	    case stp_si_prefix_nano:	printf ("nano");	break;
	    case stp_si_prefix_hecto:	printf ("hecto");	break;
	    case stp_si_prefix_micro:	printf ("micro");	break;
	    case stp_si_prefix_tera:	printf ("tera");	break;
	    case stp_si_prefix_giga:	printf ("giga");	break;
	    case stp_si_prefix_milli:	printf ("milli");	break;
	    case stp_si_prefix_peta:	printf ("peta");	break;
	    case stp_si_prefix_deci:	printf ("deci");	break;
	    case stp_si_prefix_kilo:	printf ("kilo");	break;
	    case stp_si_prefix_deca:	printf ("deca");	break;
	    case stp_si_prefix_NULL:	
	    default:
		/* nothing */ break;
	    }

	    switch (su->name()) {
	    case stp_si_unit_name_hertz:	printf ("hertz");	break;
	    case stp_si_unit_name_siemens:	printf ("siemens");	break;
	    case stp_si_unit_name_sievert:	printf ("sievert");	break;
	    case stp_si_unit_name_lux:		printf ("lux");		break;
	    case stp_si_unit_name_watt:		printf ("watt");	break;
	    case stp_si_unit_name_ohm:		printf ("ohm");		break;
	    case stp_si_unit_name_second:	printf ("second");	break;
	    case stp_si_unit_name_becquerel:	printf ("becquerel");	break;
	    case stp_si_unit_name_pascal:	printf ("pascal");	break;
	    case stp_si_unit_name_henry:	printf ("henry");	break;
	    case stp_si_unit_name_tesla:	printf ("tesla");	break;
	    case stp_si_unit_name_volt:		printf ("volt");	break;
	    case stp_si_unit_name_joule:	printf ("joule");	break;
	    case stp_si_unit_name_kelvin:	printf ("kelvin");	break;
	    case stp_si_unit_name_ampere:	printf ("ampere");	break;
	    case stp_si_unit_name_gram:		printf ("gram");	break;
	    case stp_si_unit_name_steradian:	printf ("steradian");	break;
	    case stp_si_unit_name_mole:		printf ("mole");	break;
	    case stp_si_unit_name_lumen:	printf ("lumen");	break;
	    case stp_si_unit_name_gray:		printf ("gray");	break;
	    case stp_si_unit_name_candela:	printf ("candela");	break;
	    case stp_si_unit_name_farad:	printf ("farad");	break;
	    case stp_si_unit_name_radian:	printf ("radian");	break;
	    case stp_si_unit_name_newton:	printf ("newton");	break;
	    case stp_si_unit_name_metre:	printf ("metre");	break;
	    case stp_si_unit_name_weber:	printf ("weber");	break;
	    case stp_si_unit_name_coulomb:	printf ("coulomb");	break;
	    case stp_si_unit_name_NULL:	
	    default:
		printf ("none");
		break;
	    }
	}
	if (nu-> isa (ROSE_TYPE(stp_conversion_based_unit))) {
	    stp_conversion_based_unit * cbu = ROSE_CAST(stp_conversion_based_unit,nu);
	    printf ("%s (conversion-based unit)", 
		    (cbu->name()? cbu->name(): "none"));
	}
	if (nu-> isa (ROSE_TYPE(stp_context_dependent_unit))) {
	    stp_context_dependent_unit * cdu = ROSE_CAST(stp_context_dependent_unit,nu);
	    printf ("%s (context-dependent unit)", 
		    (cdu->name()? cdu->name(): "none"));
	}
    }
}



//------------------------------------------------------------
// Construct the various common SI and English units.
//
// These functions only create one instance of a given unit in a
// design.  The first time the unit is created, we save a shortcut in
// the nametable of the design.  This lets us quickly find the unit
// again if we need it, which keeps us from creating duplicate unit
// definitions.
//
stp_named_unit * make_mm_unit(
    RoseDesign * d
    )
{
    const char * shortcut = "__CONSTANT UNIT millimetre";

    if (!d) d = ROSE.design();

    /* Use shortcut to unit if we have already created one */
    RoseObject * obj = d-> findObject (shortcut);
    if (obj) return ROSE_CAST (stp_named_unit, obj);

    // Need a complex instance of both si and length unit
    stp_si_unit * u = pnewIn(d) stp_length_unit_and_si_unit;
    u-> name (stp_si_unit_name_metre);
    u-> prefix (stp_si_prefix_milli);

    d-> addName (shortcut, u);  // save shortcut for future calls
    return u;
}



stp_named_unit * make_inch_unit(
    RoseDesign * d
    )
{
    const char * shortcut = "__CONSTANT UNIT inch";

    if (!d) d = ROSE.design();

    /* Use shortcut to unit if we have already created one */
    RoseObject * obj = d-> findObject (shortcut);
    if (obj) return ROSE_CAST (stp_named_unit, obj);

    // Inch is defined as a conversion-based unit in terms of an
    // underlying SI unit.  Usually 25.4mm or 2.54cm.  Create the
    // measure with unit to indicate this.  
    //
    stp_measure_with_unit * mwu = pnewIn(d) stp_measure_with_unit;
    mwu-> value_component (pnewIn(d) stp_measure_value);
    mwu-> value_component ()-> _length_measure (25.4);

    mwu-> unit_component (pnewIn(d) stp_unit);
    mwu-> unit_component ()-> _named_unit (make_mm_unit(d));


    // The unit itself needs a complex instance of both CBU and length
    // unit.  Conversion based unit also needs dimensional exponents
    // that show which basic quantities (length, time, mass, etc) the
    // unit describes.
    //
    stp_conversion_based_unit * u = 
	pnewIn(d) stp_conversion_based_unit_and_length_unit;

    u-> name ("inch");
    u-> dimensions (pnewIn(d) stp_dimensional_exponents (1,0,0,0,0,0,0));
    u-> conversion_factor (mwu);

    d-> addName (shortcut, u);  // save shortcut for future calls
    return u;
}



stp_named_unit * make_radian_unit(
    RoseDesign * d
    )
{
    const char * shortcut = "__CONSTANT UNIT radian";

    if (!d) d = ROSE.design();

    /* Use shortcut to unit if we have already created one */
    RoseObject * obj = d-> findObject (shortcut);
    if (obj) return ROSE_CAST (stp_named_unit, obj);

    // Need a complex instance of both si and plane angle unit
    stp_si_unit * u = pnewIn(d) stp_plane_angle_unit_and_si_unit;
    u-> name (stp_si_unit_name_radian);
    u-> prefix (stp_si_prefix_NULL);

    d-> addName (shortcut, u);  // save shortcut for future calls
    return u;
}



stp_named_unit * make_degree_unit(
    RoseDesign * d
    )
{
    const char * shortcut = "__CONSTANT UNIT degree";

    if (!d) d = ROSE.design();

    /* Use shortcut to unit if we have already created one */
    RoseObject * obj = d-> findObject (shortcut);
    if (obj) return ROSE_CAST (stp_named_unit, obj);

    // Degree is defined as a conversion-based unit in terms of the
    // underlying radian SI unit.  1 Degree == PI/180 radians.  Create
    // the measure with unit to indicate this.
    //
    stp_measure_with_unit * mwu = pnewIn(d) stp_measure_with_unit;
    mwu-> value_component (pnewIn(d) stp_measure_value);
    mwu-> value_component ()-> _plane_angle_measure (0.01745329252);

    mwu-> unit_component (pnewIn(d) stp_unit);
    mwu-> unit_component ()-> _named_unit (make_radian_unit(d));


    // The unit itself needs a complex instance of both CBU and plane
    // angle unit.  Conversion based unit also needs dimensional
    // exponents but these are all zero for angles.
    //
    stp_conversion_based_unit * u = 
	pnewIn(d) stp_conversion_based_unit_and_plane_angle_unit;

    u-> name ("degree");
    u-> dimensions (pnewIn(d) stp_dimensional_exponents (0,0,0,0,0,0,0));
    u-> conversion_factor (mwu);

    d-> addName (shortcut, u);  // save shortcut for future calls
    return u;
}



stp_named_unit * make_steradian_unit(
    RoseDesign * d
    )
{
    const char * shortcut = "__CONSTANT UNIT steradian";

    if (!d) d = ROSE.design();

    /* Use shortcut to unit if we have already created one */
    RoseObject * obj = d-> findObject (shortcut);
    if (obj) return ROSE_CAST (stp_named_unit, obj);

    // Need a complex instance of both si and length unit
    stp_si_unit * u = pnewIn(d) stp_si_unit_and_solid_angle_unit;
    u-> name (stp_si_unit_name_steradian);
    u-> prefix (stp_si_prefix_NULL);

    d-> addName (shortcut, u);  // save shortcut for future calls
    return u;
}



