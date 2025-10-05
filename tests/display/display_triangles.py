import matplotlib.pyplot as plt
import numpy as np
from   mpl_toolkits.mplot3d.art3d import Poly3DCollection

"""
Create a 3D visualization of triangles with automatic scaling

Parameters:
triangles: list of numpy arrays, each of shape (3, 3) representing triangle vertices
alpha: transparency level (0-1)
"""
def create_3d_triangles(triangles, alpha=0.7):
    fig = plt.figure(figsize=(12, 10))
    ax  = fig.add_subplot(111, projection='3d')
    
    # Create a collection for all triangles
    collection = []
    colors     = []
    
    # Generate a color for each triangle
    for i, triangle in enumerate(triangles):
        # Create Poly3DCollection for each triangle
        poly = Poly3DCollection([triangle], alpha=alpha, linewidth=2)

        # Generate beautiful colors using a colormap
        color = plt.cm.viridis(i / len(triangles))
        poly.set_facecolor(color)
        poly.set_edgecolor('black')  # Add edges for better visibility
        
        collection.append(poly)
        colors.append(color)
    
    # Add all triangles to the plot
    for poly in collection:
        ax.add_collection3d(poly)
    
    # Calculate automatic scaling
    all_points = np.vstack(triangles)
    min_coords = np.min(all_points, axis=0)
    max_coords = np.max(all_points, axis=0)
    center     = (min_coords + max_coords) / 2
    
    # Calculate the maximum range in any dimension
    max_range = np.max(max_coords - min_coords)
    
    # Set equal aspect ratio and automatic zoom
    if max_range > 0:
        # Add 10% padding
        padding = max_range * 0.1
        half_side = max_range / 2 + padding
        ax.set_xlim([center[0] - half_side, center[0] + half_side])
        ax.set_ylim([center[1] - half_side, center[1] + half_side])
        ax.set_zlim([center[2] - half_side, center[2] + half_side])
    
    # Set labels and title
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    ax.set_title('3D Triangles display', fontsize=14, pad=20)
    
    # Add grid and set viewing angle
    ax.grid(True, alpha=0.3)
    ax.view_init(elev=20, azim=45)

    # Equal aspect ratio
    try:
        ax.set_box_aspect([1, 1, 1])  # For newer matplotlib versions
    except AttributeError:
        # Fallback for older versions
        max_range = np.array([ax.get_xlim(), ax.get_ylim(), ax.get_zlim()]).ptp(axis=1).max()
        mid_x = np.mean(ax.get_xlim())
        mid_y = np.mean(ax.get_ylim())
        mid_z = np.mean(ax.get_zlim())
        ax.set_xlim(mid_x - max_range/2, mid_x + max_range/2)
        ax.set_ylim(mid_y - max_range/2, mid_y + max_range/2)
        ax.set_zlim(mid_z - max_range/2, mid_z + max_range/2)

    plt.tight_layout()
    return fig, ax
