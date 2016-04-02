#include <stdio.h>
#include "MazeParser.h"


MazeParser::MazeParser()
{
   scene = new Scene();

   scene_width = 0;
   scene_height = 0;
   cells_wide = 5;
   cells_high = 5;
   wall_height = 5;
   cell_size = 5;
   num_tex = 0;
};


MazeParser::~MazeParser()
{
   sceneCleanup();
};


void MazeParser::sceneCleanup()
{
   int i;
   /* release all the textures */
   for (i = 0; i < scene->textureCount; i++)
   {
      delete scene->textures[i];
   }
   return;
}


Scene* MazeParser::loadScene(char *name)
{
   char line_buffer[MAX_LINE_SIZE];

   fp = fopen(name,"r");

   if(!fp)
   {   /* file could not be opened properly */
      fprintf(stderr,"ERROR: Scene file %s could not be loaded\n", name);
      fflush(stderr);
      exit(-1);
   }

   /* now go through all the lines in the file */
   while(fgets(line_buffer, MAX_LINE_SIZE, fp))
   {
      /* only do something if it's not a comment and the line is not blank */
      if (!lineCanBeIgnored(line_buffer))
      {
         /* at this point it is neither a comment nor a blank line */
         parseScene(line_buffer);
      }
   }
   fclose(fp);

   return scene;
}


void MazeParser::parseScene(char *line)
{
   char command[MAX_LINE_SIZE];
   char arg[MAX_LINE_SIZE];
   char *start, *end;

   /* i need to get the first word */
   end = findFirstSpace(line);

   /* observe what the command is */
   copyString(command, line, end);

   if (!strcmp(command,"DIMENSIONS"))
   {
      /* parse the dimensions of the scene */
      start = skipSpace(end);
      end = findFirstSpace(start);
      copyString(arg, start, end);
      end = skipSpace(end);

      cells_wide = atoi(arg);

      start = skipSpace(end);
      end = findFirstSpace(start);
      copyString(arg, start, end);
      end = skipSpace(end);

      cells_high = atoi(arg);

      /* the rest of the line should be able to be ignored */
      assert(lineCanBeIgnored(end));

      fprintf(stderr,"width: %d   height: %d\n", cells_wide, cells_high);
      fflush(stderr);
   }
   else if (!strcmp(command,"HEIGHT"))
   {
      start = skipSpace(end);
      end = findFirstSpace(start);
      copyString(arg, start, end);
      end = skipSpace(end);

      wall_height = atoi(arg);

      fprintf(stderr,"wall height: %d\n", wall_height);
      fflush(stderr);
   }
   else if (!strcmp(command,"CELL"))
   {
      start = skipSpace(end);
      end = findFirstSpace(start);
      copyString(arg, start, end);
      end = skipSpace(end);

      cell_size = atoi(arg);

      fprintf(stderr,"cell size: %d\n", cell_size);
      fflush(stderr);
   }
   else if (!strcmp(command,"TEXTURES"))
   {
      start = skipSpace(end);
      end = findFirstSpace(start);
      copyString(arg, start, end);
      end = skipSpace(end);

      num_tex = atoi(arg);

      fprintf(stderr,"number of textures: %d\n", num_tex);
      fflush(stderr);

      loadTextures();
   }
   else if (!strcmp(command,"FLOORPLAN"))
   {  /* the rest of the line should be able to be ignored */
      assert(lineCanBeIgnored(end));

      loadFloorPlan();
   }
   else
   {
      fprintf(stderr,"ERROR! Unrecognized command in scene file \n");
      fprintf(stderr,"%s\n", line);
      fflush(stderr);
      exit(-1);
   }
   return;
}


void MazeParser::loadFloorPlan()
{
   int count = 0;
   char line_buffer[MAX_LINE_SIZE];
   fprintf(stderr,"Loading floor plan\n");
   fflush(stderr);

   /* compute the size of the scene */
   scene_width = cells_wide * cell_size;
   scene_height = cells_high * cell_size;

   fprintf(stderr,"size of scene: %d x %d\n", scene_width, scene_height);
   fflush(stderr);

   while((count < cells_high * 2) && fgets(line_buffer, MAX_LINE_SIZE, fp))
   {
      /* only do something if it's not a comment and the line is not blank */
      if (!lineCanBeIgnored(line_buffer))
      {
         parseFloorPlan(count, line_buffer);
         count++;
      }
   }

   /* after i have added all the walls, let us add the floor */
   float x = -(((float)scene_width) / (float)2);
   float y = (((float)scene_height) / (float)2);
   addFloor(x, y, x + scene_width, y - scene_height, scene->textures[num_tex-1], scene_width, scene_height);

   return;
}


void MazeParser::parseFloorPlan(int count, char *line)
{
   int i;
   int *walls;
   int row;
   float x,y;

   /* make room for the data */
   walls = (int *)malloc(sizeof(int) * cells_wide);
   if (!walls)
   {
      fprintf(stderr, "ERROR! Unable to allocate memory to parse floor plan\n");
      fflush(stderr);
      exit(-1);
   }

   /* first parse the line and determine where the walls are */
   for (i = 0; i < cells_wide; i++)
   {
      walls[i] = getWall(&line);
      /* note that a 0 indicates no wall, else is the wall number */

      if ((walls[i] < 0) || (walls[i] > num_tex))
      {
         fprintf(stderr,"ERROR!  Invalid wall specified\n");
         fflush(stderr);
         exit(-1);
      }
   }

   row = count/2;

   y = (((float)scene_height) / (float)2) - row * cell_size;
   x = -(((float)scene_width) / (float)2);

   if (count %2)
   {  /* count is odd, so we are specifying vertical walls */
      for (i = 0; i < cells_wide; i++)
      {
         if (walls[i])
         {
            /* add the wall to the list */
            addWall(x, y, x, y - (float)cell_size, wall_height, scene->textures[walls[i] - 1]);
         }
         x += (float)cell_size;
      }
   }
   else
   {  /* count is even, so we are specifying horizontal walls */
      for (i = 0; i < cells_wide; i++)
      {
         if (walls[i])
         {
            addWall(x, y, x + (float)cell_size, y, wall_height, scene->textures[walls[i] - 1]);
         }
         x += (float)cell_size;
      }
   }
   free(walls);
   return;
}


int MazeParser::getWall(char **line)
{
   char arg[3];
   char *start, *end;

   start = skipSpace(*line);
   end = findFirstSpace(start);
   copyString(arg, start, end);
   *line = skipSpace(end);

   return (atoi(arg));
}


void MazeParser::loadTextures()
{
   int count = 0;
   char line_buffer[MAX_LINE_SIZE];

   fprintf(stderr,"Loading textures\n");
   fflush(stderr);

   while((count < num_tex) && fgets(line_buffer, MAX_LINE_SIZE, fp))
   {
      /* only do something if it's not a comment and the line is not blank */
      if (!lineCanBeIgnored(line_buffer))
      {
         fprintf(stderr,"%s\n", line_buffer);
         fflush(stderr);
         parseTexture(count, line_buffer);
         count++;
      }
   }
   return;
}


void MazeParser::parseTexture(int count, char *line)
{
   char *start, *end;
   char tex_name[MAX_LINE_SIZE];
   char arg[MAX_LINE_SIZE];

   end = findFirstSpace(line);
   copyString(arg, line, end);
   end = skipSpace(end);

   start = skipSpace(end);
   end = findFirstSpace(start);
   copyString(tex_name, start, end);

   scene->addTexture( new Texture(tex_name) );

   return;
}


void MazeParser::addWall(float x1, float y1, float x2, float y2, int wall_height, Texture *tex)
{
   Triangle *t1, *t2;
   Vertex v0(x1, y1, 0);
   Vertex v1(x2, y2, 0);
   Vertex v2(x2, y2, wall_height);
   Vertex v3(x1, y1, wall_height);

   /* set the texture coordinates for the wall */
   v0.setCoords(0.0, 0.0);
   v1.setCoords(1.0, 0.0);
   v2.setCoords(1.0, 1.0);
   v3.setCoords(0.0, 1.0);

   t1 = new Triangle(&v0, &v1, &v2);
   t2 = new Triangle(&v2, &v3, &v0);

   t1->setTexture(tex);
   t2->setTexture(tex);

   /* add these triangles to the list */
   scene->addTriangle( t1 );
   scene->addTriangle( t2 );

   return;
}


void MazeParser::addFloor(float x1, float y1, float x2, float y2, Texture *tex, int scene_width, int scene_height)
{
   Triangle *t1, *t2;
   Vertex v0(x1, y2, 0);
   Vertex v1(x2, y2, 0);
   Vertex v2(x2, y1, 0);
   Vertex v3(x1, y1, 0);

   /* set the texture coordinates for the wall */
   v0.setCoords(0.0, 0.0);
   v1.setCoords(scene_width/4.0, 0.0);
   v2.setCoords(scene_width/4.0, scene_height/4.0);
   v3.setCoords(0.0, scene_height/4.0);

   t1 = new Triangle(&v0, &v1, &v2);
   t2 = new Triangle(&v2, &v3, &v0);

   t1->setTexture(tex);
   t2->setTexture(tex);

   /* add these triangles to the list */
   scene->addTriangle( t1 );
   scene->addTriangle( t2 );

   return;
}
