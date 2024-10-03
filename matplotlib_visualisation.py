import matplotlib.pyplot as plt
import re

def read_data(filename):
    # Ouvre le fichier et lit les lignes
    with open(filename, 'r') as file:
        data = file.readlines()

    planetes_positions = []  # Liste pour stocker les positions de chaque frame
    for line in data:
        # Utilisation de regex pour extraire les valeurs numériques
        coords = re.findall(r'\(([\d\.\-]+),([\d\.\-]+),', line)
        positions_frame = [(float(x), float(y)) for x, y in coords]
        planetes_positions.append(positions_frame)
    
    return planetes_positions

def plot_simulation(planetes_positions):
    fig, ax = plt.subplots()
    num_planets = len(planetes_positions[0])
    colors = [(0.05*i,0.95-0.05*i,0.12+0.05*i) for i in range(20)]

    for i in range(num_planets):
        x_vals = [frame[i][0] for frame in planetes_positions]
        y_vals = [frame[i][1] for frame in planetes_positions]
        ax.plot(x_vals, y_vals, label=f'Planete-{i}', color=colors[i])
    
    ax.set_xlabel('X Position')
    ax.set_ylabel('Y Position')
    ax.set_title('Simulation des positions des planètes')
    ax.legend()
    plt.show()


filename = 'data/data.txt'
positions = read_data(filename)
plot_simulation(positions)
