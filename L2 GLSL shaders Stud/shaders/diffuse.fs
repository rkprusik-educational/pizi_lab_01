// SG_lab2 23-11-2016
// diffuse.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory �wiat�a

void main( void )
{
   gl_FragColor = gl_Color; // skopiowanie g��wnego koloru (primary color)
}
