module bracket()
{
render() difference()
{
translate([25/2,0,0]) rotate([0,-90,0]) linear_extrude(height=25) polygon([[0,0], [0,25], [5,25], [30,5], [30,0]]);
translate([-10,5,5]) cube([20,25,25]);
translate([0,10,25]) rotate([90,0,0]) cylinder(r=6/2+0.5, h=20);
translate([0,15,-5]) cylinder(r=5/2, h=20);
}


}

rotate([90,0,0]) bracket();
translate([28,0,0]) rotate([90,0,0]) bracket();
