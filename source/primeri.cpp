struct Objekat {
	int broj;
	float prozirnost;
	char* ime;
};

// Креирамо складиште за објекат.
Objekat noviObjekat;


// Уписујемо податке у објекат.
static void upisPodataka() {
	noviObjekat.broj = 10;
	noviObjekat.prozirnost = 0.66f;
	noviObjekat.ime = "Neki tekst.";
}


static void glPrimer() {
	/* 
	
	Креирамо складиште за објекат. Како је у питању апстракција позива 
	коментаришемо код јер га није могуће компајлирати.

	GLuint imeObjekta;
	glGenObject(1, &imeObjekta);

	Уписујемо податке у објекат.
	glBindObject(GL_MODIFY, imeObjekta);
	glObjectParamteteri(GL_MODIFY, GL_OBJECT_COUNT, 10);
	glObjectParamteterf(GL_MODIFY, GL_OBJECT_OPACITY, 0.66f);
	glObjectParameters(GL_MODIFY, GL_OBJECT_NAME, "Neki tekst.");

	*/
}