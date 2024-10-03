import pygame
import sys

# Fonction pour lire les données du fichier texte
def read_data(file_path):
    """tableau, chaque element est une frame. Chaque frame est un tableau de n-uplets contenant les infos de chaque astre"""
    all_frames = []
    try:
        with open(file_path, "r") as file:
            for line in file:
                frame_data = []
                # Supprimer les crochets et couper la ligne
                line = line.strip()[1:-1]
                objects = line.split("),")
                for obj in objects:
                    obj = obj.strip("()")  # Supprimer les parenthèses
                    values = obj.split(",")
                    # Récupérer x, y
                    x = float(values[0])
                    y = float(values[1])
                    vx = float(values[2])
                    vy = float(values[3])
                    mass = float(values[4])
                    radius = float(values[5])
                    name = values[6]
                    frame_data.append((x, y, vx, vy, mass, radius, name))
                all_frames.append(frame_data)
        return all_frames
    except FileNotFoundError:
        print(f"Erreur: Le fichier {file_path} n'a pas été trouvé.")
        sys.exit(1)
    except Exception as e:
        print(f"Erreur lors de la lecture du fichier: {str(e)}")
        sys.exit(1)

# Fonction pour afficher les planètes dans Pygame
def visualize_data(frames):
    pygame.init()

    # Dimensions de la fenêtre
    width, height = 1200, 900
    screen = pygame.display.set_mode((width, height))
    pygame.display.set_caption('Simulation de planètes')

    # Paramètres de visualisation
    bg_color = (0, 0, 0)  # Couleur de fond noire

    # Échelle pour adapter les coordonnées des planètes à l'écran
    scale_factor = 0.05
    x_shift = 0
    y_shift = 3000 #pixels
    clock = pygame.time.Clock()
    frame_index = 0

    # Boucle principale
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        screen.fill(bg_color)  # Remplir le fond

        # Récupérer les positions de la frame actuelle
        if frame_index < len(frames):
            for i, planet_pos in enumerate(frames[frame_index]):
                x, y, vx, vy, mass, radius, name = planet_pos

                planet_color = (12+i, 253-i, 27+i)

                # Convertir les coordonnées à l'échelle de la fenêtre
                screen_x = int(width / 2 + (x+x_shift) * scale_factor)
                screen_y = int(height / 2 - (y+y_shift) * scale_factor)  # inverser y pour correspondre à Pygame
                pygame.draw.circle(screen, planet_color, (screen_x, screen_y), radius/50)
            
            frame_index += 1
        else:
            frame_index = 0  # Réinitialiser à la première frame après la dernière

        pygame.display.flip()  # Mettre à jour l'affichage
        clock.tick(350)  # Limiter à 30 FPS

    pygame.quit()

# Charger les données depuis le fichier et démarrer la visualisation

file_path = "data/2corps1astre.txt"
frames = read_data(file_path)
visualize_data(frames)
