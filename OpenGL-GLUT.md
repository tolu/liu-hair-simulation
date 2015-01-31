OpenGL ger oss tillgång till grafikhårdvara vilket gör våra program effektivare och snabbare.

För att kunna interagera med fönstersystemet (win, linux, Mac) behöver vi GLUT. GLUT låter oss få tillgång till musen, tangentbordet och andra vanliga operationer i fönstersystemen.

**OpenGL är en "state machine"** I openGL kan man bara definiera en "status" åt gången, till exempel:

    glColor3d(1,0,0);

Allt som ritas efter detta kommer att vara rött till man anropar `glColor()` på nytt med andra variabler, exvis: `glColor(1,1,1)`

**Vad innehåller OpenGL?**
C:a 200 funktioner i följande klasser:
* primitiver (plygoner, bilder)
* attributförändringar (ljuskällor, färger, linjetyper, texturer)
* visningsfunktioner (kontrollera virtuell kamera och dess lins)
* fönsterhantering (är egentligen GLUT: skärmkontroll, mus, tangentbord)
* kontrollfunktioner (stänger av eller på egenskaper hos openGL)

**Programmering**
Varje program består av tre delar:
* specificera objekten som skall renderas
* beskriva egenskaperna hos objekten
* definiera hur objekten skall visas

**Namnstandarder för openGL & GLUT**
* anrop/funktioner som börjar med `gl` = openGL
* anrop/funktioner som börjar med `glu` = openGL utilities
* anrop/funktioner som börjar med `glut`= GLUT

funtionerna heter saker i stil med:

    glVertex3f(); 
    glVertex3i();

namnen skall tolkas som att funktionen tar 3 argument av typen integer (i) resp float (f).
Vissa funktioner i openGL vill ha inputargument av typerna:
`GLfloat`, `GLdouble` eller `GLint`
det är inte konstigare än att de datatyperna är definierade i gl.h med en liten rad som säger: `typedef GLfloat float;` osv.