% 
:0097
( FANUC 0M)
( 10.01.2010 ) 
( LOAD T3 - S580 ) 
N10 G55 G40 G0 X2. Y0 M3 
N20 G43 H3 Z39. M8 
N30 G1 Z29.88 F39 
N40 X-114. F117
N50 G42 H19 Y10. F39 
N60 X11. F117
N70 Y-10. 
N80 X-114.
N90 G2 X-114. Y10. R10. F39 
N100 G0 G40 Y0
N110 G28 Z39. M5
N120 G0 Y-49. M9
N130 M0
( LOAD T5 - S350 )  
N140 G0 G40 X-910. Y0. M3
N150 G43 H5 Z54. M8
N160 G1 Z42.46 F25
N170 X-939. F73
N180 G28 Z54. M5
N190 M20
N200 M20
N210 G0 G40 X-800. Y2.40 M3
N220 G43 H5 Z54. M8
N230 G1 Z43.75 F25
N240 X-432. Y-2.40 F73
N250 G28 Z54. M5
N260 G0 X0 Y35. M9 
N270 M20
N280 M20
N290 M30
%
