// SG_lab2 23-11-2016
// diffuse.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a

void main( void )
{
   gl_FragColor = gl_Color; // skopiowanie g³ównego koloru (primary color)

   //Obliczenie wspó³czynnika jasnoœci (float intensity) jako maksimum (max()) z 0.0 oraz iloczynu
   //skalarnego (dot()) znormalizowanego (normalize()) wektora normalnej i znormalizowanego
   //wektora œwiat³a nr 0 (L[0]) intensity = max(0.0, N> (*) L>).
   float intensity = max(0.0, dot(normalize(N), normalize(L[0])));

   //Wyznaczenie koloru fragmentu (gl_FragColor) jako interpolowanego koloru z jednostki VS
   //(gl_Color).
   gl_FragColor = gl_Color;   //Is already done.
   
   //Ustalenie ostatecznej jasnoœci koloru fragmentu (tylko pola .xyz) poprzez jego przemno¿enie przez
   //wyznaczon¹ wczeœniej intensywnoœæ.
   gl_FragColor.xyz = gl_FragColor.xyz * intensity;
}
