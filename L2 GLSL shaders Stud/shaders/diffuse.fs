// SG_lab2 23-11-2016
// diffuse.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory �wiat�a

void main( void )
{
   gl_FragColor = gl_Color; // skopiowanie g��wnego koloru (primary color)

   //Obliczenie wsp�czynnika jasno�ci (float intensity) jako maksimum (max()) z 0.0 oraz iloczynu
   //skalarnego (dot()) znormalizowanego (normalize()) wektora normalnej i znormalizowanego
   //wektora �wiat�a nr 0 (L[0]) intensity = max(0.0, N> (*) L>).
   float intensity = max(0.0, dot(normalize(N), normalize(L[0])));

   //Wyznaczenie koloru fragmentu (gl_FragColor) jako interpolowanego koloru z jednostki VS
   //(gl_Color).
   gl_FragColor = gl_Color;   //Is already done.
   
   //Ustalenie ostatecznej jasno�ci koloru fragmentu (tylko pola .xyz) poprzez jego przemno�enie przez
   //wyznaczon� wcze�niej intensywno��.
   gl_FragColor.xyz = gl_FragColor.xyz * intensity;
}
