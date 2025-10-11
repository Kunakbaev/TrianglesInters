import math
from pathlib import Path
import sys

# Get the parent directory of the current script's parent
parent_dir = Path(__file__).parent.parent
sys.path.insert(0, str(parent_dir))

import common

"""

Once again, all triangles are contained in single plain: OXY (z = 0). Their points
are located on the same circle and all triangles are equilateral. By doing so,
we make sure, that their centroids (arithmetic vector sum of points in triangle)
lies in center of that point.

"""


TEST_DATA_DIR   = common.SCRIPT_DIR + "/tests_data/in_one_plane/"
NUM_TESTS       = 10
NUM_TRIANGLES   = 100000

def get_point_by_polar(phi, radius):
  return common.Point(math.cos(phi) * radius, math.sin(phi) * radius, 0)

def generate_triangles():
  triangles = []
  # NUM_TRIANGLES      = random.randint(5, 100)
  RADIUS = common.random_double(1e-3, 1e3)
  PI_3   = 2 * math.pi / 3

  for _ in range(NUM_TRIANGLES):
    phi = common.random_double(0, 2 * math.pi)
    point_a = get_point_by_polar(phi,            RADIUS)
    point_b = get_point_by_polar(phi + PI_3,     RADIUS)
    point_c = get_point_by_polar(phi + 2 * PI_3, RADIUS)
    triangles.append(common.Triangle(point_a, point_b, point_c))

  return triangles

if __name__ == "__main__":
  if len(sys.argv) != 3:
    print("Error: this script expects 2 arguments: number of triangles and destination folder")
    exit(1)

  NUM_TRIANGLES = int(sys.argv[1])
  res_folder_path = sys.argv[2]
  TEST_DATA_DIR += res_folder_path + "/"

  common.generate_tests(TEST_DATA_DIR, NUM_TESTS, generate_triangles)
