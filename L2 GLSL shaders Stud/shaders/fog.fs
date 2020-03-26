// SG_lab2 23-11-2016
// fog.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a
varying float distance;

const float specularExp = 128;
//Zdefiniowanie niemal dowolnego koloru mg³y w sta³ej const vec4 fogColor (kana³ a = 1.0).
const vec4 fogColor = (0.34, 0.59, 0.76, 1.0);

void main( void )
{

    //--3lights.fs--
    gl_FragColor = gl_Color;
    //gl_FragColor.a = gl_Color.a;

    vec3 NN = normalize(N);
    float intensity[3];
    vec3 NL[3];
    vec3 NH[3];
    vec3 diffuse;//[3];
    vec3 specular[3];

    //gl_FragColor.xyz = (0.0, 0.0, 0.0);
    diffuse = (0.0, 0.0, 0.0);
    for(int i=0; i < 3; i++)
    {
        //initializing values
        NL[i] = NH[i] = specular[i] = (0.0, 0.0, 0.0);
        intensity[i] = 0.0;

        NL[i] = normalize(L[i]);
        NH[i] = normalize(NL[i] + (0.0, 0.0, 1.0));
        intensity[i] = max(0.0, dot(NN, NL[i]));
        if(diffuse == (0.0, 0.0, 0.0))
            diffuse = gl_FragColor.xyz * intensity[i];
        if(intensity[i] > 0.0)
        {
            intensity[i] = max(0.0, dot(NN, NH[i]));
            specular[i] = pow(intensity[i], specularExp);
        }
        else
            specular[i] = intensity[i];
    
        //color for each light
        specular[0] *= (1.0, 0.0, 0.0);
        specular[1] *= (0.0, 1.0, 0.0);
        specular[2] *= (0.0, 0.0, 1.0);

        //adding light to base color
        gl_FragColor.xyz += specular[i];
    }
    //adding diffuse to base color
    gl_FragColor.xyz += diffuse;
    //reducing overlightning
    gl_FragColor.xyz /= 3;
    //--3lights.fs--


    //c. Zdefiniowanie podstawy wyk³adnika e=2.71828 i gêstoœci mg³y density = 0.9.
    float e = 2.71828;
    float density = 0.9;
    //d. Obliczenie wyk³adnika fogExp jako iloczynu gêstoœci mg³y density i d³ugoœci wektora distance.
    float fogExp = density * distance;
    //e. Podzielenie wyk³adnika przez ok. 200.0 a nastêpnie podniesienie go do kwadratu..
    fogExp /= 200;
    pow(fogExp, 2);
    //f. Wyznaczenie wartoœci wspó³czynnika zamglenia fogFactor jako wartoœci funkcji wyk³adniczej (pow)
    //przy podstawie e i wyk³adniku –fogExp, ograniczonej (clamp) do zakresu 0.0-1.0.
    float fogFactor = clamp(pow(e, -fogExp), 0.0, 1.0);
    //g. Ustalenie ostatecznej wersji koloru funkcj¹ mix(), której parametrami s¹: kolor mg³y, kolor fragmentu
    //(piksela) i wspó³czynnik zamglenia. Przy oddalaniu sceny mg³a powinna j¹ coraz bardziej zas³aniaæ. Je¿eli
    //tak nie jest, to nale¿y zamieniæ miejscami dwa pierwsze argumenty w funkcji mix().
    gl_FragColor = mix(fogColor, gl_Color, fogFactor);
}
