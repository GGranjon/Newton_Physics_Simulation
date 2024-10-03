#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <ctime>
#include <math.h>
#include <fstream>
#define cst_g 6.6743*10e-7
#define fps 60
#define simulation_time 200
#define pos_min -1000
#define pos_max 1000
#define source "data/data.txt"
using namespace std;

struct corps
{
    double x;
    double y;
    double vx;
    double vy;
    long long int masse;
    int rayon;
    string name;
};

struct star : corps{
    star(double x_, double y_, string name_){
        masse = 10e12;
        x = x_;
        y = y_;
        vx = 0.;
        vy = 0.;
        name = name_;
        rayon = 800;
    }
};

struct planet : corps{
    planet(double x_, double y_, double vx_, double vy_, string name_){
        masse = 2e10;
        x = x_;
        y = y_;
        vx = vx_;
        vy = vy_;
        name = name_;
        rayon = 50;
    }
};

double distance(const corps& a, const corps& b){
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

void update(vector<corps*>& current_positions){
    double dt = 3.0 / fps;
    double force_x, force_y, dist, accel;

    for (corps* main_corps : current_positions) {
        force_x = 0;
        force_y = 0;

        for (corps* other_corps : current_positions) {
            if (main_corps != other_corps) {

                dist = distance(*main_corps, *other_corps);
                accel = (cst_g * other_corps->masse) / (dist * dist);

                // Projection sur x et y
                force_x += accel * ((other_corps->x - main_corps->x) / dist);
                force_y += accel * ((other_corps->y - main_corps->y) / dist);
            }
        }

        main_corps->vx += force_x * dt;
        main_corps->vy += force_y * dt;
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
                file << "(" + to_string(obj->x) +","+ to_string(obj->y) +","+ to_string(obj->vx) +","+ to_string(obj->vy) +","+ to_string(obj->masse) +","+ to_string(obj->rayon) +","+ obj->name + "),";
            }
            else{
                file << "(" + to_string(obj->x) +","+ to_string(obj->y) +","+ to_string(obj->vx) +","+ to_string(obj->vy) +","+ to_string(obj->masse) +","+ to_string(obj->rayon) +","+ obj->name + ")";
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
        cout<<x_val;
        y_val = rdm_value(pos_min, pos_max);
        v_x = rdm_value(pos_min/10, pos_max/10);
        v_y = rdm_value(pos_min/10, pos_max/10);
        corps* ptr = new planet({x_val,y_val,v_x,v_y,"planete-"+to_string(i)});
        cout<<ptr->y;
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
    cout << "Combien d'etoiles?\n";
    cin >> nb_stars;
    srand((time(NULL)));
    init_randomly(nb_planets, nb_stars, current_objects);
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
