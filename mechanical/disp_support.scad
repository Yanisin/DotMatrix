hole_dia = 3.6;
corner_dia = 10;
corner_height = 7;
nut_height = 2.9;
nut_dia=0.8 + (5.5+6)/2;
nut_dia_clearance=0.4;
nut_offset = 1;
nut_recess = 1;
center_side=20;
center_height=7;
conn_width=2.54;
conn_length=2.54 * 14;
	corner_clearance=0.5;

//corner();

//body();
support();
$fn=32;
/* connector */
//color("red")
//translate([30 - conn_width,-conn_length/2,0])
//cube ([conn_width,conn_length, corner_height]);
 
module corner()
{
	
	union(){
	cylinder(h=corner_height,d=corner_dia);
	translate([0,-corner_dia/2,0])
	cube([25-conn_length/2-corner_clearance,corner_dia,corner_height]);
	rotate([0,0,-90])
	translate([0,-corner_dia/2,0])
	cube([25-conn_length/2-corner_clearance,corner_dia,corner_height]);
	}

}

module support()
{
difference(){
	body();
	holes();
	nut_holes();
	magnet_holes();
}
}

module hole(){
	cylinder(h=corner_height,d=hole_dia);
}

module nut_hole(){
	cube_width= nut_dia * sqrt(3)/2; 
	translate([0,0,nut_offset])
	cylinder(h=nut_height,d=nut_dia, $fn=6);
	translate([0,0,nut_offset+nut_recess]){
	cylinder(h=nut_height,d=nut_dia, $fn=6);
	translate([0,-cube_width/2,0])
	cube([20,cube_width,nut_height]);
}

}

module magnet_holes()
{
	magnet_x=1.5;
	magnet_y=4.5;
	magnet_z=6;
	x_offset=30 - magnet_x-0.5;
	/*0.5 is wall size around magnet*/
	y_offset = conn_length/2 + corner_clearance+0.5;
	z_offset = corner_height - magnet_z;

	translate([x_offset,y_offset,z_offset])
		cube([1.5,4.5,magnet_z]);
	translate([x_offset,-y_offset-magnet_y,z_offset])
		cube([1.5,4.5,magnet_z]);
	translate([-x_offset-magnet_x,y_offset,z_offset])
		cube([1.5,4.5,magnet_z]);
	translate([-x_offset-magnet_x,-y_offset-magnet_y,z_offset])
		cube([1.5,4.5,magnet_z]);
rotate([0,0,90]){
	translate([x_offset,y_offset,z_offset])
		cube([1.5,4.5,magnet_z]);
	translate([x_offset,-y_offset-magnet_y,z_offset])
		cube([1.5,4.5,magnet_z]);
	translate([-x_offset-magnet_x,y_offset,z_offset])
		cube([1.5,4.5,magnet_z]);
	translate([-x_offset-magnet_x,-y_offset-magnet_y,z_offset])
		cube([1.5,4.5,magnet_z]);}
}

module holes()
{
translate([25,25,0])
hole();
translate([25,-25,0])
hole();
translate([-25,25,0])
hole();
translate([-25,-25,0])
hole();
}

module nut_holes()
{
translate([25,25,0])
	rotate([0,0,45])
nut_hole();
translate([25,-25,0])
	rotate([0,0,-45])
nut_hole();
translate([-25,25,0])
	rotate([0,0,135])
nut_hole();
translate([-25,-25,0])
	rotate([0,0,-135])
nut_hole();
}


module body()
{
//translate([-center_side/2,-center_side/2,0])
//cube([center_side,center_side,center_height]);

translate([0,0,center_height/2])
linear_extrude(height = center_height, center = true, convexity = 10, slices =
20, scale = 2, $fn = 16) {
square([center_side*0.5,center_side *0.5],center=true);
}

rotate([0,0,45])
translate([-sqrt(2)*25,-2.5,0])
cube([2*sqrt(2)*25,5,corner_height]);
rotate([0,0,-45])
translate([-sqrt(2)*25,-2.5,0])
cube([2*sqrt(2)*25,5,corner_height]);
translate([25,25,0])
rotate([0,0,-90])
corner();
translate([25,-25,0])
rotate([0,0,180])
corner();
translate([-25,25,0])
rotate([0,0,0])
corner();
translate([-25,-25,0])
rotate([0,0,90])
corner();
}
