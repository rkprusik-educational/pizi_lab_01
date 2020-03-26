// SG_lab2 23-11-2016
// 3lights.fs
// 
varying vec3 N; // interpolowany wektor normalny powierzchni
varying vec3 L[ 3 ]; // maksymalnie trzy intepolowane wektory œwiat³a
const float specularExp = 128;

void main( void )
{
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
}
