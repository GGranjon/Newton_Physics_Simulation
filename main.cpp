#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <math.h>
#include <fstream>
#define G 6.6743*10e-7
#define fps 60
#define simulation_time 200
#define pos_min -1500
#define pos_max 1500
#define v_min -200
#define v_max 200
#define PI 3.14159265
#define source "data/data.txt"
using namespace std;

struct corps
{
    double x;
    double y;
    double vx;
    double vy;
    long long int masse;
    int radius;
    string type;
};

struct star : corps{
    star(double x_, double y_, string type_){
        masse = 10e12;
        x = x_;
        y = y_;
        vx = 0.;
        vy = 0.;
        type = type_;
        radius = 100;
    }
};

struct planet : corps{
    planet(double x_, double y_, double vx_, double vy_, string type_){
        masse = 2e10;
        x = x_;
        y = y_;
        vx = vx_;
        vy = vy_;
        type = type_;
        radius = 10;
    }
};

double to_radians(double degres){
    return degres*PI/180;
}

void rotation(vector<double>& coords, float theta){
    /*en degrés, sens horaire*/
    double x = coords[0];
    double y = coords[1];
    double rad_theta = to_radians(theta);
    coords[0] = x*cos(rad_theta)-y*sin(rad_theta);
    coords[1] = x*sin(rad_theta)+y*cos(rad_theta);
}

vector<vector<double>> genere_liste_planets(int nb, vector<double>& center, int h, double space, double speed){
    /*h la hauteur d'une branche (nb de points qu'on y place), b le nombre de branches autour du point, space l'espacement entre chaque points*/
    vector<vector<double>> objects;
    objects.resize(nb);
    vector<double> v_init = {speed, 0};
    int compteur = nb;
    int b = (nb%h != 0)? nb/h + 1 : nb/h;
    double theta = 360./(double)b;
    vector<double> coords;
    vector<double> speeds;
    vector<double> object;
    double x, y, vx, vy;
    
    for (int i = 0; i<b;i++){
        for (int j = 0; j<h;j++){
            if (i*h+j+1<=nb){
                y = center[1] + (j+1)*space;
                x = center[0];
                vx = speed;//+(speed/h)*j;
                vy = 0;
                speeds = {vx,vy};
                coords = {x,y};
                rotation(speeds, i*theta);
                rotation(coords, i*theta);
                object = {coords[0], coords[1], speeds[0], speeds[1]};
                objects[nb - compteur] = object;
                compteur--;
            }
        }
    }
    return objects;
}

void init_galaxy(vector<corps*>& current_objects, int nb_planets, star* star, int h, double space, double speed){
    vector<double> center = {star->x, star->y};
    vector<vector<double>> liste_planets = genere_liste_planets(nb_planets, center, h, space, speed);
    for (vector<double> obj : liste_planets){
        corps* ptr = new planet({obj[0], obj[1], obj[2], obj[3], "planete"});
        current_objects.push_back(ptr);
    }
}

double distance(const corps& a, const corps& b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void update(vector<corps*>& current_positions){
    double dt = 3.0 / fps;
    double force_x, force_y, dist, accel;

    for (size_t i = 0; i < current_positions.size(); ++i) {

        force_x = 0;
        force_y = 0;

        for (size_t j = 0; j < current_positions.size(); ++j) {

            dist = distance(*current_positions[i], *current_positions[j]);
            if (dist < 1e-5) {
                dist = 1e-5;  // Utilise une petite valeur pour éviter la division par zéro
            }
            // If a duplicate is found, erase the duplicate element

            if (i!=j && dist <= 2*(current_positions[i]->radius + current_positions[j]->radius)/3) { //collision
                double total_mass = current_positions[i]->masse + current_positions[j]->masse;
                current_positions[i]->vx = (current_positions[i]->vx * current_positions[i]->masse + current_positions[j]->vx * current_positions[j]->masse) / total_mass;
                current_positions[i]->vy = (current_positions[i]->vy * current_positions[i]->masse + current_positions[j]->vy * current_positions[j]->masse) / total_mass;
                current_positions[i]->type = (current_positions[i]->masse > current_positions[j]->masse) ? current_positions[i]->type : current_positions[j]->type;
                current_positions[i]->masse = total_mass;
                current_positions[i]->radius = sqrt(pow(current_positions[i]->radius,2)+pow(current_positions[j]->radius,2));
                current_positions.erase(current_positions.begin() + j);
                --j;
            }

            else{
                accel = (G * current_positions[j]->masse) / (dist * dist);
                // Projection sur x et y
                force_x += accel * ((current_positions[j]->x - current_positions[i]->x) / dist);
                force_y += accel * ((current_positions[j]->y - current_positions[i]->y) / dist);
            }
        }
        current_positions[i]->vx += force_x * dt;
        current_positions[i]->vy += force_y * dt;
    }


    // Mise à jour des positions
    for (corps* main_corps : current_positions) {
        main_corps->x += main_corps->vx * dt;
        main_corps->y += main_corps->vy * dt;
    }
}

void free_memory(vector<corps*> liste){
    for (corps* obj : liste){
        delete obj;
    }
}

void write_data(string src, vector<vector<corps*>>& all_data){
    ofstream file;
    file.open (src);
    for (vector<corps*> temps : all_data){
        file << "[";
        for (corps* obj : temps){
            if (obj != temps[temps.size() -1]){
                file << "(" + to_string(obj->x) +","+ to_string(obj->y) +","+ to_string(obj->vx) +","+ to_string(obj->vy) +","+ to_string(obj->masse) +","+ to_string(obj->radius) +","+ obj->type + "),";
            }
            else{
                file << "(" + to_string(obj->x) +","+ to_string(obj->y) +","+ to_string(obj->vx) +","+ to_string(obj->vy) +","+ to_string(obj->masse) +","+ to_string(obj->radius) +","+ obj->type + ")";
            }
        }
        file << "]\n";
    }
    file.close();
    cout<<"done!";
}

double rdm_value(int min, int max){
    // Initializing seed randomly for random number generator
    
    return (double)(min + rand() % (max - min + 1));
}

void init_randomly(int nb_planets, int nb_stars, vector<corps*>& current_objects){
    double x_val, y_val, v_x, v_y;
    for (int i = 0; i< nb_planets; i++){    //initializing each object
        x_val = rdm_value(pos_min, pos_max);
        y_val = rdm_value(pos_min, pos_max);
        v_x = rdm_value(v_min, v_max);
        v_y = rdm_value(v_min/10, v_max);
        corps* ptr = new planet({x_val,y_val,v_x,v_y,"planete-"+to_string(i)});
        current_objects.push_back(ptr);
    }
    for (int i = 0; i< nb_stars; i++){    //initializing each object
        x_val = rdm_value(pos_min, pos_max);
        y_val = rdm_value(pos_min, pos_max);
        v_x = rdm_value(pos_min/10, pos_max/10);
        v_y = rdm_value(pos_min/10, pos_max/10);
        corps* ptr = new star({x_val,y_val,"etoile-"+to_string(i)});
        current_objects.push_back(ptr);
    }
}

int main(int argc, char const *argv[])
{
    int nb_planets, nb_stars;
    vector<corps*> current_objects;
    vector<vector<corps*>> all_data;

    cout << "Combien de planetes?\n";
    cin >> nb_planets;
    /*cout << "Combien d'etoiles?\n";
    cin >> nb_stars;*/

    srand((time(NULL)));
    star* etoile = new star({0,0,"etoile"});
    //init_randomly(nb_planets, nb_stars, current_objects);
    init_galaxy(current_objects, nb_planets, etoile, 5, 200, 350);
    current_objects.push_back(etoile);
    int nb_frames = fps*simulation_time;
    for (int i = 0; i<nb_frames; i++){
        update(current_objects);
        // Crée une copie de l'état actuel des objets
        vector<corps*> frame_data;
        for (corps* obj : current_objects) {
            // Crée une copie de l'objet
            corps* copy = new corps(*obj);
            frame_data.push_back(copy);  // Ajoute la copie à la frame actuelle
        }
        all_data.push_back(frame_data);
    }
    write_data(source, all_data);
    for (vector<corps*> elt :all_data){
        for (corps* elt2 : elt){
            delete elt2;
        }
    }
    free_memory(current_objects);
    return 0;
}
