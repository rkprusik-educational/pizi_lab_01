// SG_lab2 23-11-2016
// specular.vs
// 

uniform vec3 lightPos[ 3 ]; // maksymalnie trzy po³o¿enia œwiat³a
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a

void main( void )
{
   // transformacja wierzcho³ka zgodnie z aktualn¹ macierz¹ widoku modelu i rzutowania
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

   // skopiowanie koloru wierzcho³ka (primary color)
   gl_FrontColor = gl_Color;

   //Wyznaczenie wektora normalnego N w przestrzeni kamery jako iloczynu macierzy gl_NormalMatrix
   //oraz wektora normalnego (gl_Normal). Wbudowana macierz gl_NormalMatrix jest odwrócon¹ i
   //transponowan¹ lew¹ górn¹ podmacierz¹ 3x3 ModelView.
   N = gl_NormalMatrix * gl_Normal;

   //Obliczenie pomocniczego wektora vec4 V (wierzcho³ek w przestrzeni widoku modelu eye-space), jako
   //iloczynu macierzy widoku modelu (gl_ModelViewMatrix) i wspó³rzêdnych wierzcho³ka
   //(gl_Vertex).
   vec4 V = gl_ModelViewMatrix * gl_Vertex;

   //Wyznaczenie wektora œwiat³a L[ 0 ] (w przestrzenie widoku modelu eye-space) do Ÿród³a œwiat³a nr 0 jako
   //ró¿nicy pomiêdzy pozycj¹ œwiat³a nr 0 (lightPos[ 0 ]) i wektorem V.xyz.
   L[ 0 ] = lightPos[ 0 ] - V.xyz;
}
