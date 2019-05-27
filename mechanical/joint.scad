
use <metric_iso_screw.scad>


thickness = 1.5;
hole_dia = 3.6;
spacing = 10;
width = 10;
cross_thickness=2;
cross_height = 10;

rows=2;
//rows=2;

multiple_x = 4;
multiple_y = 4;
multiple_spacing=3;

nut_height=2.5;
nut_dia = 1 + (5.5+6)/2;


module body()
{
$fn=32;
	union(){
		translate([0,0,0])cylinder(h=thickness,d=width);
		translate([spacing,0,0])cylinder(h=thickness,d=width);
		translate([0,spacing*(rows-1),0])cylinder(h=thickness,d=width);
		translate([spacing,spacing*(rows-1),0])cylinder(h=thickness,d=width);
		translate([0,-width/2,0])cube([spacing,width+spacing*(rows-1),thickness]);
		translate([-width/2,0,0])cube([spacing+width,spacing*(rows-1),thickness]);
		/* cross */
		translate([0,spacing/2 - cross_thickness/2,thickness]) difference(){
			union(){
				rotate([-90,0,0])translate([0,0,0])cylinder(h=cross_thickness,d=width);
				rotate([-90,0,0])translate([spacing,0,0])cylinder(h=cross_thickness,d=width);
				translate([0,0,-spacing/2])cube([spacing,cross_thickness,spacing]);
			}
			translate([-spacing/2,0,-spacing])cube([spacing*2,spacing*(rows-1),spacing]);
		}

		translate([spacing/2 + cross_thickness/2,0,thickness]) rotate([0,0,90]) difference(){
			union(){
				rotate([-90,0,0])translate([0,0,0])cylinder(h=cross_thickness,d=width);
				rotate([-90,0,0])translate([spacing,0,0])cylinder(h=cross_thickness,d=width);
				translate([0,0,-spacing/2])cube([spacing,cross_thickness,spacing]);
				}
			translate([-spacing/2,0,-spacing])cube([spacing*2,spacing*(rows-1),spacing]);
		}

	}
}

module holes()
{
$fn=32;
	for (i = [0:(rows-1)]) {
		translate([0, i * spacing,0])cylinder(h=thickness,d=hole_dia);
		translate([spacing,i *spacing,0])cylinder(h=thickness,d=hole_dia);
	}
}


module holes_thread()
{
$fn=32;
	for (i = [0:(rows-1)]) {
		translate([0, i * spacing,0])
screw_thread_iso_inner(
        d=3,
        lt=6,
        $fn=60
    );



		translate([spacing,i *spacing,0])
screw_thread_iso_inner(
        d=3,
        lt=6,
        $fn=60
    );


	}
}


module nuts()
{
$fn=6;
	for (i = [0:(rows-1)]) {
		translate([0, i * spacing,0])cylinder(h=nut_height,d=nut_dia,$fn=6);
		translate([spacing,i *spacing,0])cylinder(h=nut_height,d=nut_dia,$fn=6);
	}
}

module joint() 
{
	difference () {
		body();
		holes();
		translate([0,0,thickness-nut_height])nuts();

	}
}


module joint_thread() 
{
	difference () {
		body();
		
		holes_thread();
//		translate([0,0,thickness-nut_height])nuts();

	}

}

module multiple()
{
		size_x = width + spacing;
		size_y = width + spacing * (rows -1);
	for (x = [1:multiple_x]) {
		for (y = [1:multiple_y]) {
		translate([(x-1)*(size_x+multiple_spacing),(y-1)*(size_y+multiple_spacing),0])joint();
		}
	}
}

module joint_simple()
{
	difference () {
		body();
		
		holes();
//		translate([0,0,thickness-nut_height])nuts();

	}

}

//multiple();
//joint_thread();
joint_simple();

