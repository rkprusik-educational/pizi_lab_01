// SG_lab2 23-11-2016
// diffuse.vs
// 

uniform vec3 lightPos[ 3 ]; // maksymalnie trzy po�o�enia �wiat�a
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory �wiat�a

void main( void )
{
   // transformacja wierzcho�ka zgodnie z aktualn� macierz� widoku modelu i rzutowania
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // skopiowanie koloru wierzcho�ka (primary color)
   gl_FrontColor = gl_Color;

   //Wyznaczenie wektora normalnego N w przestrzeni kamery jako iloczynu macierzy gl_NormalMatrix
   //oraz wektora normalnego (gl_Normal). Wbudowana macierz gl_NormalMatrix jest odwr�con� i
   //transponowan� lew� g�rn� podmacierz� 3x3 ModelView.
   N = gl_NormalMatrix * gl_Normal;

   //Obliczenie pomocniczego wektora vec4 V (wierzcho�ek w przestrzeni widoku modelu eye-space), jako
   //iloczynu macierzy widoku modelu (gl_ModelViewMatrix) i wsp�rz�dnych wierzcho�ka
   //(gl_Vertex).
   vec4 V = gl_ModelViewMatrix * gl_Vertex;

   //Wyznaczenie wektora �wiat�a L[ 0 ] (w przestrzenie widoku modelu eye-space) do �r�d�a �wiat�a nr 0 jako
   //r�nicy pomi�dzy pozycj� �wiat�a nr 0 (lightPos[ 0 ]) i wektorem V.xyz.
   L[ 0 ] = lightPos[ 0 ] - V.xyz;
}
