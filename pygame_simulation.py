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

def visualize_data(frames):
    pygame.init()

    # Dimensions de la fenêtre
    screen_width = pygame.display.Info().current_w
    screen_height = pygame.display.Info().current_h
    screen = pygame.display.set_mode((screen_width, screen_height), pygame.FULLSCREEN)
    pygame.display.set_caption('Simulation de planètes')
    
    # Position de la caméra (initialement centrée)
    camera_x, camera_y = 0, 0
    camera_speed = 10

    # Charger l'image de fond
    background_image = pygame.image.load("img/background.jpg")  # Remplace par le chemin de ton image
    #background_image = pygame.transform.scale(background_image, (screen_width, screen_height))  # Redimensionner si nécessaire

    # Obtenir la taille de l'image d'origine
    image_width, image_height = background_image.get_size()

    # Calculer le rapport d'aspect de l'image et de l'écran
    image_aspect = image_width / image_height
    screen_aspect = screen_width / screen_height

    # Redimensionner l'image tout en gardant son rapport d'aspect
    if image_aspect > screen_aspect:
        new_width = screen_width
        new_height = int(screen_width / image_aspect)
    else:
        new_height = screen_height
        new_width = int(screen_height * image_aspect)

    # Redimensionner l'image
    background_image = pygame.transform.scale(background_image, (new_width, new_height))

    # Calculer la position pour centrer l'image
    image_x = (screen_width - new_width) // 2
    image_y = (screen_height - new_height) // 2
    # Paramètres de visualisation
    bg_color = (0, 0, 0)  # Couleur de fond noire

    # Échelle pour adapter les coordonnées des planètes à l'écran
    scale_factor = 0.2
    x_shift = 0
    y_shift = 0 #pixels
    clock = pygame.time.Clock()
    frame_index = 0

    # Boucle principale
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        # Gestion des touches pour le déplacement (ZQSD)
        keys = pygame.key.get_pressed()
        if keys[pygame.K_z]:  # Z pour aller en haut
            camera_y += camera_speed
        if keys[pygame.K_s]:  # S pour aller en bas
            camera_y -= camera_speed
        if keys[pygame.K_q]:  # Q pour aller à gauche
            camera_x += camera_speed
        if keys[pygame.K_d]:  # D pour aller à droite
            camera_x -= camera_speed

        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_ESCAPE:
                running = False  # Arrêter la simulation

        #screen.fill(bg_color)  # Remplir le fond
        screen.fill((0,0,0))
        screen.blit(background_image, (0, 0))

        # Récupérer les positions de la frame actuelle
        if frame_index < len(frames):
            for i, planet_pos in enumerate(frames[frame_index]):
                x, y, vx, vy, mass, radius, name = planet_pos

                planet_color = (12+i, 253-i, 27+i)

                # Convertir les coordonnées à l'échelle de la fenêtre
                screen_x = int(screen_width / 2 + (x+x_shift) * scale_factor)
                screen_y = int(screen_height / 2 - (y+y_shift) * scale_factor)  # inverser y pour correspondre à Pygame

                # Position de la planète relative à la caméra
                planet_x = screen_x + camera_x
                planet_y = screen_y + camera_y
                if "planete" in name:
                    pygame.draw.circle(screen, "red", (planet_x, planet_y), radius/5)
                else:
                    pygame.draw.circle(screen, "white", (planet_x, planet_y), radius/5)

            
            frame_index += 1
        else:
            frame_index = 0  # Réinitialiser à la première frame après la dernière

        pygame.display.flip()  # Mettre à jour l'affichage
        clock.tick(30)  # Limiter à 30 FPS

    pygame.quit()

# Charger les données depuis le fichier et démarrer la visualisation

file_path = "data/data.txt"
frames = read_data(file_path)
visualize_data(frames)
