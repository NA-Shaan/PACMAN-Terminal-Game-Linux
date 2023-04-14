#include "Ghosts.h"
Ghosts::Ghosts(std::string sprite_sheet,int posX,int posY,int obj_type, PlayableMap map)
:GameObject(sprite_sheet, posX, posY, obj_type)
{
    this->map = map;
}

Ghosts::~Ghosts()
{

}

void Ghosts::move(int delta_x, int delta_y)
{
    x += delta_x;
    y += delta_y;
}

void Ghosts:: handleState(GameObject* charac_obj_ptr)
{
    if (charac_obj_ptr->getPoints() < 1000) //easy difficulty
    {
        if (object_type == 6 || object_type == 7 || object_type == 8)
        {
            Resting();
        }
        else
        {
            Scatter();
        }
        ///make some loop for scatter chase cycle
        //put all ifs for each ghost here 
        current_speed = FRAMES_PER_MOVE;
    }
    else if (charac_obj_ptr->getPoints() < 2000 && charac_obj_ptr->getPoints() > 1000) //medium diff
    {
        ///make some loop for scatter chase cycle
         //put all ifs for each ghost here
        if (object_type == 4 || object_type == 5)
        {
            Chase(charac_obj_ptr);
        }
        else
        {
            Scatter();
        }
        
        current_speed = FRAMES_PER_MOVE-1;
    }
    else //hardest difficulty
    {
        ///make some loop for scatter chase cycle
         //put all ifs for each ghost here
        Chase(charac_obj_ptr); 
        current_speed = FRAMES_PER_MOVE-2;
    }
}

void Ghosts::Chase(GameObject* player)
{ 
    int chase_prob = (object_type == 4) ? ghost_chase_prob_R : (object_type == 5) ? ghost_chase_prob_Y : (object_type == 6) ? ghost_chase_prob_P : ghost_chase_prob_C;
    if (move_counter == 0)
    {
       Pathfind(player->getX(), player->getY(), chase_prob);
    }
    move_counter ++;
    move_counter%=(current_speed);
}

void Ghosts::Pathfind(int target_x,int target_y, int chase_prob)
{
    srand((int) time(0));
    int pos, length = 1000;
    int move_Directions[4][3] = {-1,-1,-1};
    int Directions[4][2] = {{0,1},{1,0},{0,-1},{-1,0}}; 
    int counter = 0, reverse_direc = (current_direction == LEFT_G) ? RIGHT_G : (current_direction == RIGHT_G) ? LEFT_G : (current_direction == UP_G) ? DOWN_G : UP_G;
    
    //calculate the shortest direction to move
    for (int i =0; i<4;i++)
    {
        if (map.IsMoveable(Directions[i][0] + x - 1, Directions[i][1] + y - 1, object_type) && i != reverse_direc)
        {
            move_Directions[counter][0] = Directions[i][0] + x;
            move_Directions[counter][1] = Directions[i][1] + y;
            move_Directions[counter][2] = i;

            //make a seperate length calcualtion for tunneling coordinates?
            if (abs(Directions[i][0] + x - target_x) + abs(Directions[i][1] + y - target_y) < length)
            {
                length = abs(Directions[i][0] + x - target_x) + abs(Directions[i][1] + y - target_y);
                pos = counter;
            }
            counter++;
        }
        
    }

    if (counter == 1)
    {
        moveToNewPos(move_Directions[0][0],move_Directions[0][1]);
        current_direction = move_Directions[0][2];
        return;
    }

    int other_directions[4][3] = {-1,-1,-1}, count = 0;
    for (int i=0;i<counter;i++)
    {
        if (i!=pos)
        {
            other_directions[count][0] = move_Directions[i][0];
            other_directions[count][1] = move_Directions[i][1];
            other_directions[count][2] = move_Directions[i][2];
            count++;
        }
        
    }

    //do a probability check
    bool choose_shortest_path = rand()%100<=chase_prob;
    //move towards which ever selected path
    if (choose_shortest_path)
    {
        moveToNewPos(move_Directions[pos][0],move_Directions[pos][1]);
        current_direction = move_Directions[pos][2];
    }
    else
    {
        int random_choice = rand()%(count);
        moveToNewPos(other_directions[random_choice][0],other_directions[random_choice][1]);
        current_direction = other_directions[random_choice][2];
    }
}

void Ghosts::Resting()
{
    x = 14;
    y = 15;
}


void Ghosts::Scatter()
{
    //check if at the scatter start pos
    int scatter_start_x = (object_type == 4) ? ghost_scatter_start_Rx : (object_type == 5) ? ghost_scatter_start_Yx : (object_type == 6) ? ghost_scatter_start_Px : ghost_scatter_start_Cx;
    int scatter_start_y = (object_type == 4) ? ghost_scatter_start_Ry : (object_type == 5) ? ghost_scatter_start_Yy : (object_type == 6) ? ghost_scatter_start_Py : ghost_scatter_start_Cy;
    mvprintw(40 + object_type, 40, "%i", reached_scatter_start);
    mvprintw(40 + object_type, 38, "%i", object_type);
    if (this->x == scatter_start_x + 1 && this->y == scatter_start_y + 1)
    {
        reached_scatter_start = true;
    }

    //add a check if moved to starting psotion
        //move to starting position
        //return every frame
            //set bool to false

    if (reached_scatter_start == false)
    {
        //move to starting position
        if (move_counter == 0)
        {
            Pathfind(scatter_start_x + 1, scatter_start_y + 1,75);
        }
    }
    else
    {

        int divisor=0;
        int new_X = 0, new_Y = 0;
        if (object_type == 4)
        {
            //red
            new_X = SCATTER_PATTERN_R[scatter_counter][0];
            new_Y = SCATTER_PATTERN_R[scatter_counter][1];
            divisor = 18;
        }
        else if(object_type == 5)
        {
            //yellow
            new_X = SCATTER_PATTERN_Y[scatter_counter][0];
            new_Y = SCATTER_PATTERN_Y[scatter_counter][1];
            divisor = 34;
        }
        else if(object_type == 6)
        {
            //pink
            new_X = SCATTER_PATTERN_P[scatter_counter][0];
            new_Y = SCATTER_PATTERN_P[scatter_counter][1];
            divisor = 18;
        }
        else
        {
            //cyan
            new_X = SCATTER_PATTERN_C[scatter_counter][0];
            new_Y = SCATTER_PATTERN_C[scatter_counter][1];
            divisor = 34;
        }

        if (move_counter == 0)
        {
            //chase to the 0th scatter position
            //if the ghost is in scatter and has not visted scatter 0th yet, move to it
            //otherwise, if it has been at scatter 0, iterate through scatter pattern.
            moveToNewPos(new_X + 1,new_Y + 1);
            scatter_counter++;
            scatter_counter%=divisor;
        }
    }
    move_counter++;
    move_counter%=current_speed;

}