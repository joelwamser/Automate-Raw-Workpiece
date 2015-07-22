#include <stp_schema.h>

#include "geometry_utils.h"	// for color_face

void create_any_box (
    stp_shape_representation *shape, 
    double x, 
    double y, 
    double z, 
    double x_width, 
    double y_width, 
    double z_width, 
    const char *color 	// red green yellow cyan magenta black white 
    );