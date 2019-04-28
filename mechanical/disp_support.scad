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

//corner();

//body();
support();
$fn=32;
 
module corner()
{
	cylinder(h=corner_height,d=corner_dia);

}

module support()
{
difference(){
	body();
	holes();
	nut_holes();
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
corner();
translate([25,-25,0])
corner();
translate([-25,25,0])
corner();
translate([-25,-25,0])
corner();
}
