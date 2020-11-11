#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>

static bool sbIzracunato = false;

struct Tacka {
	float x;
	float y;
};


Tacka KatmulRom(float t, Tacka p1, Tacka p2, Tacka p3, Tacka p4) {
	float t2 = t * t;
	float t3 = t2 * t;
	Tacka v; // Интерполирана тачкна

	// Израчунавање Катмул-Ром сплајна

	v.x = ((-t3 + 2 * t2 - t)*(p1.x) + 
		(3 * t3 - 5 * t2 + 2)*(p2.x) + 
		(-3 * t3 + 4 * t2 + t)* (p3.x) + 
		(t3 - t2)*(p4.x)) / 2;

	v.y = ((-t3 + 2 * t2 - t)*(p1.y) + 
		(3 * t3 - 5 * t2 + 2)*(p2.y) + 
		(-3 * t3 + 4 * t2 + t)* (p3.y) + 
		(t3 - t2)*(p4.y)) / 2;

	if (!sbIzracunato) std::cout << "v.x = " << v.x << ", " << v.y << std::endl;

	return v;
}

void inicijalizacijaScene() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}


void preoblikuj(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	if (w <= h)
		glOrtho(-50.0, 50.0, -50.0*(GLfloat)h / (GLfloat)w, 50.0*(GLfloat)h / (GLfloat)w, -50.0, 50.0);
	else
		glOrtho(-50.0, 5.0, -50.0*(GLfloat)w / (GLfloat)h, 50.0*(GLfloat)w / (GLfloat)h, -50.0, 50.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
void prikaz(void) {
	Tacka v;	// Интерполирана тачка
	Tacka p1, p2, p3, p4;

	// Наше четири тачке.

	p1.x = -90.0f; p1.y = 30.0f;
	p2.x = -30.0f; p2.y = -30.0f;
	p3.x = 30.0f; p3.y = 30.0f;
	p4.x = 90.0f; p4.y = -30.0f;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(3); // Величина једне тачке (пиксела).

	glBegin(GL_POINTS);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glVertex2f(p3.x, p3.y);
	glVertex2f(p4.x, p4.y);
	glEnd();

	unsigned int brojInterpoliranihTacaka = 0;
	for (float t = 0; t < 1; t += 0.001f) {
		brojInterpoliranihTacaka++;
		v = KatmulRom(t, p1, p2, p3, p4);
		glBegin(GL_POINTS);
		glVertex2f(v.x, v.y);
		glEnd();
	}
	if (!sbIzracunato) std::cout << "Broj interpoliranih tacaka: " << brojInterpoliranihTacaka << std::endl;

	sbIzracunato = true;
	glFlush();
}


int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Katmul-Rom splajn");
	inicijalizacijaScene();
	glutDisplayFunc(prikaz);
	glutReshapeFunc(preoblikuj);
	glutMainLoop();
}