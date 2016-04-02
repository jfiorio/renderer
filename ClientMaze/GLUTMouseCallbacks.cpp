#include "GLUTMouseCallbacks.h"

extern double eye_pos[3];  /* defined in mainMaze.cpp */
extern double eye_theta;   /* defined in mainMaze.cpp */
extern double eye_phi;     /* defined in mainMaze.cpp */

static double sight_vector[3];  /* vector in the direction of the eye */
static double up_vector[3];
static double right_vector[3];

static double drag_speed = 0.75;
static double move_speed = 0.3;

static double previous_x = 0.0,
              previous_y = 0.0;

static bool mouseJustEntered = false;


/* GLUT callback function */
void mouseEntry(int state)  // doesn't work
{
   if (state == GLUT_ENTERED)
   {
      mouseJustEntered = true;
   }
   return;
}


/* GLUT callback function */
void mouseMotion(int x, int y)
{
   if (mouseJustEntered)  // doesn't work
   {
      mouseJustEntered = false;
      previous_x = x;
      previous_y = y;
   }

   /* Jerry Dekker's idea */
   if ( abs(x - previous_x) > 20 || abs(y - previous_y) > 20 )
   {
      previous_x = x;
      previous_y = y;
   }

   /* The mouse motion changes the "spherical coordinates" of the eye. */
   eye_theta = eye_theta + drag_speed * (previous_x - x);
   eye_phi   = eye_phi   + drag_speed * (previous_y - y);

   eye_phi = (eye_phi <   0) ?   0 : eye_phi;
   eye_phi = (eye_phi > 180) ? 180 : eye_phi;

   previous_x = x;
   previous_y = y;

/*
   Compute three useful vectors to determine where the
   viewer is facing. They are used in the move functions,
   since you move in the direction you are facing.
*/

   /* compute the up vector */
   up_vector[0] = 0;
   up_vector[1] = 1;
   up_vector[2] = 0;
   rotateAboutX(up_vector, eye_phi);
   rotateAboutZ(up_vector, eye_theta);

   /* compute the line of sight vector */
   sight_vector[0] = 0;
   sight_vector[1] = 0;
   sight_vector[2] = -1;
   rotateAboutX(sight_vector, eye_phi);
   rotateAboutZ(sight_vector, eye_theta);

   /* take the cross product between sight_vector
      and up_vector to get the right_vector */
   crossProduct(right_vector, sight_vector, up_vector);

#if DEBUG2
   fprintf(stderr, "eye_theta = %f, eye_phi = %f\n", eye_theta, eye_phi);
   fflush(stderr);

   fprintf(stderr,"[% .6f % .6f % .6f]\n", up_vector[0], up_vector[1], up_vector[2]);
   fflush(stderr);

   fprintf(stderr,"{% .6f % .6f % .6f}\n", sight_vector[0], sight_vector[1], sight_vector[2]);
   fflush(stderr);

   fprintf(stderr,"<% .6f % .6f % .6f>\n", right_vector[0], right_vector[1], right_vector[2]);
   fflush(stderr);
#endif /* DEBUG2 */

   glutPostRedisplay();
   return;
}//mouseMotion()



/*
   Functions to move the viewpoint around.
   These functions are called by the GLUT
   keyboard callback handler.
*/
void moveForward(void)
{
   /* update the x and y components of the eye position
      moving in the direction of the sight vector */
   eye_pos[0] += sight_vector[0] * move_speed;
   eye_pos[1] += sight_vector[1] * move_speed;
   return;
}

void moveBack(void)
{
   /* update the x and y components of the eye position
      moving in the opposite direction of the sight vector */
   eye_pos[0] -= sight_vector[0] * move_speed;
   eye_pos[1] -= sight_vector[1] * move_speed;
   return;
}

void moveLeft(void)
{
   /* update the x and y components of the eye position
      moving in the opposite direction of the right vector */
   eye_pos[0] += -right_vector[0] * move_speed;
   eye_pos[1] += -right_vector[1] * move_speed;
   return;
}

void moveRight(void)
{
   /* update the x and y components of the eye position
      moving in the direction of the right vector */
   eye_pos[0] += right_vector[0] * move_speed;
   eye_pos[1] += right_vector[1] * move_speed;
   return;
}

void moveUp(void)
{
   /* update the z component of the eye position
      moving in the vertical direction */
   eye_pos[2] += 2;
   return;
}

void moveDown(void)
{
   /* update z component of the eye position
      moving in the vertically downward direction */
   eye_pos[2] -= 2;
   return;
}


/*
   These rotations all assume the right-hand rule,
   so if the vector is pointing towards the
   viewer, the positive rotation is counterclockwise.
*/
#define PI 3.1415926535897932384626433832795

void rotateAboutX(double *vector, double angle)
{
   float new_vector[3];

   /* note that the ith component of the vector stays the same */
   new_vector[1] = cos(angle * MY_PI/180.0) * vector[1] - sin(angle * MY_PI/180.0) * vector[2];
   new_vector[2] = sin(angle * MY_PI/180.0) * vector[1] + cos(angle * MY_PI/180.0) * vector[2];

   /* copy it back to calling vector */
   vector[1] = new_vector[1];
   vector[2] = new_vector[2];
   return;
}


#if 0
void rotateAboutY(double *vector, double angle) /* not used! */
{
   float new_vector[3];

   /* note that the jth component of the vector stays the same */
   new_vector[0] = cos(angle * MY_PI/180.0) * vector[0] + sin(angle * MY_PI/180.0) * vector[2];
   new_vector[2] = -sin(angle * MY_PI/180.0) * vector[0] + cos(angle * MY_PI/180.0) * vector[2];

   /* copy it back to calling vector */
   vector[0] = new_vector[0];
   vector[2] = new_vector[2];
   return;
}
#endif


void rotateAboutZ(double *vector, double angle)
{
   float new_vector[3];

   /* note that the kth component of the vector stays the same */
   new_vector[0] = cos(angle * MY_PI/180.0) * vector[0] - sin(angle * MY_PI/180.0) * vector[1];
   new_vector[1] = sin(angle * MY_PI/180.0) * vector[0] + cos(angle * MY_PI/180.0) * vector[1];

   /* copy it back to calling vector */
   vector[0] = new_vector[0];
   vector[1] = new_vector[1];
   return;
}


void crossProduct(double *res_vector, double *vector1, double *vector2)
{
   res_vector[0] = (vector1[1] * vector2[2]) - (vector1[2] * vector2[1]);
   res_vector[1] = (vector1[2] * vector2[0]) - (vector1[0] * vector2[2]);
   res_vector[2] = (vector1[0] * vector2[1]) - (vector1[1] * vector2[0]);
   return;
}


#define FLOAT_EPSILON 0.0000001

bool floatEqual(double a, double b)
{
   double diff = a - b;
   /* return true if they are within FLOAT_EPSILON of each other */
   return ((-FLOAT_EPSILON < diff) && (diff < FLOAT_EPSILON));
}
