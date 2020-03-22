// lighting.c
// By M.Szwoch, Based on
// OpenGL SuperBible, Chapter 17
// Demonstrates vertex and fragment shaders together
// Program by Benjamin Lipchak

#include "../shared/gltools.h"   // OpenGL toolkit
#include "../shared/glFrame.h"   // Frame toolkit

#include <stdio.h>

//added along with entry in lighting.sln properties>link>input:"legacy_stdio_definitions.lib;"
//provides missing reference to __iob_func[] usage and so fixes old-toolset problem R.P.
extern "C" { FILE __iob_func[3] = { stdin,stdout,*stderr }; }

// 3. Dodaj swoje imi� i nazwisko do belki tytu�owej
// ...
//--changed_value
//const char *authorName = "Anonim";
//--changed_value
const char* authorName = "Radoslaw Prusik";

enum ShaderTypes { SIMPLE, DIFFUSE, SPECULAR, THREELIGHTS, FOG, PARAMS, SHADERS_NUM };

GLuint vShader[SHADERS_NUM]; // identyfikatory program�w cieniowania wierzcho�k�w (VS)
GLuint fShader[SHADERS_NUM]; // identyfikatory program�w cieniowania fragment�w (FS)
GLuint progObj[SHADERS_NUM]; // obiekty program�w cieniowania (VS+FS)
GLboolean needsValidation[SHADERS_NUM]; // informacja o aktualno�ci programu cieniownaia
char* shaderNames[SHADERS_NUM] =
{ // nazwy program�w cieniowania
	"simple",
	"diffuse",
	"specular",
	"3lights",
	"fog",
	"params"
};
// aktualny program cieniowania
ShaderTypes actualShader = SIMPLE;
// rozmiary okna
GLint windowWidth = 1024;
GLint windowHeight = 768;

GLint mainMenu, shaderMenu;             // menu handles

GLfloat cameraPos[] = { 100.0f, 75.0f, 150.0f, 1.0f };
GLdouble cameraZoom = 0.4;

GLfloat lightPos0[] = { 140.0f, 250.0f, 140.0f, 1.0f };
GLfloat lightPos1[] = { -140.0f, 250.0f, 140.0f, 1.0f };
GLfloat lightPos2[] = { 0.0f, 250.0f, -200.0f, 1.0f };

GLfloat lightRotation = 0.0f;

#define MAX_INFO_LOG_SIZE 2048
GLchar infoLog[MAX_INFO_LOG_SIZE]; // bufor na b��dy obs�ugi program�w cieniowania
void FatalError(char* errorKind, char* errorDescription, char* errorDetails)
{ // wy�wietlenie informacji o b��dzie i zako�czenie programu ("radykalna" obs�uga b��d�w)
	fprintf(stderr, "!!! %s ERROR !!!\n", errorKind);
	fprintf(stderr, " %s \"%s\"\n", errorDescription, errorDetails);
	Sleep(5000);
	exit(0);
}
GLchar* LoadShaderText(const char* fileName)
{ // Za�adowanie z pliku kodu �r�d�owego programu cieniowania jako tekstu w formacie ASCIIZ
	GLchar* shaderText = NULL;
	GLint shaderLength = 0;
	FILE* fp;
	fp = fopen(fileName, "r");
	if (fp != NULL)
	{
		while (fgetc(fp) != EOF)
			shaderLength++;
		rewind(fp);
		shaderText = (GLchar*)malloc(shaderLength + 1);
		if (shaderText != NULL)
			fread(shaderText, 1, shaderLength, fp);
		shaderText[shaderLength] = '\0';
		fclose(fp);
	}
	return shaderText;
}
void LoadAndCompileShader(char* fullFileName, GLuint shaderKind, GLuint* shaderNum)
{ // za�adowanie programu cieniowania z dysku i jego kompilacja
	// za�adowanie programu cieniowania z dysku
	GLint success;
	GLchar* shString = LoadShaderText(fullFileName); // za�adowanie kodu �r�d�owego
	fprintf(stdout, "Loading '%s' shader\n", fullFileName);

	if (!shString)
		FatalError("LOADING", "Unable to load", fullFileName);

	// 5. Utworzenie obiektu cieniowania funkcj� glCreateShader()
	// Parametrem jest rodzaj obiektu cieniowania (shaderKind)
	// Wynik wywo�ania nale�y podstawi� do identyfikator obiektu (*shaderNum)
	// ...
	*shaderNum = glCreateShader(shaderKind);

	// 6. Wskazanie kodu �r�d�owego obiektu cieniowania funkcj� glShaderSource()
	// Pierwszym parametrem jest identyfikator obiektu cieniowania, 
	// drugim liczba obiekt�w (1), 
	// trzecim wska�nik na wska�nik (const GLchar **)& na kod �r�d�owy
	// czwartym NULL
	// ...
	glShaderSource(*shaderNum, 1, (const GLchar**)&shString, NULL);

	free(shString);

	// 7. Kompilacja obiektu cieniowania funkcj� glCompileShader()
	// Parametrem jest identyfikator obiektu cieniowania, 
	// ...
	glCompileShader(*shaderNum);

	// 8. Sprawdzenie statusu kompilacji funkcj� glGetShaderiv()
	// Pierwszym parametrem jest identyfikator obiektu cieniowania, 
	// drugim rodzaj sprawdzanego b��du (GL_COMPILE_STATUS)
	// trzecim adres zmiennej na zwracany kod b��du (success)
	// ...
	glGetShaderiv(*shaderNum, GL_COMPILE_STATUS, &success);

	// 9. Informacja o ewentualnym b��dzie w przypadku, gdy success == 0
	// Tekst komunikatu pobierany jest funkcj� glGetShaderInfoLog()
	//  Pierwszym parametrem jest identyfikator obiektu cieniowania, 
	//  drugim rozmiar bufora (MAX_INFO_LOG_SIZE)
	//  trzecim NULL
	//  czwartym bufor (infoLog)
	// Tekst b��du nale�y wypisa� funkcj� FatalError()
	// ...
	if (success == 0)
	{
		glGetShaderInfoLog(*shaderNum, MAX_INFO_LOG_SIZE, NULL, infoLog);
		FatalError((char*)GL_COMPILE_STATUS, infoLog, "");
	}
}
void PrepareShader(GLint shaderNum)
{ // Przygotowanie programu cieniowania
	char fullFileName[255];
	GLint success;

	// Okre�lenie w fullFileName nazwy wzgl�dnej pliku ze wskazanym przez shaderNum programem cieniowania wierzcho�k�w
	sprintf(fullFileName, "./shaders/%s.vs", shaderNames[shaderNum]);

	// 11. Za�adowanie i kompilacja programu cieniowania wierzcho�k�w funkcj� LoadAndCompileShader()
	// Pierwszym parametrem jest nazwa pliku,
	// drugim rodzaj programu cieniowania (GL_VERTEX_SHADER) 
	// trzecim adres odpowiedniego pola w tablicy identyfikator�w obiekt�w cieniowania vShader
	// ...
	LoadAndCompileShader(fullFileName, GL_VERTEX_SHADER, &vShader[shaderNum]);

	// 12. Okre�lenie nazwy wzgl�dnej pliku ze wskazanym (shaderNum) programem cieniowania fragment�w
	// Pliki maj� rozszerzenie "fs"
	// ...
	sprintf(fullFileName, "./shaders/%s.fs", shaderNames[shaderNum]);

	// 13. Za�adowanie i kompilacja programu cieniowania fragment�w: (GL_FRAGMENT_SHADER, fShader)
	// ...
	LoadAndCompileShader(fullFileName, GL_FRAGMENT_SHADER, &fShader[shaderNum]);


	// 14. Utworzenie obiektu programu cieniowania funkcj� glCreateProgram()
	// Identyfikator obiektu programu cieniowania nale�y podstawi� do odpowiedniego miejsca tablicy progObj
	// ...
	progObj[shaderNum] = glCreateProgram();

	// 15. Do��czenie do obiektu programu cieniowania skompilowanego kodu VS funkcj� glAttachShader()
	// Pierwszym parametrem jest identyfikator obiektu cieniowania, 
	// drugim parametrem identyfikator obiektu VS
	// ...
	glAttachShader(progObj[shaderNum], vShader[shaderNum]);

	// 16. Do��czenie do obiektu programu cieniowania skompilowanego kodu FS
	// ...
	glAttachShader(progObj[shaderNum], fShader[shaderNum]);

	// 17. ��czenie programu cieniowania funkcj� glLinkProgram()
	// Parametrem jest identyfikator obiektu programu cieniowania
	// ...
	glLinkProgram(progObj[shaderNum]);

	// 18. Sprawdzenie statusu ��czenia funkcj� glGetProgramiv()
	// Pierwszym parametrem jest identyfikator obiektu programu cieniowania, 
	// drugim rodzaj sprawdzanego b��du (GL_LINK_STATUS)
	// trzecim adres zmiennej na zwracany kod b��du (success)
	// ...
	glGetProgramiv(progObj[shaderNum], GL_LINK_STATUS, &success);

	// 19. Informacja o ewentualnym b��dzie w przypadku, gdy success == 0
	// SHADER LINKING", "Unable to link"
	// ...
	if (success == 0)
	{
		glGetShaderInfoLog(shaderNum, MAX_INFO_LOG_SIZE, NULL, infoLog);
		FatalError((char*)GL_COMPILE_STATUS, infoLog, "");
		// 20. Ustawienie w odpowiednim miejscu tablicy needsValidation[] 
		// informacji o konieczno�ci walidacji programu cieniowania (GL_TRUE)
		// ...
		needsValidation[shaderNum] = GL_TRUE;
	}

}
// ------------------------------------------------------------------------
void DrawModels(void)
{ // narysowanie obiekt�w na scenie, ustawienie parametr�w Uniform dla jednostek cieniowania
	M3DVector3f lightPos0Eye, lightPos1Eye, lightPos2Eye;
	M3DMatrix44f mv;

	// transformacja �wiat�a do przestrzeni kamery (eye space)
	glPushMatrix();
	glRotatef(lightRotation, 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, mv);
	m3dTransformVector3(lightPos0Eye, lightPos0, mv);


	if (actualShader >= THREELIGHTS)
	{
		m3dTransformVector3(lightPos1Eye, lightPos1, mv);
		m3dTransformVector3(lightPos2Eye, lightPos2, mv);
	}

	glPopMatrix();
	GLint uniformLoc;

	// 29. Pobranie identyfikatora zmiennej Uniform dla �r�d�a �wiat�a nr 0 funkcj� glGetUniformLocation() 
	// Pierwszym parametrem jest aktualny identyfikator obiektu programu cieniowania
	// drugim parametrem jest nazwa zmiennej Uniform "lightPos[0]"
	// Wynik nale�y podstawi� do zmiennej uniformLoc
	// ...


	// 30. Je�eli zwr�cony identyfikator jest r�ny od -1, ustawienie aktualnej pozycji �wiat�a 0 funkcj� glUniform3fv()
	// Pierwszym parametrem jest identyfikator zmiennej, 
	// drugim parametrem jest liczba argument�w (1)
	// trzecim parametrem wektor wsp�rz�dnych o�wietlenia o (lightPos0Eye)
	// ...



	// 37. Analogiczne ustawienie parametru dla �wiat�a nr 1
	// ...




	// 37. Analogiczne ustawienie parametru dla �wiat�a nr 2
	// ...




	// Narysowanie niebieskiej podstawy obiekt�w
	glColor3f(0.0f, 0.0f, 0.90f);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, -25.0f, -100.0f);
	glVertex3f(-100.0f, -25.0f, 100.0f);
	glVertex3f(100.0f, -25.0f, 100.0f);
	glVertex3f(100.0f, -25.0f, -100.0f);
	glEnd();
	// czerwony sze�cian
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidCube(48.0f);
	// zielona sfera
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glTranslatef(-60.0f, 0.0f, 0.0f);
	glutSolidSphere(25.0f, 50, 50);
	glPopMatrix();
	// bordowy torus
	glColor3f(1.0f, 0.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 60.0f);
	glutSolidTorus(8.0f, 16.0f, 50, 50);
	glPopMatrix();
	// ��ty sto�ek
	glColor3f(1.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(60.0f, 0.0f, -24.0f);
	glutSolidCone(25.0f, 50.0f, 50, 50);
	glPopMatrix();
	// morski czajniczek
	glColor3f(0.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -60.0f);
	glutSolidTeapot(25.0f);
	glPopMatrix();
}
void RenderScene(void)
{ // rysowanie sceny
	// ustawienie perspektywy
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (windowWidth > windowHeight)
	{
		GLdouble ar = (GLdouble)windowWidth / (GLdouble)windowHeight;
		glFrustum(-ar * cameraZoom, ar * cameraZoom, -cameraZoom, cameraZoom, 1.0, 1000.0);
	}
	else
	{
		GLdouble ar = (GLdouble)windowHeight / (GLdouble)windowWidth;
		glFrustum(-cameraZoom, cameraZoom, -ar * cameraZoom, ar * cameraZoom, 1.0, 1000.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glViewport(0, 0, windowWidth, windowHeight);
	// wyczyszczenie okna
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLint success;
	// 20. Walidacja aktualnego (actualShader) programu cieniowania. 
	// WY��CZNIE w przypadku, gdy potrzebuje walidacji needsValidation[]
	// UWAGA! actualShader jest indeksem!!!!!
	// ...
	if (needsValidation[actualShader])
	{
		// 21. Walidacja aktualnego (actualShader) programu cieniowania funkcj� glValidateProgram()
		// Parametrem jest aktualny identyfikator obiektu programu cieniowania
		// ...
		glValidateProgram(actualShader);

		// 22. Sprawdzenie statusu walidacji funkcj� glGetProgramiv()
		// Pierwszym parametrem jest identyfikator obiektu programu cieniowania, 
		// drugim rodzaj sprawdzanego b��du (GL_VALIDATE_STATUS)
		// trzecim adres zmiennej na zwracany kod b��du (success)
		// ...
		glGetProgramiv(actualShader, GL_VALIDATE_STATUS, &success);

		// 23. Informacja o ewentualnym b��dzie w przypadku, gdy success == 0
		// "SHADER VALIDATION", "Unable to validate"
		// ...
		if (success == 0)
		{
			glGetShaderInfoLog(actualShader, MAX_INFO_LOG_SIZE, NULL, infoLog);
			FatalError((char*)GL_VALIDATE_STATUS, infoLog, "");
		}

		// 24. Wyzerowanie informacji o potrzebie walidacji aktualnego programu cieniowania
		// ...
		needsValidation[actualShader] = GL_FALSE;
	}
	// Narysowanie modeli 
	DrawModels();
	if (glGetError() != GL_NO_ERROR)
		fprintf(stderr, "GL Error!\n");
	// narysowanie sceny
	glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{ // inicjalizacja sceny
	fprintf(stdout, "Masters of GLSL shaders (2016)\n\n");
	// 4. Sprawdzenie spe�nienia minimalnych wymog�w wersji cieniowania SM (shadermodel)
	if (!GLEE_VERSION_2_0 && (!GLEE_ARB_fragment_shader || !GLEE_ARB_vertex_shader ||
		!GLEE_ARB_shader_objects || !GLEE_ARB_shading_language_100))
		FatalError("OpenGL version", "OpenGL version", "earlier then 2.0");

	fprintf(stdout, "Controls:\n");
	fprintf(stdout, "\tR/L arrows\t+/- rotate lights\n\n");
	fprintf(stdout, "\tx/X\t\tMove +/- in x direction\n");
	fprintf(stdout, "\ty/Y\t\tMove +/- in y direction\n");
	fprintf(stdout, "\tz/Z\t\tMove +/- in z direction\n\n");
	fprintf(stdout, "\tq, ESC\t\tExit demo\n\n");

	// ustawienia OpenGL
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // czarne t�o
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);

	// 25. Wczytanie i przygotowanie wszystkich SHADERS_NUM program�w cieniowania funkcj� PrepareShader()
	// ...
	for(int i = 0; i < SHADERS_NUM; i++)
		PrepareShader(i);

	// 26. Ustawienie aktualnego (actualShader) programu cieniowania funkcj� glUseProgram()
	// Parametrem jest identyfikator aktualnego programu cieniowania
	// UWAGA! actualShader jest indeksem!!!!!
	// ...
	glUseProgram(actualShader);
}
char* SetTitle(void)
{
	static char buffer[200];
	sprintf(buffer, "Master of GLSL shaders by %s [%s]", authorName, shaderNames[actualShader]);
	return(buffer);
}
void KeyPressFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ': // spacja
		actualShader = (ShaderTypes)((actualShader + 1) % SHADERS_NUM); // naprzemienne w��czenie kolejnych program�w cieniowania
		glutSetWindowTitle(SetTitle());

		// 27. Ustawienie nowego (actualShader) programu cieniowania funkcj� glUseProgram()
		// Parametrem jest identyfikator aktualnego programu cieniowania
		// ...
		glUseProgram(actualShader);

		break;
	case 'x':
		cameraPos[0] += 5.0f;
		break;
	case 'X':
		cameraPos[0] -= 5.0f;
		break;
	case 'y':
		cameraPos[1] += 5.0f;
		break;
	case 'Y':
		cameraPos[1] -= 5.0f;
		break;
	case 'z':
		cameraPos[2] += 5.0f;
		break;
	case 'Z':
		cameraPos[2] -= 5.0f;
		break;
	case 'q':
	case 'Q':
	case 27: /* ESC */
		glutDestroyWindow(glutGetWindow());
		break;
	}
	glutPostRedisplay(); // od�wie�enie okna
}
void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		lightRotation -= 5.0f;
		break;
	case GLUT_KEY_RIGHT:
		lightRotation += 5.0f;
		break;
	case GLUT_KEY_UP:
		cameraPos[1] += 5.0f;
		break;
	case GLUT_KEY_DOWN:
		cameraPos[1] -= 5.0f;
		break;
	}
	glutPostRedisplay(); // od�wie�enie okna
}
void ChangeSize(int w, int h)
{
	windowWidth = w;
	windowHeight = h;
}

int main(int argc, char* argv[])
{
	GLint i;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow(SetTitle()); // utworzenie okna
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(KeyPressFunc);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();

	if (glDeleteProgram && glDeleteShader)
	{
		// 26. Zwolnienie pami�ci dla wszystkich (SHADERS_NUM) obiekt�w programu cieniowania 
		// oraz jednostek cieniowania VS i FS funkcjami glDeleteProgram() oraz glDeleteShader()
		// ...
		for (i = 0; i < SHADERS_NUM; i++)
		{
			glDeleteProgram(progObj[i]);
			glDeleteShader(vShader[i]);
			glDeleteShader(fShader[i]);
		}
	}
	return 0;
}
