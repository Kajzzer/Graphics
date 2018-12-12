/* Computer Graphics and Game Technology, Assignment Box2D game
 *
 * Student name ....
 * Student email ...
 * Collegekaart ....
 * Date ............
 * Comments ........
 *
 *
 * (always fill in these fields before submitting!!)
 */

#include <cstdio>

#ifdef __APPLE__
#include <OpenGL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <Box2D/Box2D.h>

#include "levels.h"

unsigned int reso_x = 800, reso_y = 600; // Window size in pixels
const float world_x = 8.f, world_y = 6.f; // Level (world) size in meters

int last_time;
int frame_count;

// Information about the levels loaded from files will be available in these.
unsigned int num_levels;
unsigned int current_level;
level_t *levels;

b2Vec2 gravity(0.0f, -5.0f);
b2World* world = new b2World(gravity);
b2Body* ball;
b2Body* end;

/*
 * Load a given world, i.e. read the world from the `levels' data structure and
 * convert it into a Box2D world.
 */
void load_world(unsigned int level)
{
    if (level >= num_levels)
    {
        // Note that level is unsigned but we still use %d so -1 is shown as
        // such.
        printf("Warning: level %d does not exist.\n", level);
        return;
    }

    // Create a Box2D world and populate it with all bodies for this level
    // (including the ball).

    // Define ball body
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(levels[level].start.x, levels[level].start.y);
    ballBodyDef.angle = 0.0f;

    // Define circle shape
    b2CircleShape circle;
    circle.m_p.Set(0.0f, 0.0f);
    circle.m_radius = 0.2f;

    // Define ball fixture
    b2FixtureDef ballFixture;
    ballFixture.shape = &circle;
    ballFixture.density = 1.0f;

    // Create ball body
    ball = world->CreateBody(&ballBodyDef);
    ball->CreateFixture(&ballFixture);


    b2BodyDef endBodyDef;
    endBodyDef.type = b2_staticBody;
    endBodyDef.position.Set(levels[level].end.x, levels[level].end.y);
    endBodyDef.angle = 0.0f;

    // Define box shape
    b2PolygonShape box;

    b2Vec2 *endVertices = new b2Vec2[4];

    endVertices[0] = b2Vec2(levels[level].end.x-0.5, levels[level].end.y-10);
    endVertices[1] = b2Vec2(levels[level].end.x-0.5, levels[level].end.y+10);
    endVertices[2] = b2Vec2(levels[level].end.x+0.5, levels[level].end.y-10);
    endVertices[3] = b2Vec2(levels[level].end.x+0.5, levels[level].end.y+10);

    box.Set(endVertices , 4);

    // Define ball fixture
    b2FixtureDef endFixture;
    endFixture.shape = &box;
    endFixture.density = 1.0f;

    // Create ball body
    end = world->CreateBody(&endBodyDef);
    end->CreateFixture(&endFixture);

    unsigned int i, j;

    b2Body* body;

    for(i = 0; i < levels[level].num_polygons; i++)
    {
        b2PolygonShape shape;
        b2BodyDef levelBodyDef;
        b2FixtureDef levelFixtureDef;

        unsigned int num_verts = levels[level].polygons[i].num_verts;

        b2Vec2 *vertices = new b2Vec2[num_verts];
        for(j = 0; j < num_verts; j++)
        {
            vertices[j] = b2Vec2(levels[level].polygons[i].verts[j].x, levels[level].polygons[i].verts[j].y);
        }

        shape.Set(vertices, num_verts);

        levelBodyDef.type = b2_staticBody;
        levelBodyDef.position.Set(levels[level].polygons[i].position.x,levels[level].polygons[i].position.y);
        levelBodyDef.angle = 0.0f;

        levelFixtureDef.shape = &shape;
        levelFixtureDef.density = 1.0f;

        body = world->CreateBody(&levelBodyDef);
        body->CreateFixture(&levelFixtureDef);
    }

}


/*
 * Called when we should redraw the scene (i.e. every frame).
 * It will show the current framerate in the window title.
 */
void draw(void)
{
    unsigned int i, j;
    int time = glutGet(GLUT_ELAPSED_TIME);
    int frametime = time - last_time;
    int circle_triangles = 30;
    double ballx = ball->GetPosition().x;
    double bally = ball->GetPosition().y;
    double ballr = ball->GetFixtureList()[0].GetShape()->m_radius;

    double endx = end->GetPosition().x;
    double endy = end->GetPosition().y;


    double pi = 3.141592653589793;
    float timeStep = 1.0f/60.0f;
    int velocityIterations = 6;
    int positionIterations = 2;
    frame_count++;

    // Clear the buffer
    glColor3f(0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //
    // Do any logic and drawing here.
    //
    world->Step(timeStep, velocityIterations, positionIterations);

    // Draw a red ball
    glColor3f(1, 0, 1);
    // printf("%f %f\n", ballx, bally);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(ballx, bally);
    for (i = 0; i <= circle_triangles; i++)
    {
        glVertex2f(ballx + ballr * cos(i * 2 * pi / circle_triangles),
                   bally + ballr * sin(i * 2 * pi / circle_triangles));
    }
    glEnd();

    glColor3f(0.0f, 1.0f, 0.0f);

    for(i = 0; i < levels[current_level].num_polygons; i++)
    {

        unsigned int num_verts = levels[current_level].polygons[i].num_verts;
        glBegin(GL_POLYGON);

        for(j = 0; j < num_verts; j++)
        {
            glVertex2f(levels[current_level].polygons[i].verts[j].x, levels[current_level].polygons[i].verts[j].y);
        }

        glEnd();
    }

    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
        glVertex2f(endx-0.05f, endy-0.05f);
        glVertex2f(endx-0.05f, endy+0.05f);
        glVertex2f(endx+0.05f, endy+0.05f);
        glVertex2f(endx+0.05f, endy-0.05f);
    glEnd();


    b2AABB aabbA;
    aabbA.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
    aabbA.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
    b2Fixture* fixture = ball->GetFixtureList();
    while (fixture != NULL)
    {
        aabbA.Combine(aabbA, fixture->GetAABB(0));
        fixture = fixture->GetNext();
    }

    b2AABB aabbB;
    aabbB.lowerBound = b2Vec2(FLT_MAX,FLT_MAX);
    aabbB.upperBound = b2Vec2(-FLT_MAX,-FLT_MAX);
    b2Fixture* fixture2 = end->GetFixtureList();
    while (fixture2 != NULL)
    {
        aabbB.Combine(aabbB, fixture2->GetAABB(0));
        fixture2 = fixture2->GetNext();
    }

    int overlap = b2TestOverlap(aabbA, aabbB);
    if (overlap)
    {
        current_level++;
        delete world;
        world = new b2World(gravity);
        load_world(current_level);
    }


    // Show rendered frame
    glutSwapBuffers();

    // Display fps in window title.
    if (frametime >= 1000)
    {
        char window_title[128];
        snprintf(window_title, 128,
                "Box2D: %f fps, level %d/%d",
                frame_count / (frametime / 1000.f), current_level+1, num_levels);
        glutSetWindowTitle(window_title);
        last_time = time;
        frame_count = 0;
    }
}

/*
 * Called when window is resized. We inform OpenGL about this, and save this
 * for future reference.
 */
void resize_window(int width, int height)
{
    glViewport(0, 0, width, height);
    reso_x = width;
    reso_y = height;
}

/*
 * Called when the user presses a key.
 */
void key_pressed(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27: // Esc
        case 'q':
            exit(0);
            break;
        // Add any keys you want to use, either for debugging or gameplay.
        default:
            break;
    }
}

/*
 * Called when the user clicked (or released) a mouse buttons inside the window.
 */
void mouse_clicked(int button, int state, int x, int y)
{

}

/*
 * Called when the mouse is moved to a certain given position.
 */
void mouse_moved(int x, int y)
{

}


int main(int argc, char **argv)
{
    // Create an OpenGL context and a GLUT window.
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(reso_x, reso_y);
    glutCreateWindow("Box2D");

    glewInit();

    // Bind all GLUT events do callback function.
    glutDisplayFunc(&draw);
    glutIdleFunc(&draw);
    glutReshapeFunc(&resize_window);
    glutKeyboardFunc(&key_pressed);
    glutMouseFunc(&mouse_clicked);
    glutMotionFunc(&mouse_moved);
    glutPassiveMotionFunc(&mouse_moved);

    // Initialise the matrices so we have an orthogonal world with the same size
    // as the levels, and no other transformations.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, world_x, 0, world_y, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Read the levels into a bunch of structs.
    num_levels = load_levels(&levels);
    printf("Loaded %d levels.\n", num_levels);

    // Load the first level (i.e. create all Box2D stuff).
    current_level = 0;
    load_world(current_level);

    last_time = glutGet(GLUT_ELAPSED_TIME);
    frame_count = 0;
    glutMainLoop();

    return 0;
}