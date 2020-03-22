// SG_lab2 23-11-2016
// fog.vs
// Zast�pienie podstawowej funkcjonalno�ci sta�ego potoku renderowania

uniform vec3 lightPos[ 3 ]; // maksymalnie trzy po�o�enia �wiat�a
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory �wiat�a

void main( void )
{
   // transformacja wierzcho�ka zgodnie z aktualn� macierz� widoku modelu i rzutowania
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // skopiowanie koloru wierzcho�ka (primary color)
   gl_FrontColor = gl_Color;
}
