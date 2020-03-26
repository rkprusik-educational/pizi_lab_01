// SG_lab2 23-11-2016
// specular.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a
//Zdefiniowanie w sta³ej const float specularExp wartoœci wyk³adnika odbicia lustrzanego (128).
const float specularExp = 128;

void main( void )
{
   gl_FragColor = gl_Color; // skopiowanie g³ównego koloru (primary color)

   //b. Zapisanie w zmiennej vec3 NN znormalizowanego wektora normalnego
   vec3 NN = normalize(N);
   //c. Zapisanie w zmiennej vec3 NL znormalizowanego wektora œwiat³a nr 0
   vec3 NL = normalize(L[0]);
   //d. Wyliczenie w zmiennej vec3 NH znormalizowanej sumy wektora NL oraz wektora (0.0, 0.0, 1.0)
   vec3 NH = normalize(NL + (0.0, 0.0, 1.0));
   //e. Obliczenie sk³adowej rozproszonej oœwietlenia (podobnie jak w 32.a i b) i zapisanie jej w zmiennej
   //tymczasowej vec3 diffuse.
    float intensity = max(0.0, dot(NN, NL));
    vec3 diffuse = gl_FragColor.xyz * intensity;
   
   //f. Obliczenie sk³adowej odbicia lustrzanego vec3 specular, która jest równa wektorowi zerowemu,
   //je¿eli intensywnoœæ intensity jest mniejsza lub równa 0.0. W przeciwnym wypadku, wyliczana jest na
   //nowo intensywnoœæ, podobnie jak dla sk³adowej rozproszonej, jednak korzystaj¹c z wektora NH zamiast
   //NL, a nastêpnie wyliczana jest sk³adowa odbicia lustrzanego jako intensywnoœæ do potêgi (pow())
   //zdefiniowanego wczeœniej wyk³adnika specularExp, zrzutowana na wektor vec3.

   vec3 specular;
   if(intensity > 0.0)
   {
        intensity = max(0.0, dot(NN, NH));
        specular = (vec3) pow(intensity, specularExp);
   }
   else
        specular = intensity;

   //g. Obliczenie koloru fragmentu (tylko pola .xyz) jako sumy sk³adowej œwiat³a rozproszonego (diffuse) i
   //odbicia lustrzanego (specular).
   gl_FragColor.xyz = diffuse + specular;
   //h. Skopiowanie przezroczystoœci fragmentu z przezroczystoœci koloru interpolowanego
   //gl_FragColor.a = gl_Color.a;
}
