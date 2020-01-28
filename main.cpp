


#include <math.h>
#include <iostream>
#include <GL/gl.h>     // openGL header
#include <GL/glu.h>   // glut header
#include <GL/glut.h>   // glut header

using namespace std;

class CColor {
public:
	CColor(){}

    CColor (GLfloat rin, GLfloat gin, GLfloat bin) : r(rin), g(gin), b(bin) {}
    GLfloat r;
    GLfloat g;
    GLfloat b;
};
class CPoint {
public:
    GLint x;
    GLint y;
};

class CPolygon {
public:
    CPoint *points;
    GLint idx;
    CColor lineColor;//= new CColor(0.0, 0.0, 0.0);
    CColor fillColor;//= new CColor(1.0, 1.0, 1.0);
};

GLint w = 600; // window width
GLint h = 500; // window height

GLint rule = 0; // default rule is rule 0

// points array
GLint pointIdx = 0; // number of last point in current polygon
CPoint * points = new CPoint[100]; // points created so far for current polygon

CPoint pointToMove;
GLint polygonIndex;
GLint edgeIndex;

// coloring for polygons
CColor lineColor(0.0, 0.0, 0.0); // line color
CColor fillColor(1.0, 1.0, 1.0); // fill color

GLint polygonIdx = 0; // number of last polygon
CPolygon * polygons = new CPolygon[100]; // polygons created so far

// state management
enum STATE_TYPE {
    IDLE, // right click menu is in place
    DRAW_POLYGON, // drawing a polygon, right click will stop polygon draw
    DRAW_PIXELS, // draw pixels based on rule.
    MOVE_EDGE, // user selects an edge to move
    EDGE_FOUND, // edge found and user can move it
};
STATE_TYPE state = IDLE; // default state is IDLE

enum MENU_TYPE
{
    EXIT,
    START_POLYGON,
    MOVE,
    BLACK,
    WHITE,
    BLUE,
    RED,
    GREEN,
    YELLOW,
    PURPLE,
    ORANGE,
    CYAN,
    GRAY,
    SEA_BLUE,
    OPEN_GREEN,
    DARK_PURPLE,
    BROWN,
    DARK_BROWN,
    DARK_GREEN,
    RULE0,
    RULE1,
    RULE2,
    RULE3,
    RULE4,
};

// this functions are used for drawing
void draw();
void drawLine(CPoint cp1, CPoint cp2, CColor cc);
void drawPixels(int rule);

void processMenu(int);
void processMenuLine(int);
void processMenuRules(int);
void processMenuRules(int);
void createMenu();

bool innerPoint(CPoint P, CPoint * V, int n, int rule);
int isLeft( CPoint P0, CPoint P1, CPoint P2 );
int cn_PnPoly( CPoint P, CPoint* V, int n );
int wn_PnPoly( CPoint P, CPoint* V, int n );

void drawLine(CPoint cp1, CPoint cp2, CColor cc) {
    // setup the color
    glColor3f(cc.r, cc.g, cc.b);
    
    // setup line drawing
    glBegin(GL_LINES);
    
    // setup line locations
    glVertex2d(cp1.x, cp1.y);
    glVertex2d(cp2.x, cp2.y);
    
    // end drawing lines
    glEnd();
    
    // rerender the screen
    glutPostRedisplay();
}

// process action menu selections
void processMenu(int item)
{
    switch (item)
    {
        case EXIT:
            exit(0);
            break;
        case START_POLYGON:            
            // reset points index and points array
            pointIdx = 0;
            points = new CPoint[100];
            
            // detach menu from right button click
            glutDetachMenu(GLUT_RIGHT_BUTTON);
            
            // change state to draw polygons
            state = DRAW_POLYGON;
            
            break;
        case MOVE:
            state = MOVE_EDGE;
        default:
            break;
    }
    glutPostRedisplay();
    return;
}

// process line menu selections
void processMenuLine(int action) {
    switch (action) {
        case BLACK:
            lineColor = {0.0, 0.0, 0.0};
            break;
        case WHITE:
            lineColor = {1.0, 1.0, 1.0};
            break;
        case BLUE:
            lineColor = {0.0, 0.0, 1.0};
            break;
        case GREEN:
            lineColor = {0.0, 1.0, 0.0};
            break;
        case RED:
            lineColor = {1.0, 0.0, 0.0};
            break;
        case YELLOW:
            lineColor = {1.0, 1.0, 0.0};
            break;
        case PURPLE:
            lineColor = {1.0, 0.0, 1.0};
            break;
        case CYAN:
            lineColor = {0.0, 1.0, 1.0};
            break;
        case GRAY:
            lineColor = {0.6, 0.6, 0.6};
            break;
        case SEA_BLUE:
            lineColor = {0.2, 0.6, 0.8};
            break;
        case ORANGE:
            lineColor = {1.0, 0.6, 0.1};
            break;
        case OPEN_GREEN:
            lineColor = {0.0, 0.8, 0.2};
            break;
        case BROWN:
            lineColor = {0.8, 0.6, 0.4};
            break;
        case DARK_PURPLE:
            lineColor = {0.5, 0.2, 0.5};
            break;
        case DARK_BROWN:
            lineColor = {0.8, 0.0, 0.2};
            break;
        case DARK_GREEN:
            lineColor = {0.0, 0.3, 0.2};
            break;
        default:
            break;
    }
}

// process fill menu selections
void processMenuFill(int action) {
    switch (action) {
        case BLACK:
            fillColor = {0.0, 0.0, 0.0};
            break;
        case WHITE:
            fillColor = {1.0, 1.0, 1.0};
            break;
        case BLUE:
            fillColor = {0.0, 0.0, 1.0};
            break;
        case GREEN:
            fillColor = {0.0, 1.0, 0.0};
            break;
        case RED:
            fillColor = {1.0, 0.0, 0.0};
            break;
        case YELLOW:
            fillColor = {1.0, 1.0, 0.0};
            break;
        case PURPLE:
            fillColor = {1.0, 0.0, 1.0};
            break;
        case CYAN:
            fillColor = {0.0, 1.0, 1.0};
            break;
		case GRAY:
            fillColor = {0.6, 0.6, 0.6};
            break;
        case SEA_BLUE:
            fillColor = {0.2, 0.6, 0.8};
            break;
        case ORANGE:
            fillColor = {1.0, 0.6, 0.1};
            break;
        case OPEN_GREEN:
            fillColor = {0.0, 0.8, 0.2};
            break;
        case BROWN:
            fillColor = {0.8, 0.6, 0.4};
            break;
        case DARK_PURPLE:
            fillColor = {0.5, 0.2, 0.5};
            break;
        case DARK_BROWN:
            fillColor = {0.8, 0.0, 0.2};
            break;
        case DARK_GREEN:
            fillColor = {0.0, 0.3, 0.2};
            break;
        default:
            break;
    }
}

// process rule menu selections
void processMenuRules(int action) {
    switch (action) {
        case RULE0:
            state = DRAW_PIXELS;
            rule = 0;
        case RULE1:
            state = DRAW_PIXELS;
            rule = 1;
            break;
        case RULE2:
            state = DRAW_PIXELS;
            rule = 2;
            break;
        case RULE3:
            state = DRAW_PIXELS;
            rule = 3;
            break;
        case RULE4:
            state = DRAW_PIXELS;
            rule = 4;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void createMenu() {
    // create action submenu
    int actionSubmenu = glutCreateMenu(processMenu);
    glutAddMenuEntry("Polygon", START_POLYGON);
    glutAddMenuEntry("Move", MOVE);
    glutAddMenuEntry("Exit", EXIT);
    
    int lineColorSubmenu = glutCreateMenu(processMenuLine);
    glutAddMenuEntry("Black", BLACK);
    glutAddMenuEntry("White", WHITE);
    glutAddMenuEntry("Red", RED);
    glutAddMenuEntry("Blue", BLUE);
    glutAddMenuEntry("Green", GREEN);
    glutAddMenuEntry("Yellow", YELLOW);
    glutAddMenuEntry("Purple", PURPLE);
    glutAddMenuEntry("Cyan", CYAN);
    glutAddMenuEntry("Gray", GRAY);
    glutAddMenuEntry("Sea Blue", SEA_BLUE);
    glutAddMenuEntry("Orange", ORANGE);
    glutAddMenuEntry("Open Green", OPEN_GREEN);
    glutAddMenuEntry("Brown", BROWN);
    glutAddMenuEntry("Dark Purple", DARK_PURPLE);
    glutAddMenuEntry("Dark Brown", DARK_BROWN);
    glutAddMenuEntry("Dark Green", DARK_GREEN);

    int fillColorSubmenu = glutCreateMenu(processMenuFill);
    glutAddMenuEntry("Black", BLACK);
    glutAddMenuEntry("White", WHITE);
    glutAddMenuEntry("Red", RED);
    glutAddMenuEntry("Blue", BLUE);
    glutAddMenuEntry("Green", GREEN);
    glutAddMenuEntry("Yellow", YELLOW);
    glutAddMenuEntry("Purple", PURPLE);
    glutAddMenuEntry("Cyan", CYAN);
    glutAddMenuEntry("Gray", GRAY);
    glutAddMenuEntry("Sea Blue", SEA_BLUE);
    glutAddMenuEntry("Orange", ORANGE);
    glutAddMenuEntry("Open Green", OPEN_GREEN);
    glutAddMenuEntry("Brown", BROWN);
    glutAddMenuEntry("Dark Purple", DARK_PURPLE);
    glutAddMenuEntry("Dark Brown", DARK_BROWN);
    glutAddMenuEntry("Dark Green", DARK_GREEN);
    
    int ruleSubmenu = glutCreateMenu(processMenuRules);
    glutAddMenuEntry("Rule 0", RULE0);
    glutAddMenuEntry("Rule 1", RULE1);
    glutAddMenuEntry("Rule 2", RULE2);
    glutAddMenuEntry("Rule 3", RULE3);
    glutAddMenuEntry("Rule 4", RULE4);
    
    glutCreateMenu(processMenu);
    glutAddSubMenu("ACTION", actionSubmenu);
    glutAddSubMenu("LINE_COLOR", lineColorSubmenu);
    glutAddSubMenu("FILL_COLOR", fillColorSubmenu);
    glutAddSubMenu("Rules", ruleSubmenu);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// create menu end **************

// mouse ************************
void processMouse(int b, int s, int x, int y);

void processMouse(int b, int s, int x, int y) {
    // process left click
    if (b == GLUT_LEFT_BUTTON) {
        // process move - edge selection
        if (state == MOVE_EDGE && s == GLUT_DOWN) {
            for (int i=0; i<polygonIdx; i++) {     //pairnw to poligono kai to apothikeuo 
                CPolygon polygon = polygons[i];
                for (int j=0; j<polygon.idx; j++) {
                    CPoint edge = polygon.points[j];     //pairnw tis korifes twn poligonwn kai tis apothikeuw
                    
                    GLint xDistance = abs(x - edge.x);
                    GLint yDistance = abs(y - edge.y);
                    
                    if (xDistance < 5 && yDistance < 5) {
                        state = EDGE_FOUND;
                        polygonIndex = i;                          //poio poligono einai 
                        edgeIndex = j;                             //korifi pou exoume piasei gia metakinisi
                    }
                }
            }
            return;
        }
        
        if (state == EDGE_FOUND && s == GLUT_UP) {
            state = IDLE;

            polygons[polygonIndex].points[edgeIndex].x = x;      //edw auto pou kanoume einai oti otan afisoume to koumpi pou patame 
            polygons[polygonIndex].points[edgeIndex].y = y;      // h nea korifi apothikeyetai sto poligono to neo
			if(edgeIndex==polygons[polygonIndex].idx-1){
            	polygons[polygonIndex].points[0].x = x;
            	polygons[polygonIndex].points[0].y = y;
			}
            glutPostRedisplay();
            return;
        }
        
        // process mouse up - start polygon creation
        if (s == GLUT_UP) {                                                             //zografizei thn grami apo to neo simeio x,y
            CPoint point = {x, y};                                                      //sto proigoumeno
            points[pointIdx] = point;
            if (pointIdx != 0) {
                drawLine(points[pointIdx-1], points[pointIdx], lineColor);
            }
            pointIdx++;
        }
    }
    // process right click
    else if (b == GLUT_RIGHT_BUTTON && s==GLUT_UP) {
        // reattach menu
        createMenu();
        
        // change state to idle
        state = IDLE;
        
        // last point in polygon
        CPoint lastPoint = {points[0].x, points[0].y};
        points[pointIdx] = lastPoint;
        pointIdx++;
        
        // store current polygon to list of polygons
        CPolygon polygon;
		polygon.lineColor.r = lineColor.r;
		polygon.lineColor.g = lineColor.g;
		polygon.lineColor.b = lineColor.b;
		polygon.fillColor.r = fillColor.r;
		polygon.fillColor.g = fillColor.g;
		polygon.fillColor.b = fillColor.b;
        //polygon.lineColor = new CColor(lineColor.r, lineColor.g, lineColor.b);
        //polygon.fillColor = new CColor(fillColor.r, fillColor.g, fillColor.b);
        polygon.idx = pointIdx;
        polygon.points = points;
        polygons[polygonIdx] = polygon;
        polygonIdx++;
		        
		lineColor.r = 0;
		lineColor.g = 0;
		lineColor.b = 0;
		fillColor.r = 1;
		fillColor.g = 1;
		fillColor.b = 1;
    }
}
// mouse end ********************

/*
 * Main
 */
int main(int argc, char ** argv) {
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(w, h);
    glutCreateWindow("PLY701");
    glutPositionWindow(5, 250);
    glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode (GL_MODELVIEW);
	gluOrtho2D (0.0, w, h, 0.0);
    createMenu();
    glutMouseFunc(processMouse);
    glutDisplayFunc(draw);
    glutMainLoop();
    
    return 0;
}

void draw() {
    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (state == DRAW_PIXELS) {
        drawPixels(rule);
    }
    
    // draw polygons in the way they were created
    for (int i=0; i<polygonIdx; i++) {
        CPolygon cpo = polygons[i];
        
        glColor3f(cpo.lineColor.r, cpo.lineColor.g, cpo.lineColor.b);
        glBegin(GL_LINE_LOOP);
        for (int j=0; j<cpo.idx; j++) {
            glVertex2d(cpo.points[j].x, cpo.points[j].y);
        }
        glEnd();
    }
    
    // draw lines for the polygon created now
    if (state == DRAW_POLYGON && pointIdx > 0) {
        for (int i=1; i<pointIdx; i++) {
            drawLine(points[i-1], points[i], lineColor);
        }
    }
    
    glutSwapBuffers();
}

void drawPixels(int rule) {
    bool isIn = false;
    int polygonsCounter = 0;
    int pointsFoundInPolygon = 0;
    
    // every row
    for (int i=0; i<w; i++) {
        // every column
        for (int j=0; j<h; j++) {
            // current pixel
            CPoint point = {i, j};
            CColor color(0.0, 0.0, 0.0);
            
            // check all polygos
            for (int k=0; k<polygonIdx; k++) {
                // current polygon
                CPolygon p = polygons[k];
                
                // number of points in polygon
                int n = p.idx;
                
                // check if point is in
                isIn = innerPoint(point, p.points, n-1, rule);
                if (isIn == true) {
                    polygonsCounter++;
                    color.r += p.fillColor.r;
                    color.g += p.fillColor.g;
                    color.b += p.fillColor.b;
                }
            }
            
            // if found at least part of a polygon it should be drawn
            // with the polygon color
            if (polygonsCounter > 0) {
                pointsFoundInPolygon++;
                color.r = color.r / polygonsCounter;
                color.g = color.g / polygonsCounter;
                color.b = color.b / polygonsCounter;
                
                glColor3f(color.r, color.g, color.b);
            } else {
                glColor3f(1.0, 1.0, 1.0);
            }
            
            // draw the point appropriately
            glBegin(GL_POINTS);
            glVertex2i(i, j);
            glEnd();
            
            // reset polygons counter for next pixel
            polygonsCounter = 0;
        }
    }
}

bool innerPoint(CPoint P, CPoint * V, int n, int rule) {
    int in;
    switch (rule) {
        case 0:
            break;
        case 1:
            in = cn_PnPoly(P, V, n);
            if (in == 1) {
                return 1; // point is in the polygon
            }
            return 0; // point is out of the polygon
            break;
        case 2:
            in = wn_PnPoly(P, V, n);
            if (in != 0) {
                return 1; // point is in
            } else {
                return 0; // point is out
            }
            break;
        case 3:
            in = wn_PnPoly(P, V, n);
            if (int(ceil(abs(in)/2)) % 2 == 1) {
                return true; // point is in the polygon
            } else {
                return false; // pointn is out of the polygon
            }
            break;
        case 4:
            in = wn_PnPoly(P, V, n);
            if (in > 0) {
                return true;
            } else {
                return false;
            }
            break;
        default:
            break;
    }
    return 0; // error case
}

int cn_PnPoly( CPoint P, CPoint * V, int n )
{
    int xmax = -1;                            //thetoume autes tis times gia na einai i sarosi anadromika mexri na vroume telika gia to poligono ta xmax kai xmin
    int xmin = w + 1;
    int ymax = -1;
    int ymin = h + 1;
    
    // check if point is within polygon boundaries
    for (int i=0; i<n; i++) {
        if (V[i].x > xmax) {
            xmax = V[i].x;
        }
        
        if (V[i].x < xmin) {
            xmin = V[i].x;
        }
        
        if (V[i].y > ymax) {
            ymax = V[i].y;
        }
        
        if (V[i].y < ymin) {
            ymin = V[i].y;
        }
    }
    
    if (P.x > xmax || P.x < xmin || P.y > ymax || P.y < ymin) {
        return 0; // is out
    }

    
    int cn = 0;
    for (int i=0; i<n; i++) {
        if (((V[i].y <= P.y) && (V[i+1].y > P.y))
                ||
                ((V[i].y > P.y) && (V[i+1].y <=  P.y)))
        {
            float vt = (float)(P.y  - V[i].y) / (V[i+1].y - V[i].y);        //
            if (P.x <  V[i].x + vt * (V[i+1].x - V[i].x)) {                   // tomh x me y=p.y
                ++cn;
            }
        }
    }
    
    return (cn&1);    // 0 if even (out), and 1 if  odd (in)
    
}

int wn_PnPoly( CPoint P, CPoint* V, int n )
{
    int    wn = 0;    // the  winding number counter
    
    // loop through all edges of the polygon
    for (int i=0; i<n; i++) {   // edge from V[i] to  V[i+1]
        if (V[i].y <= P.y) {          // start y <= P.y
            if (V[i+1].y  > P.y)      // an upward crossing
                if (isLeft( V[i], V[i+1], P) > 0)  // P left of  edge
                    ++wn;            // have  a valid up intersect
        }
        else {                        // start y > P.y (no test needed)
            if (V[i+1].y  <= P.y)     // a downward crossing
                if (isLeft( V[i], V[i+1], P) < 0)  // P right of  edge
                    --wn;            // have  a valid down intersect
        }
    }
    return wn;
}

int isLeft( CPoint P0, CPoint P1, CPoint P2 )
{
    return ( (P1.x - P0.x) * (P2.y - P0.y)
            - (P2.x -  P0.x) * (P1.y - P0.y) );
}
