#include "stdafx.h"
#include <GL/glut.h>
#include <stdlib.h>
#include "materials.h"
#include "colors.h"

// stałe do obsługi menu podręcznego
enum
{
	BRASS, // mosiądz
	BRONZE, // brąz
	POLISHED_BRONZE, // polerowany brąz
	CHROME, // chrom
	COPPER, // miedź
	POLISHED_COPPER, // polerowana miedź
	GOLD, // złoto
	POLISHED_GOLD, // polerowane złoto
	PEWTER, // grafit (cyna z ołowiem)
	SILVER, // srebro
	POLISHED_SILVER, // polerowane srebro
	EMERALD, // szmaragd
	JADE, // jadeit
	OBSIDIAN, // obsydian
	PEARL, // perła
	RUBY, // rubin
	TURQUOISE, // turkus
	BLACK_PLASTIC, // czarny plastik
	BLACK_RUBBER, // czarna guma
	FULL_WINDOW, // aspekt obrazu - całe okno
	ASPECT_1_1, // aspekt obrazu 1:1
	EXIT, // wyjście

	LIGHT_DIRECTIONAL,
	SOLID,
	WIRED
};
// aspekt obrazu
int aspect = FULL_WINDOW;
// rozmiary bryły obcinania
const GLdouble left = -18.0;
const GLdouble right = 18.0;
const GLdouble bottom = -18.0;
const GLdouble top = 18.0;
const GLdouble near_ = 70.0;
const GLdouble far_ = 120.0;
// współczynnik skalowania
GLfloat scale = 1;
// właściwości materiału - domyślnie mosiądz
const GLfloat *ambient = BrassAmbient;
const GLfloat *diffuse = BrassDiffuse;
const GLfloat *specular = BrassSpecular;
GLfloat shininess = BrassShininess;
// kąty obrotu
GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;
// przesunięcie
GLfloat translatex = 0.0;
GLfloat translatey = 0.0;
// swiatlo
int light = LIGHT_DIRECTIONAL;
// kierunek źródła światła
GLfloat light_position[4] =
{
	0.0,0.0,0.2,0.0
};
// kąty obrotu kierunku źródła światła
GLfloat light_rotatex = 0.0;
GLfloat light_rotatey = 0.0;
// wskaŸnik naciśnięcia lewego przycisku myszki
int button_state = GLUT_UP;
// poło¿enie kursora myszki
int button_x, button_y;

int Solid = 0;

// funkcja rysująca piramidę
void Pyramid()
{
	int lvl = 9;
	double z = 2;
	for (int i = lvl; i > 0; i--)
	{
			glTranslatef(0.0, 0.0, z);
			double x = i;
			double y = i;
			double x1 = i+2;
			double y1 = i+2;
			for (int j = 0; j < i; j++)
			{
				for (int k = 1; k <= i; k++)
				{
					glPushMatrix();
					glTranslatef(x1, y1, 0.0);

					switch (Solid)
					{
					case 0:
						glutWireTorus(0.3, 0.7, 4.0, 8.0);
						break;
					case 1:
						glutSolidTorus(0.3, 0.7, 4.0, 8.0);
						break;
					default:
						break;
					}

					glPopMatrix();
					x1 -= 2;
				}
				y1 -= 2;
				x1 = i + 2;
			}
	}

}
// funkcja generująca scenę 3D
void Display()
{
	// kolor tła - zawartość bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);
	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();
	// włączenie testu bufora głębokości
	glEnable(GL_DEPTH_TEST);
	// włączenie oświetlenia
	glEnable(GL_LIGHTING);
	// włączenie światła GL_LIGHT0 z parametrami domyślnymi
	glEnable(GL_LIGHT0);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(light_rotatex, 1.0, 0, 0);
	glRotatef(light_rotatey, 0, 1.0, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
	// przesunięcie układu współrzędnych obiektu do środka bryły odcinania
	glTranslatef(0, 0, -(near_ + far_) / 2);
	// przesunięcie obiektu - ruch myszką
	glTranslatef(translatex, translatey, 0.0);
	// skalowanie obiektu - klawisze "+" i "-"
	glScalef(scale, scale, scale);
	// właściwości materiału
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	// obroty obiektu - klawisze kursora
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);
	// kolor krawędzi obiektu
	glColor3f(0.0, 0.0, 0.0);
	// rysowanie piramidy
	Pyramid();
	// skierowanie poleceń do wykonania
	glFlush();
	// zamiana buforów koloru
	glutSwapBuffers();
}
// zmiana wielkości okna
void Reshape(int width, int height)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, width, height);
	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);
	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();
	// parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokość okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom*height / width, top*height / width, near_,
				far_);
		else
			// szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left*width / height, right*width / height, bottom, top,
					near_, far_);
	}
	else
		glFrustum(left, right, bottom, top, near_, far_);
	// generowanie sceny 3D
	Display();
}
// obsługa klawiatury
void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		scale += 0.1;
	else
		// klawisz -
		if (key == '-' && scale > 0.1)
			scale -= 0.1;
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
// obsługa klawiszy funkcyjnych i klawiszy kursora
void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;
		// kursor w górę
	case GLUT_KEY_UP:
		rotatex -= 1;
		break;
		// kursor w prawo
	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;
		// kursor w dół
	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
// obsługa przycisków myszki
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapamiętanie stanu lewego przycisku myszki
		button_state = state;
		// zapamiętanie poło¿enia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}
// obsługa ruchu kursora myszki
void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		translatex += 1.1 *(right - left) / glutGet(GLUT_WINDOW_WIDTH) * (x - button_x);
		button_x = x;
		translatey += 1.1 *(top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) * (button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}
// obsługa menu podręcznego
void Menu(int value)
{
	switch (value)
	{
		// materiał - mosiądz
	case BRASS:
		ambient = BrassAmbient;
		diffuse = BrassDiffuse;
		specular = BrassSpecular;
		shininess = BrassShininess;
		Display();
		break;
		// materiał - brąz
	case BRONZE:
		ambient = BronzeAmbient;
		diffuse = BronzeDiffuse;
		specular = BronzeSpecular;
		shininess = BronzeShininess;
		Display();
		break;
		// materiał - polerowany brąz
	case POLISHED_BRONZE:
		ambient = PolishedBronzeAmbient;
		diffuse = PolishedBronzeDiffuse;
		specular = PolishedBronzeSpecular;
		shininess = PolishedBronzeShininess;
		Display();
		break;
		// materiał - chrom
	case CHROME:
		ambient = ChromeAmbient;
		diffuse = ChromeDiffuse;
		specular = ChromeSpecular;
		shininess = ChromeShininess;
		Display();
		break;
		// materiał - miedź
	case COPPER:
		ambient = CopperAmbient;
		diffuse = CopperDiffuse;
		specular = CopperSpecular;
		shininess = CopperShininess;
		Display();
		break;
		// materiał - polerowana miedź
	case POLISHED_COPPER:
		ambient = PolishedCopperAmbient;
		diffuse = PolishedCopperDiffuse;
		specular = PolishedCopperSpecular;
		shininess = PolishedCopperShininess;
		Display();
		break;
		// materiał - złoto
	case GOLD:
		ambient = GoldAmbient;
		diffuse = GoldDiffuse;
		specular = GoldSpecular;
		shininess = GoldShininess;
		Display();
		break;
		// materiał - polerowane złoto
	case POLISHED_GOLD:
		ambient = PolishedGoldAmbient;
		diffuse = PolishedGoldDiffuse;
		specular = PolishedGoldSpecular;
		shininess = PolishedGoldShininess;
		Display();
		break;
		// materiał - grafit (cyna z ołowiem)
	case PEWTER:
		ambient = PewterAmbient;
		diffuse = PewterDiffuse;
		specular = PewterSpecular;
		shininess = PewterShininess;
		Display();
		break;
		// materiał - srebro
	case SILVER:
		ambient = SilverAmbient;
		diffuse = SilverDiffuse;
		specular = SilverSpecular;
		shininess = SilverShininess;
		Display();
		break;
		// materiał - polerowane srebro
	case POLISHED_SILVER:
		ambient = PolishedSilverAmbient;
		diffuse = PolishedSilverDiffuse;
		specular = PolishedSilverSpecular;
		shininess = PolishedSilverShininess;
		Display();
		break;
		// materiał - szmaragd
	case EMERALD:
		ambient = EmeraldAmbient;
		diffuse = EmeraldDiffuse;
		specular = EmeraldSpecular;
		shininess = EmeraldShininess;
		Display();
		break;
		// materiał - jadeit
	case JADE:
		ambient = JadeAmbient;
		diffuse = JadeDiffuse;
		specular = JadeSpecular;
		shininess = JadeShininess;
		Display();
		break;
		// materiał - obsydian
	case OBSIDIAN:
		ambient = ObsidianAmbient;
		diffuse = ObsidianDiffuse;
		specular = ObsidianSpecular;
		shininess = ObsidianShininess;
		Display();
		break;
		// materiał - perła
	case PEARL:
		ambient = PearlAmbient;
		diffuse = PearlDiffuse;
		specular = PearlSpecular;
		shininess = PearlShininess;
		Display();
		break;
		// metariał - rubin
	case RUBY:
		ambient = RubyAmbient;
		diffuse = RubyDiffuse;
		specular = RubySpecular;
		shininess = RubyShininess;
		Display();
		break;
		// materiał - turkus
	case TURQUOISE:
		ambient = TurquoiseAmbient;
		diffuse = TurquoiseDiffuse;
		specular = TurquoiseSpecular;
		shininess = TurquoiseShininess;
		Display();
		break;
		// materiał - czarny plastik
	case BLACK_PLASTIC:
		ambient = BlackPlasticAmbient;
		diffuse = BlackPlasticDiffuse;
		specular = BlackPlasticSpecular;
		shininess = BlackPlasticShininess;
		Display();
		break;
		// materiał - czarna guma
	case BLACK_RUBBER:
		ambient = BlackRubberAmbient;
		diffuse = BlackRubberDiffuse;
		specular = BlackRubberSpecular;
		shininess = BlackRubberShininess;
		Display();
		break;
		// obszar renderingu - całe okno
	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;
		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;
	case SOLID:
		Solid = 1;
		break;
	case WIRED:
		Solid = 0;
		break;

		// wyjście
	case EXIT:
		exit(0);
	}
}
int main(int argc, char *argv[])
{
	// inicjalizacja biblioteki GLUT
	glutInit(&argc, argv);
	// inicjalizacja bufora ramki
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	// rozmiary głównego okna programu
	glutInitWindowSize(800, 800);
	// utworzenie głównego okna programu
	glutCreateWindow("Stos modelowania");
	// dołączenie funkcji generującej scenę 3D
	glutDisplayFunc(Display);
	// dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutReshapeFunc(Reshape);
	// dołączenie funkcji obsługi klawiatury
	glutKeyboardFunc(Keyboard);
	// dołączenie funkcji obsługi klawiszy funkcyjnych i klawiszy kursora
	glutSpecialFunc(SpecialKeys);
	// obsługa przycisków myszki
	glutMouseFunc(MouseButton);
	// obsługa ruchu kursora myszki
	glutMotionFunc(MouseMotion);

	// utworzenie podmenu - Materiał
	int MenuMaterial = glutCreateMenu(Menu);
#ifdef WIN32
	glutAddMenuEntry("Mosiadz", BRASS);
	glutAddMenuEntry("Braz", BRONZE);
	glutAddMenuEntry("Polerowany braz", POLISHED_BRONZE);
	glutAddMenuEntry("Chrom", CHROME);
	glutAddMenuEntry("Miedz", COPPER);
	glutAddMenuEntry("Polerowana miedz", POLISHED_COPPER);
	glutAddMenuEntry("Zloto", GOLD);
	glutAddMenuEntry("Polerowane zloto", POLISHED_GOLD);
	glutAddMenuEntry("Grafit (cyna z olowiem)", PEWTER);
	glutAddMenuEntry("Srebro", SILVER);
	glutAddMenuEntry("Polerowane srebro", POLISHED_SILVER);
	glutAddMenuEntry("Szmaragd", EMERALD);
	glutAddMenuEntry("Jadeit", JADE);
	glutAddMenuEntry("Obsydian", OBSIDIAN);
	glutAddMenuEntry("Perla", PEARL);
	glutAddMenuEntry("Rubin", RUBY);
	glutAddMenuEntry("Turkus", TURQUOISE);
	glutAddMenuEntry("Czarny plastik", BLACK_PLASTIC);
	glutAddMenuEntry("Czarna guma", BLACK_RUBBER);
#else
	glutAddMenuEntry("Mosiadz", BRASS);
	glutAddMenuEntry("Braz", BRONZE);
	glutAddMenuEntry("Polerowany braz", POLISHED_BRONZE);
	glutAddMenuEntry("Chrom", CHROME);
	glutAddMenuEntry("Miedz", COPPER);
	glutAddMenuEntry("Polerowana miedz", POLISHED_COPPER);
	glutAddMenuEntry("Zloto", GOLD);
	glutAddMenuEntry("Polerowane zloto", POLISHED_GOLD);
	glutAddMenuEntry("Grafit (cyna z ołowiem)", PEWTER);
	glutAddMenuEntry("Srebro", SILVER);
	glutAddMenuEntry("Polerowane srebro", POLISHED_SILVER);
	glutAddMenuEntry("Szmaragd", EMERALD);
	glutAddMenuEntry("Jadeit", JADE);
	glutAddMenuEntry("Obsydian", OBSIDIAN);
	glutAddMenuEntry("Perla", PEARL);
	glutAddMenuEntry("Rubin", RUBY);
	glutAddMenuEntry("Turkus", TURQUOISE);
	glutAddMenuEntry("Czarny plastik", BLACK_PLASTIC);
	glutAddMenuEntry("Czarna guma", BLACK_RUBBER);
#endif
	// utworzenie podmenu - solid czy wired
	int MenuSolid = glutCreateMenu(Menu);
#ifdef WIN32
	glutAddMenuEntry("SOLID", SOLID);
#else
	glutAddMenuEntry("SOLID", SOLID);
#endif
	glutAddMenuEntry("WIRED", WIRED);
	// utworzenie podmenu - aspekt obrazu
	int MenuAspect = glutCreateMenu(Menu);
#ifdef WIN32
	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#else
	glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
#endif
	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);
	// menu główne
	glutCreateMenu(Menu);
#ifdef WIN32
	glutAddSubMenu("Material", MenuMaterial);
#else
	glutAddSubMenu("Material", MenuMaterial);
#endif
#ifdef WIN32
	glutAddSubMenu("Solid/Wired", MenuSolid);
#else
	glutAddSubMenu("Solid/Wired", MenuSolid);
#endif
	glutAddSubMenu("Aspekt obrazu", MenuAspect);
#ifdef WIN32
	glutAddMenuEntry("Wyjscie", EXIT);
#else
	glutAddMenuEntry("Wyjscie", EXIT);
#endif
	// określenie przycisku myszki obsługującego menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	// wprowadzenie programu do obsługi pętli komunikatów
	glutMainLoop();
	return 0;
}